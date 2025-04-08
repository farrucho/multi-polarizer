#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>

class Switch {
    uint8_t endstop_pin;
    int endstopState;
    
    public:
        Switch(uint8_t pin){
            endstop_pin = pin;
        }

        void enable(){
            pinMode(endstop_pin, INPUT_PULLUP);
        }

        bool isTrigger(){
            endstopState = digitalRead(endstop_pin);
            return (endstopState == LOW);
        }
};

#endif