#include "webui_runtime.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <input/input.h>
#include <usb/usbmain.h>
#include <xenos/xenos.h>
#include <cache.h>
typedef struct { uint32_t a[4],base,b[8],width,height; } VideoInfo;
static uint32_t *fb; static int sw,sh,tw,th;
static size_t pi(int x,int y){return (size_t)((((y>>5)*32*tw+((x>>5)<<10)+(x&3)+((y&1)<<2)+(((x&31)>>2)<<3)+(((y&31)>>1)<<6))^((y&8)<<2)));}
static uint32_t cv(uint32_t c){return ((c&255)<<24)|(((c>>8)&255)<<16)|(((c>>16)&255)<<8)|255;}
static void px(int x,int y,uint32_t c){if(fb&&x>=0&&y>=0&&x<sw&&y<sh)fb[pi(x,y)]=cv(c);}
static void box(int x,int y,int w,int h,uint32_t c){int a,b;if(w<=0||h<=0)return;if(x<0){w+=x;x=0;}if(y<0){h+=y;y=0;}if(x+w>sw)w=sw-x;if(y+h>sh)h=sh-y;for(b=y;b<y+h;b++)for(a=x;a<x+w;a++)px(a,b,c);}
static const unsigned char f[36][5]={
{126,17,17,17,126},{127,73,73,73,54},{62,65,65,65,34},{127,65,65,34,28},{127,73,73,73,65},{127,9,9,9,1},{62,65,73,73,122},{127,8,8,8,127},{0,65,127,65,0},{32,64,65,63,1},{127,8,20,34,65},{127,64,64,64,64},{127,2,12,2,127},{127,4,8,16,127},{62,65,65,65,62},{127,9,9,9,6},{62,65,81,33,94},{127,9,25,41,70},{70,73,73,73,49},{1,1,127,1,1},{63,64,64,64,63},{31,32,64,32,31},{63,64,56,64,63},{99,20,8,20,99},{7,8,112,8,7},{97,81,73,69,67},
{62,81,73,69,62},{0,66,127,64,0},{66,97,81,73,70},{33,65,69,75,49},{24,20,18,127,16},{39,69,69,69,57},{60,74,73,73,48},{1,113,9,5,3},{54,73,73,73,54},{6,73,73,41,30},{0,54,54,0,0}};
static void glyph(int x,int y,char ch,uint32_t c){int i,col,row;ch=(char)toupper((unsigned char)ch);if(ch>='A'&&ch<='Z')i=ch-'A';else if(ch>='0'&&ch<='9')i=26+ch-'0';else if(ch==':')i=36;else if(ch==' ')return;else if(ch=='-'){box(x,y+3,4,1,c);return;}else if(ch=='.'){box(x,y+6,1,1,c);return;}else if(ch=='!'){box(x+2,y,1,5,c);box(x+2,y+6,1,1,c);return;}else{box(x,y+6,3,1,c);return;}if(i>=36)return;for(col=0;col<5;col++)for(row=0;row<7;row++)if(f[i][col]&(1u<<row))px(x+col,y+row,c);}
static void txt(int x,int y,const char*s,uint32_t c,int maxw){int start=x;if(!s)return;while(*s){if(*s=='\n'||x-start+6>maxw){x=start;y+=10;if(*s=='\n'){s++;continue;}}glyph(x,y,*s++,c);x+=6;}}
static void render(const WebElement*e,size_t n,int sel,const char*msg,uint32_t bg){size_t i;box(0,0,sw,sh,bg);box(0,0,sw,65,0x172536);txt(28,20,"HTML 360 NATIVE APP",0x62E6D2,sw-56);box(28,75,sw-56,2,0x33485B);for(i=0;i<n;i++){if(!e[i].text||!e[i].text[0])continue;if(e[i].is_button){box(e[i].x-7,e[i].y-6,e[i].w+14,e[i].h+12,(int)i==sel?0x62E6D2:0x31475C);box(e[i].x-4,e[i].y-3,e[i].w+8,e[i].h+6,(int)i==sel?0x24374A:e[i].background);}txt(e[i].x+4,e[i].y+4,e[i].text,e[i].foreground,e[i].w-8);}box(28,sh-58,sw-56,1,0x33485B);txt(28,sh-43,msg&&*msg?msg:"D-PAD SELECT   A ACTIVATE   START EXIT",0xB9C7D8,sw-56);dcache_flush(fb,(size_t)tw*th*4);}
void webui_run(const WebElement*e,size_t n,uint32_t bg){VideoInfo*vi=(VideoInfo*)0x80000200ec806100ULL;struct controller_data_s old,c;size_t i;int sel=-1;char msg[180]="D-PAD SELECT   A ACTIVATE   START EXIT";memset(&old,0,sizeof(old));xenos_init(VIDEO_MODE_AUTO);sw=(int)vi->width;sh=(int)vi->height;tw=(sw+31)&~31;th=(sh+31)&~31;fb=(uint32_t*)((uintptr_t)vi->base|0x8000000000000000ULL);for(i=0;i<n;i++)if(e[i].is_button){sel=(int)i;break;}usb_init();usb_do_poll();for(;;){memset(&c,0,sizeof(c));if(get_controller_data(&c,0)){if(c.start&&!old.start)break;if(c.down&&!old.down){size_t j;for(j=(size_t)(sel<0?0:sel+1);j<n;j++)if(e[j].is_button){sel=(int)j;break;}if(j==n)for(j=0;j<n;j++)if(e[j].is_button){sel=(int)j;break;}}if(c.up&&!old.up){size_t j;for(j=(size_t)(sel<=0?n:sel);j>0;j--)if(e[j-1].is_button){sel=(int)(j-1);break;}}if(c.a&&!old.a&&sel>=0&&e[sel].action&&e[sel].action[0]){strncpy(msg,e[sel].action,sizeof(msg)-1);msg[sizeof(msg)-1]=0;}old=c;}render(e,n,sel,msg,bg);usb_do_poll();}}
