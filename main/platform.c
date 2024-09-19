#include <string.h>
#include "platform.h"

/* macro */
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/* globals */
spi_device_handle_t spi_handler;
size_t              spi_max_size;

/*
 * platform_init()
 */
uint8_t
platform_init (usec_ctx *ctx)
{
  esp_err_t ret;

  spi_bus_config_t buscfg = {
    .miso_io_num   = GPIO_MISO,
    .mosi_io_num   = GPIO_MOSI,
    .sclk_io_num   = GPIO_SCLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1
  };

  spi_device_interface_config_t devcfg = {
    .clock_speed_hz = 20000000,
    .mode           = 0,
    .spics_io_num   = -1,
    .queue_size     = 8
  };

  /* init spi module */
  ret = spi_bus_initialize(SPI_DEVICE, &buscfg, SPI_DMA_CH_AUTO);
  if (ret != ESP_OK)
    return USEC_DEV_ERR;

  ret = spi_bus_add_device(SPI_DEVICE, &devcfg, &spi_handler);
  if (ret != ESP_OK)
    return USEC_DEV_ERR;

  /* check spi max transfer size */
  spi_bus_get_max_transaction_len(SPI_DEVICE, &spi_max_size);

  /* init gpio direction */
  gpio_set_direction(GPIO_RDY, GPIO_MODE_INPUT);
  gpio_set_direction(GPIO_RST, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_CS0, GPIO_MODE_OUTPUT);

  /* set default gpio values */
  gpio_set_level(GPIO_RST, GPIO_HIGH);
  gpio_set_level(GPIO_CS0, GPIO_HIGH);

  /* init platform specific data - not used */
  ctx->platform = NULL;

  return USEC_DEV_OK;
}

/*
 * platform_deinit()
 */
uint8_t
platform_deinit (usec_ctx *ctx)
{
  return USEC_DEV_OK;
}

/*
 * platform_delay_ms()
 */
uint8_t
platform_delay_ms (void      *platform,
                   uint16_t   delay_ms)
{
  vTaskDelay(delay_ms/portTICK_PERIOD_MS);

  return USEC_DEV_OK;
}

/*
 * platform_hw_reset()
 */
uint8_t
platform_hw_reset (void *platform)
{
  gpio_set_level (GPIO_RST, GPIO_LOW);
  platform_delay_ms (platform, 100);
  gpio_set_level (GPIO_RST, GPIO_HIGH);
  platform_delay_ms (platform, 200);

  return USEC_DEV_OK;
}

/*
 * platform_spi_write_byte()
 */
uint8_t
platform_spi_write_byte (void    *platform,
                         uint8_t  val)
{
  spi_transaction_t transfer;
  esp_err_t ret;

  memset(&transfer, 0, sizeof(transfer));
  transfer.length    = 1*8;
  transfer.tx_buffer = &val;

  ret = spi_device_polling_transmit(spi_handler, &transfer);
  if (ret != ESP_OK)
    return USEC_DEV_ERR;

  return USEC_DEV_OK;
}

/*
 * platform_spi_write_bytes()
 */
uint8_t
platform_spi_write_bytes (void      *platform,
                          uint8_t   *data,
                          uint32_t   len)
{
  spi_transaction_t transfer;
  uint32_t transfer_left;
  uint32_t transfer_size;

  transfer_left = len;

  while (1)
    {
      /* transfer size */
      transfer_size = MIN(transfer_left, spi_max_size);

      memset (&transfer, 0, sizeof(transfer));
      transfer.length    = transfer_size * 8; /* bits */
      transfer.tx_buffer = data;
      spi_device_polling_transmit(spi_handler, &transfer);

      transfer_left -= transfer_size;
      data += transfer_size;

      if (transfer_left == 0)
        break;
    }

  return USEC_DEV_OK;
}

/*
 * platform_spi_read_byte()
 */
uint8_t
platform_spi_read_byte (void *platform)
{
  spi_transaction_t transfer;
  uint8_t data = 0x00;

  memset (&transfer, 0, sizeof(transfer));
  transfer.length    = 1 * 8; /* bits */
  transfer.tx_buffer = &data;
  transfer.flags     = SPI_TRANS_USE_RXDATA;

  spi_device_polling_transmit (spi_handler, &transfer);

  return transfer.rx_data[0];
}

/*
 * platform_spi_cs_high()
 */
uint8_t
platform_spi_cs_high (void     *platform,
                      uint8_t   cs_num)
{
  gpio_set_level (GPIO_CS0, GPIO_HIGH);
  return USEC_DEV_OK;
}

/*
 * platform_spi_cs_low()
 */
uint8_t
platform_spi_cs_low (void     *platform,
                     uint8_t   cs_num)
{
  gpio_set_level (GPIO_CS0, GPIO_LOW);
  return USEC_DEV_OK;
}

/*
 * platform_gpio_read()
 */
uint8_t
platform_gpio_read (void *platform)
{
  return gpio_get_level (GPIO_RDY);
}
