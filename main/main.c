#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "usec_dev.h"
#include "img_1_1bpp.h"
#include "img_2_1bpp.h"
#include "img_8bpp.h"

/*
 * eink_task()
 */
static void
eink_task (void *data)
{
  usec_ctx *ctx;
  uint8_t usec_temp;
  uint8_t status;

  printf ("[status] app init...\n\r");

  /* initialize controller */
  ctx = usec_init();
  if (ctx == NULL)
    {
    printf ("[error] cannot initialize e-ink controller\n\r");
      while(1);
    }

  printf ("[status] screen width - %ld [px]\n\r", ctx->dev_width[0]);
  printf ("[status] screen height - %ld [px]\n\r", ctx->dev_height[0]);

  /* read temperature value */
  status = usec_get_temp (ctx, &usec_temp);
  if (status != USEC_DEV_OK)
    {
    printf ("[error] cannot read temperature value\n\r");

      usec_deinit (ctx);
      while(1);
    }
  printf ("[status] screen temperature: %d [degC]\n\r", usec_temp);

  /*
   * DEMO MAINLOOP
   */
  while(1)
    {
      /* cleanup display - UPDATE_MODE_INIT */
      status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_INIT, 1);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot cleanup display\n\r");
        while(1);
      }

      /* upload 8BPP image into internal buffer */
      status = usec_img_upload (ctx, (uint8_t*)img_8bpp, sizeof (img_8bpp),
                  IMG_8BPP, 400, 300, 800, 600);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot upload 'img_8bpp' data\n\r");
        while(1);
      }

      /* update display - UPDATE_MODE_GC16 */
      status = usec_img_update (ctx, 400, 300, 800, 600, UPDATE_MODE_GC16, 1);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot update display\n\r");
        while(1);
      }

      /* delay only for demo purposes */
      vTaskDelay (2000/portTICK_PERIOD_MS);

      /* switch to 1BPP mode */
      status = usec_1bpp_mode (ctx, ENABLE_1BPP);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot switch display to 1BPP mode\n\r");
        while(1);
      }

      /* upload 1BPP image to internal buffer */
      status = usec_img_upload (ctx, (uint8_t*)img_1_1bpp, sizeof(img_1_1bpp),
                  IMG_1BPP, 0, 0, 1600, 1200);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot upload 'img_1_1bpp' data\n\r");
        while(1);
      }

      /* update display */
      status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_GC16, 1);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot update display\n\r");
        while(1);
      }

      /* delay only for demo purposes */
      vTaskDelay (2000/portTICK_PERIOD_MS);

      /* upload 1BPP image to internal buffer */
      status = usec_img_upload (ctx, (uint8_t*)img_2_1bpp,
                  sizeof(img_2_1bpp), IMG_1BPP, 0, 0, 1600, 1200);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot upload 'img_2_1bpp' data\n\r");
        while(1);
      }

      /* update display */
      status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_GC16, 1);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot update display\n\r");
        while(1);
      }

      /* disable 1BPP mode if we want to display 8BPP image */
      status = usec_1bpp_mode (ctx, DISABLE_1BPP);
      if (status != USEC_DEV_OK)
      {
        printf ("[error] cannot disable 1BPP mode\n\r");
        while(1);
      }
    }
}

/*
 * app_main()
 */
void app_main(void)
{
  /* run demo task */
  xTaskCreate (eink_task, "eink-task", 4096, NULL, tskIDLE_PRIORITY, NULL);
}
