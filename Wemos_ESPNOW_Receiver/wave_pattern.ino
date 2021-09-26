void wave_pattern(bool Red, bool Green) {
  if (millis() < light_turned_on + initial_interval + pattern_interval) {

  }
  CRGBPalette16 red_palette = German_flag_smooth_gp;
  CRGBPalette16 green_palette = girlcat_gp;
  uint8_t paletteIndex = 0;

  while (millis() < light_turned_on + initial_interval + pattern_interval) {
    EVERY_N_MILLISECONDS(50) {
      if (Red == true && Green == true) {
        // Right Side
        fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, red_palette, 255, LINEARBLEND);
        // Left Side
        fill_palette(leds, int(NUM_LEDS / 2), paletteIndex, 255 / NUM_LEDS, green_palette, 255, LINEARBLEND);
      } else if (Red == true && Green == false) {
        fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, red_palette, 255, LINEARBLEND);
      }  else if (Red == false && Green == true) {
        fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, green_palette, 255, LINEARBLEND);
      }
    }
    EVERY_N_MILLISECONDS(10) {
      paletteIndex++;
    }
    EVERY_N_MILLISECONDS(200) {
      wdt_reset();
    }
    FastLED.show();
  }
}
