#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include <stdint.h>
#include <stddef.h>

/* esp peripherals */
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "usec_dev.h"

/******************************************************************************/

#ifdef CONFIG_IDF_TARGET_ESP32

#define SPI_DEVICE    HSPI_HOST
typedef enum {
  GPIO_MISO = 25,
  GPIO_MOSI = 23,
  GPIO_SCLK = 19,
  GPIO_CS0  = 21,
  GPIO_RST  = 18,
  GPIO_RDY  = 5
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32S2 || \
              CONFIG_IDF_TARGET_ESP32S3

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 37,
  GPIO_MOSI = 35,
  GPIO_SCLK = 36,
  GPIO_CS0  = 4,
  GPIO_RST  = 5,
  GPIO_RDY  = 6
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32C3 || \
              CONFIG_IDF_TARGET_ESP32C2 || \
              CONFIG_IDF_TARGET_ESP32C6

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 2
  GPIO_MOSI = 7,
  GPIO_SCLK = 6,
  GPIO_CS0  = 9,
  GPIO_RST  = 4,
  GPIO_RDY  = 5
} gpio_name;

#elif defined CONFIG_IDF_TARGET_ESP32H2

#define SPI_DEVICE    SPI2_HOST
typedef enum {
  GPIO_MISO = 0
  GPIO_MOSI = 5,
  GPIO_SCLK = 4,
  GPIO_CS0  = 10,
  GPIO_RST  = 11,
  GPIO_RDY  = 12
} gpio_name;

#endif

typedef enum {
  GPIO_HIGH = 1,
  GPIO_LOW  = 0
} gpio_val;

/******************************************************************************/

uint8_t
platform_init                (usec_ctx  *ctx);

uint8_t
platform_deinit              (usec_ctx  *ctx);

uint8_t
platform_delay_ms            (void      *platform,
                              uint16_t   delay_ms);

uint8_t
platform_hw_reset            (void      *platform);

uint8_t
platform_spi_write_byte      (void      *platform,
                              uint8_t    val);

uint8_t
platform_spi_write_bytes     (void      *platform,
                              uint8_t   *data,
                              uint32_t   len);

uint8_t
platform_spi_read_byte       (void      *platform);

uint8_t
platform_spi_cs_high         (void      *platform,
                              uint8_t    cs_num);

uint8_t
platform_spi_cs_low          (void      *platform,
                              uint8_t    cs_num);

uint8_t
platform_gpio_read           (void      *platform);

#endif /* __PLATFORM_H_ */
