/**
 * This file is part of Wio Helium Monitor.
 *
 *   Wio Helium Monitor is free software created by Paul Pinault aka disk91. 
 *   You can redistribute it and/or modify it under the terms of the 
 *   GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *  Wio Helium Monitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Wio LoRaWan Field Tester.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author : Paul Pinault (disk91.com)
 */    
#include <Arduino.h>
#include <math.h>
#include "config.h"
#include "fonts.h"
#include "state.h"
#include "TFT_eSPI.h"
#include "ui.h"

TFT_eSPI tft;
#define X_OFFSET 2
#define Y_OFFSET 0
#define X_SIZE   80
#define Y_SIZE   20
#define R_SIZE    4 
#define BOX_SPACING 2
//                    RRRRRGGGGGGBBBBB
#define TFT_GRAY    0b1010010100010000
#define TFT_GRAY10  0b0100001100001000
#define TFT_GRAY20  0b0010000110000100

/*
#define HIST_X_OFFSET 2
#define HIST_Y_OFFSET 75
#define HIST_X_SIZE 315
#define HIST_X_TXTSIZE X_SIZE-3
#define HIST_Y_SIZE 160
#define HIST_X_BAR_OFFSET 50
#define HIST_X_BAR_SPACE 2
*/
#define MIN_SNR -20
#define MAX_SNR 30
#define MAX_RETRY 8
#define MAX_HS 20
#define MAX_DIST  64000

#define SELECTED_NONE   0
#define SELECTED_POWER  1
#define SELECTED_SF     2
#define SELECTED_RETRY  3

ui_t ui;


void initScreen() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP); 
  ui.screenInitialized = false;
}

void displayTitle() {
    char title[128];
    uint8_t model = HWTARGET;
    tft.setTextColor(TFT_GRAY);
    tft.setFreeFont(FS9);     // Select the original small TomThumb font
    sprintf(title,"Wio Helium Monitor");
    tft.drawString(title,(320-150)/2, 85, GFXFF);  
    sprintf(title,"Version %s (%s)", VERSION, "...");
    tft.drawString(title,(320-180)/2, 115, GFXFF);  
    //sprintf(title,"WIO_HM_%02X%02X%02X%02X%02X", loraConf.deveui[3],loraConf.deveui[4], loraConf.deveui[5], loraConf.deveui[6], loraConf.deveui[7]);
    //tft.drawString(title,(320-160)/2, 180, GFXFF);
}

void displaySetup() {
    char title[128];
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GRAY);
    tft.setFreeFont(FS9);     // Select the original small TomThumb font
    sprintf(title,"Wio Helium Monitor");
    tft.drawString(title,(320-150)/2, 20, GFXFF);  
    
    tft.drawRoundRect(5,75,310,110,10,TFT_WHITE);
    sprintf(title,"To setup: connect your smartphone");
    tft.drawString(title,10, 80, GFXFF);  
    sprintf(title,"on WioHeliumMonitor WiFi");
    tft.drawString(title,10, 100, GFXFF);  
    sprintf(title,"then browser 192.168.1.1");
    tft.drawString(title,10, 120, GFXFF);  
    sprintf(title,"select your WiFi network");
    tft.drawString(title,10, 140, GFXFF);  
    sprintf(title,"enter password and Submit");
    tft.drawString(title,10, 160, GFXFF);  
}

void displaySplash() {
  // Totally useless so totally mandatory
  tft.fillScreen(TFT_BLACK);
  #ifdef WITH_SPLASH 
    tft.drawRoundRect((320-200)/2,200,200,10,5,TFT_WHITE);
    tft.setTextColor(TFT_GRAY);
    tft.setFreeFont(FS9);     // Select the original small TomThumb font
    tft.drawString("disk91.com",(320-90)/2,215, GFXFF);  
    for ( int i = 10 ; i < 100 ; i+=4 ) {
      tft.fillRoundRect((320-200)/2+2,202,((204*i)/100),6,3,TFT_WHITE);
      #if (defined WITH_SPLASH_HELIUM) && ( WITH_SPLASH_HELIUM == 1 )
        draw_splash_helium(HELIUM_XCENTER, (240-100)/2, i);
      #endif
    }
  #endif
}

void clearScreen() {
    tft.fillScreen(TFT_BLACK);  
}


