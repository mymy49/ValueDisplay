# ValueDisplay

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Doxygen Build](https://github.com/mymy49/ValueDisplay/actions/workflows/doxygen.yml/badge.svg)](https://github.com/mymy49/ValueDisplay/actions)
[![Doxygen API Docs](https://img.shields.io/badge/docs-API%20Reference-brightgreen.svg)](https://mymy49.github.io/ValueDisplay/)

Modular visual display widgets and instrument gauges built on the **TurtleShip** GUI framework for **yss OS**.

---

## 1. Overview & Key Features

The **ValueDisplay** module provides a collection of reusable, flicker-free UI widgets designed for embedded instruments, dashboard displays, and numeric monitoring systems. Leveraging an off-screen double-buffering architecture, it renders graphics into a working `FrameBuffer` before blitting to the target display, eliminating visible screen tearing and flicker.

### Key Highlights
- **Off-Screen Double-Buffering & Selective Redraw**:
  - Graphical elements are composed inside a working `FrameBuffer` (e.g. `FrameBufferRgb565LE`) and blitted to the physical display via a `Brush`.
  - Only modified sub-areas (such as the numerical readout box or progress bar fill) are transferred on dynamic updates, minimizing bus traffic and maximizing update frame rates.
- **Comprehensive Widget Suite**:
  - **`BaseDisplay`**:
    - Abstract foundation providing bounding box geometry (`Area`), background colors (`Color`), and surface binding (`setFrameBuffer()`, `setLcd()`).
    - Standardized rendering lifecycle: `drawBackground()` and `drawValue()`.
  - **`ValueDisplay`**:
    - Formatted floating-point numeric readout with configurable `sprintf` format specifiers (e.g. `"%3.1f"`, `"%02.0f"`).
    - Customizable title label and measurement unit suffix (e.g. `"km/h"`, `"V"`, `"°C"`).
    - Interactive numeric editing mode with cursor blinking visualization (`drawValueWithCursor()`).
  - **`AlertValueDisplay`**:
    - Extends `ValueDisplay` with tri-state threshold-based alerting.
    - Automatically switches text color across **Low Alert Zone** (`value < lowThreshold`), **Normal Zone**, and **High Alert Zone** (`value >= highThreshold`).
  - **`ProgressDisplay`**:
    - Horizontal percentage progress bar indicator (0.0% ~ 100.0%).
    - Inset track groove, configurable fill/track colors, and automatic boundary clamping.
  - **`RpmBarDisplay`**:
    - Automotive-style engine tachometer gauge.
    - Dual-bar visual design: static segmented top guideline bar divided into 4 operating zones (Idle, Normal, Warning, Over-rev) and dynamic filling bar underneath.

---

## 2. System Specifications & Hardware Environment

### Supported Targets & Tools
- **Target Microcontrollers & Development Boards**:
  - Nuvoton NuMicro® M251 series (e.g., **NuMaker-M251KG** / Cortex®-M23).
  - Any microcontroller platform supported by the **yss OS** framework.
- **Display Compatibility**:
  - Any display controller supported by the **TurtleShip** graphics engine (e.g., ST7789V, ILI9341, ST7735).
  - Validated on the **2.8-inch TFT Touch Shield Rev2.1** (240x320 RGB565).
- **Required Software & Toolchains**:
  - Toolchain: ARM GNU Toolchain or CrossWorks for ARM (v5.x or higher).
    - **Note on CrossWorks for ARM**: CrossWorks for ARM does not natively support the Nuvoton M251 series. You must install the custom [CrossworksForARM_Nuvoton_M251 Support Package](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) before building M251 projects.
  - Operating System: [yss OS](https://github.com/mymy49/yss) (embedded RTOS & HAL).
  - Graphics Framework: [TurtleShip](https://github.com/mymy49/TurtleShip) (providing `FrameBuffer`, `Brush`, `Area`, `Color`).
  - Language: C++11 or higher.

---

## 3. Installation & Project Setup

### Cloning with Submodules
To clone a project that includes this repository as a submodule:
```bash
git clone --recursive https://github.com/mymy49/NuMaker-M251KG_example.git
```

To add this repository as a submodule into your existing yss OS application:
```bash
git submodule add https://github.com/mymy49/ValueDisplay.git module/ValueDisplay
```

### Prerequisites: Nuvoton M251 Support Package for CrossWorks for ARM
CrossWorks for ARM does not natively provide device support for Nuvoton NuMicro® M251 microcontrollers in its default package manager. A custom device support package must be installed beforehand:

1. Clone or download the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) repository.
2. Open the package project file (`.hzp`) in CrossWorks for ARM and build it (`F7` or `Build -> Build Project`) to generate the package file (`.hzq`).
3. In CrossWorks for ARM, navigate to **Tools -> Packages -> Manually Install Packages...** and select the generated `.hzq` package file.
4. Once installed, M251 target device definitions, CMSIS headers, and project templates will be fully available in CrossWorks for ARM.

### Build and Firmware Flashing
- **CrossWorks for ARM**:
  1. Ensure the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) package has been installed.
  2. Open the project solution file (e.g., `Example.hzp`).
  3. Select target configuration (`Example THUMB Release` or `Example THUMB Debug`).
  4. Build solution via `F7` or `Build -> Build Solution`.
  5. Connect the board via Nu-Link ICE and flash with `Ctrl+D` (`Target -> Download`).
- **Command Line (pyOCD / OpenOCD)**:
  ```bash
  # Flash prebuilt binary using pyocd
  pyocd flash -t m251kg firmware.bin
  ```

---

## 4. Quick Start & Example Code

Below are practical examples demonstrating how to use `ProgressDisplay`, `ValueDisplay`, and `AlertValueDisplay` with yss OS and TurtleShip:

### 1. Horizontal Progress Bar (`ProgressDisplay`)
*(From touch calibration in `page_touchCalibration.cpp`)*
```cpp
#include <bsp.h>
#include <ValueDisplay/ProgressDisplay.h>

ProgressDisplay progressBar;

void initializeCalibrationProgress(void)
{
    // 1. Associate off-screen buffer and LCD drawing surface
    progressBar.setFrameBuffer(brush);
    progressBar.setLcd(lcd);

    // 2. Configure bounding area and colors
    progressBar.setArea({50, 200, 140, 30});
    progressBar.setBgColor(0x30, 0x30, 0x30);
    progressBar.setColor(Color{0x30, 0xFF, 0x30}, Color{0x80, 0x80, 0x80});

    // 3. Render initial background and empty progress bar (0%)
    progressBar.drawBackground();
    progressBar.drawValue(0);
}

void updateCalibrationProgress(float percent)
{
    // Dynamically update progress (automatically clamped between 0 and 100)
    progressBar.drawValue(percent);
}
```

### 2. Tri-State Alert Sensor Readout (`AlertValueDisplay`)
```cpp
#include <bsp.h>
#include <ValueDisplay/AlertValueDisplay.h>
#include <font/Ubuntu_14_B.h>
#include <font/Ubuntu_20_B.h>

AlertValueDisplay coolantGauge;

void setupCoolantGauge(void)
{
    // 1. Configure geometry and drawing surfaces
    coolantGauge.setFrameBuffer(brush);
    coolantGauge.setLcd(lcd);
    coolantGauge.setArea({10, 40, 130, 60});
    coolantGauge.setBgColor(0x20, 0x20, 0x20);

    // 2. Configure title label ("COOLANT" at top-left)
    coolantGauge.setTitleConfig(Font_Ubuntu_14_B, {5, 5}, Color{0xAA, 0xAA, 0xAA}, "COOLANT");

    // 3. Configure numeric value sub-box
    coolantGauge.setValueConfig(Font_Ubuntu_20_B, {5, 25, 80, 30}, {2, 5},
                                Color{0x00, 0xFF, 0x00}, Color{0x10, 0x10, 0x10}, "%3.0f");

    // 4. Configure measurement unit ("'C")
    coolantGauge.setUnitConfig(Font_Ubuntu_14_B, {90, 25, 35, 30}, Color{0xFF, 0xFF, 0xFF}, "'C");

    // 5. Configure alert thresholds: Low < 50°C (Blue), High >= 105°C (Red)
    coolantGauge.setAlertValue(50.0f, 105.0f);
    coolantGauge.setAlertColor(Color{0x00, 0x80, 0xFF}, Color{0x00, 0xFF, 0x00}, Color{0xFF, 0x00, 0x00});

    // 6. Draw static background, title, and unit
    coolantGauge.drawBackground();
}

void updateCoolantTemperature(float temperature)
{
    // Only the numeric sub-box is redrawn with dynamic color switching
    coolantGauge.drawValue(temperature);
}
```

### 3. Engine Tachometer Bar (`RpmBarDisplay`)
```cpp
#include <bsp.h>
#include <ValueDisplay/RpmBarDisplay.h>

RpmBarDisplay tachometer;

void setupTachometer(void)
{
    tachometer.setFrameBuffer(brush);
    tachometer.setLcd(lcd);
    tachometer.setArea({20, 30, 200, 35});
    tachometer.setBgColor(Color{0x00, 0x00, 0x00});
    tachometer.setBarBgColor(Color{0x25, 0x25, 0x25});

    // Configure RPM full scale and operating zones
    tachometer.setMaxRpm(8000);
    tachometer.setZone(1000, 5500, 6800); // Idle, Warning, Redline limits
    tachometer.setToplineThickness(3);

    tachometer.drawBackground();
}

void updateEngineRpm(float currentRpm)
{
    tachometer.drawValue(currentRpm);
}
```

---

## 5. Documentation & References

- **Doxygen API Reference**: [https://mymy49.github.io/ValueDisplay/](https://mymy49.github.io/ValueDisplay/)
- **TurtleShip GUI Framework**: [https://mymy49.github.io/TurtleShip/](https://mymy49.github.io/TurtleShip/)
- **Doxygen Main Page**: This `README.md` file serves as the main page of the generated Doxygen documentation (`USE_MDFILE_AS_MAINPAGE = README.md`).

---

## 6. License & Author

- **Author**: Yoon-Ki Hong
- **License**: MIT License. See [LICENSE](LICENSE) for full terms and conditions.
