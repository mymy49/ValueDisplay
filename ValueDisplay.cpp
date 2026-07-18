#include <bsp.h>
#include <ValueDisplay.h>
#include <string.h>

ValueDisplay::ValueDisplay()
{
	mFb = nullptr;
	mValueFont = nullptr;
	mUnitFont = nullptr;
	mLcd = nullptr;

	mBgColor.setColor({0x30, 0x30, 0x30});

	memset(mUnit, 0, sizeof(mUnit));
	memset(mTitle, 0, sizeof(mTitle));
}

void ValueDisplay::setUnitConfig(Font &font, Area area, Color fontColor, const char *unit)
{
	int32_t len = strlen(unit);
	mUnitFont = &font;
	mUnitArea = area;
	mUnitColor = fontColor;
	if(len < 32)
		strcpy(mUnit, unit);
}

void ValueDisplay::setValueConfig(Font &font, Area area, Position valuePos, Color fontColor, Color bgColor, const char *format)
{
	int32_t len = strlen(format);
	mValueFont = &font;
	mValueArea = area;
	mValueColor = fontColor;
	mValueBgColor = bgColor;
	mValuePos = valuePos;
	if(len < 64)
		strcpy(mFormat, format);
}

void ValueDisplay::setTitleConfig(Font &font, Position pos, Color fontColor, const char *title)
{
	int32_t len = strlen(title);
	mTitleFont = &font;
	mTitlePos = pos;
	mTitleColor = fontColor;
	if(len < 32)
		strcpy(mTitle, title);	
}

void ValueDisplay::drawBackground()
{
	char str[64];

	if(mFb == nullptr || mLcd == nullptr)
		return;

	mFb->setSize(mArea.getSize());
	mFb->setBackgroundColor(mBgColor);
	mFb->clear();

	if(mTitleFont != nullptr)
	{
		mFb->setBrushColor(mTitleColor);
		mFb->setFont(*mTitleFont);
		mFb->drawString(mTitlePos, mTitle);
	}

	mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
	
	Area area = mArea;
	area += mUnitArea.getPosition();
	
	if(mUnitArea.getSize().getHeight() * mUnitArea.getSize().getWidth())
	{
		mFb->setSize(mUnitArea.getSize());
		mFb->setBackgroundColor(mValueBgColor);
		mFb->clear();

		if(mUnitFont != nullptr)
		{
			mFb->setFont(*mUnitFont);
			mFb->setBrushColor(mUnitColor);
			mFb->drawString(Brush::ALGIN_LEFT_BOT, mUnit);
		}
		mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
	}
}

void ValueDisplay::drawValue(float value)
{
	char str[64];

	if(mFb == nullptr || mValueFont == nullptr || mLcd == nullptr)
		return;

	sprintf(str, mFormat, value);

	Area area = mArea;
	area += mValueArea.getPosition();

	mFb->setSize(mValueArea.getSize());
	mFb->setBackgroundColor(mValueBgColor);
	mFb->setFont(*mValueFont);
	mFb->setBrushColor(mValueColor);
	mFb->setFont(*mValueFont);
	mFb->clear();
	mFb->drawString(mValuePos, str);
	mLcd->drawBitmap(area.getPosition(), mFb->getBitmap());
}
