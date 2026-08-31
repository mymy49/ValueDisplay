#include "ProgressDisplay.h"

ProgressDisplay::ProgressDisplay(void)
{
	mBgColor.setColor(0x30, 0x30, 0x30);
	mProgressColor.setColor(0x30, 0xFF, 0x30);
	mProgressBgColor.setColor(0x80, 0x80, 0x80);

	mFb = nullptr;
	mLcd = nullptr;
}

void ProgressDisplay::setColor(Color progress, Color background)
{
	mProgressColor = progress;
	mProgressBgColor = background;
}

void ProgressDisplay::drawValue(float value)
{
	if(mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	Position pos = mArea.getPosition();
	float width;

	if(value > 100.f)
		value = 100.f;
	else if(value < 0.f)
		value = 0.f;

	area.subWidth(10);
	area.subHeight(10);
	area.setX(0);
	area.setY(0);

	pos.addX(5);
	pos.addY(5);
	
	mFb->setSize(area);
	mFb->setBackgroundColor(mProgressBgColor);
	mFb->clear();
	
	width = area.getWidth();
	area.setWidth(value / 100.f * width);
	mFb->setBrushColor(mProgressColor);
	mFb->fillRectangular(area);
	
	mLcd->drawBitmap(pos, mFb->getBitmap());
}

void ProgressDisplay::drawBackground()
 {
	if(mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	area.subHeight(10);
	area.subWidth(10);
	area.setX(5);
	area.setY(5);

	mFb->setSize(mArea);
	mFb->setBrushColor(mProgressBgColor);
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();
	mFb->fillRectangular(area);

	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
}

