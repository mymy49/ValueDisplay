/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef __H_
#define VALUE_DISPLAY__H_

#include <TurtleShip.h>

#include "BaseDisplay.h"

class RpmBarDisplay : public BaseDisplay
{
protected :
	Color mIdleColor, mNormalColor, mWarningColor, mOverColor, mBarBgColor;
	int32_t mIdleRpm, mWarningRpm, mOverRpm, mMaxRpm, mToplineThickness;

public :
	RpmBarDisplay();

	void setIdleColor(Color color);

	void setNormalColor(Color color);

	void setWarningColor(Color color);

	void setOverColor(Color color);

	void setBarBgColor(Color color);

	void setMaxRpm(int32_t rpm);

	void setToplineThickness(int32_t thickness);

	virtual void drawValue(float value) override;

	virtual void drawBackground() override;
};

#endif

