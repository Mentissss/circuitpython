// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

#define MICROPY_HW_BOARD_NAME "Adafruit TRRS Trinkey M0"
#define MICROPY_HW_MCU_NAME "samd21e18"

#define MICROPY_HW_NEOPIXEL (&pin_PA01)
#define MICROPY_HW_NEOPIXEL_COUNT (1)

#define DEFAULT_I2C_BUS_SCL (&pin_PA09)
#define DEFAULT_I2C_BUS_SDA (&pin_PA08)

#define IGNORE_PIN_PA00     1

#define IGNORE_PIN_PA10     1
#define IGNORE_PIN_PA11     1
// no PA12
// no PA13
#define IGNORE_PIN_PA14     1
#define IGNORE_PIN_PA15     1
#define IGNORE_PIN_PA16     1
#define IGNORE_PIN_PA17     1
#define IGNORE_PIN_PA18     1
#define IGNORE_PIN_PA19     1
// no PA20
// no PA21
#define IGNORE_PIN_PA22     1
#define IGNORE_PIN_PA23     1
// USB is always used internally so skip the pin objects for it.
#define IGNORE_PIN_PA24     1
#define IGNORE_PIN_PA25     1
// no PA26
#define IGNORE_PIN_PA27     1
#define IGNORE_PIN_PA28     1
// no PA29
#define IGNORE_PIN_PA30     1
#define IGNORE_PIN_PA31     1

// A couple Learn examples do `array.array('d', ...)` so enable it.
#define MICROPY_PY_DOUBLE_TYPECODE 1
