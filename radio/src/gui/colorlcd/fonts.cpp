/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x 
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "rle.h"

const uint16_t font_tinsize_specs[] = {
#include "font_9.specs"
};

const unsigned char font_tinsize[] = {
#include "font_9.lbm"
};

#if defined(HARDWARE_TOUCH)
const uint16_t font_smlsize_specs[] = {
#include "font_15.specs"
};

const unsigned char font_smlsize[] = {
#include "font_15.lbm"
};

const uint16_t font_stdsize_specs[] = {
#include "font_17.specs"
};

const unsigned char font_stdsize[] = {
#include "font_17.lbm"
};

const uint16_t font_stdsizebold_specs[] = {
#include "font_bold17.specs"
};

const unsigned char font_stdsizebold[] = {
#include "font_bold17.lbm"
};
#else
const uint16_t font_smlsize_specs[] = {
#include "font_13.specs"
};

const unsigned char font_smlsize[] = {
#include "font_13.lbm"
};

const uint16_t font_stdsize_specs[] = {
#include "font_16.specs"
};

const unsigned char font_stdsize[] = {
#include "font_16.lbm"
};

const uint16_t font_stdsizebold_specs[] = {
#include "font_bold16.specs"
};

const unsigned char font_stdsizebold[] = {
#include "font_bold16.lbm"
};
#endif

const uint16_t font_midsize_specs[] = {
#include "font_24.specs"
};

const unsigned char font_midsize[] = {
#include "font_24.lbm"
};

const uint16_t font_dblsize_specs[] = {
#include "font_32.specs"
};

const unsigned char font_dblsize[] = {
#include "font_32.lbm"
};

const uint16_t font_xxlsize_specs[] = {
#include "font_64.specs"
};

const unsigned char font_xxlsize[] = {
#include "font_64.lbm"
};

#if !defined(BOOT)
const uint16_t * const fontspecsTable[16] = {
  font_stdsize_specs,     font_tinsize_specs, font_smlsize_specs, font_midsize_specs, font_dblsize_specs, font_xxlsize_specs, font_stdsize_specs, font_stdsize_specs,
  font_stdsizebold_specs, font_tinsize_specs, font_smlsize_specs, font_midsize_specs, font_dblsize_specs, font_xxlsize_specs, font_stdsize_specs, font_stdsize_specs
};

const uint8_t * fontsTable[16] = {
  font_stdsize,     font_tinsize, font_smlsize, font_midsize, font_dblsize, font_xxlsize, font_stdsize, font_stdsize,
  font_stdsizebold, font_tinsize, font_smlsize, font_midsize, font_dblsize, font_xxlsize, font_stdsize, font_stdsize
};
#else
const uint16_t * const fontspecsTable[1] = { font_stdsize_specs };
const uint8_t * fontsTable[1] = { font_stdsize };
#endif

uint8_t * decompressFont(const uint8_t * font)
{
  uint16_t width = *((uint16_t *)font);
  uint16_t height = *(((uint16_t *)font)+1);

  size_t font_size = width * height;
  uint8_t* dec_buf = (uint8_t*)malloc(font_size + 4);

  // copy width / height
  memcpy(dec_buf,font,4);

  rle_decode_8bit(dec_buf+4, font_size, font+4);
  return dec_buf;
}

void loadFonts()
{
  static bool fonts_loaded = false;
  if (fonts_loaded) return;

#if !defined(BOOT)
  int i=0;
  for (; i<9; i++)
    fontsTable[i] = decompressFont(fontsTable[i]);

  for (; i<16; i++)
    fontsTable[i] = fontsTable[i-9];
#else
  fontsTable[0] = decompressFont(fontsTable[0]);
#endif

  fonts_loaded = true;
}
