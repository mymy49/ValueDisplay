/**
 * @file RpmBarDisplay.cpp
 * @brief Implementation of RpmBarDisplay member functions.
 */

#include "RpmBarDisplay.h"

/**
 * @details Initializes the RPM bar gauge with default automotive tachometer
 * thresholds and color schemes:
 * - Idle RPM threshold: 1100 RPM (Color: Blue, RGB: 0x00, 0x00, 0xFF)
 * - Warning RPM threshold: 5000 RPM (Color: Green, RGB: 0x00, 0xFF, 0x00)
 * - Over-rev RPM threshold: 6000 RPM (Color: Yellow, RGB: 0xFF, 0xFF, 0x00)
 * - Maximum RPM scale: 8000 RPM (Color: Red, RGB: 0xFF, 0x00, 0x00)
 * - Top guide line thickness: 5 pixels
 * - Background color: Black (RGB: 0x00, 0x00, 0x00)
 * - Bar groove background color: Dark gray (RGB: 0x30, 0x30, 0x30)
 *
 * Internal pointers to the off-screen frame buffer and LCD drawing brush are
 * initialized to nullptr.
 */
RpmBarDisplay::RpmBarDisplay(void)
{
	mIdleRpm = 1100;
	mWarningRpm = 5000;
	mOverRpm = 6000;
	mMaxRpm = 8000;

	mToplineThickness = 5;

	mIdleColor.setColor({0x00, 0x00, 0xFF});
	mNormalColor.setColor({0x00, 0xFF, 0x00});
	mWarningColor.setColor({0xFF, 0xFF, 0x00});
	mOverColor.setColor({0xFF, 0x00, 0x00});
	mBgColor.setColor({0x00, 0x00, 0x00});
	mBarBgColor.setColor({0x30, 0x30, 0x30});

	mFb = nullptr;
	mLcd = nullptr;
}

/**
 * @details Configures the display color representing the engine idle zone (from
 * 0 up to mIdleRpm).
 *
 * @param[in] color The Color object for the idle zone.
 */
void RpmBarDisplay::setIdleColor(Color color)
{
	mIdleColor = color;
}

/**
 * @details Configures the display color representing the normal cruising engine
 * speed range (from mIdleRpm up to mWarningRpm).
 *
 * @param[in] color The Color object for the normal driving zone.
 */
void RpmBarDisplay::setNormalColor(Color color)
{
	mNormalColor = color;
}

/**
 * @details Configures the display color representing the high warning engine
 * speed range (from mWarningRpm up to mOverRpm).
 *
 * @param[in] color The Color object for the warning zone.
 */
void RpmBarDisplay::setWarningColor(Color color)
{
	mWarningColor = color;
}

/**
 * @details Configures the display color representing the redline / over-rev danger
 * range (from mOverRpm up to mMaxRpm).
 *
 * @param[in] color The Color object for the over-rev zone.
 */
void RpmBarDisplay::setOverColor(Color color)
{
	mOverColor = color;
}

/**
 * @details Sets the background color of the dynamic RPM bar track behind the
 * active colored bar indicator.
 *
 * @param[in] color The Color object for the dynamic bar track background.
 */
void RpmBarDisplay::setBarBgColor(Color color)
{
	mBarBgColor = color;
}

/**
 * @details Renders the dynamic RPM bar filled proportionally to the given RPM value.
 *
 * The method performs the following steps:
 * 1. Computes the inner bar area with a 3-pixel horizontal margin on each side and
 *    vertically positioned beneath the top reference guideline (offset by mToplineThickness + 6).
 * 2. Clears the bar area with mBarBgColor.
 * 3. Calculates the active bar width as `(value / mMaxRpm) * totalWidth`.
 * 4. Determines the fill brush color based on current RPM zone:
 *    - value < mIdleRpm: mIdleColor
 *    - value < mWarningRpm: mNormalColor
 *    - value < mOverRpm: mWarningColor
 *    - value >= mOverRpm: mOverColor
 * 5. Fills the dynamic bar rectangle and blits the resulting bitmap directly onto
 *    the target LCD surface at the appropriate screen coordinates.
 *
 * @param[in] value Current engine RPM value to display.
 */
