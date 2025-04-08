//There shall be no need to change anything in this file, unless you know what you are doing

#include <Arduino.h>
#include "rec.h"
#include "pdefault.h"

#include "p0.h"
P0 PP0;

#if NPROTOCOLS > 0
#include "p1.h"
P1 PP1;
#endif

#if NPROTOCOLS > 1
#include "p2.h"
P2 PP2;
#endif

#if NPROTOCOLS > 2
#include "p3.h"
P3 PP3;
#endif

#if NPROTOCOLS > 3
#include "p4.h"
P4 PP4;
#endif

#if NPROTOCOLS > 4
#include "p5.h"
P5 PP5;
#endif

#if NPROTOCOLS > 5
#include "p6.h"
P6 PP6;
#endif

#if NPROTOCOLS > 6
#include "p7.h"
P7 PP7;
#endif

#if NPROTOCOLS > 7
#include "p8.h"
P8 PP8;
#endif

#if NPROTOCOLS > 8
#include "p9.h"
P9 PP9;
#endif

#if NPROTOCOLS > 9
#include "p10.h"
P10 PP10;
#endif

experiment expr;



void serialEvent() {
  while (Serial.available()) {
    expr.receive (Serial.read());
  }
}

void init_pointers() {
  expr.protocol[0] = &PP0;
#if NPROTOCOLS > 0
  expr.protocol[1] = &PP1;
#endif
#if NPROTOCOLS > 1
  expr.protocol[2] = &PP2;
#endif
#if NPROTOCOLS > 2
  expr.protocol[3] = &PP3;
#endif
#if NPROTOCOLS > 3
  expr.protocol[4] = &PP4;
#endif
#if NPROTOCOLS > 4
  expr.protocol[5] = &PP5;
#endif
#if NPROTOCOLS > 5
  expr.protocol[6] = &PP6;
#endif
#if NPROTOCOLS > 6
  expr.protocol[7] = &PP7;
#endif
#if NPROTOCOLS > 7
  expr.protocol[8] = &PP8;
#endif
#if NPROTOCOLS > 8
  expr.protocol[9] = &PP9;
#endif
#if NPROTOCOLS > 9
  expr.protocol[10] = &PP10;
#endif
}