/**
 * @file AlertValueDisplay.cpp
 * @brief Implementation of AlertValueDisplay member functions.
 */

#include "AlertValueDisplay.h"
#include <string.h>
#include <stdio.h>

/**
 * @details Initializes an AlertValueDisplay instance with default threshold values
 * and standard alert colors. The default normal color is set to green (RGB: 0x00,
 * 0xFF, 0x00), low alert color to blue (RGB: 0x00, 0x00, 0xFF), and high alert
 * color to red (RGB: 0xFF, 0x00, 0x00). The default low threshold is set to 10.0f
 * and the high threshold to 90.0f.
 */
AlertValueDisplay::AlertValueDisplay()
{
	mNormalColor.setColor(0x00, 0xFF, 0x00);
	mLowColor.setColor(0x00, 0x00, 0xFF);
	mHighColor.setColor(0xFF, 0x00, 0x00);
	mLowThreshold = 10;
	mHighThreshold = 90;
}

/**
 * @details Evaluates the provided numerical value against the configured low and
 * high threshold limits:
 * - If value < mLowThreshold: mValueColor is set to mLowColor (low alert state).
 * - If mLowThreshold <= value < mHighThreshold: mValueColor is set to mNormalColor (normal state).
 * - If value >= mHighThreshold: mValueColor is set to mHighColor (high alert state).
 *
 * After determining the active text color, it invokes ValueDisplay::drawValue(value)
 * to perform formatting, off-screen rendering, and display blitting.
 *
 * @param[in] value The floating-point value to evaluate and display.
 */
void AlertValueDisplay::drawValue(float value)
{
	// Update text color depending on threshold range
	if (value < mLowThreshold)
		mValueColor = mLowColor;
	else if (value < mHighThreshold)
		mValueColor = mNormalColor;
	else
		mValueColor = mHighColor;

	// Render the value using base class implementation
	ValueDisplay::drawValue(value);
}

/**
 * @details Sets the three distinct colors corresponding to the three threshold
 * operational zones: low condition (e.g. blue for cold/low pressure), normal
 * operating range (e.g. green), and high condition (e.g. red for overheat/overpressure).
 *
 * @param[in] lowColor    Color applied when the value falls below the low threshold.
 * @param[in] normalColor Color applied when the value is within normal boundaries.
 * @param[in] highColor   Color applied when the value reaches or exceeds the high threshold.
 */
void AlertValueDisplay::setAlertColor(Color lowColor, Color normalColor, Color highColor)
{
	mNormalColor = normalColor;
	mLowColor = lowColor;
	mHighColor = highColor;
}

/**
 * @details Sets the boundary limits separating low, normal, and high alert states.
 * Values below lowThreshold will trigger the low alert color, values above or
 * equal to highThreshold will trigger the high alert color, and values in between
 * will display in the normal color.
 *
 * @param[in] lowThreshold  The lower threshold value.
 * @param[in] highThreshold The upper threshold value.
 */
void AlertValueDisplay::setAlertValue(float lowThreshold, float highThreshold)
{
	mLowThreshold = lowThreshold;
	mHighThreshold = highThreshold;
}
