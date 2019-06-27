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

#include "radio_version.h"
#include "opentx.h"
#include "libopenui.h"

RadioVersionPage::RadioVersionPage():
  PageTab(STR_MENUVERSION, ICON_RADIO_VERSION)
{
}

void RadioVersionPage::build(FormWindow * window)
{
  FormGridLayout grid;
  grid.setLabelWidth(120);
  grid.spacer(PAGE_PADDING);

  getCPUUniqueID(reusableBuffer.version.id);

  new StaticText(window, grid.getLabelSlot(), "FW Version");
  new StaticText(window, grid.getFieldSlot(), vers_stamp);
  grid.nextLine();

  new StaticText(window, grid.getLabelSlot(), "FW Date");
  new StaticText(window, grid.getFieldSlot(), date_stamp);
  grid.nextLine();

  new StaticText(window, grid.getLabelSlot(), "FW Time");
  new StaticText(window, grid.getFieldSlot(), time_stamp);
  grid.nextLine();

  new StaticText(window, grid.getLabelSlot(), "Data version");
  new StaticText(window, grid.getFieldSlot(), eeprom_stamp);
  grid.nextLine();

  new StaticText(window, grid.getLabelSlot(), "CPU UID");
  new StaticText(window, grid.getFieldSlot(), reusableBuffer.version.id);
  grid.nextLine();
  grid.nextLine();

  auto button = new TextButton(window, grid.getCenteredSlot(250), STR_FACTORYRESET,
                               [=]() -> int8_t {
                                   new Dialog(WARNING_TYPE_CONFIRM, STR_CONFIRMRESET, STR_POPUPS_ENTER_EXIT, [=]() {
                                       storageEraseAll(false);
                                       NVIC_SystemReset();
                                       return 0;
                                   });
                                   return 0;
                               });

  window->setFirstField(button);
}