void RpmBarDisplay::drawValue(float value)
{
	if (mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	float width;

	// Inset horizontally by 6px (3px each side) and vertically below the topline guide
	area.subWidth(6);
	area.subHeight(mToplineThickness + 6);
	area.setX(0);
	area.setY(0);

	// Clear dynamic bar area with track background color
	mFb->setSize(area.getSize());
	mFb->setBackgroundColor(mBarBgColor);
	mFb->clear();

	// Calculate filled width proportional to mMaxRpm
	width = area.getWidth();
	area.setWidth(value / (float)mMaxRpm * width);

	// Select fill color based on the current RPM zone
	if (value < mIdleRpm)
		mFb->setBrushColor(mIdleColor);
	else if (value < mWarningRpm)
		mFb->setBrushColor(mNormalColor);
	else if (value < mOverRpm)
		mFb->setBrushColor(mWarningColor);
	else
		mFb->setBrushColor(mOverColor);

	mFb->fillRectangular(area);

	// Position bar below the topline guide and draw to LCD
	area = mArea;
	area.addX(3);
	area.addY(mToplineThickness + 3);

	mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
}

/**
 * @details Renders the widget background and the segmented top reference guideline.
 * The top reference guideline is partitioned into four colored segments proportional
 * to their respective RPM ranges relative to mMaxRpm:
 * 1. Idle segment: width proportional to `mIdleRpm / mMaxRpm`, drawn in `mIdleColor`.
 * 2. Normal segment: width proportional to `(mWarningRpm - mIdleRpm) / mMaxRpm`, drawn in `mNormalColor`.
 * 3. Warning segment: width proportional to `(mOverRpm - mWarningRpm) / mMaxRpm`, drawn in `mWarningColor`.
 * 4. Over-rev segment: width proportional to `(mMaxRpm - mOverRpm) / mMaxRpm`, drawn in `mOverColor`.
 *
 * Once all segments are rendered into the off-screen frame buffer, the composited
 * background is transferred to the LCD surface.
 */
void RpmBarDisplay::drawBackground()
{
	if (mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	area.setHeight(mToplineThickness);
	area.subWidth(6);
	area.setX(3);
	area.setY(3);

	float width = area.getWidth();

	// Clear full widget area with background color
	mFb->setSize(mArea.getSize());
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();

	// Draw idle zone segment on the topline
	area.setWidth((float)mIdleRpm / (float)mMaxRpm * width);
	mFb->setBrushColor(mIdleColor);
	mFb->fillRectangular(area);

	// Draw normal zone segment on the topline
	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mWarningRpm - mIdleRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mNormalColor);
	mFb->fillRectangular(area);

	// Draw warning zone segment on the topline
	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mOverRpm - mWarningRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mWarningColor);
	mFb->fillRectangular(area);

	// Draw over-rev / redline zone segment on the topline
	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mMaxRpm - mOverRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mOverColor);
	mFb->fillRectangular(area);

	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
}

/**
 * @details Configures the full-scale maximum RPM value used for calculating
 * relative bar widths and segment proportions across the widget.
 *
 * @param[in] rpm Maximum RPM scale limit (e.g. 8000, 9000).
 */
void RpmBarDisplay::setMaxRpm(int32_t rpm)
{
	mMaxRpm = rpm;
}

/**
 * @details Sets the vertical thickness of the top segmented reference guideline bar
 * in pixels. Adjusting this value alters both the guideline height and the vertical
 * offset of the main dynamic bar beneath it.
 *
 * @param[in] thickness Height in pixels for the top reference line.
 */
void RpmBarDisplay::setToplineThickness(int32_t thickness)
{
	mToplineThickness = thickness;
}
