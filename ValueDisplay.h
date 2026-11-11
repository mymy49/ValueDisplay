/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ValueDisplay.h
 * @brief Widget for displaying numerical values with optional title, unit text, and cursor blinking support.
 */

#ifndef VALUE_DISPLAY__H_
#define VALUE_DISPLAY__H_

#include "BaseDisplay.h"

/**
 * @class ValueDisplay
 * @brief Display widget capable of rendering a formatted numeric value, unit label, title, and interactive cursor blinking.
 *
 * @details
 * `ValueDisplay` extends `BaseDisplay` to provide a complete, modular gauge widget suitable for
 * instruments, sensor readouts, dashboards, and numeric input editing dialogs.
 *
 * ### Key Features
 * - **Title Label**: Displays an identifier string (e.g. "SPEED", "VOLTAGE") at an arbitrary position offset.
 * - **Formatted Value**: Formats floating-point numbers using standard `sprintf` format specifiers
 *   (e.g. `"%3.1f"`, `"%02.0f"`), rendered inside its own sub-box area.
 * - **Measurement Unit**: Displays the measurement unit suffix (e.g. `"km/h"`, `"V"`, `"°C"`) aligned
 *   at the bottom-left of the configured unit area.
 * - **Cursor Blinking Support**: `drawValueWithCursor()` enables cursor editing visualization by
 *   periodically replacing a character at a specified index with a space character when the cursor is off.
 * - **Selective Refreshing**: `drawValue()` redraws only the numerical value sub-box, avoiding costly
 *   full-widget repaints on every value update.
 *
 * ### Example Usage
 * @code
 * #include <ValueDisplay/ValueDisplay.h>
 * #include <font/Ubuntu_14_B.h>
 * #include <font/Ubuntu_20_B.h>
 *
 * ValueDisplay speedGauge;
 *
 * void setupSpeedGauge(void)
 * {
 *     // 1. Associate working framebuffer, target LCD surface, and bounding geometry
 *     speedGauge.setFrameBuffer(brush);
 *     speedGauge.setLcd(lcd);
 *     speedGauge.setArea({10, 20, 130, 60});
 *     speedGauge.setBgColor(0x20, 0x20, 0x20);
 *
 *     // 2. Configure title label ("SPEED" at top-left)
 *     speedGauge.setTitleConfig(Font_Ubuntu_14_B, {5, 5}, Color{0xAA, 0xAA, 0xAA}, "SPEED");
 *
 *     // 3. Configure formatted value sub-box (white text on dark background)
 *     speedGauge.setValueConfig(Font_Ubuntu_20_B, {5, 25, 80, 30}, {2, 5},
 *                               Color{0xFF, 0xFF, 0xFF}, Color{0x10, 0x10, 0x10}, "%3.0f");
 *
 *     // 4. Configure measurement unit label ("km/h")
 *     speedGauge.setUnitConfig(Font_Ubuntu_14_B, {90, 25, 35, 30}, Color{0x80, 0xFF, 0x80}, "km/h");
 *
 *     // 5. Draw static background, title, and unit box
 *     speedGauge.drawBackground();
 * }
 *
 * void updateSpeed(float currentSpeed)
 * {
 *     // 6. Dynamically update the displayed number
 *     speedGauge.drawValue(currentSpeed);
 * }
 *
 * void updateEditingValue(float targetValue, uint8_t cursorIndex, bool cursorBlinkState)
 * {
 *     // 7. Update value with blinking cursor indicator during numeric entry
 *     speedGauge.drawValueWithCursor(targetValue, cursorIndex, cursorBlinkState);
 * }
 * @endcode
 */
class ValueDisplay : public BaseDisplay
{
protected :
	Area mUnitArea;           ///< Display bounding area for the unit label
	Area mValueArea;          ///< Display bounding area for the formatted value text
	Position mTitlePos;       ///< Relative text coordinates for the title
	Position mValuePos;       ///< Relative text coordinates for the formatted value
	Color mUnitColor;         ///< Font color for the unit label
	Color mValueColor;        ///< Font color for the formatted value
	Color mTitleColor;        ///< Font color for the title
	Color mValueBgColor;      ///< Background fill color for the value and unit areas
	Font *mUnitFont;          ///< Pointer to font used for the unit text
	Font *mValueFont;         ///< Pointer to font used for the value text
	Font *mTitleFont;         ///< Pointer to font used for the title text
	char mUnit[32];           ///< String buffer holding the unit label
	char mFormat[64];         ///< String buffer holding the printf-compatible formatting string
	char mTitle[32];          ///< String buffer holding the title text

public :
	/**
	 * @brief Constructs a new ValueDisplay object with default values and styles.
	 */
	ValueDisplay();

	/**
	 * @brief Formats and renders the given floating-point value onto the LCD.
	 * @param[in] value The floating-point value to be displayed.
	 */
	void drawValue(float value) override;

	/**
	 * @brief Formats and renders the numerical value with an optional cursor blink effect at a specified character index.
	 * @param[in] value The floating-point value to display.
	 * @param[in] index 0-based character index within the formatted string where the cursor is placed.
	 * @param[in] on    True to display the character normally; false to replace it with a space for blink effect.
	 */
	void drawValueWithCursor(float value, uint8_t index, bool on);

	/**
	 * @brief Renders the static background, title label, and unit string onto the LCD.
	 */
	void drawBackground() override;

	/**
	 * @brief Configures font, area, color, and text string for the unit label.
	 * @param[in] font      Reference to the font to use for the unit.
	 * @param[in] area      Display boundary area for the unit.
	 * @param[in] fontColor Text color for the unit.
	 * @param[in] unit      Null-terminated string representing the unit (e.g. "km/h", "°C").
	 */
	void setUnitConfig(Font &font, Area area, Color fontColor, const char *unit);

	/**
	 * @brief Configures font, area, position, colors, and format string for the numeric value.
	 * @param[in] font      Reference to the font to use for the value.
	 * @param[in] area      Display boundary area for the value box.
	 * @param[in] valuePos  Text drawing offset inside the value box.
	 * @param[in] fontColor Text color for the numeric value.
	 * @param[in] bgColor   Background fill color behind the value.
	 * @param[in] format    Format string for sprintf (e.g. "%.1f", "%03d").
	 */
	void setValueConfig(Font &font, Area area, Position valuePos, Color fontColor, Color bgColor, const char *format);

	/**
	 * @brief Configures font, position, color, and text string for the title.
	 * @param[in] font      Reference to the font to use for the title.
	 * @param[in] pos       Text coordinate offset relative to the widget area.
	 * @param[in] fontColor Text color for the title.
	 * @param[in] title     Null-terminated string for the widget title.
	 */
	void setTitleConfig(Font &font, Position pos, Color fontColor, const char *title);
};

#endif
