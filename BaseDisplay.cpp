/**
 * @file BaseDisplay.cpp
 * @brief Implementation of BaseDisplay member functions.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
 */

#include "BaseDisplay.h"
#include <string.h>

/**
 * @details Initializes a new instance of the `BaseDisplay` class. Resets the
 *          internal off-screen framebuffer pointer (`mFb`) and the target display drawing
 *          interface brush (`mLcd`) to `nullptr`, ensuring that rendering methods safely
 *          return early until both drawing targets are configured.
 * @see BaseDisplay::setFrameBuffer, BaseDisplay::setLcd
 * @todo none
 * @bug none
 */
BaseDisplay::BaseDisplay(void)
{
	// Initialize off-screen framebuffer and target LCD surface pointers to nullptr
	mFb = nullptr;
	mLcd = nullptr;
}

/**
 * @details Assigns the target LCD drawing surface (`brush`) by storing its address
 *          into `mLcd`. All subsequent screen output operations (such as blitting off-screen
 *          framebuffers to the display) are directed to this brush.
 * @see BaseDisplay::setFrameBuffer
 * @todo none
 * @bug none
 */
void BaseDisplay::setLcd(Brush &brush)
{
	// Bind target LCD drawing surface address
	mLcd = &brush;
}

/**
 * @details Associates a working off-screen `FrameBuffer` with the widget by storing
 *          its memory address. Off-screen rendering prevents visual flickering before
 *          the final composited bitmap is blitted to the display surface.
 * @see BaseDisplay::getFrameBuffer, BaseDisplay::setLcd
 * @todo none
 * @bug none
 */
void BaseDisplay::setFrameBuffer(FrameBuffer &fb)
{
	// Bind working off-screen framebuffer address
	mFb = &fb;
}

/**
 * @details Stores the pointer to an off-screen `FrameBuffer` instance. Can be
 *          passed as `nullptr` to detach the current framebuffer.
 * @see BaseDisplay::getFrameBuffer
 * @todo none
 * @bug none
 */
void BaseDisplay::setFrameBuffer(FrameBuffer *fb)
{
	// Store working off-screen framebuffer pointer directly
	mFb = fb;
}

/**
 * @details Updates the widget's bounding area by dereferencing the provided `Area`
 *          pointer. The screen coordinates (X, Y) and dimensions (width, height) are copied
 *          directly into member variable `mArea`.
 * @see BaseDisplay::setSize, BaseDisplay::setPosition
 * @todo none
 * @bug none
 */
void BaseDisplay::setArea(Area *area)
{
	// Copy bounding box geometry from dereferenced Area pointer
	mArea = *area;
}

/**
 * @details Overwrites the widget's bounding area with the passed `Area` value,
 *          defining its screen placement and visual dimensions.
 * @see BaseDisplay::setSize, BaseDisplay::setPosition
 * @todo none
 * @bug none
 */
void BaseDisplay::setArea(Area area)
{
	// Assign bounding box geometry by value
	mArea = area;
}

/**
 * @details Explicitly modifies the horizontal and vertical pixel dimensions of
 *          the widget bounding area without altering its current screen coordinate offsets
 *          (X and Y).
 * @see BaseDisplay::getSize, Area::setWidth, Area::setHeight
 * @todo none
 * @bug none
 */
void BaseDisplay::setSize(uint16_t width, uint16_t height)
{
	// Update horizontal and vertical dimensions of the widget area
	mArea.setWidth(width);
	mArea.setHeight(height);
}

/**
 * @details Sets the widget dimensions from a `Size` object while preserving the
 *          existing top-left screen position coordinates.
 * @see BaseDisplay::getSize
 * @todo none
 * @bug none
 */
void BaseDisplay::setSize(Size size)
{
	// Assign new width and height via Area's Size assignment operator
	mArea = size;
}

/**
 * @details Retrieves the current visual dimensions (width and height) of the
 *          widget bounding area (`mArea`).
 * @see BaseDisplay::setSize, Area::getSize
 * @todo none
 * @bug none
 */
Size BaseDisplay::getSize(void)
{
	// Return current width and height of the widget area
	return mArea.getSize();
}

/**
 * @details Relocates the widget on the display surface by updating the top-left
 *          X and Y coordinates within `mArea`, preserving its current width and height.
 * @see BaseDisplay::getPosition
 * @todo none
 * @bug none
 */
void BaseDisplay::setPosition(Position pos)
{
	// Update top-left coordinates via Area's Position assignment operator
	mArea = pos;
}

/**
 * @details Configures the default background fill color for the widget using a
 *          `Color` object. This color is used when clearing or redrawing the base widget area.
 * @see BaseDisplay::drawBackground
 * @todo none
 * @bug none
 */
void BaseDisplay::setBgColor(Color color)
{
	// Store widget background fill color
	mBgColor = color;
}

/**
 * @details Configures the default background color by specifying individual 8-bit
 *          RGBA channel intensities.
 * @see Color::setColor, BaseDisplay::drawBackground
 * @todo none
 * @bug none
 */
void BaseDisplay::setBgColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	// Encode individual RGBA components into mBgColor
	mBgColor.setColor(red, green, blue, alpha);
}

/**
 * @details Retrieves the current screen placement coordinates (X, Y) of the
 *          widget relative to the display root.
 * @see BaseDisplay::setPosition, Area::getPosition
 * @todo none
 * @bug none
 */
Position BaseDisplay::getPosition(void)
{
	// Return top-left screen coordinate of the widget area
	return mArea.getPosition();
}

/**
 * @details Returns the pointer to the currently assigned off-screen `FrameBuffer`
 *          object (`mFb`), allowing caller inspection or external modifications.
 * @see BaseDisplay::setFrameBuffer
 * @todo none
 * @bug none
 */
FrameBuffer *BaseDisplay::getFrameBuffer(void)
{
	// Return pointer to attached working framebuffer
	return mFb;
}
