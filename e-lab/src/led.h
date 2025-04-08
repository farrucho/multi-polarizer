#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {
    uint8_t ledPin;

    public:
        Led(uint8_t pin){
            ledPin = pin;
        }

        void enable(){
            pinMode(ledPin, OUTPUT);
        }

        void turnOn(uint8_t pwmValue){
            // mudar intensidade
            analogWrite(ledPin, pwmValue);
        }

        void turnOff(){
            analogWrite(ledPin, 0);
        }

};

#endif
