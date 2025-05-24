#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>


class Stepper {
    uint8_t dir_pin,step_pin,enable_pin;
    double rpm,step_high_delay,step_low_delay,step_frequency;

  public:
        Stepper (double rotpermin, uint8_t steppin, uint8_t dirpin, uint8_t enablepin) {
            dir_pin = dirpin;
            step_pin = steppin;
            enable_pin = enablepin; 

            rpm = rotpermin;
            step_high_delay = 0.1; // [ms], tem que ser > 1.9 microsegundos
            //step_low_delay = 4;
            step_low_delay = 0.3/rpm*1000 - step_high_delay; // [ms]
            //step_frequency = 1/(step_low_delay + step_high_delay); // [Khz]
	    

            if (rpm > 900) { // nao ultrapassar os rpm maximos
                Serial.println("ERROR: STEP_FREQUENCY exceeds 210 kHz!");
                while (1);
            }
        }

        void enable() {  // Declarar pins output
            pinMode(enable_pin, OUTPUT);
            pinMode(dir_pin, OUTPUT);
            pinMode(step_pin, OUTPUT);
            
            digitalWrite(enable_pin, LOW);   // Ativar stepper
        }
        
        void step(uint8_t direction){ // Faz um único step
            digitalWrite(dir_pin, direction);   // Sentido ponteiros relógio

            digitalWrite(step_pin, HIGH);
            delay(step_high_delay);
            digitalWrite(step_pin, LOW);
            delay(step_low_delay);
        }


        void nsteps(int steps, uint8_t direction){ // Faz N-steps
            for(int i = 0; i < steps; i++){
                step(direction);
            }
        }


        void rotate(double degree,uint8_t direction){ // Faz N-steps para rodar alfa-graus
            for(int i = 0; i < degree/1.8; i++){
                step(direction);
            }
        }


        void turnOff(){
            digitalWrite(enable_pin, HIGH);   // Desativar stepper
        }

};

#endif
