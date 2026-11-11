/**
 * @file BaseDisplay.cpp
 * @brief Implementation of BaseDisplay member functions.
 */

#include "BaseDisplay.h"
#include <string.h>

/**
 * @details Initializes a new instance of the BaseDisplay class. It resets the
 * internal off-screen framebuffer pointer (mFb) and the target display drawing
 * interface brush (mLcd) to nullptr, ensuring that rendering methods will safely
 * return early until both drawing targets are configured.
 */
BaseDisplay::BaseDisplay(void)
{
	mFb = nullptr;
	mLcd = nullptr;
}

/**
 * @details Assigns the target LCD drawing surface (brush) by storing its address
 * into mLcd. All subsequent screen output operations (such as copying off-screen
 * framebuffers to the display) will be directed to this brush.
 *
 * @param[in] brush Reference to the target LCD Brush/surface to draw on.
 */
void BaseDisplay::setLcd(Brush &brush)
{
	mLcd = &brush;
}

/**
 * @details Associates a working off-screen FrameBuffer with the widget by storing
 * its memory address. Off-screen rendering techniques are utilized by this widget
 * to prevent visual flickering before the final composited bitmap is blitted to
 * the display surface.
 *
 * @param[in] fb Reference to the FrameBuffer instance to be used.
 */
void BaseDisplay::setFrameBuffer(FrameBuffer &fb)
{
	mFb = &fb;
}

/**
 * @details Stores the pointer to an off-screen FrameBuffer instance. Can be
 * passed as nullptr to detach the current framebuffer.
 *
 * @param[in] fb Pointer to the target FrameBuffer object, or nullptr to disassociate.
 */
void BaseDisplay::setFrameBuffer(FrameBuffer *fb)
{
	mFb = fb;
}

/**
 * @details Updates the widget's bounding area by dereferencing the provided Area
 * pointer. The screen coordinates (X, Y) and dimensions (width, height) are copied
 * directly into member variable mArea.
 *
 * @param[in] area Pointer to the Area structure containing position and dimension settings.
 */
void BaseDisplay::setArea(Area *area)
{
	mArea = *area;
}

/**
 * @details Overwrites the widget's bounding area with the passed Area value,
 * defining its screen placement and visual dimensions.
 *
 * @param[in] area The Area object containing position and dimension settings.
 */
void BaseDisplay::setArea(Area area)
{
	mArea = area;
}

/**
 * @details Explicitly modifies the horizontal and vertical pixel dimensions of
 * the widget bounding area without altering its current screen coordinate offsets
 * (X and Y).
 *
 * @param[in] width  The width of the display widget in pixels.
 * @param[in] height Height of the display widget in pixels.
 */
void BaseDisplay::setSize(uint16_t width, uint16_t height)
{
	mArea.setWidth(width);
	mArea.setHeight(height);
}

/**
 * @details Sets the widget dimensions from a Size object while preserving the
 * existing top-left screen position coordinates.
 *
 * @param[in] size The Size structure specifying the width and height in pixels.
 */
void BaseDisplay::setSize(Size size)
{
	mArea = size;
}

/**
 * @details Retrieves the current visual dimensions (width and height) of the
 * widget bounding area.
 *
 * @return Size A Size object containing the width and height in pixels.
 */
Size BaseDisplay::getSize(void)
{
	return mArea.getSize();
}

/**
 * @details Relocates the widget on the display surface by updating the top-left
 * X and Y coordinates within mArea, preserving its current width and height.
 *
 * @param[in] pos The new screen coordinate Position.
 */
void BaseDisplay::setPosition(Position pos)
{
	mArea = pos;
}

/**
 * @details Configures the default background fill color for the widget using a
 * Color object. This color is used when clearing or redrawing the base widget area.
 *
 * @param[in] color The Color to apply to the widget background.
 */
void BaseDisplay::setBgColor(Color color)
{
	mBgColor = color;
}

/**
 * @details Configures the default background color by specifying individual 8-bit
 * RGBA channel intensities.
 *
 * @param[in] red   Intensity of the red color component (0 to 255).
 * @param[in] green Intensity of the green color component (0 to 255).
 * @param[in] blue  Intensity of the blue color component (0 to 255).
 * @param[in] alpha Opacity/alpha transparency level (0 to 255, where 255 is fully opaque).
 */
void BaseDisplay::setBgColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	mBgColor.setColor(red, green, blue, alpha);
}

/**
 * @details Retrieves the current screen placement coordinates (X, Y) of the
 * widget relative to the display root.
 *
 * @return Position The current top-left screen position.
 */
Position BaseDisplay::getPosition(void)
{
	return mArea.getPosition();
}

/**
 * @details Returns the pointer to the currently assigned off-screen FrameBuffer
 * object, allowing caller inspection or external modifications.
 *
 * @return FrameBuffer* Pointer to the current FrameBuffer instance, or nullptr if none is configured.
 */
FrameBuffer *BaseDisplay::getFrameBuffer(void)
{
	return mFb;
}
