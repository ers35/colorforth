#include "colorforth.h"
#include "cf-stdio.h"

String readString;
struct state *state;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  state = colorforth_newstate();

  Serial.println("OK");
}

void loop() {

  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    parse_colorforth(state, c);
  }
}
