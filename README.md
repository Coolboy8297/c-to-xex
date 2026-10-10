# C → XEX (Xbox 360)

Minimal template + **GitHub Actions** that turns `.c` files into Xbox 360 homebrew binaries using **libxenon**.

Push code → GitHub builds it → download the `.elf32` (and optionally `.xex`).

## How to use (easiest way)

1. **Fork** this repository (or create a new repo and copy these files).
2. Put your `.c` / `.cpp` files in the `source/` folder.
3. Push to `main` (or `master`).
4. Go to the **Actions** tab → click the latest workflow run → download the **xbox360-homebrew** artifact.
5. Inside the zip you will find:
   - `*.elf32` ← this is what you use with XeLL
   - `*.elf`
   - `*.xex` (only if xextool was available)

You can also trigger a build manually with the **Run workflow** button.

## Local build (optional)

```bash
# Requires libxenon / devkitXenon installed, or use the Docker image:
docker run --rm -v "$PWD":/app -w /app free60/libxenon:latest make

# or with environment set:
export DEVKITXENON=/usr/local/xenon
export PATH=$DEVKITXENON/bin:$PATH
make
```

## Project layout

```
├── .github/workflows/build.yml   ← automatic builder
├── Makefile
├── source/
│   └── main.c                    ← put your code here
├── data/                         ← binary assets (optional)
├── README.md
└── .gitignore
```

## Running on hardware

You need an RGH / JTAG / XeLL-capable Xbox 360.

1. Copy the `.elf32` to the root of a FAT32 USB stick.
2. Rename it to `xenon.elf`.
3. Boot XeLL — it loads the file automatically.

## Notes

- Uses the open-source **libxenon** toolchain via the official `free60/libxenon` Docker image.
- No official Microsoft XDK required.
- The binary is unsigned homebrew — it will not run on a stock retail dashboard.

## License

Public domain. Do whatever you want.
