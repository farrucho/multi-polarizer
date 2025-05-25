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
      // lamp.turnOff();
      // led.turnOn(255);
      // double stepAngle = 3.6;// METER MULTILPLO AQUI PARA NAO CONFUNDIR CONTAGENS DOS ANGULOS
      // double limitAngle = 16;
      // Serial.print("MEDICOES ESPERADAS:");
      // Serial.print(pow((double)limitAngle/stepAngle,5));
      // Serial.print("\n");
      // double current1 = 47;
      // double current2 = 23;
      // double current3 = 47;
      // double current4 = 3;
      // double current5 = 13;
      
      // uint8_t resetDir = HIGH;
      // uint8_t dirToTop = LOW;

      // //reset steppers
      // for(int stepperIndex=0;stepperIndex < 5;stepperIndex++){
      //     while(!switchArray[stepperIndex].isTrigger()) {
      //         // stepperArray[stepperIndex].rotate(1.8,resetDir);
      //         stepperArray[stepperIndex].step(resetDir);
      //     }
      // }

      // current1 = 22.32;
      // stepperArray[0].nsteps(22.32/0.36,dirToTop);
      // current2 = 0;
      // current3 = 0;
      // current4 = 0;
      // current5 = 0;

      
  
      // for(double a1 = 0; a1 <= limitAngle;a1+=stepAngle){
      //     current2 = 0;
      //     while(!switchArray[1].isTrigger()) {
      //         stepperArray[1].step(resetDir);
      //     }
          
      //     for(double a2 = 0; a2 <= limitAngle;a2+=stepAngle){
      //         current3 = 0;
      //         while(!switchArray[2].isTrigger()) {
      //             stepperArray[2].step(resetDir);
      //         }
      //         for(double a3 = 0; a3 <= limitAngle;a3+=stepAngle){
      //             current4 = 0;
      //             while(!switchArray[3].isTrigger()) {
      //                 stepperArray[3].step(resetDir);
      //             }
      //             for(double a4 = 0; a4 <= limitAngle;a4+=stepAngle){
      //                 current5 = 0;
      //                 while(!switchArray[4].isTrigger()) {
      //                     stepperArray[4].step(resetDir);
      //                 }
      //                 for(double a5 = 0; a5 <= limitAngle;a5+=stepAngle){
      //                     delay(10);
      //                     Serial.print(current1);
      //                     Serial.print(";");
      //                     Serial.print(current2);
      //                     Serial.print(";");
      //                     Serial.print(current3);
      //                     Serial.print(";");
      //                     Serial.print(current4);
      //                     Serial.print(";");
      //                     Serial.print(current5);
      //                     Serial.print(";");
      //                     Serial.print(photodiode.getVoltage());
      //                     Serial.print(";\n");
      //                     stepperArray[4].nsteps(stepAngle/0.36,dirToTop);
      //                     current5 += stepAngle;
      //                 }
      //                 stepperArray[3].nsteps(stepAngle/0.36,dirToTop);
      //                 current4 += stepAngle;
      //             }
      //             stepperArray[2].nsteps(stepAngle/0.36,dirToTop);
      //             current3 += stepAngle;
      //         }
      //         stepperArray[1].nsteps(stepAngle/0.36,dirToTop);
      //         current2 += stepAngle;
      //     }
      //     stepperArray[0].nsteps(stepAngle/0.36,dirToTop);
      //     current1 += stepAngle;
      // }

  
	// debuggin 
	//led.turnOn(255);
	//while(1==1){
	//	Serial.println(photodiode.getVoltage());
	//}

    //   // NORMAL PROTOCL
    //   // // // comment esta linha if (DEBUG) Serial.println("1_started");

      // efetuar varrimento
      Serial.print("DAT\n\r");
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

  } PP1;
