/**
 * @file ProgressDisplay.cpp
 * @brief Implementation of ProgressDisplay member functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "ProgressDisplay.h"

/**
 * @details Initializes a `ProgressDisplay` instance with default colors:
 *          - Outer widget background: dark-gray (RGB: `0x30, 0x30, 0x30`)
 *          - Active progress indicator: bright green (RGB: `0x30, 0xFF, 0x30`)
 *          - Track/groove background: medium-gray (RGB: `0x80, 0x80, 0x80`)
 *          Internal pointers to the off-screen frame buffer and display surface brush
 *          are initialized to `nullptr`.
 * @see ProgressDisplay::setColor, ProgressDisplay::drawBackground
 * @todo none
 * @bug none
 */
ProgressDisplay::ProgressDisplay(void)
{
	// Configure default outer bezel, active progress bar, and track groove colors
	mBgColor.setColor(0x30, 0x30, 0x30);
	mProgressColor.setColor(0x30, 0xFF, 0x30);
	mProgressBgColor.setColor(0x80, 0x80, 0x80);

	// Initialize surface pointers to nullptr
	mFb = nullptr;
	mLcd = nullptr;
}

/**
 * @details Updates the color configuration for the active progress bar fill (`mProgressColor`)
 *          and the background track groove (`mProgressBgColor`).
 * @see ProgressDisplay::drawValue, ProgressDisplay::drawBackground
 * @todo none
 * @bug none
 */
void ProgressDisplay::setColor(Color progress, Color background)
{
	// Store active progress bar fill color and inner groove background color
	mProgressColor = progress;
	mProgressBgColor = background;
}

/**
 * @details Clamps the provided value between `0.0f` and `100.0f` to prevent visual
 *          rendering artifacts or overflow outside the allocated boundaries. Calculates
 *          the inner progress bar area by applying a 5-pixel margin on all four sides of the
 *          widget area.
 *          The method then:
 *          1. Resizes the frame buffer to the inner track dimensions.
 *          2. Clears it with the groove background color (`mProgressBgColor`).
 *          3. Computes the active bar width proportional to the percentage (`value / 100.0f * totalWidth`).
 *          4. Draws a solid filled rectangle with the active progress color (`mProgressColor`).
 *          5. Blits the composited progress bar bitmap onto the target LCD at the offset position.
 * @see BaseDisplay::drawValue, ProgressDisplay::drawBackground, ProgressDisplay::setColor
 * @todo none
 * @bug none
 */
void ProgressDisplay::drawValue(float value)
{
	// Abort if off-screen framebuffer or target LCD surface is not attached
	if (mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	Position pos = mArea.getPosition();
	float width;

	// Clamp input value to 0.0% ~ 100.0%
	if (value > 100.f)
		value = 100.f;
	else if (value < 0.f)
		value = 0.f;

	// Inset 5 pixels on all sides for the inner progress track
	area.subWidth(10);
	area.subHeight(10);
	area.setX(0);
	area.setY(0);

	pos.addX(5);
	pos.addY(5);

	// Clear the progress bar track with groove background color
	mFb->setSize(area);
	mFb->setBackgroundColor(mProgressBgColor);
	mFb->clear();

	// Calculate filled width and draw active progress bar
	width = area.getWidth();
	area.setWidth(value / 100.f * width);
	mFb->setBrushColor(mProgressColor);
	mFb->fillRectangular(area);

	// Blit the updated progress track to the LCD
	mLcd->drawBitmap(pos, mFb->getBitmap());
}

/**
 * @details Renders the static widget background and the unfilled progress track.
 *          Configures the frame buffer to match the entire widget area, clears it with
 *          the outer background color (`mBgColor`), draws an inner groove rectangle using
 *          `mProgressBgColor` with a 5-pixel inset margin on all sides, and blits the complete
 *          background bitmap to the LCD display.
 * @see BaseDisplay::drawBackground, ProgressDisplay::drawValue
 * @todo none
 * @bug none
 */
void ProgressDisplay::drawBackground()
{
	// Abort if off-screen framebuffer or target LCD surface is not attached
	if (mFb == nullptr || mLcd == nullptr)
		return;

	// Calculate inner track area with 5-pixel margin
	Area area = mArea;
	area.subHeight(10);
	area.subWidth(10);
	area.setX(5);
	area.setY(5);

	// Render background and initial empty track
	mFb->setSize(mArea);
	mFb->setBrushColor(mProgressBgColor);
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();
	mFb->fillRectangular(area);

	// Blit full widget background to the LCD
	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
}
