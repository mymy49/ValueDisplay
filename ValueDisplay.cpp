/**
 * @file ValueDisplay.cpp
 * @brief Implementation of ValueDisplay member functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "ValueDisplay.h"
#include <string.h>
#include <stdio.h>

/**
 * @details Initializes a new `ValueDisplay` instance. Sets internal pointers for
 *          the working frame buffer, LCD drawing surface brush, and fonts (value, unit,
 *          title) to `nullptr` to prevent invalid memory dereferences before explicit
 *          configuration. Sets the default background color to dark-gray (RGB: `0x30, 0x30, 0x30`),
 *          and clears the unit and title character buffers with zeroes.
 * @see ValueDisplay::setTitleConfig, ValueDisplay::setValueConfig, ValueDisplay::setUnitConfig
 * @todo none
 * @bug none
 */
ValueDisplay::ValueDisplay()
{
	// Initialize surface and font pointers to nullptr
	mFb = nullptr;
	mValueFont = nullptr;
	mUnitFont = nullptr;
	mLcd = nullptr;

	// Set default dark gray outer background color
	mBgColor.setColor({0x30, 0x30, 0x30});

	// Zero-initialize unit and title string buffers
	memset(mUnit, 0, sizeof(mUnit));
	memset(mTitle, 0, sizeof(mTitle));
}

/**
 * @details Configures all presentation properties for the measurement unit label.
 *          Stores the font reference, boundary area, and text color, and copies up to
 *          31 characters of the unit string into the internal buffer `mUnit`, ensuring safe
 *          null-termination.
 * @see ValueDisplay::drawBackground
 * @todo none
 * @bug none
 */
void ValueDisplay::setUnitConfig(Font &font, Area area, Color fontColor, const char *unit)
{
	int32_t len = strlen(unit);
	// Cache unit font, relative bounding box, and text color
	mUnitFont = &font;
	mUnitArea = area;
	mUnitColor = fontColor;
	// Copy unit label string if it fits within the 32-byte buffer
	if (len < 32)
		strcpy(mUnit, unit);
}

/**
 * @details Configures the display parameters for the numeric value. Stores the
 *          font reference, value area box, relative text offset position, font color,
 *          background color, and the `printf`-compatible format specifier string (e.g. `"%.1f"`,
 *          `"%d"`). Buffer length is safeguarded to a maximum of 63 characters to prevent
 *          buffer overflow.
 * @see ValueDisplay::drawValue, ValueDisplay::drawValueWithCursor
 * @todo none
 * @bug none
 */
void ValueDisplay::setValueConfig(Font &font, Area area, Position valuePos, Color fontColor, Color bgColor, const char *format)
{
	int32_t len = strlen(format);
	// Cache value font, sub-box geometry, text offset, foreground color, and background color
	mValueFont = &font;
	mValueArea = area;
	mValueColor = fontColor;
	mValueBgColor = bgColor;
	mValuePos = valuePos;
	// Copy sprintf format specifier if it fits within the 64-byte buffer
	if (len < 64)
		strcpy(mFormat, format);
}

/**
 * @details Sets up the title label configuration. Stores the font reference,
 *          coordinate offset relative to the widget area, text color, and copies up to 31
 *          characters of the title string into `mTitle`.
 * @see ValueDisplay::drawBackground
 * @todo none
 * @bug none
 */
void ValueDisplay::setTitleConfig(Font &font, Position pos, Color fontColor, const char *title)
{
	int32_t len = strlen(title);
	// Cache title font, relative position offset, and text color
	mTitleFont = &font;
	mTitlePos = pos;
	mTitleColor = fontColor;
	// Copy title string if it fits within the 32-byte buffer
	if (len < 32)
		strcpy(mTitle, title);
}

/**
 * @details Renders the static visual elements of the widget, including the overall
 *          widget background, the optional title text, and the unit label sub-box. The
 *          operation resizes the off-screen frame buffer, clears it with the background
 *          color, draws the title if configured, transfers the composited bitmap to the LCD,
 *          and subsequently draws the unit sub-box if its dimensions are non-zero.
 * @see BaseDisplay::drawBackground, ValueDisplay::setTitleConfig, ValueDisplay::setUnitConfig
 * @todo none
 * @bug none
 */
