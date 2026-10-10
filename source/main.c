/*
 * VEYAORA — Xbox 360 homebrew prototype
 * Built for the existing LibXenon project.
 *
 * This version provides the first controller-driven VEYAORA shell and a
 * read-only scan of filesystem paths that are already mounted by the runtime.
 * It does NOT mount the Xbox 360 HDD/USB by itself, and it does NOT launch XEX
 * files. Those need a verified storage/runtime integration (for example, an
 * Aurora-compatible plugin or a XEX build with the appropriate environment).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <console/console.h>
#include <input/input.h>
#include <usb/usbmain.h>
#include <xenon_soc/xenon_power.h>
#include <xenos/xenos.h>

#define MAX_GAMES 96
#define NAME_LEN  96
#define PATH_LEN  256
#define MAX_ROOTS 12

typedef struct {
    char name[NAME_LEN];
    char path[PATH_LEN];
    bool folder;
} GameEntry;

static GameEntry games[MAX_GAMES];
static unsigned int game_count = 0;
static unsigned int selected = 0;
static int page = 0;
static char status_line[160] = "Ready. Choose Scan Library to search mounted paths.";
static const char *scan_roots[MAX_ROOTS] = {
    "/usb0", "/Usb0", "/usb", "/mnt/usb0", "/media/usb0",
    "/hdd", "/Hdd1", "/dev_hdd0", "/mnt/hdd", "/games",
    "/Games", "/"
};

static bool path_is_dir(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static bool path_is_file(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static void add_game(const char *name, const char *path, bool folder) {
    unsigned int i;
    if (!name || !path || game_count >= MAX_GAMES) return;
    for (i = 0; i < game_count; ++i) {
        if (strcmp(games[i].path, path) == 0) return;
    }
    strncpy(games[game_count].name, name, NAME_LEN - 1);
    games[game_count].name[NAME_LEN - 1] = '\0';
    strncpy(games[game_count].path, path, PATH_LEN - 1);
    games[game_count].path[PATH_LEN - 1] = '\0';
    games[game_count].folder = folder;
    ++game_count;
}

static void join_path(char *out, size_t out_size, const char *a, const char *b) {
    size_t n;
    if (!out || out_size == 0) return;
    n = strlen(a);
    snprintf(out, out_size, "%s%s%s", a, (n && a[n - 1] == '/') ? "" : "/", b);
}

/* Read-only scan: detect folders containing default.xex and standalone .xex files. */
static void scan_one_root(const char *root) {
    DIR *dir = opendir(root);
    struct dirent *ent;
    if (!dir) return;
    while ((ent = readdir(dir)) != NULL && game_count < MAX_GAMES) {
        char child[PATH_LEN];
        if (ent->d_name[0] == '.') continue;
        join_path(child, sizeof(child), root, ent->d_name);
        if (path_is_dir(child)) {
            char default_xex[PATH_LEN];
            join_path(default_xex, sizeof(default_xex), child, "default.xex");
            if (path_is_file(default_xex)) add_game(ent->d_name, default_xex, true);
            /* One extra level covers common Games/<title>/default.xex layouts. */
            DIR *sub = opendir(child);
            struct dirent *subent;
            if (sub) {
                while ((subent = readdir(sub)) != NULL && game_count < MAX_GAMES) {
                    char nested[PATH_LEN];
                    char nested_xex[PATH_LEN];
                    if (subent->d_name[0] == '.') continue;
                    join_path(nested, sizeof(nested), child, subent->d_name);
                    if (!path_is_dir(nested)) continue;
                    join_path(nested_xex, sizeof(nested_xex), nested, "default.xex");
                    if (path_is_file(nested_xex)) add_game(subent->d_name, nested_xex, true);
                }
                closedir(sub);
            }
        } else {
            const char *dot = strrchr(ent->d_name, '.');
            if (dot && (strcmp(dot, ".xex") == 0 || strcmp(dot, ".XEX") == 0))
                add_game(ent->d_name, child, false);
        }
    }
    closedir(dir);
}

