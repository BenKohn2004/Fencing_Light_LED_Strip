void beatsin_simple_double_pattern(bool Red, bool Green) {
  if (millis() < light_turned_on + initial_interval + pattern_interval) {
    debugln("Running Beat Sine Pattern.");
  }
  while (millis() < light_turned_on + initial_interval + pattern_interval) {
    EVERY_N_MILLISECONDS(200) {
      wdt_reset();
    }
    // Waves for LED position
    uint8_t posBeat  = beatsin8(20, 0, NUM_LEDS - 1, 0, 0);
    uint8_t posBeat3 = beatsin16(20, 0, NUM_LEDS - 1, 0, 127);

    // Wave for LED color
    uint8_t colBeat_Green  = beatsin8(45, 80, 130, 0, 0); // Green
    uint8_t colBeat_Red  = beatsin8(45, 0, 45, 0, 0); // Red

    if (Red == true && Green == true) {
      // Left Side
      leds[(posBeat) / 2]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat3) / 2]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat) / 2 * (-1) + NUM_LEDS / 2]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat3) / 2 * (-1) + NUM_LEDS / 2]  = CHSV(colBeat_Green, 255, 255);
      // Right Side
      leds[(posBeat) / 2  + NUM_LEDS / 2]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat3) / 2 + NUM_LEDS / 2]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat) / 2 * (-1) + NUM_LEDS]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat3) / 2 * (-1) + NUM_LEDS]  = CHSV(colBeat_Red, 255, 255);
    } else if (Red == true && Green == false) {
      leds[(posBeat)]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat3)]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat) * (-1) + NUM_LEDS]  = CHSV(colBeat_Red, 255, 255);
      leds[(posBeat3) * (-1) + NUM_LEDS]  = CHSV(colBeat_Red, 255, 255);
    }  else if (Red == false && Green == true) {
      leds[(posBeat)]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat3)]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat) * (-1) + NUM_LEDS]  = CHSV(colBeat_Green, 255, 255);
      leds[(posBeat3) * (-1) + NUM_LEDS]  = CHSV(colBeat_Green, 255, 255);
    }
    fadeToBlackBy(leds, NUM_LEDS, 5);
    FastLED.show();
  }
}
