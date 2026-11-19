/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file RpmBarDisplay.h
 * @brief Multi-segment tachometer bar display widget for engine RPM visualization.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
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
	Color mIdleColor;           ///< Fill and topline color for the idle RPM zone (`0 ~ mIdleRpm`).
	Color mNormalColor;         ///< Fill and topline color for the normal RPM zone (`mIdleRpm ~ mWarningRpm`).
	Color mWarningColor;        ///< Fill and topline color for the warning RPM zone (`mWarningRpm ~ mOverRpm`).
	Color mOverColor;           ///< Fill and topline color for the over-rev / redline RPM zone (`mOverRpm ~ mMaxRpm`).
	Color mBarBgColor;          ///< Background fill color of the dynamic RPM bar groove.
	int32_t mIdleRpm;           ///< Upper RPM boundary of the idle zone.
	int32_t mWarningRpm;        ///< Lower RPM boundary of the warning zone.
	int32_t mOverRpm;           ///< Lower RPM boundary of the over-rev / redline zone.
	int32_t mMaxRpm;            ///< Full-scale maximum RPM value of the gauge.
	int32_t mToplineThickness;  ///< Vertical height in pixels of the top segmented reference guideline.

public :
	/**
	 * @brief Constructs a new RpmBarDisplay object with default thresholds (1100, 5000, 6000, 8000 RPM) and colors.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Surface pointers (`mFb` and `mLcd`) must be configured before invoking drawing methods.
	 * @note Sets topline thickness to 5 pixels and zone colors to Blue (idle), Green (normal), Yellow (warning), Red (over-rev).
	 * @pre none
	 * @post `RpmBarDisplay` instance is constructed with default tachometer parameters.
	 * @deprecated none
	 */
	RpmBarDisplay();

	/**
	 * @brief Sets the color representing the idle RPM range.
	 * @param[in] color Color for idle RPM (`0 ~ mIdleRpm`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Call `drawBackground()` after modifying topline zone colors to refresh the top guideline.
	 * @pre none
	 * @post `mIdleColor` is updated to `color`.
	 * @deprecated none
	 */
	void setIdleColor(Color color);

	/**
	 * @brief Sets the color representing the normal RPM range.
	 * @param[in] color Color for normal RPM (`mIdleRpm ~ mWarningRpm`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Call `drawBackground()` after modifying topline zone colors to refresh the top guideline.
	 * @pre none
	 * @post `mNormalColor` is updated to `color`.
	 * @deprecated none
	 */
	void setNormalColor(Color color);

	/**
	 * @brief Sets the color representing the warning RPM range.
	 * @param[in] color Color for warning RPM (`mWarningRpm ~ mOverRpm`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Call `drawBackground()` after modifying topline zone colors to refresh the top guideline.
	 * @pre none
	 * @post `mWarningColor` is updated to `color`.
	 * @deprecated none
	 */
	void setWarningColor(Color color);

	/**
	 * @brief Sets the color representing the over-rev / redline RPM range.
	 * @param[in] color Color for over-rev RPM (`mOverRpm ~ mMaxRpm`).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Call `drawBackground()` after modifying topline zone colors to refresh the top guideline.
	 * @pre none
	 * @post `mOverColor` is updated to `color`.
	 * @deprecated none
	 */
	void setOverColor(Color color);

	/**
	 * @brief Sets the background color of the dynamic bar track.
	 * @param[in] color Background color of the bar groove.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Takes effect on the next `drawValue()` invocation.
	 * @pre none
	 * @post `mBarBgColor` is updated to `color`.
	 * @deprecated none
	 */
	void setBarBgColor(Color color);

	/**
	 * @brief Sets the maximum scale value of the RPM gauge.
	 * @param[in] rpm Maximum RPM value (e.g. 8000).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning `rpm` must be greater than `mOverRpm` and non-zero to avoid division by zero.
	 * @note Call `drawBackground()` after changing `mMaxRpm` to recalculate top guideline segment widths.
	 * @pre `rpm > 0`
	 * @post `mMaxRpm` is updated to `rpm`.
	 * @deprecated none
	 */
	void setMaxRpm(int32_t rpm);

	/**
	 * @brief Sets the vertical thickness of the top segmented guide bar.
	 * @param[in] thickness Height in pixels.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning `thickness + 6` must be less than the total widget height.
	 * @note Call `drawBackground()` after changing topline thickness.
	 * @pre none
	 * @post `mToplineThickness` is updated to `thickness`.
	 * @deprecated none
	 */
	void setToplineThickness(int32_t thickness);

	/**
	 * @brief Renders the dynamic RPM bar filled proportionally to the given RPM value.
	 * @param[in] value Current engine RPM value.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Returns early without drawing if `mFb` or `mLcd` is `nullptr`.
	 * @note Overrides `BaseDisplay::drawValue()`. Automatically selects the active zone color for the bar.
	 * @pre `setFrameBuffer()`, `setLcd()`, `setArea()`, and `drawBackground()` should be called first.
	 * @post The dynamic lower RPM bar is rendered and blitted onto the LCD.
	 * @deprecated none
	 */
	void drawValue(float value) override;

	/**
	 * @brief Renders the widget background and the segmented top reference guideline.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Returns early without drawing if `mFb` or `mLcd` is `nullptr`.
	 * @note Overrides `BaseDisplay::drawBackground()`.
	 * @pre `setFrameBuffer()`, `setLcd()`, and `setArea()` must be called beforehand.
	 * @post The outer background and 4-segment colored top reference line are blitted onto the LCD.
	 * @deprecated none
	 */
	void drawBackground() override;
};

#endif
