/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BaseDisplay.h
 * @brief Base class for display widgets and value indicators.
 */

#ifndef BASE_DISPLAY__H_
#define BASE_DISPLAY__H_

#include <TurtleShip.h>

/**
 * @class BaseDisplay
 * @brief Abstract base class providing common geometry, framebuffer, and drawing interfaces for display widgets.
 *
 * @details
 * The `BaseDisplay` class serves as the core foundation for visual UI indicators within the
 * TurtleShip GUI framework (e.g. `ValueDisplay`, `ProgressDisplay`, `RpmBarDisplay`).
 *
 * ### Rendering Architecture
 * This class uses an off-screen double-buffering architecture to eliminate visual screen flickering:
 * 1. Off-screen Rendering: Graphical components are first rendered into a working `FrameBuffer` (`mFb`).
 * 2. Blitting: Once rendering is complete, the resulting bitmap is transferred to the physical LCD
 *    display surface via a `Brush` (`mLcd`).
 *
 * ### Key Responsibilities
 * - Geometry Management: Stores and manipulates widget boundaries, screen coordinates, width, and height via `mArea`.
 * - Style Management: Manages base background fill color (`mBgColor`).
 * - Surface Binding: Connects to off-screen buffer (`mFb`) and screen output brush (`mLcd`).
 * - Virtual Pipeline: Enforces implementation of `drawBackground()` and `drawValue()` across all derived widgets.
 *
 * ### Typical Derived Implementation Example
 * @code
 * class CustomGauge : public BaseDisplay
 * {
 * public:
 *     virtual void drawBackground() override
 *     {
 *         if (mFb == nullptr || mLcd == nullptr)
 *             return;
 *
 *         mFb->setSize(mArea.getSize());
 *         mFb->setBackgroundColor(mBgColor);
 *         mFb->clear();
 *         mLcd->drawBitmap(mArea.getPosition(), mFb->getBitmap());
 *     }
 *
 *     virtual void drawValue(float value) override
 *     {
 *         // Render dynamic custom graphics and blit to mLcd...
 *     }
 * };
 *
 * // Usage:
 * CustomGauge gauge;
 * gauge.setFrameBuffer(brush);
 * gauge.setLcd(lcd);
 * gauge.setArea({10, 20, 100, 40});
 * gauge.setBgColor(0x30, 0x30, 0x30);
 * gauge.drawBackground();
 * gauge.drawValue(42.0f);
 * @endcode
 */
class BaseDisplay
{
protected :
	FrameBuffer *mFb;    ///< Pointer to working framebuffer for off-screen rendering
	Area mArea;          ///< Display bounding area and screen coordinates
	Color mBgColor;      ///< Widget background color
	Brush *mLcd;         ///< Pointer to target drawing surface (LCD display interface)

public :
	/**
	 * @brief Constructs a new BaseDisplay object.
	 */
	BaseDisplay(void);

	/**
	 * @brief Sets the framebuffer reference used for off-screen rendering.
	 * @param[in] fb Reference to the framebuffer object.
	 */
	void setFrameBuffer(FrameBuffer &fb);

	/**
	 * @brief Sets the framebuffer pointer used for off-screen rendering.
	 * @param[in] fb Pointer to the framebuffer object.
	 */
	void setFrameBuffer(FrameBuffer *fb);

	/**
	 * @brief Sets the widget display area (position and dimensions) by pointer.
	 * @param[in] area Pointer to the Area object.
	 */
	void setArea(Area *area);

	/**
	 * @brief Sets the widget display area (position and dimensions) by value.
	 * @param[in] area The Area object.
	 */
	void setArea(Area area);

	/**
	 * @brief Sets the width and height of the display widget.
	 * @param[in] width  Width in pixels.
	 * @param[in] height Height in pixels.
	 */
	void setSize(uint16_t width, uint16_t height);

	/**
	 * @brief Sets the dimensions of the display widget using a Size object.
	 * @param[in] size The Size object containing width and height.
	 */
	void setSize(Size size);

	/**
	 * @brief Gets the current dimensions of the display widget.
	 * @return Size The current size of the widget.
	 */
	Size getSize();

	/**
	 * @brief Sets the screen position coordinates of the display widget.
	 * @param[in] pos The target screen position.
	 */
	void setPosition(Position pos);

	/**
	 * @brief Sets the background color of the widget using a Color object.
	 * @param[in] color The background color.
	 */
	void setBgColor(Color color);

	/**
	 * @brief Sets the background color of the widget using RGBA color components.
	 * @param[in] red   Red component (0 ~ 255).
	 * @param[in] green Green component (0 ~ 255).
	 * @param[in] blue  Blue component (0 ~ 255).
	 * @param[in] alpha Alpha transparency component (0 ~ 255, default: 0xFF).
	 */
	void setBgColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

	/**
	 * @brief Gets the current screen position coordinates of the display widget.
	 * @return Position The current screen position.
	 */
	Position getPosition(void);

	/**
	 * @brief Gets the pointer to the currently associated framebuffer.
	 * @return FrameBuffer* Pointer to the framebuffer, or nullptr if not set.
	 */
	FrameBuffer *getFrameBuffer(void);

	/**
	 * @brief Sets the font reference for text rendering.
	 * @param[in] font Reference to the Font object.
	 */
	void setFont(Font &font);

	/**
	 * @brief Sets the target LCD display drawing brush/surface.
	 * @param[in] brush Reference to the target Brush/display surface.
	 */
	void setLcd(Brush &brush);

	/**
	 * @brief Renders the numerical value onto the display.
	 * @param[in] value The floating-point value to display.
	 */
	virtual void drawValue(float value) = 0;

	/**
	 * @brief Renders the static background graphics of the widget onto the display.
	 */
	virtual void drawBackground() = 0;
};

#endif
