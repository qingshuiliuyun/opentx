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

#include "filechoice.h"
#include "menu.h"
#include "draw_functions.h"
#include "sdcard.h"

const uint8_t LBM_FOLDER[] = {
#include "mask_folder.lbm"
};

// comparison, not case sensitive.
bool compare_nocase(const std::string &first, const std::string &second)
{
  return strcasecmp(first.c_str(), second.c_str()) < 0;
}

FileChoice::FileChoice(Window * parent, const rect_t & rect, std::string folder, const char * extension, int maxlen, std::function<std::string()> getValue, std::function<void(std::string)> setValue):
  FormField(parent, rect),
  folder(std::move(folder)),
  extension(extension),
  maxlen(maxlen),
  getValue(std::move(getValue)),
  setValue(std::move(setValue))
{
}

void FileChoice::paint(BitmapBuffer * dc)
{
  FormField::paint(dc);

  const char * displayedValue = getValue().c_str();

  LcdFlags textColor;
  if (editMode)
    textColor = TEXT_INVERTED_COLOR;
  else if (hasFocus())
    textColor = TEXT_INVERTED_BGCOLOR;
  else if (displayedValue[0] == '\0')
    textColor = CURVE_AXIS_COLOR;
  else
    textColor = TEXT_COLOR;

  dc->drawText(FIELD_PADDING_LEFT, FIELD_PADDING_TOP, displayedValue[0] == '\0' ? "---" : displayedValue, textColor);
  dc->drawBitmapPattern(rect.w - 20, (rect.h - 11) / 2, LBM_FOLDER, textColor);
}

void FileChoice::openMenu()
{
  FILINFO fno;
  DIR dir;
  std::list<std::string> files;
  const char * fnExt;
  uint8_t fnLen, extLen;

  FRESULT res = f_opendir(&dir, folder.c_str()); // Open the directory
  if (res == FR_OK) {
    bool firstTime = true;
    for (;;) {
      res = sdReadDir(&dir, &fno, firstTime);
      if (res != FR_OK || fno.fname[0] == 0)
        break; // break on error or end of dir
      if (fno.fattrib & AM_DIR)
        continue; // skip subfolders
      if (fno.fattrib & AM_HID)
        continue; // skip hidden files
      if (fno.fattrib & AM_SYS)
        continue; // skip system files

      fnExt = getFileExtension(fno.fname, 0, 0, &fnLen, &extLen);
      fnLen -= extLen;

      if (!fnLen || fnLen > maxlen)
        continue; // wrong size
      if (extension && !isExtensionMatching(fnExt, extension))
        continue; // wrong extension

      files.emplace_back(fno.fname, fnLen);
    }

    if (files.size() > 0) {
      // sort files
      files.sort(compare_nocase);

      files.push_front("");

      auto menu = new Menu();
      int count = 0;
      int current = -1;
      std::string value = getValue();
      for (const auto &file: files) {
        menu->addLine(file, [=]() {
            setValue(file);
        });
        // TRACE("%s %d %s %d", value.c_str(), value.size(), file.c_str(), file.size());
        if (value.compare(file) == 0) {
          // TRACE("OK");
          current = count;
        }
        ++count;
      }

      if (current >= 0) {
        menu->select(current);
      }

      menu->setCloseHandler([=]() {
          editMode = false;
          setFocus();
      });
    }
    else {
      TRACE("NO FILES !!!!!");
      // TODO popup "NO FILES"
    }
  }
}

#if defined(HARDWARE_KEYS)
void FileChoice::onKeyEvent(event_t event)
{
  TRACE_WINDOWS("%s received event 0x%X", getWindowDebugString().c_str(), event);

  if (event == EVT_KEY_BREAK(KEY_ENTER)) {
    editMode = true;
    invalidate();
    openMenu();
  }
  else {
    FormField::onKeyEvent(event);
  }
}
#endif

#if defined(HARDWARE_TOUCH)
bool FileChoice::onTouchEnd(coord_t, coord_t)
{
  openMenu();
  setFocus();
  return true;
}
#endif
