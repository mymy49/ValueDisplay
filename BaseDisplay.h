/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

#ifndef BASE_DISPLAY__H_
#define BASE_DISPLAY__H_

#include <TurtleShip.h>

class BaseDisplay
{
protected :
	FrameBuffer *mFb;
	Area mArea;
	Color mBgColor;
	Brush *mLcd;

public :
	BaseDisplay(void);

	void setFrameBuffer(FrameBuffer &fb);

	void setFrameBuffer(FrameBuffer *fb);

	void setArea(Area &area);

	void setSize(uint16_t width, uint16_t height);

	void setSize(Size size);

	Size getSize();

	void setPosition(Position pos);

	void setBgColor(Color color);

	Position getPosition(void);

	FrameBuffer *getFrameBuffer(void);

	void setFont(Font &font);

	void setLcd(Brush &brush);

	virtual void drawValue(float value) = 0;

	virtual void drawBackground() = 0;
};

#endif

