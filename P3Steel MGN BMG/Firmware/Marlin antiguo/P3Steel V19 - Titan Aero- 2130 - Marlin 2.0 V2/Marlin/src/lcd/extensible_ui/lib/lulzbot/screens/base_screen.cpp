/*******************
 * base_screen.cpp *
 *******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void BaseScreen::onEntry() {
  CommandProcessor cmd;
  cmd.set_button_style_callback(buttonStyleCallback);
  UIScreen::onEntry();
}

bool BaseScreen::buttonStyleCallback(CommandProcessor &cmd, uint8_t tag, uint8_t &style, uint16_t &options, bool post) {
  if (post) {
    cmd.colors(normal_btn);
    return false;
  }

  #ifdef LCD_TIMEOUT_TO_STATUS
    if (EventLoop::get_pressed_tag() != 0) {
      reset_menu_timeout();
    }
  #endif

  if (tag != 0 && EventLoop::get_pressed_tag() == tag) {
    options = OPT_FLAT;
  }

  if (style & cmd.STYLE_DISABLED) {
    cmd.tag(0);
    style &= ~cmd.STYLE_DISABLED;
    cmd.colors(disabled_btn);
    return true; // Call me again to reset the colors
  }
  return false;
}

void BaseScreen::onIdle() {
  #ifdef LCD_TIMEOUT_TO_STATUS
    const uint32_t elapsed = millis() - last_interaction;
    if (elapsed > uint32_t(LCD_TIMEOUT_TO_STATUS) * 1000) {
      reset_menu_timeout();
      GOTO_SCREEN(StatusScreen);
    }
  #endif
}

void BaseScreen::reset_menu_timeout() {
  #ifdef LCD_TIMEOUT_TO_STATUS
    last_interaction = millis();
  #endif
}

#ifdef LCD_TIMEOUT_TO_STATUS
  uint32_t BaseScreen::last_interaction;
#endif

#endif // LULZBOT_TOUCH_UI
