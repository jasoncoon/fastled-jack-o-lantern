// Based on the FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    0
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    24
#define BRIGHTNESS  64
#define FPS         60

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*SimplePatternList[])();

// List of patterns to cycle through.  Each is defined as a separate function below.
SimplePatternList gPatterns = { pulse, randomSparks };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

uint8_t hue = random8(86); // red, orange, yellow, or green ("scary colors")

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FPS);

  // do some periodic updates
  EVERY_N_SECONDS( 10 ) {
    nextPattern();  // change patterns periodically
  }

  EVERY_N_SECONDS( 3 ) {
    hue = random8(86); // red, orange, yellow, or green ("scary colors")
  }

  random16_add_entropy( random() );
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void randomSparks()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  uint8_t i = random8(NUM_LEDS);
  leds[i] = CHSV(hue, 255, 255);
}

void pulse()
{
  uint8_t value = beatsin8(240);
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, value));
}

