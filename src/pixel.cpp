/*
 * VisualBoyAdvanced - Nintendo Gameboy/GameboyAdvance (TM) emulator
 * Copyrigh(c) 1999-2002 Forgotten (vb@emuhq.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "System.h"

extern int RGB_LOW_BITS_MASK;

void Pixelate(u8 *srcPtr, u32 srcPitch, u8 *deltaPtr,
          u8 *dstPtr, u32 dstPitch, int width, int height)
{
  u8 *nextLine, *finish;
  u32 colorMask = ~(RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 16));
  
  nextLine = dstPtr + dstPitch;
  
  do {
    u32 *bP = (u32 *) srcPtr;
    u32 *xP = (u32 *) deltaPtr;
    u32 *dP = (u32 *) dstPtr;
    u32 *nL = (u32 *) nextLine;
    u32 currentPixel;
    u32 nextPixel;
    u32 currentDelta;
    u32 nextDelta;
    
    finish = (u8 *) bP + ((width + 2) << 1);
    nextPixel = *bP++;
    nextDelta = *xP++;
    
    do {
      currentPixel = nextPixel;
      currentDelta = nextDelta;
      nextPixel = *bP++;
      nextDelta = *xP++;
      
      if ((nextPixel != nextDelta) || (currentPixel != currentDelta)) {
    u32 colorA, colorB, product;
    
    *(xP - 2) = currentPixel;
    colorA = currentPixel & 0xffff;

    colorB = (currentPixel & 0xffff0000) >> 16;
    product = (((colorA & colorMask) >> 1) & colorMask) >> 1;
    *(nL) = product | (product << 16);
    *(dP) = (colorA)|product<<16;

    colorA = nextPixel & 0xffff;
    product = (((colorB & colorMask) >> 1) & colorMask) >> 1;
    *(nL + 1) = product | (product << 16);
    *(dP + 1) = (colorB)| product<<16;
      }
      
      dP += 2;
      nL += 2;
    } while ((u8 *) bP < finish);
    
    deltaPtr += srcPitch;
    srcPtr += srcPitch;
    dstPtr += dstPitch * 2;
    nextLine += dstPitch * 2;
  }
  while (--height);
}
