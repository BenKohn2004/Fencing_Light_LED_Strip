#include<ESP8266WiFi.h>
#include<espnow.h>
#include <FastLED.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define MY_NAME   "SLAVE_NODE"
#define NUM_LEDS  50
#define DATA_PIN   4 //D2

// Define the array of leds
CRGB leds[NUM_LEDS];

struct __attribute__((packed)) dataPacket {
  bool sensor1;
  bool sensor2;
  bool sensor3;
  bool sensor4;
};

// Setting to True, removes white lights
bool saber_epee = true;

bool sensor1_prev = false;
bool sensor2_prev = false;
bool sensor3_prev = false;
bool sensor4_prev = false;

bool green_light_turnedon = false;
bool red_light_turnedon = false;
bool white_green_light_turnedon = false;
bool white_red_light_turnedon = false;

// The time in millis() when a light turned on
int light_turned_on = 0;
// Shows that a light has turned on from all lights off
bool a_light_turned_on = false;
// Shows if new data is available for display
bool new_data = false;
// Time Between Lights starting and Pattern
int initial_interval = 2000;
// Time that the pattern plays
int pattern_interval = 3000;
// Fade to Black time after Pattern
int fade_to_black_interval = 500;

DEFINE_GRADIENT_PALETTE( girlcat_gp ) {
  0, 173, 229, 51,
  73, 139, 199, 45,
  140,  46, 176, 37,
  204,   7, 88,  5,
  255,   0, 24,  0
};

DEFINE_GRADIENT_PALETTE( German_flag_smooth_gp ) {
  0,   0,  0,  0,
  63,  42,  0,  0,
  127, 255,  0,  0,
  191, 255, 55,  0,
  255, 255, 255,  0
};

// Outputs for HID
// Green Light
#define led0 14               //D5
// Red Light
#define led1 5                //D1
// Green White Light
#define led2 12                //D6
// Red White Light
#define led3 13                //D7

void setup() {

  Serial.begin(115200);     // initialize serial port


  debugln("");
  debugln("");
  debugln("");
  debug("Initializing...");
  debugln(MY_NAME);
  debug("My MAC address is: ");
  debugln(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if (esp_now_init() != 0) {
    debugln("ESP-NOW initialization failed");
    return;
  }

  esp_now_register_recv_cb(dataReceived);   // this function will get called once all data is sent

  debugln("Initialized.");

  FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);

  //  Uncomment to add on HID Pro Micro Mini
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  debugln("Setup Complete");

  FastLED.clear();
  FastLED.show();
}

void loop() {

  EVERY_N_MILLISECONDS(3000) {
    debug(".");
  }
  EVERY_N_MILLISECONDS(600000) {
    // Clears LEDs every 10 minutes
    FastLED.clear();
    FastLED.show();
  }
  // Assumes no new lights after the initial interval period and runs a custom pattern
  // Only runs a pattern if a colored light is lit
  // Assumes the Lower Part of the LED strip is on the Right Side of the Fencing Strip
  // Runs receiveddata when data is received from connected 8266 chip

  if (a_light_turned_on == true) {
    debugln("Starting the Lighting Pattern");
    if (green_light_turnedon == true && red_light_turnedon == true)
    {
      debugln("The Red and Green Lights are both lit.");
    }
    if (green_light_turnedon == false && red_light_turnedon == true)
    {
      debugln("The Red Light is lit.");
    }
    if (green_light_turnedon == true && red_light_turnedon == false)
    {
      debugln("The Green Light is lit.");
    }

    // Runs the pattern if a_light_turned_on and the initial interval has been exceeded
    if (millis() - light_turned_on > initial_interval && a_light_turned_on == true) {
      // Semi Randomly chooses a pattern
      switch (random(1, 6)) {
        case 1:
          beatsin_composite_pattern(red_light_turnedon, green_light_turnedon);
          break;
        case 2:
          beatsin_simple_pattern(red_light_turnedon, green_light_turnedon);
          break;
        case 3:
          beatsin_simple_double_pattern(red_light_turnedon, green_light_turnedon);
          break;
        case 4:
          sparkle_pattern(red_light_turnedon, green_light_turnedon);
          break;
        case 5:
          wave_pattern(red_light_turnedon, green_light_turnedon);
          break;
      }
      // Clears LEDs after Pattern run
      while (millis() < light_turned_on + initial_interval + pattern_interval + fade_to_black_interval) {
        EVERY_N_MILLISECONDS(200) {
          wdt_reset();
        }
        EVERY_N_MILLISECONDS(10) {
          fadeToBlackBy(leds, NUM_LEDS, 15);
          FastLED.show();
        }
      }
    }
  }

  // Resets the Pattern Lights
  red_light_turnedon = false;
  green_light_turnedon = false;
  a_light_turned_on = false;

  // Clears LEDs
  FastLED.clear();
  FastLED.show();
}

