#include <bsp.h>
#include <RpmBarDisplay.h>

RpmBarDisplay::RpmBarDisplay(void)
{
	mIdleRpm = 1100;
	mWarningRpm = 5000;
	mOverRpm = 6000;
	mMaxRpm = 8000;

	mToplineThickness = 5;

	mIdleColor.setColor({0x00, 0x00, 0xFF});
	mNormalColor.setColor({0x00, 0xFF, 0x00});
	mWarningColor.setColor({0xFF, 0xFF, 0x00});
	mOverColor.setColor({0xFF, 0x00, 0x00});
	mBgColor.setColor({0x00, 0x00, 0x00});
	mBarBgColor.setColor({0x30, 0x30, 0x30});

	mFb = nullptr;
	mLcd = nullptr;
}

void RpmBarDisplay::setIdleColor(Color color)
{
	mIdleColor = color;
}

void RpmBarDisplay::setNormalColor(Color color)
{
	mNormalColor = color;
}

void RpmBarDisplay::setWarningColor(Color color)
{
	mWarningColor = color;
}

void RpmBarDisplay::setOverColor(Color color)
{
	mOverColor = color;
}

void RpmBarDisplay::setBarBgColor(Color color)
{
	mBarBgColor = color;
}

void RpmBarDisplay::drawValue(float value)
{
	if(mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	float width;

	area.subWidth(6);
	area.subHeight(mToplineThickness + 6);
	area.setX(0);
	area.setY(0);
	
	mFb->setSize(area.getSize());
	mFb->setBackgroundColor(mBarBgColor);
	mFb->clear();
	
	width = area.getWidth();
	area.setWidth(value / (float)mMaxRpm * width);

	if(value < mIdleRpm)
		mFb->setBrushColor(mIdleColor);
	else if(value < mWarningRpm)
		mFb->setBrushColor(mNormalColor);
	else if(value < mOverRpm)
		mFb->setBrushColor(mWarningColor);
	else
		mFb->setBrushColor(mOverColor);

	mFb->fillRectangular(area);

	area = mArea;
	area.addX(3);
	area.addY(mToplineThickness + 3);
	
	mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
}

void RpmBarDisplay::drawBackground()
 {
	if(mFb == nullptr || mLcd == nullptr)
		return;

	Area area = mArea;
	area.setHeight(mToplineThickness);
	area.subWidth(6);
	area.setX(3);
	area.setY(3);

	float width = area.getWidth();

	mFb->setSize(mArea.getSize());
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();

	area.setWidth((float)mIdleRpm / (float)mMaxRpm * width);
	mFb->setBrushColor(mIdleColor);
	mFb->fillRectangular(area);

	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mWarningRpm - mIdleRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mNormalColor);
	mFb->fillRectangular(area);

	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mOverRpm - mWarningRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mWarningColor);
	mFb->fillRectangular(area);

	area.addX(area.getSize().getWidth());
	area.setWidth((float)(mMaxRpm - mOverRpm) / (float)mMaxRpm * width);
	mFb->setBrushColor(mOverColor);
	mFb->fillRectangular(area);

	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
}

void RpmBarDisplay::setMaxRpm(int32_t rpm)
{
	mMaxRpm = rpm;
}

void RpmBarDisplay::setToplineThickness(int32_t thickness)
{
	mToplineThickness = thickness;
}


