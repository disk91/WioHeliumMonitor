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
 #ifndef __MEMORY_H__
 #define __MEMORY_H__

#define SERIAL_NUMBER_WORD_0 *(volatile uint32_t *)(0x008061FC)
#define SERIAL_NUMBER_WORD_1 *(volatile uint32_t *)(0x00806010)
#define SERIAL_NUMBER_WORD_2 *(volatile uint32_t *)(0x00806014)
#define SERIAL_NUMBER_WORD_3 *(volatile uint32_t *)(0x00806018)

void memoryTraceInit();
uint32_t usedHeap();
uint32_t usedStack();
uint32_t usedMemory();
uint32_t getSamd51Uid();

 #endif
