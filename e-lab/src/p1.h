//Protocol 1 class
//Defining every function is opcional, as they can be defined as common for all protocols
//Defining them here will override the default only for this protocol

#include "rec.h"

#include "led.h"
#include "photodiode.h"
#include "stepper.h"
#include "switch.h"

#include "user_define.h"

Switch switch_E0(3);
Switch switch_X(2);
Switch switch_E1(14);
Switch switch_Y(15);
Switch switch_Z(18);

double RPM_GLOBAL = 100;

Stepper stepper_E0(RPM_GLOBAL,26,28,24);
Stepper stepper_E1(RPM_GLOBAL,36,34,30);
Stepper stepper_X(RPM_GLOBAL,A0,A1,38);
Stepper stepper_Y(RPM_GLOBAL,A6,A7,A2);
Stepper stepper_Z(RPM_GLOBAL,46,48,A8);

// StepperOLD stepperArray[5] = {stepper_E0,stepper_E1,stepper_X,stepper_Y,stepper_Z};
// Stepper stepperArray[5] = {stepper_E0,stepper_Z,stepper_E1,stepper_Y,stepper_X};
// Switch switchArray[5] = {switch_E0,switch_Z,switch_E1,switch_Y,switch_X};

// Stepper stepperArray[5] = {stepper_X,stepper_Y,stepper_E1,stepper_Z,stepper_E0};
// Switch switchArray[5] = {switch_X,switch_Y,switch_E1,switch_Z,switch_E0};

// Stepper stepperArray[5] = {stepper_E0,stepper_Z,stepper_E1,stepper_Y,stepper_X};
Stepper stepperArray[5] = {stepper_X, stepper_Y, stepper_E1, stepper_Z, stepper_E0};
//Switch switchArray[5] = {switch_E0,switch_Z,switch_E1,switch_Y,switch_X};
Switch switchArray[5] = {switch_X, switch_Y, switch_E1, switch_Z, switch_E0};


Photodiode photodiode(A9);

Led led(44);
Led lamp(9);

