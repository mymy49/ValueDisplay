/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef ALERT_VALUE_DISPLAY__H_
#define ALERT_VALUE_DISPLAY__H_

#include "ValueDisplay.h"

class AlertValueDisplay : public ValueDisplay
{
protected :
	Color mLowColor;
	Color mHighColor;
	Color mNormalColor;
	float mLowThreshold;
	float mHighThreshold;

public :
	AlertValueDisplay();

	virtual void drawValue(float value) override;

	void setAlertColor(Color lowColor, Color normalColor, Color highColor);

	void setAlertValue(float lowThreshold, float highThreshold);
};

#endif

