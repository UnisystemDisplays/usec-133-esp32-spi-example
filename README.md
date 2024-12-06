OVERVIEW
--------

This repository contains example of [*usec-133-generic-spi-lib*](https://github.com/UnisystemDisplays/usec-133-generic-spi-lib) library integration for [*ESP32 MCUs*](https://www.espressif.com/en/products/socs/esp32) (series of low-cost, low-power system on a chip microcontrollers with integrated Wi-Fi and dual-mode Bluetooth). Please check [*usec-133-generic-spi-lib*](https://github.com/UnisystemDisplays/usec-133-generic-spi-lib) project repository for more info about library itself.

PREREQUISITES
-------------

To get started, please make sure, that all libraries and tools for C/C++ development on *ESP32* are properly installed and configured on your machine - visit [*ESP32 Get Started*](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) for more info.

CONNECTIONS
-----------

For different chip the connections may be different - please check *main/platform.h* file for specific pinouts.

|  USEC module connector | ESP32 |
| ---- | ----- |
| VBUS | 5V    |
| GND  | GND   |
| SCLK | 19    |
| MISO | 25    |
| MOSI | 23    |
| CS   | 21    |
| RDY  | 5     |
| RST  | 18    |

COMPILATION
-----------

[1] Set up the environment variables:

`. /path/to/esp-idf/export.sh`

[2] Clone *usec-133-esp32-spi-example* repository:

`git clone https://github.com/UnisystemDisplays/usec-133-esp32-spi-example.git`

[3] Navigate to your *usec-133-esp32-spi-example* directory and set the target:

`cd usec-133-esp32-spi-example`
`idf.py set-target esp32|esp32s2|esp32c3|esp32s3|esp32c2|esp32c6|esp32h2`

[4] Build the project by running:

`idf.py build`

[5] Flash deivce:

`idf.py -p PORT flash`

GETTING HELP
------------

Please contact Unisystem support - [*<lukasz.skalski@unisystem.com>*](lukasz.skalski@unisystem.com) or [*<jacek.marcinkowski@unisystem.com>*](jacek.marcinkowski@unisystem.com)

LICENSE
-------

See *LICENSE.txt* file for details.
