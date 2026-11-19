/*
 * Copyright (c) 2024 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file BaseDisplay.h
 * @brief Base class for display widgets and value indicators.
 * @author Yoon-Ki Hong
 * @date 2026-11-18
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
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Working framebuffer (`mFb`) and LCD brush (`mLcd`) must be configured before rendering.
	 * @note Initializes `mFb` and `mLcd` pointers to `nullptr`.
	 * @pre none
	 * @post `BaseDisplay` base members are initialized to safe defaults.
	 * @deprecated none
	 */
	BaseDisplay(void);

	/**
	 * @brief Sets the framebuffer reference used for off-screen rendering.
	 * @param[in] fb Reference to the `FrameBuffer` object.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning The referenced `FrameBuffer` must have sufficient memory allocated for the widget's sub-areas.
	 * @note Stores the address of `fb` in `mFb`.
	 * @pre `fb` must be allocated via `malloc()`.
	 * @post `mFb` points to `fb`.
	 * @deprecated none
	 */
	void setFrameBuffer(FrameBuffer &fb);

	/**
	 * @brief Sets the framebuffer pointer used for off-screen rendering.
	 * @param[in] fb Pointer to the `FrameBuffer` object, or `nullptr` to detach.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Drawing methods will return early without rendering if `mFb` is `nullptr`.
	 * @note Stores `fb` directly in `mFb`.
	 * @pre none
	 * @post `mFb` is updated to `fb`.
	 * @deprecated none
	 */
	void setFrameBuffer(FrameBuffer *fb);

	/**
	 * @brief Sets the widget display area (position and dimensions) by pointer.
	 * @param[in] area Pointer to the `Area` object defining top-left coordinates and size.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning `area` must not be `nullptr`.
	 * @note Copies the dereferenced `Area` value into `mArea`.
	 * @pre `area` must point to a valid `Area` instance.
	 * @post `mArea` is updated with the new position and size.
	 * @deprecated none
	 */
	void setArea(Area *area);

	/**
	 * @brief Sets the widget display area (position and dimensions) by value.
	 * @param[in] area The `Area` object defining top-left coordinates and size.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Call `drawBackground()` after changing the widget area to refresh the screen.
	 * @pre none
	 * @post `mArea` is updated to `area`.
	 * @deprecated none
	 */
	void setArea(Area area);

	/**
	 * @brief Sets the width and height of the display widget.
	 * @param[in] width  Width in pixels.
	 * @param[in] height Height in pixels.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Ensure the attached `FrameBuffer` (`mFb`) is large enough for the new dimensions.
	 * @note Preserves the existing top-left screen position of `mArea`.
	 * @pre none
	 * @post `mArea` width and height are updated.
	 * @deprecated none
	 */
	void setSize(uint16_t width, uint16_t height);

	/**
	 * @brief Sets the dimensions of the display widget using a `Size` object.
	 * @param[in] size The `Size` object containing width and height in pixels.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Ensure the attached `FrameBuffer` (`mFb`) is large enough for the new dimensions.
	 * @note Preserves the existing top-left screen position of `mArea`.
	 * @pre none
	 * @post `mArea` dimensions are updated to `size`.
	 * @deprecated none
	 */
	void setSize(Size size);

	/**
	 * @brief Gets the current dimensions of the display widget.
	 * @param[in] none
	 * @return `Size` object containing the current width and height of the widget in pixels.
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Queries `mArea.getSize()`.
	 * @pre none
	 * @post Widget state remains unchanged.
	 * @deprecated none
	 */
	Size getSize();

	/**
	 * @brief Sets the screen position coordinates of the display widget.
	 * @param[in] pos The target top-left screen `Position`.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Preserves the existing width and height of `mArea`.
	 * @pre none
	 * @post `mArea` top-left position is updated to `pos`.
	 * @deprecated none
	 */
	void setPosition(Position pos);

	/**
	 * @brief Sets the background color of the widget using a `Color` object.
	 * @param[in] color The background `Color`.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Takes effect on the next `drawBackground()` call.
	 * @pre none
	 * @post `mBgColor` is updated to `color`.
	 * @deprecated none
	 */
	void setBgColor(Color color);

	/**
	 * @brief Sets the background color of the widget using RGBA color components.
	 * @param[in] red   Red component (0 ~ 255).
	 * @param[in] green Green component (0 ~ 255).
	 * @param[in] blue  Blue component (0 ~ 255).
	 * @param[in] alpha Alpha transparency component (0 ~ 255, default: 0xFF).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Takes effect on the next `drawBackground()` call.
	 * @pre none
	 * @post `mBgColor` is updated with the specified RGBA components.
	 * @deprecated none
	 */
	void setBgColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);

	/**
	 * @brief Gets the current screen position coordinates of the display widget.
	 * @param[in] none
	 * @return `Position` representing the top-left screen coordinates of the widget.
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning none
	 * @note Queries `mArea.getPosition()`.
	 * @pre none
	 * @post Widget state remains unchanged.
	 * @deprecated none
	 */
	Position getPosition(void);

	/**
	 * @brief Gets the pointer to the currently associated framebuffer.
	 * @param[in] none
	 * @return `FrameBuffer*` pointer to the attached working framebuffer, or `nullptr` if not set.
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning May return `nullptr` if `setFrameBuffer()` has not been called.
	 * @note none
	 * @pre none
	 * @post Widget state remains unchanged.
	 * @deprecated none
	 */
	FrameBuffer *getFrameBuffer(void);

	/**
	 * @brief Sets the font reference for text rendering.
	 * @param[in] font Reference to the `Font` object.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Reserved prototype in `BaseDisplay`; use derived class font configuration methods (e.g. `ValueDisplay::setValueConfig()`).
	 * @note none
	 * @pre none
	 * @post none
	 * @deprecated none
	 */
	void setFont(Font &font);

	/**
	 * @brief Sets the target LCD display drawing brush/surface.
	 * @param[in] brush Reference to the target `Brush` display surface.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning The referenced `Brush` instance must remain valid throughout the lifetime of this widget.
	 * @note Stores `&brush` in `mLcd`.
	 * @pre Target LCD driver must be initialized.
	 * @post `mLcd` points to `brush`.
	 * @deprecated none
	 */
	void setLcd(Brush &brush);

	/**
	 * @brief Renders the numerical value onto the display.
	 * @param[in] value The floating-point value to display.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Pure virtual function; must be implemented by derived display widget classes.
	 * @note Renders into `mFb` and blits the updated region onto `mLcd`.
	 * @pre `mFb`, `mLcd`, and `mArea` must be configured before calling.
	 * @post The dynamic value portion of the widget is updated on the LCD.
	 * @deprecated none
	 */
	virtual void drawValue(float value) = 0;

	/**
	 * @brief Renders the static background graphics of the widget onto the display.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2026-11-18
	 * @warning Pure virtual function; must be implemented by derived display widget classes.
	 * @note Should be called once during page initialization or after layout changes.
	 * @pre `mFb`, `mLcd`, and `mArea` must be configured before calling.
	 * @post The static background and labels of the widget are drawn on the LCD.
	 * @deprecated none
	 */
	virtual void drawBackground() = 0;
};

#endif
