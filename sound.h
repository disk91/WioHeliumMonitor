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

#ifndef __SOUND_H__
#define __SOUND_H__

void setupSound();
void playSound();


static unsigned int sOnWidth = 30;
static unsigned int sOnHeight = 24;

#define SOUND_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

static char *soundOn_data =
  "^P<W^P<W^04U^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^P<W"
  "^P<W^04U^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^P<W^P<W"
  "^@8V^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X\\/TML;WNQM,#^P<W^@8V"
  "^@8V^`@X^P<W^@8V^`@X_`P\\`@X^`````````````@X^_PL[^@8V^@8V_@HZ^`@X"
  "]`0T^@8V^`@X^P<W^@8V^`@X_0DY^@8VW.D98V^@!!!!!!!!YO,C_@HZ^`@X]`0T"
  "^@8V_PL[]`0T`P\\_````````````````````````^@8V^@8V^`@X^P<W^@8V^@8V"
  "^P<W^P<W^@8V^P<W^`@XQM,#,CYO!!!!!!!!!!!!XN\\?^`@X^P<W^@8V_`P\\]P,S"
  "B97&OLK[````````````````````````^P<W^P<W^@8V^@8V^`@X^P<W^@8V^P<W"
  "^P<W]P,SI[/D!!!!!!!!!!!!!!!!!!!!X^`@^@8V^@8V_PL[````_`P\\H*S=<7VN"
  "W^P<````````````````````^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W[?HJAI+#"
  "!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X^`@X\\_`PML+S`@X^]`0T?XN\\B97&_`P\\"
  "````````````````^@8V^@8V]@(RWNL;UN,3T]`0T-T-QM,#7&B9!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!X.T=_@HZ`0T]P<W^<7VNQ]0$````S=H*<GZOU^04````````"
  "````````^P<WZ/4E15&\"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!X^`@^@8V`````0T]E*#1>(2U^04U_0DY>86VHJ[?````````````````"
  "^P<WJ;7F!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "Y/$A^`@X````````W.D9<GZOSML+````J[?H<W^P_@HZ````````````^@8VJK;G"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!X>X>_`P\\"
  "````````_PL[=8&RJ+3E````R=8&<GZO[/DI````````````^@8VJ;7F!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!XN\\?_PL[````````"
  "````D9W.DI[/````V>86<GZOWNL;````````````^`@XI+#A!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Y?(B_0DY````````````EZ/4"
  "C9G*````V>86<7VNW.D9````````````^P<WIK+C!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!X^`@_0DY`````````P\\_?8FZGJK;````"
  "T-T-<7VNZ/4E````````````^@8VJ;7F!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!XN\\?_@HZ````````ZO<G<7VNP<W^````ML+S<7VN"
  "]`0T````````````^P<WIK+C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!X^`@_0DY````````KKKK<7VN[/DI`P\\_A9'\"C9G*````````"
  "````````^`@XS-D)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!Y?(B^P<W````V^@8<'RMJ;7F````X.T=<'RMQM,#````````````````"
  "^@8V^@8VS=H*H*S=E*#1CIK+C)C)@HZ_!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "XN\\?_0DY````VN<7A9'\"]`0T`@X^E:'2=8&R]`0T````````````````^@8V^@8V"
  "^`@X^P<W^@8V^@8V^P<W^P<WR=8&.45V!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X"
  "````````_@HZ````Q-$!<'RMQ=(\"````````````````````^`@X^P<W^04U^@8V"
  "_0DY^`@X^@8V^P<W^`@XWNL;8&R=!!!!!!!!!!!!!!!!!!!!Y/$A^04U`0T]````"
  "````\\O\\O<7VNF*35_`P\\````````````````````^P<W^P<W^@8V^@8V^`@X^P<W"
  "^@8V^P<W^P<W^@8V[OLKBI;'!!!!!!!!!!!!!!!!X^`@^@8V_0DY````````````"
  "WNL;]`0T````````````````````````^@8V^@8V_@HZ^`@X^04U^@8V^`@X^P<W"
  "^@8V^`@X_0DY]`0TL+SM!!!!!!!!!!!!X>X>_@HZ^`@X_`P\\````````````````"
  "````````````````````````^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W"
  "^`@X^@8V^@8VT=X.0T]`C9G*]`0T^`@X^P<W^P<W`P\\_````````````````````"
  "````````````````^P<W^P<W^@8V^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W"
  "^`@X^@8V^@8V^P<W^P<W^@8V^@8V^`@X_0DY````````````````````````````"
  "````````^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X^@8V^@8V^P<W"
  "^P<W^@8V^@8V^`@X^P<W^@8V^@8V^`@X`P\\_````````````````````````````"
  "";

static unsigned int sOffWidth = 30;
static unsigned int sOffHeight = 24;

