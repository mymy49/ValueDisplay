/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef PROGRESS_DISPLAY__H_
#define PROGRESS_DISPLAY__H_

#include <TurtleShip/TurtleShip.h>

#include "BaseDisplay.h"

class ProgressDisplay : public BaseDisplay
{
protected :
	Color mProgressBgColor, mProgressColor;

public :
	ProgressDisplay();

	void drawValue(float value) override;

	void drawBackground() override;

	void setColor(Color progress, Color background);
};

#endif

