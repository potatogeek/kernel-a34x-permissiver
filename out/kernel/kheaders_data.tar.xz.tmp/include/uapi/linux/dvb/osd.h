/* SPDX-License-Identifier: LGPL-2.1+ WITH Linux-syscall-note */


#ifndef _DVBOSD_H_
#define _DVBOSD_H_

#include <linux/compiler.h>

typedef enum {
  // All functions return -2 on "not open"
  OSD_Close=1,    // ()
  // Disables OSD and releases the buffers
  // returns 0 on success
  OSD_Open,       // (x0,y0,x1,y1,BitPerPixel[2/4/8](color&0x0F),mix[0..15](color&0xF0))
  // Opens OSD with this size and bit depth
  // returns 0 on success, -1 on DRAM allocation error, -2 on "already open"
  OSD_Show,       // ()
  // enables OSD mode
  // returns 0 on success
  OSD_Hide,       // ()
  // disables OSD mode
  // returns 0 on success
  OSD_Clear,      // ()
  // Sets all pixel to color 0
  // returns 0 on success
  OSD_Fill,       // (color)
  // Sets all pixel to color <col>
  // returns 0 on success
  OSD_SetColor,   // (color,R{x0},G{y0},B{x1},opacity{y1})
  // set palette entry <num> to <r,g,b>, <mix> and <trans> apply
  // R,G,B: 0..255
  // R=Red, G=Green, B=Blue
  // opacity=0:      pixel opacity 0% (only video pixel shows)
  // opacity=1..254: pixel opacity as specified in header
  // opacity=255:    pixel opacity 100% (only OSD pixel shows)
  // returns 0 on success, -1 on error
  OSD_SetPalette, // (firstcolor{color},lastcolor{x0},data)
  // Set a number of entries in the palette
  // sets the entries "firstcolor" through "lastcolor" from the array "data"
  // data has 4 byte for each color:
  // R,G,B, and a opacity value: 0->transparent, 1..254->mix, 255->pixel
  OSD_SetTrans,   // (transparency{color})
  // Sets transparency of mixed pixel (0..15)
  // returns 0 on success
  OSD_SetPixel,   // (x0,y0,color)
  // sets pixel <x>,<y> to color number <col>
  // returns 0 on success, -1 on error
  OSD_GetPixel,   // (x0,y0)
  // returns color number of pixel <x>,<y>,  or -1
  OSD_SetRow,     // (x0,y0,x1,data)
  // fills pixels x0,y through  x1,y with the content of data[]
  // returns 0 on success, -1 on clipping all pixel (no pixel drawn)
  OSD_SetBlock,   // (x0,y0,x1,y1,increment{color},data)
  // fills pixels x0,y0 through  x1,y1 with the content of data[]
  // inc contains the width of one line in the data block,
  // inc<=0 uses blockwidth as linewidth
  // returns 0 on success, -1 on clipping all pixel
  OSD_FillRow,    // (x0,y0,x1,color)
  // fills pixels x0,y through  x1,y with the color <col>
  // returns 0 on success, -1 on clipping all pixel
  OSD_FillBlock,  // (x0,y0,x1,y1,color)
  // fills pixels x0,y0 through  x1,y1 with the color <col>
  // returns 0 on success, -1 on clipping all pixel
  OSD_Line,       // (x0,y0,x1,y1,color)
  // draw a line from x0,y0 to x1,y1 with the color <col>
  // returns 0 on success
  OSD_Query,      // (x0,y0,x1,y1,xasp{color}}), yasp=11
  // fills parameters with the picture dimensions and the pixel aspect ratio
  // returns 0 on success
  OSD_Test,       // ()
  // draws a test picture. for debugging purposes only
  // returns 0 on success
// TODO: remove "test" in final version
  OSD_Text,       // (x0,y0,size,color,text)
  OSD_SetWindow, //  (x0) set window with number 0<x0<8 as current
  OSD_MoveWindow, //  move current window to (x0, y0)
  OSD_OpenRaw,	// Open other types of OSD windows
} OSD_Command;

typedef struct osd_cmd_s {
	OSD_Command cmd;
	int x0;
	int y0;
	int x1;
	int y1;
	int color;
	void __user *data;
} osd_cmd_t;


typedef enum {
	OSD_BITMAP1,           
	OSD_BITMAP2,           
	OSD_BITMAP4,           
	OSD_BITMAP8,           
	OSD_BITMAP1HR,         
	OSD_BITMAP2HR,         
	OSD_BITMAP4HR,         
	OSD_BITMAP8HR,         
	OSD_YCRCB422,          
	OSD_YCRCB444,          
	OSD_YCRCB444HR,        
	OSD_VIDEOTSIZE,        
	OSD_VIDEOHSIZE,        
	OSD_VIDEOQSIZE,        
	OSD_VIDEODSIZE,        
	OSD_VIDEOTHSIZE,       
	OSD_VIDEOTQSIZE,       
	OSD_VIDEOTDSIZE,       
	OSD_VIDEONSIZE,        
	OSD_CURSOR             
} osd_raw_window_t;

typedef struct osd_cap_s {
	int  cmd;
#define OSD_CAP_MEMSIZE         1  
	long val;
} osd_cap_t;


#define OSD_SEND_CMD            _IOW('o', 160, osd_cmd_t)
#define OSD_GET_CAPABILITY      _IOR('o', 161, osd_cap_t)

#endif
