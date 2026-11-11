/**
 * @file ValueDisplay.cpp
 * @brief Implementation of ValueDisplay member functions.
 */

#include "ValueDisplay.h"
#include <string.h>
#include <stdio.h>

/**
 * @details Initializes a new ValueDisplay instance. It sets internal pointers for
 * the working frame buffer, LCD drawing surface brush, and fonts (value, unit,
 * title) to nullptr to prevent invalid memory dereferences before explicit
 * configuration. It sets the default background color to dark-gray (RGB: 0x30, 0x30,
 * 0x30), and clears the unit and title character buffers with zeroes.
 */
ValueDisplay::ValueDisplay()
{
	mFb = nullptr;
	mValueFont = nullptr;
	mUnitFont = nullptr;
	mLcd = nullptr;

	mBgColor.setColor({0x30, 0x30, 0x30});

	memset(mUnit, 0, sizeof(mUnit));
	memset(mTitle, 0, sizeof(mTitle));
}

/**
 * @details Configures all presentation properties for the measurement unit label.
 * It stores the font reference, boundary area, and text color, and copies up to
 * 31 characters of the unit string into the internal buffer mUnit, ensuring safe
 * null-termination.
 *
 * @param[in] font      Reference to the font to be used when rendering the unit string.
 * @param[in] area      Display boundary area allocating screen space for the unit label.
 * @param[in] fontColor Text brush color used to draw the unit characters.
 * @param[in] unit      Null-terminated C-string representing the measurement unit.
 */
void ValueDisplay::setUnitConfig(Font &font, Area area, Color fontColor, const char *unit)
{
	int32_t len = strlen(unit);
	mUnitFont = &font;
	mUnitArea = area;
	mUnitColor = fontColor;
	if (len < 32)
		strcpy(mUnit, unit);
}

/**
 * @details Configures the display parameters for the numeric value. It stores the
 * font reference, value area box, relative text offset position, font color,
 * background color, and the printf-compatible format specifier string (e.g. "%.1f",
 * "%d"). Buffer length is safeguarded to a maximum of 63 characters to prevent
 * buffer overflow.
 *
 * @param[in] font      Reference to the font used for rendering the numeric value.
 * @param[in] area      Bounding area defining the size and offset of the value sub-box.
 * @param[in] valuePos  Internal text coordinate offset within the value sub-box.
 * @param[in] fontColor Color of the value text characters.
 * @param[in] bgColor   Background fill color behind the value text.
 * @param[in] format    Format specification string (e.g. "%.2f") used by sprintf.
 */
void ValueDisplay::setValueConfig(Font &font, Area area, Position valuePos, Color fontColor, Color bgColor, const char *format)
{
	int32_t len = strlen(format);
	mValueFont = &font;
	mValueArea = area;
	mValueColor = fontColor;
	mValueBgColor = bgColor;
	mValuePos = valuePos;
	if (len < 64)
		strcpy(mFormat, format);
}

/**
 * @details Sets up the title label configuration. It stores the font reference,
 * coordinate offset relative to the widget area, text color, and copies up to 31
 * characters of the title string into mTitle.
 *
 * @param[in] font      Reference to the font used for the title label.
 * @param[in] pos       Offset position of the title text relative to the widget top-left corner.
 * @param[in] fontColor Color of the title text characters.
 * @param[in] title     Null-terminated string for the widget title.
 */
void ValueDisplay::setTitleConfig(Font &font, Position pos, Color fontColor, const char *title)
{
	int32_t len = strlen(title);
	mTitleFont = &font;
	mTitlePos = pos;
	mTitleColor = fontColor;
	if (len < 32)
		strcpy(mTitle, title);
}

/**
 * @details Renders the static visual elements of the widget, including the overall
 * widget background, the optional title text, and the unit label sub-box. The
 * operation resizes the off-screen frame buffer, clears it with the background
 * color, draws the title if configured, transfers the composited bitmap to the LCD,
 * and subsequently draws the unit sub-box if its dimensions are non-zero.
 */
void ValueDisplay::drawBackground()
{
	char str[64];

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
 * to the configured format string (mFormat). Then it resizes the off-screen frame
 * buffer to match mValueArea, clears it with mValueBgColor, renders the formatted
 * string using mValueFont, and blits the resulting bitmap directly onto the target
 * LCD at the calculated absolute screen coordinates.
 *
 * @param[in] value The floating-point numerical value to format and render.
 */
void ValueDisplay::drawValue(float value)
{
	char str[64];

	if (mFb == nullptr || mValueFont == nullptr || mLcd == nullptr)
		return;

	// Format numerical value string using the configured format
	sprintf(str, mFormat, value);

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
 * @details Formats the given floating-point value into a string using mFormat and
 * applies a cursor blinking effect at a specific character position (`index`).
 *
 * If `on` is false and `index` is within the valid length of the formatted string,
 * the character at that index is replaced with an ASCII space (' '), simulating a
 * blinking cursor. The resulting string is then rendered into the off-screen frame
 * buffer and transferred to the LCD at the widget's value area position.
 *
 * @param[in] value The floating-point numerical value to format.
 * @param[in] index Character position in the formatted string (0-based) where the cursor blinks.
 * @param[in] on    Blink phase state: true to render the character normally, false to hide it (blank).
 */
void ValueDisplay::drawValueWithCursor(float value, uint8_t index, bool on)
{
	char str[64];
	uint32_t len;

	if (mFb == nullptr || mValueFont == nullptr || mLcd == nullptr)
		return;

	sprintf(str, mFormat, value);
	len = strlen(str);

	// Hide character at cursor index when cursor is in the "off" phase
	if (!on && len > index)
		str[index] = ' ';

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
