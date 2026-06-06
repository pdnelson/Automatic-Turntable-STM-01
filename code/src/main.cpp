#include <TurntableState.h>

TurntableState* state;

void setup() {
  state = new TurntableState();
}

void loop() {
  state->monitor();
}