// ========================================
// Print ping history
// ========================================
#define HIST_PING_X_OFFSET 2
#define HIST_PING_X_SIZE 315
#define HIST_PING_ITXT_Y_OFFSET 30
#define HIST_PING_ITXT_Y_SIZE 20
#define HIST_PING_I_Y_OFFSET (HIST_PING_ITXT_Y_OFFSET+HIST_PING_ITXT_Y_SIZE)
#define HIST_PING_I_Y_SIZE   80
#define HIST_PING_I_MAXRTT   80
#define HIST_PING_EI_BREAK_SIZE 5
#define HIST_PING_ETXT_Y_OFFSET (HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE+HIST_PING_EI_BREAK_SIZE)
#define HIST_PING_ETXT_Y_SIZE 20
#define HIST_PING_E_Y_OFFSET (HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE+HIST_PING_ETXT_Y_SIZE+HIST_PING_EI_BREAK_SIZE)
#define HIST_PING_E_Y_SIZE   80
#define HIST_PING_E_MAXRTT  400
#define HIST_PING_X_BAR_OFFSET 50
#define HIST_PING_X_BAR_SPACE 2

void refreshPing() {
  char sTmp[10];

  // No need to refresh every time
  if ( !ui.screenInitialized ) {
    tft.fillRect(HIST_PING_X_OFFSET,HIST_PING_ITXT_Y_OFFSET,HIST_PING_X_SIZE,HIST_PING_ITXT_Y_SIZE+HIST_PING_I_Y_SIZE,TFT_BLACK);
    tft.fillRect(HIST_PING_X_OFFSET,HIST_PING_ETXT_Y_OFFSET,HIST_PING_X_SIZE,HIST_PING_ETXT_Y_SIZE+HIST_PING_E_Y_SIZE,TFT_BLACK);
    tft.setFreeFont(FF25);    
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Hotspot ping",HIST_PING_X_OFFSET,HIST_PING_ITXT_Y_OFFSET,GFXFF);
    tft.drawRoundRect(HIST_PING_X_OFFSET,HIST_PING_I_Y_OFFSET,HIST_PING_X_SIZE,HIST_PING_I_Y_SIZE,R_SIZE,TFT_WHITE);
    tft.drawString("External ping",HIST_PING_X_OFFSET,HIST_PING_ETXT_Y_OFFSET,GFXFF);
    tft.drawRoundRect(HIST_PING_X_OFFSET,HIST_PING_E_Y_OFFSET,HIST_PING_X_SIZE,HIST_PING_E_Y_SIZE,R_SIZE,TFT_WHITE);
    ui.screenInitialized = true;
  }

  // clean the previous bars
  int xSz = (HIST_PING_X_SIZE - (HIST_PING_X_OFFSET+HIST_PING_X_BAR_OFFSET + MAXBUFFER*HIST_PING_X_BAR_SPACE)) / MAXBUFFER;
  int xOffset = HIST_PING_X_OFFSET+HIST_PING_X_SIZE-xSz-HIST_PING_X_BAR_SPACE;
  for ( int i = 0 ; i < MAXBUFFER ; i++ ) {
     tft.fillRect(xOffset,HIST_PING_I_Y_OFFSET+1,xSz,HIST_PING_I_Y_SIZE-2,TFT_BLACK);
     tft.fillRect(xOffset,HIST_PING_E_Y_OFFSET+1,xSz,HIST_PING_E_Y_SIZE-2,TFT_BLACK);
     xOffset -= xSz + HIST_PING_X_BAR_SPACE;
  }
  // Redraw lines
  //tft.drawLine(HIST_X_OFFSET+2,HIST_Y_OFFSET+10,HIST_X_SIZE-2,HIST_Y_OFFSET+10,TFT_GRAY);  
  for ( int i = 20 ; i < HIST_PING_I_Y_SIZE ; i+=20 ) {
    tft.drawLine(HIST_PING_X_OFFSET+2,HIST_PING_I_Y_OFFSET+4+i,HIST_PING_X_SIZE-2,HIST_PING_I_Y_OFFSET+4+i,TFT_GRAY20);
  }
  sprintf(sTmp,"%d ms",HIST_PING_I_MAXRTT); 
  tft.setFreeFont(FM9);    
  tft.setTextColor(TFT_GRAY);
  tft.drawString(sTmp,HIST_PING_X_OFFSET+3,HIST_PING_I_Y_OFFSET+3,GFXFF);


  for ( int i = 20 ; i < HIST_PING_E_Y_SIZE ; i+=20 ) {
    tft.drawLine(HIST_PING_X_OFFSET+2,HIST_PING_E_Y_OFFSET+4+i,HIST_PING_X_SIZE-2,HIST_PING_E_Y_OFFSET+4+i,TFT_GRAY20);
  }
  sprintf(sTmp,"%d ms",HIST_PING_E_MAXRTT); 
  tft.setFreeFont(FM9);    
  tft.setTextColor(TFT_GRAY);
  tft.drawString(sTmp,HIST_PING_X_OFFSET+3,HIST_PING_E_Y_OFFSET+3,GFXFF);
 
  xOffset = HIST_PING_X_OFFSET+HIST_PING_X_SIZE-xSz-HIST_PING_X_BAR_SPACE;
  for ( int i = 0 ; i < state.elements ; i++ ) {
     int idx = getIndexInBuffer(state.elements-(i+1));
     if ( idx != MAXBUFFER ) {
          uint32_t irtt = (state.avgIntPing[idx])*8;
          if ( irtt > HIST_PING_I_MAXRTT ) irtt = HIST_PING_I_MAXRTT; 
          uint16_t color = TFT_GREEN;
          if ( irtt > 32 ) color = TFT_RED;
          else if ( irtt > 16 ) color = TFT_ORANGE;
          else if ( irtt > 8 ) color = TFT_DARKGREEN;
          if ( irtt == 0 ) {
            tft.drawLine(xOffset+1,(HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE-4)+3,xOffset+xSz-2,(HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE-4)-3,TFT_RED);
            tft.drawLine(xOffset+1,(HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE-4)-3,xOffset+xSz-2,(HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE-4)+3,TFT_RED);        
          } else {
            irtt = ((HIST_PING_I_Y_SIZE-6)*irtt)/HIST_PING_I_MAXRTT;
            tft.fillRect(xOffset,(HIST_PING_I_Y_OFFSET+HIST_PING_I_Y_SIZE-4)-irtt,xSz,irtt,color);
          }

          uint32_t ertt = (state.avgExtPing[idx])*8;
          if ( ertt > HIST_PING_E_MAXRTT ) ertt = HIST_PING_E_MAXRTT; 
          color = TFT_GREEN;
          if ( ertt > 300 ) color = TFT_RED;
          else if ( ertt > 200 ) color = TFT_ORANGE;
          else if ( ertt > 120 ) color = TFT_DARKGREEN;
          if ( ertt == 0 ) {
            tft.drawLine(xOffset+1,(HIST_PING_E_Y_OFFSET+HIST_PING_E_Y_SIZE-4)+3,xOffset+xSz-2,(HIST_PING_E_Y_OFFSET+HIST_PING_E_Y_SIZE-4)-3,TFT_RED);
            tft.drawLine(xOffset+1,(HIST_PING_E_Y_OFFSET+HIST_PING_E_Y_SIZE-4)-3,xOffset+xSz-2,(HIST_PING_E_Y_OFFSET+HIST_PING_E_Y_SIZE-4)+3,TFT_RED);        
          } else {
            ertt = ((HIST_PING_E_Y_SIZE-6)*ertt)/HIST_PING_E_MAXRTT;
            tft.fillRect(xOffset,(HIST_PING_E_Y_OFFSET+HIST_PING_E_Y_SIZE-4)-ertt,xSz,ertt,color);
          }

     }
     xOffset -= xSz + HIST_PING_X_BAR_SPACE;
  }
}

void refresUI() {

  bool hasAction = false;
  bool forceRefresh = false;
  bool configHasChanged = false;
  
  if (digitalRead(WIO_KEY_C) == LOW) {

    hasAction=true;
  } else if (digitalRead(WIO_KEY_B) == LOW) {
  
    hasAction=true;
  } else if (digitalRead(WIO_KEY_A) == LOW) {

    hasAction=true;
  } else if (digitalRead(WIO_5S_UP) == LOW) {

    hasAction=true;
  } else if (digitalRead(WIO_5S_DOWN) == LOW) {
 
    hasAction=true;
  } else if (digitalRead(WIO_5S_RIGHT) == LOW) {

    hasAction=true;
  } else if (digitalRead(WIO_5S_LEFT) == LOW) {

    hasAction=true;
  } else if (digitalRead(WIO_5S_PRESS) == LOW) {

    hasAction=true;
  }

  // refresh the graph history part
  if ( state.hasRefreshed == true || ui.screenInitialized == false) {
//    ui.lastWrId = state.writePtr;
    refreshPing();
    state.hasRefreshed = false;
  }  

  // avoid re-entering
  if ( hasAction ) delay(300); 
}