static char *soundOff_data =
  "^P<W^P<W^04U^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^P<W"
  "^P<W^04U^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^P<W^P<W"
  "^@8V^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X\\/TML;WNQM,#^P<W^@8V"
  "^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^@8V^@8V_@HZ^`@X"
  "]`0T^@8V^`@X^P<W^@8V^`@X_0DY^@8VW.D98V^@!!!!!!!!YO,C_@HZ^`@X]`0T"
  "^@8V^`@X^P<W^@8V^`@X_0DY^@8V]`0T^`@X^`@X^@8V^@8V^`@X^P<W^@8V^@8V"
  "^P<W^P<W^@8V^P<W^`@XQM,#,CYO!!!!!!!!!!!!XN\\?^`@X^P<W^@8V^@8V^P<W"
  "^P<W^@8V^P<W^`@X^@8V^@8V^P<W^P<W^P<W^P<W^@8V^@8V^`@X^P<W^@8V^P<W"
  "^P<W]P,SI[/D!!!!!!!!!!!!!!!!!!!!X^`@^@8V^@8V^`@X^P<W^@8V^P<W^P<W"
  "^@8V^@8V^P<W^`@X^@8V^@8V^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W[?HJAI+#"
  "!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X"
  "^@8V^@8V^P<W^P<W^@8V^@8V]@(RWNL;UN,3T]`0T-T-QM,#7&B9!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!X.T=_@HZ^`@X]`0T^04U^`@X^P<W^@8V^`@X_0DY^@8V]P,S"
  "^`@X^`@X^P<WZ/4E15&\"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!X^`@^@8V^@8VLK[O)#!AQM,#^P<W^P<W^@8V^@8VOLK[!!!!P<W^^@8V"
  "^P<WJ;7F!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "Y/$A^04U^@8VGJK;!!!!!1%\"QM,#^P<W^@8VM\\/T!!!!!!!!N<7V^@8V^@8VJK;G"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!X>X>_0DY"
  "^P<W]0$QA9'\"!!!!!A)#QM,#NL;W!!!!!!!!C)C)^04U^`@X^@8VJ;7F!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X^P<W^@8V"
  "]0$Q@X^`!!!!!Q-$!!!!!!!!B97&]@(R^P<W^P<W^`@XI+#A!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Y?(B]`0T^@8V_0DY^`@X\\_`P"
  ":'2E!!!!!!!!<7VN]P,S_@HZ^@8V^@8V^P<WIK+C!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!X^`@^@8V^@8V^`@X^P<WR-4%!!!!!!!!"
  "!!!!(\"Q=T-T-^`@X^@8V^@8V^@8VJ;7F!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X^P<W^@8VR=8&!!!!!!!!AY/$:G:G!!!!"
  "*C9GUN,3^P<W^P<W^P<WIK+C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!!!!!!!!!X^`@^@8V^@8VT=X.!!!!!!!!B)3%]P,S[OLK;'BI!!!!.45V"
  "W>H:^@8V^`@XS-D)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "!!!!!!!!Y?(B]`0T^@8VIK+C!!!!A9'\"]P,S^`@X^@8VZ_@H<'RM!!!!L[_P^@8V"
  "^@8V^@8VS=H*H*S=E*#1CIK+C)C)@HZ_!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
  "XN\\?^`@X^P<W[_PLR=8&]P,S^P<W^@8V^P<W^`@X\\O\\ORM<']@(R^P<W^@8V^@8V"
  "^`@X^P<W^@8V^@8V^P<W^P<WR=8&.45V!!!!!!!!!!!!!!!!!!!!!!!!XN\\?^`@X"
  "^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X^@8V^@8V^P<W^P<W^`@X^P<W^04U^@8V"
  "_0DY^`@X^@8V^P<W^`@XWNL;8&R=!!!!!!!!!!!!!!!!!!!!Y/$A^04U^@8V_0DY"
  "^`@X^@8V^P<W^`@X^@8V^04U^P<W_0DY^@8V^@8V^P<W^P<W^@8V^@8V^`@X^P<W"
  "^@8V^P<W^P<W^@8V[OLKBI;'!!!!!!!!!!!!!!!!X^`@^@8V^@8V^`@X^P<W^@8V"
  "^P<W^P<W^@8V^@8V^P<W^`@X^@8V^@8V^@8V^@8V_@HZ^`@X^04U^@8V^`@X^P<W"
  "^@8V^`@X_0DY]`0TL+SM!!!!!!!!!!!!X>X>_@HZ^`@X^04U^@8V^`@X^P<W^@8V"
  "^`@X_0DY^@8V]`0T^`@X^`@X^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W"
  "^`@X^@8V^@8VT=X.0T]`C9G*]`0T^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X"
  "^@8V^@8V^P<W^P<W^P<W^P<W^@8V^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W"
  "^`@X^@8V^@8V^P<W^P<W^@8V^@8V^`@X^P<W^@8V^P<W^P<W^@8V^@8V^P<W^`@X"
  "^@8V^@8V^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X^@8V^@8V^P<W"
  "^P<W^@8V^@8V^`@X^P<W^@8V^@8V^P<W^P<W^@8V^P<W^`@X^@8V^@8V^P<W^P<W"
  "";

#endif
