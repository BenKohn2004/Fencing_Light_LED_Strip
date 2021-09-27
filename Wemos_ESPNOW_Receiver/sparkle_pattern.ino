void sparkle_pattern(bool Red, bool Green) {
  if (millis() < light_turned_on + initial_interval + pattern_interval)
  {
    debugln("Running Sparkle Pattern.");
  }

  uint8_t colorIndex[NUM_LEDS];
  CRGBPalette16 red_palette = German_flag_smooth_gp;
  CRGBPalette16 green_palette = girlcat_gp;
  while (millis() < light_turned_on + initial_interval + pattern_interval) {
    EVERY_N_MILLISECONDS(50) {
      if (Red == true && Green == true) {
        // Left Side
        leds[random8(0, int(NUM_LEDS / 2) - 1)] = ColorFromPalette(green_palette, random8(), 255, LINEARBLEND);
        // Right Side
        leds[random8(int(NUM_LEDS / 2), NUM_LEDS - 1)] = ColorFromPalette(red_palette, random8(), 255, LINEARBLEND);
      } else if (Red == true && Green == false) {
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(red_palette, random8(), 255, LINEARBLEND);
      }  else if (Red == false && Green == true) {
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(green_palette, random8(), 255, LINEARBLEND);
      }
    }
    EVERY_N_MILLISECONDS(200) {
      wdt_reset();
    }
    // Fade all LEDs down by 1 in brightness each time this is called
    fadeToBlackBy(leds, NUM_LEDS, 1);
    FastLED.show();
  }
}
