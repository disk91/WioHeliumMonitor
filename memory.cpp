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
#include "FreeRTOS.h"
#include "portable.h" 
#include "config.h"
#include "memory.h"

extern "C" char* sbrk(int incr);
char * baseStack;
char * baseHeap;

uint32_t usedStack() {
  char top;
  uint32_t r;
  
  if ( &top > baseStack ) {
     r = &top - baseStack;
  } else {
     r = 0;
  }
  
  LOGF(("Stack size: %d\r\n",r));
  return r;
}

uint32_t usedHeap() {
  uint32_t r = (char*)(sbrk(0)) - baseHeap;
  LOGF(("Arduino heap size: %d\r\n",r));

  size_t m = xPortGetFreeHeapSize();
  LOGF(("freeRTOS heap size: %d\r\n",m));

  return r+m;
}

uint32_t usedMemory() {
  return usedStack() + usedHeap();
}


void memoryTraceInit() {
  char stk;
  baseStack = &stk;
  baseStack -= 8; // overhaed related to this function call
  baseHeap = (char*)(sbrk(0));
}

uint32_t getSamd51Uid() {
  uint32_t pdwUniqueID[4];
  pdwUniqueID[0] = SERIAL_NUMBER_WORD_0;
  pdwUniqueID[1] = SERIAL_NUMBER_WORD_1;
  pdwUniqueID[2] = SERIAL_NUMBER_WORD_2;
  pdwUniqueID[3] = SERIAL_NUMBER_WORD_3;

  LOGF(("SN : %08X %08X %08X %08X\r\n",pdwUniqueID[0], pdwUniqueID[1], pdwUniqueID[2], pdwUniqueID[3]));
  uint32_t r = 0;
  for ( int i = 0 ; i < 4 ; i++) {
    r += pdwUniqueID[i];
  }
  LOGF(("SN : 0x%08X\r\n",r));
  return r;
}
