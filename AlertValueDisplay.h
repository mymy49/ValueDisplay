/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file AlertValueDisplay.h
 * @brief Display widget extending ValueDisplay with threshold-based alert colors.
 */

#ifndef ALERT_VALUE_DISPLAY__H_
#define ALERT_VALUE_DISPLAY__H_

#include "ValueDisplay.h"

/**
 * @class AlertValueDisplay
 * @brief Specialized ValueDisplay that changes text color based on low/high threshold comparisons.
 *
 * @details
 * `AlertValueDisplay` inherits from `ValueDisplay` and provides an automatic tri-state color-alerting
 * mechanism based on configurable low and high thresholds.
 *
 * ### Operating Threshold Zones
 * - **Low Alert Zone (`value < mLowThreshold`)**:
 *   Renders the numerical value in `mLowColor` (e.g. Blue for cold engine coolant or low fuel pressure).
 * - **Normal Zone (`mLowThreshold <= value < mHighThreshold`)**:
 *   Renders the numerical value in `mNormalColor` (e.g. Green for safe operating conditions).
 * - **High Alert Zone (`value >= mHighThreshold`)**:
 *   Renders the numerical value in `mHighColor` (e.g. Red for overheating, overpressure, or redline conditions).
 *
 * All other features (title, formatted string, measurement unit, cursor blinking, background rendering)
 * are fully inherited from `ValueDisplay`.
 *
 * ### Example Usage
 * @code
 * #include <ValueDisplay/AlertValueDisplay.h>
 * #include <font/Ubuntu_14_B.h>
 * #include <font/Ubuntu_20_B.h>
 *
 * AlertValueDisplay tempGauge;
 *
 * void setupTempGauge(void)
 * {
 *     // 1. Configure geometry, framebuffer, and LCD brush
 *     tempGauge.setFrameBuffer(brush);
 *     tempGauge.setLcd(lcd);
 *     tempGauge.setArea({10, 90, 130, 60});
 *     tempGauge.setBgColor(0x20, 0x20, 0x20);
 *
 *     // 2. Configure title, value box, and unit
 *     tempGauge.setTitleConfig(Font_Ubuntu_14_B, {5, 5}, Color{0xAA, 0xAA, 0xAA}, "COOLANT");
 *     tempGauge.setValueConfig(Font_Ubuntu_20_B, {5, 25, 80, 30}, {2, 5},
 *                              Color{0x00, 0xFF, 0x00}, Color{0x10, 0x10, 0x10}, "%3.0f");
 *     tempGauge.setUnitConfig(Font_Ubuntu_14_B, {90, 25, 35, 30}, Color{0xFF, 0xFF, 0xFF}, "'C");
 *
 *     // 3. Set alert thresholds: Low < 50.0'C, High >= 105.0'C
 *     tempGauge.setAlertValue(50.0f, 105.0f);
 *
 *     // 4. Set alert colors: Blue (cold), Green (normal), Red (overheat)
 *     tempGauge.setAlertColor(Color{0x00, 0x80, 0xFF}, Color{0x00, 0xFF, 0x00}, Color{0xFF, 0x00, 0x00});
 *
 *     // 5. Draw static background
 *     tempGauge.drawBackground();
 * }
 *
 * void updateTemperature(float coolantTemp)
 * {
 *     // 6. Draw value (color updates automatically depending on coolantTemp)
 *     tempGauge.drawValue(coolantTemp);
 * }
 * @endcode
 */
class AlertValueDisplay : public ValueDisplay
{
protected :
	Color mLowColor;       ///< Text color applied when value falls below low threshold
	Color mHighColor;      ///< Text color applied when value exceeds high threshold
	Color mNormalColor;    ///< Text color applied when value is within normal operating range
	float mLowThreshold;   ///< Threshold value for low alert condition
	float mHighThreshold;  ///< Threshold value for high alert condition

public :
	/**
	 * @brief Constructs a new AlertValueDisplay object with default thresholds (10 / 90) and colors.
	 */
	AlertValueDisplay();

	/**
	 * @brief Evaluates the value against thresholds to select the appropriate alert color, then renders it.
	 * @param[in] value The floating-point value to display.
	 */
	void drawValue(float value) override;

	/**
	 * @brief Sets the alert colors for low, normal, and high value ranges.
	 * @param[in] lowColor    Color for values below the low threshold.
	 * @param[in] normalColor Color for values within normal range.
	 * @param[in] highColor   Color for values exceeding the high threshold.
	 */
	void setAlertColor(Color lowColor, Color normalColor, Color highColor);

	/**
	 * @brief Sets the low and high alert threshold limits.
	 * @param[in] lowThreshold  The lower threshold value.
	 * @param[in] highThreshold The upper threshold value.
	 */
	void setAlertValue(float lowThreshold, float highThreshold);
};

#endif
