/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file ProgressDisplay.h
 * @brief Horizontal progress bar widget for displaying percentage (0 ~ 100%).
 */

#ifndef PROGRESS_DISPLAY__H_
#define PROGRESS_DISPLAY__H_

#include <TurtleShip.h>
#include "BaseDisplay.h"

/**
 * @class ProgressDisplay
 * @brief Widget for drawing a horizontal progress bar indicator.
 *
 * @details
 * `ProgressDisplay` is a dedicated progress visualization widget that displays a horizontal progress bar
 * representing values between 0.0% and 100.0%.
 *
 * ### Visual Layout
 * - **Outer Background**: Filled with `mBgColor` across the entire widget area.
 * - **Track Groove**: Inset by 5 pixels on all sides (top, bottom, left, right) and filled with
 *   `mProgressBgColor`.
 * - **Active Bar**: Drawn inside the track groove starting from the left edge, with its width proportional
 *   to `(value / 100.0f) * trackWidth` and filled with `mProgressColor`.
 *
 * ### Value Protection
 * Input values passed to `drawValue()` are automatically clamped between `0.0f` and `100.0f` to prevent
 * visual glitches and buffer overruns.
 *
 * ### Example Usage (from Touch Calibration Application)
 * @code
 * #include <ValueDisplay/ProgressDisplay.h>
 *
 * void pageDisplayTouchCalibration(void*)
 * {
 *     ProgressDisplay progressBar;
 *     ElapsedTime pressTime;
 *
 *     // 1. Associate off-screen buffer and LCD drawing surface
 *     progressBar.setFrameBuffer(brush);
 *     progressBar.setLcd(lcd);
 *
 *     // 2. Set widget area and colors
 *     progressBar.setArea({50, 200, 140, 30});
 *     progressBar.setBgColor(0x30, 0x30, 0x30);
 *     progressBar.setColor(Color{0x30, 0xFF, 0x30}, Color{0x80, 0x80, 0x80});
 *
 *     // 3. Render initial background and empty progress bar (0%)
 *     progressBar.drawBackground();
 *     progressBar.drawValue(0);
 *
 *     // 4. Update progress dynamically in a loop or callback
 *     while (calibrating)
 *     {
 *         int32_t elapsedMs = (int32_t)pressTime.getMsec();
 *         float progressPercent = (float)elapsedMs / 50.0f; // 0 to 100% over 5000 ms
 *
 *         lcd.lock();
 *         progressBar.drawValue(progressPercent);
 *         lcd.unlock();
 *
 *         thread::yield();
 *     }
 * }
 * @endcode
 */
class ProgressDisplay : public BaseDisplay
{
protected :
	Color mProgressBgColor, mProgressColor;  ///< Background color of the groove and fill color of progress bar

public :
	/**
	 * @brief Constructs a new ProgressDisplay object with default colors.
	 */
	ProgressDisplay();

	/**
	 * @brief Renders the progress bar filling proportional to the given percentage value.
	 * @param[in] value Progress percentage value (clamped between 0.0f and 100.0f).
	 */
	void drawValue(float value) override;

	/**
	 * @brief Renders the outer widget background and the empty progress track groove.
	 */
	void drawBackground() override;

	/**
	 * @brief Sets the progress fill and track background colors.
	 * @param[in] progress   Fill color for the active progress bar.
	 * @param[in] background Track background color of the groove.
	 */
	void setColor(Color progress, Color background);
};

#endif
