/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file RpmBarDisplay.h
 * @brief Multi-segment tachometer bar display widget for engine RPM visualization.
 */

#ifndef RPM_BAR_DISPLAY__H_
#define RPM_BAR_DISPLAY__H_

#include <TurtleShip.h>
#include "BaseDisplay.h"

/**
 * @class RpmBarDisplay
 * @brief Widget displaying engine RPM as a segmented colored bar and a colored top guideline.
 *
 * @details
 * `RpmBarDisplay` is an automotive-style tachometer gauge widget designed to visualize engine
 * revolutions per minute (RPM). It combines a static multi-colored top guideline bar with a
 * dynamically filling bar underneath.
 *
 * ### Dual-Bar Architecture
 * 1. **Top Segmented Guideline**:
 *    Drawn across the top with a thickness of `mToplineThickness` pixels. It remains statically
 *    divided into four color segments proportional to their respective operating RPM zones:
 *    - **Idle Zone** (`0 ~ mIdleRpm`): Rendered in `mIdleColor` (Default: Blue).
 *    - **Normal Zone** (`mIdleRpm ~ mWarningRpm`): Rendered in `mNormalColor` (Default: Green).
 *    - **Warning Zone** (`mWarningRpm ~ mOverRpm`): Rendered in `mWarningColor` (Default: Yellow).
 *    - **Over-rev / Redline Zone** (`mOverRpm ~ mMaxRpm`): Rendered in `mOverColor` (Default: Red).
 *
 * 2. **Dynamic RPM Bar**:
 *    Positioned beneath the top guideline and inset by 3 pixels horizontally. Its width expands
 *    proportionately to `(value / mMaxRpm) * trackWidth`. The bar's color dynamically switches to match
 *    the active zone that the current RPM falls into.
 *
 * ### Example Usage
 * @code
 * #include <ValueDisplay/RpmBarDisplay.h>
 *
 * RpmBarDisplay tachometer;
 *
 * void setupTachometer(void)
 * {
 *     // 1. Associate off-screen buffer and LCD surface
 *     tachometer.setFrameBuffer(brush);
 *     tachometer.setLcd(lcd);
 *
 *     // 2. Configure bounding area and background colors
 *     tachometer.setArea({20, 40, 200, 35});
 *     tachometer.setBgColor(Color{0x00, 0x00, 0x00});       // Outer bezel background
 *     tachometer.setBarBgColor(Color{0x25, 0x25, 0x25});    // Dynamic bar track background
 *
 *     // 3. Configure full-scale RPM and top guideline thickness
 *     tachometer.setMaxRpm(8000);
 *     tachometer.setToplineThickness(4);
 *
 *     // 4. Customize zone colors (optional; standard defaults are already set)
 *     tachometer.setIdleColor(Color{0x00, 0x80, 0xFF});     // Blue
 *     tachometer.setNormalColor(Color{0x00, 0xFF, 0x00});   // Green
 *     tachometer.setWarningColor(Color{0xFF, 0xFF, 0x00});  // Yellow
 *     tachometer.setOverColor(Color{0xFF, 0x00, 0x00});     // Red
 *
 *     // 5. Draw static background and top guideline segments
 *     tachometer.drawBackground();
 * }
 *
 * void updateEngineSpeed(float currentRpm)
 * {
 *     // 6. Update dynamic RPM bar (e.g. 4500 RPM will display in Green)
 *     tachometer.drawValue(currentRpm);
 * }
 * @endcode
 */
class RpmBarDisplay : public BaseDisplay
{
protected :
	Color mIdleColor, mNormalColor, mWarningColor, mOverColor, mBarBgColor;  ///< Segment and track colors
	int32_t mIdleRpm, mWarningRpm, mOverRpm, mMaxRpm, mToplineThickness;     ///< Threshold RPM limits and topline height

public :
	/**
	 * @brief Constructs a new RpmBarDisplay object with default thresholds (1100, 5000, 6000, 8000 RPM) and colors.
	 */
	RpmBarDisplay();

	/**
	 * @brief Sets the color representing the idle RPM range.
	 * @param[in] color Color for idle RPM.
	 */
	void setIdleColor(Color color);

	/**
	 * @brief Sets the color representing the normal RPM range.
	 * @param[in] color Color for normal RPM.
	 */
	void setNormalColor(Color color);

	/**
	 * @brief Sets the color representing the warning RPM range.
	 * @param[in] color Color for warning RPM.
	 */
	void setWarningColor(Color color);

	/**
	 * @brief Sets the color representing the over-rev / redline RPM range.
	 * @param[in] color Color for over-rev RPM.
	 */
	void setOverColor(Color color);

	/**
	 * @brief Sets the background color of the dynamic bar track.
	 * @param[in] color Background color of the bar groove.
	 */
	void setBarBgColor(Color color);

	/**
	 * @brief Sets the maximum scale value of the RPM gauge.
	 * @param[in] rpm Maximum RPM value (e.g. 8000).
	 */
	void setMaxRpm(int32_t rpm);

	/**
	 * @brief Sets the vertical thickness of the top segmented guide bar.
	 * @param[in] thickness Height in pixels.
	 */
	void setToplineThickness(int32_t thickness);

	/**
	 * @brief Renders the dynamic RPM bar filled proportionally to the given RPM value.
	 * @param[in] value Current engine RPM value.
	 */
	void drawValue(float value) override;

	/**
	 * @brief Renders the widget background and the segmented top reference guideline.
	 */
	void drawBackground() override;
};

#endif
