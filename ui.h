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
#include "TFT_eSPI.h"

#ifndef __UI_H
#define __UI_H


#ifdef WITH_SPLASH_HELIUM
  #define HELIUM_XCENTER (320-100)/2
#endif

typedef struct s_ui {
  boolean screenInitialized;  // true when already printed

} ui_t;

extern ui_t ui;
extern TFT_eSPI tft;

void initScreen();
void displaySplash();
void displayTitle();
void clearScreen();

void displaySetup();
void draw_splash_helium(int xOffset, int yOffset, int density);
void refreshUI();
void connectingWifi();

#endif
