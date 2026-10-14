#pragma once

#include "config.h"
#include "raygui.h"
#include "ui/colors.h"

Font loadGuiFont() {
  Font font = LoadFontEx("assets/Bai_Jamjuree/BaiJamjuree-Regular.ttf", BIG_TEXT_HEIGHT, 0, 0);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
  return font;
}

void initializeGuiStyle(const Font font) {
  GuiSetFont(font);

  GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
  GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, TEXT_LINE_SPACING_HEIGHT);

  GuiSetIconScale(BUTTON_ICON_SCALE);

  GuiSetStyle(DEFAULT, BORDER_WIDTH, BORDER_THICKNESS);
  GuiSetStyle(TOGGLE, GROUP_PADDING, BUTTON_SPACING_Y);

  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(COLOR_GRAY_096));
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(COLOR_GRAY_064));
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(COLOR_GRAY_160));

  GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_160));
  GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_128));
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(COLOR_LITE));

  GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(COLOR_LITE));
  GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(COLOR_LITE));
  GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(COLOR_GRAY_064));

  GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, ColorToInt(COLOR_GRAY_064));
  GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(COLOR_GRAY_048));
  GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, ColorToInt(COLOR_GRAY_096));

  GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
  GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
}
