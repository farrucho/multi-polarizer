#ifndef USER_DEFINE_H
#define USER_DEFINE_H

#include <Arduino.h>

#define NARGUMENTS 8
// angulo1emsteps, angulo2emsteps, angulo3emsteps, angulo4emsteps, angulo5emsteps, polarizer a varrer(1 a 5)           , angulolimitevarrimento, PWMLed(0-255)
//                                                                                             , 0-> nao varrer | 1,2,..,5 -> varrer
// cfg 0 0 0 0 0 1 100 255
#define NPROTOCOLS 1
#define BAUDRATE 115200
#define DEBUG 1 // 0 for no debug (pure rec), 1 for more verbose output
#define TIMEOUT 30000 //miliseconds
extern uint8_t dirToTop;
extern uint8_t resetDir;

extern int vrefMode;

#endif
