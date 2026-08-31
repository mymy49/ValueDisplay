#include "BaseDisplay.h"
#include <string.h>

BaseDisplay::BaseDisplay(void)
{
	mFb = nullptr;
	mLcd = nullptr;
}

void BaseDisplay::setLcd(Brush &brush)
{
	mLcd = &brush;
}

void BaseDisplay::setFrameBuffer(FrameBuffer &fb)
{
	mFb = &fb;
}

void BaseDisplay::setFrameBuffer(FrameBuffer *fb)
{
	mFb = fb;
}

void BaseDisplay::setArea(Area &area)
{
	mArea = area;
}

void BaseDisplay::setSize(uint16_t width, uint16_t height)
{
	mArea.setWidth(width);
	mArea.setHeight(height);
}

void BaseDisplay::setSize(Size size)
{
	mArea = size;
}

Size BaseDisplay::getSize(void)
{
	return mArea.getSize();
}

void BaseDisplay::setPosition(Position pos)
{
	mArea = pos;
}

void BaseDisplay::setBgColor(Color color)
{
	mBgColor = color;
}

void BaseDisplay::setBgColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBgColor.setColor(red, green, blue, alpha);
}

Position BaseDisplay::getPosition(void)
{
	return mArea.getPosition();
}

FrameBuffer *BaseDisplay::getFrameBuffer(void)
{
	return mFb;
}

