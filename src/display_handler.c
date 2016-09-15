/**
 *
 * FPGA Display Handler IP Core By Mehran Ahadi (http://mehran.ahadi.me)
 * This IP allows you to draw shapes and print texts on VGA screen.
 * Copyright (C) 2015-2016  Mehran Ahadi
 * This work is released under MIT License.
 *
 * Driver Functions Definition File
 */
 

//////////////////////////////////////////////////////////////////////////////
// Version:           1.00.a
// Description:       display_handler Driver Source File
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "display_handler.h"

/************************** Function Definitions ***************************/

////////////////////////////// Initializers //////////////////////////////

unsigned long DISPLAY_HANDLER_BASEADDR;
int DISPLAY_HANDLER_TEXT_STARTLINE = 27;
int DISPLAY_HANDLER_TEXT_CURSOR_Y = 27;
int DISPLAY_HANDLER_TEXT_CURSOR_X = 0;
int DISPLAY_HANDLER_RES_X = 640;
int DISPLAY_HANDLER_RES_Y = 480;
int DISPLAY_HANDLER_CHARS_X = 80;
int DISPLAY_HANDLER_CHARS_Y = 30;


void setBaseAddr(unsigned long baseaddr){
	DISPLAY_HANDLER_BASEADDR = baseaddr;
}

void setResolution(int x, int y){
	DISPLAY_HANDLER_RES_X = x;
	DISPLAY_HANDLER_RES_Y = y;
	DISPLAY_HANDLER_CHARS_X = x / 8;
	DISPLAY_HANDLER_CHARS_Y = y / 16;
}

void setLineCount(int count){
	if (count > DISPLAY_HANDLER_CHARS_Y) count = DISPLAY_HANDLER_CHARS_Y;
	if (count < 1) count = 1;
	DISPLAY_HANDLER_TEXT_STARTLINE = DISPLAY_HANDLER_TEXT_CURSOR_Y = DISPLAY_HANDLER_CHARS_Y - count;
}

////////////////////////////// Basic functions //////////////////////////////

void pixel_set(unsigned int x, unsigned int y){
	if(x >= DISPLAY_HANDLER_RES_X || y >= DISPLAY_HANDLER_RES_Y) return;
	XIo_Out8(DISPLAY_HANDLER_BASEADDR+(y*1024)+x, 1);
}

void pixel_clear(unsigned int x, unsigned int y){
	if(x >= DISPLAY_HANDLER_RES_X || y >= DISPLAY_HANDLER_RES_Y) return;
	XIo_Out8(DISPLAY_HANDLER_BASEADDR+(y*1024)+x, 0);
}

unsigned char pixel_get(unsigned int x, unsigned int y){
	if(x >= DISPLAY_HANDLER_RES_X || y >= DISPLAY_HANDLER_RES_Y) return 0;
	return XIo_In8(DISPLAY_HANDLER_BASEADDR+(y*1024)+x);
}

void dump(){
	int i,j;
	for(i=0;i<1024;i++){
		for(j=0;j<1024;j++){
			xil_printf("%d", pixel_get(j,i));
		}
	}
	print("\n");
}

////////////////////////////// Geometric objects //////////////////////////////

void line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
	int dx = _abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = _abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		pixel_set(x0,y0);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void triangle(	unsigned int x0, unsigned int y0,
				unsigned int x1, unsigned int y1, 
				unsigned int x2, unsigned int y2){
		_tripart(x0,y0,x1,y1,x2,y2);
		_tripart(x0,y0,x2,y2,x1,y1);
		_tripart(x1,y1,x2,y2,x0,y0);
}

void rectangle(	unsigned int x0, unsigned int y0,
				unsigned int x1, unsigned int y1, 
				unsigned int x2, unsigned int y2,
				unsigned int x3, unsigned int y3) {
	
	triangle(x0,y0,x1,y1,x2,y2);
	triangle(x1,y1,x2,y2,x3,y3);
	triangle(x0,y0,x2,y2,x3,y3);
	
}

void circle(unsigned int x0, unsigned int y0, unsigned int radius)
{
	int x,y;
	for(y=y0-radius; y<y0+radius; y++){
		for(x=x0-radius; x<x0+radius; x++){
			if((x-x0)*(x-x0) + (y-y0)*(y-y0) < radius*radius){
				pixel_set(x,y);
			}
		}
	}
}

