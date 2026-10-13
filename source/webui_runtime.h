#ifndef WEBUI_RUNTIME_H
#define WEBUI_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
typedef struct { int x,y,w,h; uint32_t background,foreground; int is_button; const char *text,*action; } WebElement;
void webui_run(const WebElement *elements,size_t count,uint32_t background);
#endif
