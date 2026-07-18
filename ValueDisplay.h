/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef VALUE_DISPLAY__H_
#define VALUE_DISPLAY__H_

#include "BaseDisplay.h"

class ValueDisplay : public BaseDisplay
{
protected :
	Area mUnitArea, mValueArea;
	Position mTitlePos, mValuePos;
	Color mUnitColor, mValueColor, mTitleColor, mValueBgColor;
	Font *mUnitFont, *mValueFont, *mTitleFont;
	char mUnit[32];
	char mFormat[64];
	char mTitle[32];

public :
	ValueDisplay();

	virtual void drawValue(float value) override;

	virtual void drawBackground() override;

	void setUnitConfig(Font &font, Area area, Color fontColor, const char *unit);

	void setValueConfig(Font &font, Area area, Position valuePos, Color fontColor, Color bgColor, const char *format);

	void setTitleConfig(Font &font, Position pos, Color fontColor, const char *title);
};

#endif

