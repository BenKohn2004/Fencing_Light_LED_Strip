# Fencing_Light_LED_Strip
Fencing_Light_LED_Strip for SG and Leon Paul Scoreboxes

The output of a Leon Paul or St George scorebox is done through 8 pins, either RJ45 or 8 pin din connector:
https://www.leonpaul.com/content/leon-paul-wireless-fencing-instructions.pdf

The power connections are used with pins that denote each lamp color: Green/Red/White Green/White Red/Yellow Green/Yellow Red.

This project uses the connections to four relays which connect the Transmitting ESP8266 Wemos input pins to ground creating a signal. The Transmitting ESP8266 Wemos then sends the signal to a Receiving ESP8266 Wemos using the Arduino ESPN-NOW library. The receiving ESP8266 Wemos then uses the FastLED library to send data signals to an individually addressable LED strip.

The receiving ESP8266 Wemos is programmed to turn on the Green and/or Red Lights for an initial period, about 2 seconds, then runs a randomly selected pattern for about three seconds over the entire LED strip based on the illuminated score lights.

The ESP8266 requires the additional board manager JSON: http://arduino.esp8266.com/stable/package_esp8266com_index.json and was run as LOLIN(WEMOS) D1 R2 and mini