void ValueDisplay::drawBackground()
{
	char str[64];

	// Abort rendering if off-screen framebuffer or target LCD brush is unconfigured
	if (mFb == nullptr || mLcd == nullptr)
		return;

	// Fill widget background with background color
	mFb->setSize(mArea.getSize());
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();

	// Render title text if font is set
	if (mTitleFont != nullptr)
	{
		mFb->setBrushColor(mTitleColor);
		mFb->setFont(*mTitleFont);
		mFb->drawString(mTitlePos, mTitle);
	}

	// Blit outer background and title onto the target LCD surface
	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());

	// Render unit text if unit area has valid dimensions
	Area area = mArea;
	area += mUnitArea.getPosition();

	if (mUnitArea.getSize().getHeight() * mUnitArea.getSize().getWidth())
	{
		mFb->setSize(mUnitArea.getSize());
		mFb->setBackgroundColor(mValueBgColor);
		mFb->clear();

		if (mUnitFont != nullptr)
		{
			mFb->setFont(*mUnitFont);
			mFb->setBrushColor(mUnitColor);
			mFb->drawString(Brush::ALGIN_LEFT_BOT, mUnit);
		}
		mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
	}
}

/**
 * @details Formats the given floating-point number into a string buffer according
 *          to the configured format string (`mFormat`). Resizes the off-screen frame
 *          buffer to match `mValueArea`, clears it with `mValueBgColor`, renders the formatted
 *          string using `mValueFont`, and blits the resulting bitmap directly onto the target
 *          LCD at the calculated absolute screen coordinates.
 * @see BaseDisplay::drawValue, ValueDisplay::setValueConfig, ValueDisplay::drawValueWithCursor
 * @todo none
 * @bug none
 */
void ValueDisplay::drawValue(float value)
{
	char str[64];

	// Abort rendering if required buffer, font, or LCD pointer is missing
	if (mFb == nullptr || mValueFont == nullptr || mLcd == nullptr)
		return;

	// Format numerical value string using the configured format
	sprintf(str, mFormat, value);

	// Calculate absolute screen coordinates for the value sub-box
	Area area = mArea;
	area += mValueArea.getPosition();

	// Render value text into off-screen buffer and copy to LCD
	mFb->setSize(mValueArea.getSize());
	mFb->setBackgroundColor(mValueBgColor);
	mFb->setFont(*mValueFont);
	mFb->setBrushColor(mValueColor);
	mFb->clear();
	mFb->drawString(mValuePos, str);
	mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
}

/**
 * @details Formats the given floating-point value into a string using `mFormat` and
 *          applies a cursor blinking effect at a specific character position (`index`).
 *          If `on` is `false` and `index` is within the valid length of the formatted string,
 *          the character at that index is replaced with an ASCII space (`' '`), simulating a
 *          blinking cursor. The resulting string is then rendered into the off-screen frame
 *          buffer and transferred to the LCD at the widget's value area position.
 * @see ValueDisplay::drawValue, ValueDisplay::setValueConfig
 * @todo none
 * @bug none
 */
void ValueDisplay::drawValueWithCursor(float value, uint8_t index, bool on)
{
	char str[64];
	uint32_t len;

	// Abort rendering if required buffer, font, or LCD pointer is missing
	if (mFb == nullptr || mValueFont == nullptr || mLcd == nullptr)
		return;

	// Format numerical value into local string buffer
	sprintf(str, mFormat, value);
	len = strlen(str);

	// Hide character at cursor index when cursor is in the "off" phase
	if (!on && len > index)
		str[index] = ' ';

	// Calculate absolute screen coordinates for the value sub-box
	Area area = mArea;
	area += mValueArea.getPosition();

	// Render value text into off-screen buffer and copy to LCD
	mFb->setSize(mValueArea.getSize());
	mFb->setBackgroundColor(mValueBgColor);
	mFb->setFont(*mValueFont);
	mFb->setBrushColor(mValueColor);
	mFb->clear();
	mFb->drawString(mValuePos, str);
	mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
}