extern class P1: public proto {
  public:
    void led_on(){
      led.enable();
      led.turnOn(expr.auxiliar_param[0]);
    }
    void led_off(){
      led.turnOff();
    }
    void lamp_on(){
      lamp.enable();
      lamp.turnOn(expr.auxiliar_param[0]);
    }
    void lamp_off(){
      lamp.turnOff();
    }
    void set_maxstep(){
      expr.MAXIMUM_STEP = expr.auxiliar_param[0];
    }

    bool isSwitchTrigger(int n){
      return switchArray[n].isTrigger();
    }

    void move_forward(){
      // uint8_t dirToTop = LOW;
      stepperArray[expr.auxiliar_param[0]-1].enable();
      stepperArray[expr.auxiliar_param[0]-1].nsteps(expr.auxiliar_param[1],dirToTop);
      stepperArray[expr.auxiliar_param[0]-1].turnOff();
    }

    void move_backward(){
      // uint8_t resetDir = HIGH;
      stepperArray[expr.auxiliar_param[0]-1].enable();
      stepperArray[expr.auxiliar_param[0]-1].nsteps(expr.auxiliar_param[1],resetDir);
      stepperArray[expr.auxiliar_param[0]-1].turnOff();
    }

    void go_to_origin(){
      // uint8_t resetDir = HIGH;
      stepperArray[expr.auxiliar_param[0]-1].enable();
      while(!switchArray[expr.auxiliar_param[0]-1].isTrigger()) {
        stepperArray[expr.auxiliar_param[0]-1].step(resetDir);
      }
      stepperArray[expr.auxiliar_param[0]-1].turnOff();
    }

    float photodiode_value(){
      return photodiode.getVoltage();
    }

    void update_Vref(){
      photodiode.changeReference();
    }
    

    void stopping() {
      // if (DEBUG) Serial.println("1_stopping");

      stepper_E0.turnOff();
      stepper_E1.turnOff();
      stepper_X.turnOff();
      stepper_Y.turnOff();
      stepper_Z.turnOff();

      led.turnOff();
      lamp.turnOff();   
    }

    void configuring() {
      // if (DEBUG) Serial.println("1_configuring");

      // ter a certeza que esta tudo ligado para configurar
      switch_E0.enable();
      switch_E1.enable();
      switch_X.enable();
      switch_Y.enable();
      switch_Z.enable();
      
      stepper_E0.enable();
      stepper_E1.enable();
      stepper_X.enable();
      stepper_Y.enable();
      stepper_Z.enable();
      
      photodiode.enable();
      
      led.enable();
      lamp.enable();

      // acender o led
      // if(expr.param[0] == 1){
      lamp.turnOn(255);      
      led.turnOn(expr.param[7]);
      // }else{
      //   led.turnOff();
      // }

      // reset a todos os gears
      // uint8_t resetDir = HIGH;
      for(int stepperIndex=0;stepperIndex < 5;stepperIndex++){
          while(!switchArray[stepperIndex].isTrigger()) {
              // stepperArray[stepperIndex].rotate(1.8,resetDir);
              stepperArray[stepperIndex].step(resetDir);
          }
      }
      // acabou o reset dos gear
      

      // rodar polarizadores
      for (int j = 0; j < 5; ++j)
      {
        // uint8_t dirToTop = LOW;
        if(expr.param[j] >= 0 && expr.param[j] <= expr.MAXIMUM_STEP){
            stepperArray[j].nsteps(expr.param[j],dirToTop);
            // polarizer j rodado
        }else{
            Serial.println("ERR dados incorretos não vai rodar polarizadores");
            // nao vai rodar dados incorretos
        }
      }
    }

    void starting() {
      // if (DEBUG) Serial.println("1_starting");

      // ter a certeza que esta tudo ligado para começar

      switch_E0.enable();
      switch_E1.enable();
      switch_X.enable();
      switch_Y.enable();
      switch_Z.enable();
      
      stepper_E0.enable();
      stepper_E1.enable();
      stepper_X.enable();
      stepper_Y.enable();
      stepper_Z.enable();
      
      photodiode.enable();
      
      led.enable();
      led.turnOn(expr.param[7]);
      lamp.enable();
      lamp.turnOn(255);    
      delay(3000);
}

  void reseting() {
    // if (DEBUG) Serial.println("1_starting");

    // ter a certeza que esta tudo ligado para começar

    switch_E0.enable();
    switch_E1.enable();
    switch_X.enable();
    switch_Y.enable();
    switch_Z.enable();
    
    stepper_E0.enable();
    stepper_E1.enable();
    stepper_X.enable();
    stepper_Y.enable();
    stepper_Z.enable();
    
    photodiode.enable();
    
    // reset a todos os gears
    // uint8_t resetDir = HIGH;
    for(int stepperIndex=0;stepperIndex < 5;stepperIndex++){
        while(!switchArray[stepperIndex].isTrigger()) {
            // stepperArray[stepperIndex].rotate(1.8,resetDir);
            stepperArray[stepperIndex].step(resetDir);
        }
    }

  }

    void started() {
    // FITTING BRUTEFORCE ALGORITHM
      bool bruteForce = false;
      if(bruteForce == true){
        led.turnOn(255);
        
        // first polarizer sweepping
        this->reseting();

        
        double initial_angle = 25.2;
        double max_angle = 64.8;
        
        // sweep_1
        // set all to initial_angle
//        for(int i = 0; i < 5; i++){
//          stepperArray[i].nsteps(initial_angle/0.36,dirToTop);
//        }

	// corrigir ruido inicial
//	for(int i = 0; i < 10; i++){
//		photodiode.getVoltage(100);
//		delay(10);	
//	}

//        for(double current_angle = initial_angle; current_angle <= max_angle; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage(100));
//          Serial.print("\r\n");
//          for(int i = 0; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }
//
        // sweep_2
        double first_max = 27.36;
//        stepperArray[0].nsteps(first_max/0.36,dirToTop);
//        for(int i = 1; i < 5; i++){
//          stepperArray[i].nsteps(initial_angle/0.36,dirToTop);
//        }
//        
//        // corrigir ruido inicial
//        for(int i = 0; i < 10; i++){
//                photodiode.getVoltage(100);
//                delay(10);      
//        }

//        for(double current_angle = initial_angle; current_angle <= max_angle; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage(100));
//          Serial.print("\r\n");
//          for(int i = 1; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }

        // sweep_3
        double second_max = 31.32;
//        stepperArray[0].nsteps(first_max/0.36,dirToTop);
//        stepperArray[1].nsteps(second_max/0.36,dirToTop);
//        for(int i = 2; i < 5; i++){
//          stepperArray[i].nsteps(initial_angle/0.36,dirToTop);
//        }
//
//        // corrigir ruido inicial
//        for(int i = 0; i < 10; i++){
//                photodiode.getVoltage(100);
//                delay(10);      
//        }
//        
//        for(double current_angle = initial_angle; current_angle <= max_angle; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage(100));
//          Serial.print("\r\n");
//          for(int i = 2; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }
//        
        // sweep_4
        double third_max = 29.88;
//        stepperArray[0].nsteps(first_max/0.36,dirToTop);
//        stepperArray[1].nsteps(second_max/0.36,dirToTop);
//        stepperArray[2].nsteps(third_max/0.36,dirToTop);
//        for(int i = 3; i < 5; i++){
//          stepperArray[i].nsteps(initial_angle/0.36,dirToTop);
//        }
        
        // corrigir ruido inicial
//        for(int i = 0; i < 10; i++){
//                photodiode.getVoltage(100);
//                delay(10);      
//        }
//
//
//        for(double current_angle = initial_angle; current_angle <= max_angle; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage(100));
//          Serial.print("\r\n");
//          for(int i = 3; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }

        // sweep_5
//        double forth_max = 29.52;
//        stepperArray[0].nsteps(first_max/0.36,dirToTop);
//        stepperArray[1].nsteps(second_max/0.36,dirToTop);
//        stepperArray[2].nsteps(third_max/0.36,dirToTop);
//        stepperArray[3].nsteps(forth_max/0.36,dirToTop);
//        for(int i = 4; i < 5; i++){
//          stepperArray[i].nsteps(initial_angle/0.36,dirToTop);
//        }
        
        // corrigir ruido inicial
//        for(int i = 0; i < 10; i++){
//                photodiode.getVoltage(100);
//                delay(10);      
//        }

//        for(double current_angle = initial_angle; current_angle <= max_angle; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage());
//          Serial.print("\r\n");
//          for(int i = 4; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }

        // sweep_full
//        double fifth_max = 29.88;
//        stepperArray[0].nsteps(first_max/0.36,dirToTop);
//        stepperArray[1].nsteps(second_max/0.36,dirToTop);
//        stepperArray[2].nsteps(third_max/0.36,dirToTop);
//        stepperArray[3].nsteps(forth_max/0.36,dirToTop);
//        stepperArray[4].nsteps(fifth_max/0.36,dirToTop);

//        for(double current_angle = first_max; current_angle <= 252; current_angle+=0.36) {
//          photodiode.getVoltage();
//          photodiode.getVoltage();
//          delay(10);
//          photodiode.getVoltage();
//          Serial.print(current_angle);
//          Serial.print(" ");
//          Serial.print(photodiode.getVoltage());
//          Serial.print("\n");
//          for(int i = 0; i < 5; i++){
//            stepperArray[i].step(dirToTop);
//          }
//        }
    
      }else{
        // efetuar varrimento
        Serial.print("DAT\n\r");
	
	// corrigir ruido inicial
        for(int i = 0; i < 10; i++){
                photodiode.getVoltage(100);
                delay(10);      
        }

        // if (DEBUG) Serial.println("\n");
  
        // uint8_t resetDir = LOW;
        
        if(expr.param[5] == 0){
            for(int i=0;i<10;i++){
              delay(4);
              Serial.print("\n");
              Serial.print(i);
              Serial.print("\t");
              Serial.print("NaN");
              Serial.print("\t");
              Serial.print(photodiode.getVoltage());
        Serial.print("\r");
            }
        }
  
        if(expr.param[5] > 0 && expr.param[5] < 6){
          int initialSteps = expr.param[(int)expr.param[5]-1]; // step inicial do varrimento, pegar no numero (1-5) polarizer a varrer e subtrair 1 para obter index correto dos parametros
          for(int currentStep = initialSteps; currentStep < expr.param[6];currentStep=currentStep+1){
            stepperArray[expr.param[5]-1].step(dirToTop);
            delay(30);
            //  #1   |   348º   |    503mv    |    
            Serial.print("\n");
            Serial.print(currentStep-initialSteps+1);
            Serial.print("\t");
            Serial.print(currentStep);
            Serial.print("\t");
            Serial.print(photodiode.getVoltage());
            Serial.print("\r");
            // if(currentStep < expr.param[7] - 1){
            //   Serial.print("\n");
            // }
          }
        }
        
        // Serial.print("\r");
        // if (DEBUG) Serial.println("\n");
  
        //End data transmission
        Serial.print("END\r");
        // if (DEBUG) Serial.println("\n");
        delay(1000);
        led.turnOff();
        lamp.turnOff();
      }

    }

  } PP1;
