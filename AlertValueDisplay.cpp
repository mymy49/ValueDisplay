/**
 * @file AlertValueDisplay.cpp
 * @brief Implementation of AlertValueDisplay member functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "AlertValueDisplay.h"
#include <string.h>
#include <stdio.h>

/**
 * @details Initializes an `AlertValueDisplay` instance with default threshold values
 *          and standard alert colors. The default normal color is set to green (RGB: `0x00, 0xFF, 0x00`),
 *          low alert color to blue (RGB: `0x00, 0x00, 0xFF`), and high alert
 *          color to red (RGB: `0xFF, 0x00, 0x00`). The default low threshold is set to `10.0f`
 *          and the high threshold to `90.0f`.
 * @see AlertValueDisplay::setAlertColor, AlertValueDisplay::setAlertValue
 * @todo none
 * @bug none
 */
AlertValueDisplay::AlertValueDisplay()
{
	// Configure default zone colors: Green (normal), Blue (low), Red (high)
	mNormalColor.setColor(0x00, 0xFF, 0x00);
	mLowColor.setColor(0x00, 0x00, 0xFF);
	mHighColor.setColor(0xFF, 0x00, 0x00);
	// Configure default lower and upper alert threshold boundaries
	mLowThreshold = 10;
	mHighThreshold = 90;
}

/**
 * @details Evaluates the provided numerical value against the configured low and
 *          high threshold limits:
 *          - If `value < mLowThreshold`: `mValueColor` is set to `mLowColor` (low alert state).
 *          - If `mLowThreshold <= value < mHighThreshold`: `mValueColor` is set to `mNormalColor` (normal state).
 *          - If `value >= mHighThreshold`: `mValueColor` is set to `mHighColor` (high alert state).
 *          After determining the active text color, invokes `ValueDisplay::drawValue(value)`
 *          to perform formatting, off-screen rendering, and display blitting.
 * @see ValueDisplay::drawValue, AlertValueDisplay::setAlertColor, AlertValueDisplay::setAlertValue
 * @todo none
 * @bug none
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
 *          operational zones: low condition (e.g. blue for cold/low pressure), normal
 *          operating range (e.g. green), and high condition (e.g. red for overheat/overpressure).
 * @see AlertValueDisplay::drawValue, AlertValueDisplay::setAlertValue
 * @todo none
 * @bug none
 */
void AlertValueDisplay::setAlertColor(Color lowColor, Color normalColor, Color highColor)
{
	// Store low, normal, and high alert zone text colors
	mNormalColor = normalColor;
	mLowColor = lowColor;
	mHighColor = highColor;
}

/**
 * @details Sets the boundary limits separating low, normal, and high alert states.
 *          Values below `lowThreshold` trigger the low alert color, values above or
 *          equal to `highThreshold` trigger the high alert color, and values in between
 *          display in the normal color.
 * @see AlertValueDisplay::drawValue, AlertValueDisplay::setAlertColor
 * @todo none
 * @bug none
 */
void AlertValueDisplay::setAlertValue(float lowThreshold, float highThreshold)
{
	// Store lower and upper alert threshold limits
	mLowThreshold = lowThreshold;
	mHighThreshold = highThreshold;
}