static void scan_library(void) {
    unsigned int i;
    game_count = 0;
    selected = 0;
    for (i = 0; i < MAX_ROOTS && game_count < MAX_GAMES; ++i) {
        /* Avoid recursively scanning the root filesystem itself. */
        if (strcmp(scan_roots[i], "/") == 0) continue;
        if (path_is_dir(scan_roots[i])) scan_one_root(scan_roots[i]);
    }
    if (game_count) {
        snprintf(status_line, sizeof(status_line), "Scan complete: %u candidate XEX entries found.", game_count);
    } else {
        snprintf(status_line, sizeof(status_line), "No games found in mounted scan paths. Storage may not be mounted in LibXenon.");
    }
}

static void draw_shell(void) {
    unsigned int i;
    console_clrscr();
    printf("\n");
    printf("  V E Y A O R A\n");
    printf("  PLAY BEYOND WHAT'S NEXT\n");
    printf("  ------------------------------------------------------------\n");
    printf("  HOME      LIBRARY      DEVICES      SETTINGS\n");
    printf("  ------------------------------------------------------------\n\n");
    if (page == 0) {
        printf("  WELCOME BACK\n\n");
        printf("  [A] Open Game Library\n");
        printf("  [X] Scan mounted storage for XEX games\n");
        printf("  [Y] About this prototype\n\n");
        printf("  Library entries: %u\n", game_count);
    } else if (page == 1) {
        printf("  GAME LIBRARY  |  %u entries\n\n", game_count);
        if (!game_count) {
            printf("  No entries scanned yet. Press X to scan mounted paths.\n");
        } else {
            for (i = 0; i < game_count && i < 12; ++i) {
                printf("  %c %s\n", (i == selected) ? '>' : ' ', games[i].name);
            }
            if (selected < game_count) {
                printf("\n  Selected path:\n  %s\n", games[selected].path);
                printf("\n  [A] Launch (not available in this LibXenon build)\n");
            }
        }
        printf("\n  [B] Back   [X] Rescan   [UP/DOWN] Select\n");
    } else if (page == 2) {
        printf("  ABOUT VEYAORA\n\n");
        printf("  Xbox 360 proof-of-concept shell.\n");
        printf("  Controller navigation and read-only XEX discovery are experimental.\n\n");
        printf("  This ELF cannot automatically use Aurora's configured paths or\n");
        printf("  launch games without a compatible integration layer.\n\n");
        printf("  [B] Back\n");
    }
    printf("\n  ------------------------------------------------------------\n");
    printf("  STATUS: %s\n", status_line);
    printf("\n  VEYAORA prototype | LibXenon\n");
}

int main(void) {
    struct controller_data_s oldc;
    memset(&oldc, 0, sizeof(oldc));
    xenos_init(VIDEO_MODE_AUTO);
    console_init();
    xenon_make_it_faster(XENON_SPEED_FULL);
    usb_init();
    usb_do_poll();
    strcpy(status_line, "System ready. Press A to open Library or X to scan.");

    while (1) {
        struct controller_data_s c;
        memset(&c, 0, sizeof(c));
        if (get_controller_data(&c, 0)) {
            if (c.start && !oldc.start) break;
            if (c.b && !oldc.b) page = 0;
            if (page == 0) {
                if (c.a && !oldc.a) page = 1;
                if (c.x && !oldc.x) { scan_library(); page = 1; }
                if (c.y && !oldc.y) page = 2;
            } else if (page == 1) {
                if (c.x && !oldc.x) scan_library();
                if (c.up && !oldc.up && selected > 0) --selected;
                if (c.down && !oldc.down && selected + 1 < game_count) ++selected;
                if (c.a && !oldc.a && game_count) {
                    strcpy(status_line, "Launch needs Aurora/XEX runtime integration; no game was started.");
                }
            }
            oldc = c;
        }
        draw_shell();
        usb_do_poll();
    }
    return 0;
}