void circleUnfilled(unsigned int x0, unsigned int y0, unsigned int radius)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        pixel_set(x0 + x, y0 + y);
        pixel_set(x0 + y, y0 + x);
        pixel_set(x0 - y, y0 + x);
        pixel_set(x0 - x, y0 + y);
        pixel_set(x0 - x, y0 - y);
        pixel_set(x0 - y, y0 - x);
        pixel_set(x0 + y, y0 - x);
        pixel_set(x0 + x, y0 - y);

        y += 1;
        err += 1 + 2*y;
        if (2*(err-x) + 1 > 0)
        {
            x -= 1;
            err += 1 - 2*x;
        }
    }
}

////////////////////////////// Absolutely positioned text printing //////////////////////////////

void printCharOn(char c, unsigned int x, unsigned int y){
	if(c>=128) {
		return;
	}
	
	c -= 32;
	int cc = c * 16;

	int xx;
	int yy;
	for(yy = y; yy-y<16; yy++){
		for(xx = x; xx-x<8; xx++){
			if(display_handler_charmap[cc+yy-y][xx-x] == '1'){
				pixel_set(xx, yy);
			}else{
				pixel_clear(xx, yy);
			}
		}
	}
	
}

void printStringOn(char* chars, unsigned int x, unsigned int y){
	int l;
	for(l = 0; l < strlen(chars); l++){
		printCharOn(chars[l], x+(l*8),y);
	}
}

////////////////////////////// Linear text printing //////////////////////////////

void printString(char* chars){
	int l, lo=0;
	int y = DISPLAY_HANDLER_TEXT_CURSOR_Y;
	for(l = 0; l < strlen(chars); l++){
		
		if(chars[l] == '\r'){
			DISPLAY_HANDLER_TEXT_CURSOR_X = 0;
			lo = l+1;
		}

		if(chars[l] == '\n'){
			DISPLAY_HANDLER_TEXT_CURSOR_Y++;
			DISPLAY_HANDLER_TEXT_CURSOR_X--;
		}

		if(chars[l] == '\t'){
			DISPLAY_HANDLER_TEXT_CURSOR_X += 8;
		}

		if(DISPLAY_HANDLER_TEXT_CURSOR_X+l-lo >= DISPLAY_HANDLER_CHARS_X ){
			DISPLAY_HANDLER_TEXT_CURSOR_Y++;
			DISPLAY_HANDLER_TEXT_CURSOR_X = DISPLAY_HANDLER_TEXT_CURSOR_X+l-lo - DISPLAY_HANDLER_CHARS_X;
			lo = l;
		}

		if(DISPLAY_HANDLER_TEXT_CURSOR_Y >= DISPLAY_HANDLER_CHARS_Y) {
			DISPLAY_HANDLER_TEXT_CURSOR_Y = DISPLAY_HANDLER_CHARS_Y - 1;
			_shiftLineUp();
		}
		
		if(chars[l] >= 32 && chars[l] < 128)
			printCharOn(chars[l], (DISPLAY_HANDLER_TEXT_CURSOR_X+l-lo)*8, DISPLAY_HANDLER_TEXT_CURSOR_Y*16);
	}
	DISPLAY_HANDLER_TEXT_CURSOR_X = DISPLAY_HANDLER_TEXT_CURSOR_X+l-lo;
}

////////////////////////////// Utility functions //////////////////////////////

unsigned int _abs (int a){
	return a>0?a:(a*-1);
}

void _tripart(unsigned int x0, unsigned int y0,
		unsigned int x1, unsigned int y1, 
		unsigned int x2, unsigned int y2) {
  int dx = _abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = _abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    line(x0,y0, x2,y2);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void _shiftLineUp(){
	int x,y;
	for(y=(DISPLAY_HANDLER_TEXT_STARTLINE+1)*16; y<DISPLAY_HANDLER_RES_Y; y++){
		for(x = 0; x<1024; x++){
			XIo_Out8(DISPLAY_HANDLER_BASEADDR+((y-16)*1024)+x, 
					XIo_In8(DISPLAY_HANDLER_BASEADDR+(y*1024)+x));
		}
	}
	
	for(y=DISPLAY_HANDLER_RES_Y - 16; y<DISPLAY_HANDLER_RES_Y; y++){
		for(x = 0; x<1024; x++){
			XIo_Out8(DISPLAY_HANDLER_BASEADDR+((y)*1024)+x, 0);
		}
	}
}

