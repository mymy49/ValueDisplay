#include "AlertValueDisplay.h"
#include <string.h>
#include <stdio.h>

AlertValueDisplay::AlertValueDisplay()
{
	mNormalColor.setColor(0x00, 0xFF, 0x00);
	mLowColor.setColor(0x00, 0x00, 0xFF);
	mHighColor.setColor(0xFF, 0x00, 0x00);
	mLowThreshold = 10;
	mHighThreshold = 90;
}

void AlertValueDisplay::drawValue(float value)
{
	if(value < mLowThreshold)
		mValueColor = mLowColor;
	else if(value < mHighThreshold)
		mValueColor = mNormalColor;
	else
		mValueColor = mHighColor;

	ValueDisplay::drawValue(value);
}

void AlertValueDisplay::setAlertColor(Color lowColor, Color normalColor, Color highColor)
{
	mNormalColor = normalColor;
	mLowColor = lowColor;
	mHighColor = highColor;
}

void AlertValueDisplay::setAlertValue(float lowThreshold, float highThreshold)
{
	mLowThreshold = lowThreshold;
	mHighThreshold = highThreshold;
}

