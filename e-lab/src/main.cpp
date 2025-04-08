#include <Arduino.h>
#include "rec.h"
#include "user_define.h"


void setup() {
  expr.begin("ELAB_FIVEPOLARIZER");
}

void loop() {
  expr.state_machine();
}