void dataReceived(uint8_t *senderMac, uint8_t *data, uint8_t dataLength) {
  char macStr[18];
  dataPacket packet;

  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);

  debugln();
  debug("Received data from: ");
  debugln(macStr);

  memcpy(&packet, data, sizeof(packet));

  // Green/Red/White_Green/White_Red
  debug("Green/Red/WG/WR: ");
  debug(packet.sensor1);
  debug(packet.sensor4);
  debug(packet.sensor3);
  debugln(packet.sensor2);

  debug("Previous Green/Red/WG/WR: ");
  debug(sensor1_prev);
  debug(sensor4_prev);
  debug(sensor3_prev);
  debugln(sensor2_prev);

  // Tests if the previous data is the same as the current data
  // Only considers Red and Green Lights for new data
  if (sensor1_prev == packet.sensor1 && sensor4_prev == packet.sensor4) {
    new_data = false;
  } else {
    new_data = true;
    debugln("New Data Received");
    // Tests if the New Data is going from No Lights to Lights
    // Only considers Red and Green Lights
    if (sensor1_prev + sensor4_prev == 0) {
      debug("Previous sensor total is: ");
      debug(sensor1_prev + sensor2_prev + sensor3_prev + sensor4_prev);
      debug(sensor1_prev + sensor4_prev);
      debugln(". A light turned on");
      light_turned_on = millis();
      a_light_turned_on = true;
    }
  }

  if (new_data == true && a_light_turned_on == false) {
    debugln("The lights have turned off");
  }
  // Assigns Lights to the New Data and Runs a custom pattern
  // Updates the Lights during the initial Interval Period
  if (new_data == true) {
    debugln("Within the Lighting Cycle");
    if (millis() - light_turned_on < initial_interval) {
      EVERY_N_MILLISECONDS(200) {
        wdt_reset();
      }
      // Turn on Lights Initially
      if (packet.sensor4 == true or red_light_turnedon == true) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        red_light_turnedon = true;
      } else if ((packet.sensor2 == true or white_red_light_turnedon == true) && saber_epee == false) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        white_red_light_turnedon = true;
      }
      else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
      }
      if (packet.sensor1 == true or green_light_turnedon == true) {
        fill_solid(leds, int(NUM_LEDS / 2), CRGB::Green);
        green_light_turnedon = true;
      } else if ((packet.sensor3 == true or white_green_light_turnedon == true) && saber_epee == false) {
        fill_solid(leds, int(NUM_LEDS / 2), CRGB::White);
        white_green_light_turnedon = true;
      }
      else {
        fill_solid(leds, int(NUM_LEDS / 2), CRGB::Black);
      }
      FastLED.show();
      new_data = false;
    }
  }

  // Sets previous sensor data to current sensor data
  sensor1_prev = packet.sensor1;
  sensor2_prev = packet.sensor2;
  sensor3_prev = packet.sensor3;
  sensor4_prev = packet.sensor4;

  // Red and Green Lights are used for the Pattern
  green_light_turnedon = packet.sensor1;
  red_light_turnedon = packet.sensor4;

  // Used to connect to HID
  // Receiving a 0 outputs Low
  if (packet.sensor1 == 0)  digitalWrite(led0, LOW);
  if (packet.sensor2 == 0)  digitalWrite(led1, LOW);
  if (packet.sensor3 == 0)  digitalWrite(led2, LOW);
  if (packet.sensor4 == 0)  digitalWrite(led3, LOW);

  // Receiving a 1 outputs High
  if (packet.sensor1 == 1)  digitalWrite(led0, HIGH);
  if (packet.sensor2 == 1)  digitalWrite(led1, HIGH);
  if (packet.sensor3 == 1)  digitalWrite(led2, HIGH);
  if (packet.sensor4 == 1)  digitalWrite(led3, HIGH);
}
