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
#include "config.h"
#include "helium_logo.h"
#include "ui.h"

void draw_splash_helium(int xOffset, int yOffset, int density) {

  const char * logo = helium_data;
  for ( int y = yOffset ; y < yOffset+helium_height ; y++ ) {
    for ( int x  = xOffset ; x < xOffset+helium_width ; x++) {
        uint8_t pixel[3];
        HELIUM_PIXEL(logo,pixel);
        uint16_t r = ((pixel[0] >> 3) * density) / 100;
        uint16_t g = ((pixel[1] >> 2) * density) / 100;
        uint16_t b = ((pixel[2] >> 3) * density) / 100;
        uint16_t color = ( ( r << 11 ) & 0xF800 ) | ( ( g << 5 ) & 0x07E0 ) | ( b & 0x001F ); 
        if ( pixel[2] > 2*pixel[0] ) { 
          tft.drawPixel(x,y,color);
        }
    }
  }
  
}
