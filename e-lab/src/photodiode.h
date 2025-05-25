#ifndef PHOTODIODE_H
#define PHOTODIODE_H

#include <Arduino.h>

#include <user_define.h>

class Photodiode {
    uint8_t analog_pin;
    
    public:
        Photodiode(uint8_t pin){
            analog_pin = pin;
        }

        void enable(){
            pinMode(analog_pin, INPUT);
        }
        
        void changeReference(){
            analogRead(analog_pin);  // dummy read
            if (vrefMode == 1){
                // usar 1.1V
                analogReference(INTERNAL1V1);
                // Serial.print("1.1");
            }else if (vrefMode == 2){
                // usar 2.56V
                analogReference(INTERNAL2V56);
                // Serial.print("2.56");
            }else if (vrefMode == 3){
                // usar 5V
                analogReference(DEFAULT);
                // Serial.print("DEFAULT");
            }
            delay(20);
            getVoltage(); // para dar throw e atualizar valores seguintes
        }

        float getVoltage(){
            if (VOLTAGEREF1_1V == true){
                // usar 1.1V
                analogReference(INTERNAL1V1);
            }else{
                analogReference(INTERNAL2V56);
            }
            analogRead(analog_pin);
            int sumAnalogRead = 0;
            // int repetitions = 13;
            int repetitions = 20;
            for(int i = 0; i < repetitions;i++){
                sumAnalogRead+=analogRead(analog_pin);
            }
            // float measuredVoltage = analogValue*1.1/1023;
            float measuredNumber = (float)sumAnalogRead/repetitions;
            return measuredNumber;
        }
};

#endif
