//There shall be no need to change anything in this file, unless you know what you are doing

#ifndef REC_H
#define REC_H

#include <string.h>
#include <Arduino.h>
#include "user_define.h"

#define ID_SIZE  30
#define RBUF_SIZE 80
#define BAUDRATE 115200




extern void serialEvent();
extern void init_pointers();

class proto {
  public:
    virtual void stopping();
    virtual void configuring();
    virtual void starting();
    virtual void started();
    virtual void reseting();

    virtual void led_on();
    virtual void led_off();
    virtual void lamp_on();
    virtual void lamp_off();

    virtual void set_maxstep();
    virtual bool isSwitchTrigger(int n);
    
    virtual void move_forward();
    virtual void move_backward();
    virtual void go_to_origin();
    virtual float photodiode_value();
};


extern class experiment {
  private:

    enum {
      STOPPING,
      STOPPED,
      CONFIGURING,
      CONFIGURED,
      STARTING,
      STARTED,
      RESETING,
      RESETED
    } state;

    const char state_name[8][12] = {
      "STOPPING",
      "STOPPED",
      "CONFIGURING",
      "CONFIGURED",
      "STARTING",
      "STARTED",
      "RESETING",
      "RESETED"
    };

    char id[ID_SIZE + 1];
    char rbuf[RBUF_SIZE];
    int command_received, stop, reset;

public:
    unsigned int p_run; //current protocol
    int param[NARGUMENTS];
    int auxiliar_param[NARGUMENTS];
    proto *protocol[NPROTOCOLS + 1];
    
    //experiment vars
    int MAXIMUM_STEP = 900;
    
    

    void begin (char *a) {
      strncpy(id, a, ID_SIZE);
      id[ID_SIZE] = '\0';

      state = STOPPED;
      p_run = 1;

      Serial.begin(BAUDRATE);
      while (!Serial);
      // if (DEBUG) Serial.println("ELAB_FIVEPOLARIZER");

      init_pointers();

    }

    void receive (char a) {
      static int j = 0;

      rbuf[j] = a;

      if ((rbuf[0] == 's') &&
          (rbuf[1] == 't') &&
          (rbuf[2] == 'p') &&
          (rbuf[3] == '\r'))
        stop = 1;
      else if ((rbuf[0] == 'r') &&
               (rbuf[1] == 's') &&
               (rbuf[2] == 't') &&
               (rbuf[3] == '\r'))
        reset = 1;
      else if (a == '\r') {
        command_received = j;
        j = 0;
      }
      else if (j > 79)
        j = 0;
      else
        ++j;
    }

    void state_machine() {
      switch (state) {
        case STOPPING:
          protocol[p_run]->stopping();
          state = STOPPED;
        case STOPPED:
          rec_generic_driver();
          break;
        case CONFIGURING:
          protocol[p_run]->configuring();
          state = CONFIGURED;
          Serial.print("CFGOK\r");
          if (DEBUG) Serial.print("\n");
        case CONFIGURED:
          rec_generic_driver();
          break;
        case STARTING:
          protocol[p_run]->starting();
          state = STARTED;
          Serial.print("STROK\n\r");
          // if (DEBUG) Serial.print("\n");
          protocol[p_run]->started();
        case STARTED:
          rec_generic_driver();
          break;
        case RESETING:
          protocol[p_run]->reseting();
          state = RESETED;
          Serial.print("RSTOK\r");
          if (DEBUG) Serial.print("\n");
        case RESETED:
          rec_generic_driver();
          break;
      }
    }

  private:

    void ids() {
      Serial.print("IDS\t");
      Serial.print(id);
      Serial.print("\t");
      Serial.print(state_name[state]);
      Serial.print("\r");
      if (DEBUG) Serial.print("\n");
    }

    void rec_generic_driver() {
      static unsigned long int ids_time, out_time;
      unsigned long int new_time;

      new_time = millis();

      if (new_time > ids_time + 20000) {
        ids();
        ids_time = new_time;
      }

      if (new_time > out_time + TIMEOUT)
      {
        state = STOPPING;
        out_time = new_time;
      }

      if (state == STOPPED)
        out_time = new_time;

      if (command_received) {
        if (strncmp(rbuf, "?", 1) == 0)
        {
          Serial.print("HELP:\r\n\r\n");
          Serial.print("cur\r\n");
          Serial.print("str\r\n");
          Serial.print("stp\r\n");
          Serial.print("rst\r\n");
          Serial.print("ids\r\n");
          Serial.print("cfg theta1[0:MAXIMUM_STEP] theta2[0:MAXIMUM_STEP] theta3[0:MAXIMUM_STEP] theta4[0:MAXIMUM_STEP] theta5[0:MAXIMUM_STEP] polarizer_to_sweep[1:5] theta_max[0:MAXIMUM_STEP] PWMLed[0:255]\r\n\r\n");
          Serial.print("led on [0:255]\r\n");
          Serial.print("led off\r\n");
          Serial.print("lamp on [0:255]\r\n");
          Serial.print("lamp off\r\n\r\n");
          Serial.print("set maxstep [0:1000]\r\n\r\n");
          Serial.print("set vref1_1 [0:1] // a fim de não saturar leitura do photodiodo, if 1 Vref=1.1V else Vref=2.56V \r\n");
          Serial.print("move forward polarizer_to_sweep[1:5] steps[0:MAXIMUM_STEP]\r\n");
          Serial.print("move backward polarizer_to_sweep[1:5] steps[0:MAXIMUM_STEP]\r\n");
          Serial.print("go to origin polarizer_to_sweep[1:5]\r\n\r\n");
          Serial.print("MAXIMUM_STEP: ");
          Serial.print(MAXIMUM_STEP);
          Serial.print(" steps\r\n");
          Serial.print("Reference Voltage: ");
          if(VOLTAGEREF1_1V){
            Serial.print(" 1.1V");
          }else{
            Serial.print(" 2.56V");
          }
          Serial.print("\r\n");
          Serial.print("Photodiode Value (0-1023): ");
          Serial.print(protocol[p_run]->photodiode_value());
          Serial.print("\r\n");
          Serial.print("Switch 1 Triggered: ");
          Serial.print(protocol[p_run]->isSwitchTrigger(0));
          Serial.print("\r\n");
          Serial.print("Switch 2 Triggered: ");
          Serial.print(protocol[p_run]->isSwitchTrigger(1));
          Serial.print("\r\n");
          Serial.print("Switch 3 Triggered: ");
          Serial.print(protocol[p_run]->isSwitchTrigger(2));
          Serial.print("\r\n");
          Serial.print("Switch 4 Triggered: ");
          Serial.print(protocol[p_run]->isSwitchTrigger(3));
          Serial.print("\r\n");
          Serial.print("Switch 5 Triggered: ");
          Serial.print(protocol[p_run]->isSwitchTrigger(4));
          Serial.print("\r\n");
          // Serial.print("help\n");
          // Serial.print("?\n");



// angulo1emsteps, angulo2emsteps, angulo3emsteps, angulo4emsteps, angulo5emsteps, polarizer a varrer(1 a 5)           , angulolimitevarrimento, PWMLed(0-255)
//                                                                                             , 0-> nao varrer | 1,2,..,5 -> varrer

        }
        if (strncmp(rbuf, "ids", 3) == 0)
        {
          ids();
        }
        if (strncmp(rbuf, "led on", 6) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip led
          auxiliar_parts = strtok (NULL, "\t "); // skip on
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }

          protocol[p_run]->led_on();
          Serial.print("led_on\r\n");
        }
        if (strncmp(rbuf, "led off", 7) == 0)
        {
          protocol[p_run]->led_off();
          Serial.print("led_off\r\n");
        }
        if (strncmp(rbuf, "lamp on", 7) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip lamp
          auxiliar_parts = strtok (NULL, "\t "); // skip on
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          protocol[p_run]->lamp_on();
          Serial.print("lamp_on\r\n");
        }
        if (strncmp(rbuf, "lamp off",8) == 0)
        {
          protocol[p_run]->lamp_off();
          Serial.print("lamp_off\r\n");
        }
        if (strncmp(rbuf, "set maxstep",11) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip lamp
          auxiliar_parts = strtok (NULL, "\t "); // skip on
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          protocol[p_run]->set_maxstep();
          Serial.print("set_maxstep\r\n");
        }
        if (strncmp(rbuf, "set vref1_1",11) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip set
          auxiliar_parts = strtok (NULL, "\t "); // skip vref1_1
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          if(auxiliar_param[0] == 1){
            VOLTAGEREF1_1V = true;
          }else if(auxiliar_param[0] == 0){
            VOLTAGEREF1_1V = false;
          }
          Serial.print("set_VOLTAGEREF1_1V_");
          Serial.print(auxiliar_param[0]);
          Serial.print("\r\n");
        }
        if (strncmp(rbuf, "move forward",12) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip lamp
          auxiliar_parts = strtok (NULL, "\t "); // skip on
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          protocol[p_run]->move_forward();
          Serial.print("move_forward\r\n");
        }
        if (strncmp(rbuf, "move backward",13) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip lamp
          auxiliar_parts = strtok (NULL, "\t "); // skip on
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          protocol[p_run]->move_backward();
          Serial.print("move_backward\r\n");
        }
        if (strncmp(rbuf, "go to origin",12) == 0)
        {
          char *auxiliar_parts;
          auxiliar_parts = strtok(rbuf, "\t ");
          sscanf(auxiliar_parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          auxiliar_parts = strtok (NULL, "\t "); // skip go
          auxiliar_parts = strtok (NULL, "\t "); // skip to
          auxiliar_parts = strtok (NULL, "\t "); // skip origin
          for (int k = 0; auxiliar_parts != NULL; ++k)
          {
            sscanf(auxiliar_parts, "%d", auxiliar_param + k);
            auxiliar_parts = strtok (NULL, "\t ");
          }
          protocol[p_run]->go_to_origin();
          Serial.print("go_to_origin\r\n");
        }
        if (strncmp(rbuf, "cfg", 3) == 0)
        {
          char *parts;
          parts = strtok (rbuf, "\t ");
          // parts = strtok (NULL, "\t "); //repetition intended
          sscanf(parts, "%d", &p_run);
          if (p_run > NPROTOCOLS)
            p_run = 0;
          parts = strtok (NULL, "\t "); //repetition intended #2
          for (int j = 0; parts != NULL; ++j)
          {
            sscanf(parts, "%d", param + j);
            parts = strtok (NULL, "\t ");
          }
          // assegurar que config nao passa do angulo limite
          for (int j = 0; j < 5; ++j)
          {
            if(param[j] > MAXIMUM_STEP){
              param[j] = MAXIMUM_STEP;
            }
          } 
            if(param[6] > MAXIMUM_STEP){
              param[6] = MAXIMUM_STEP;
            }
	  if(param[7] > 255){
		param[7] = 255;	  
	  }
          Serial.print("CFG\t");
          // Serial.print(p_run);
          for (int j = 0; j < NARGUMENTS; ++j)
          {
            Serial.print("\t");
            Serial.print(param[j]);
          }
          Serial.print("\r");
          if (DEBUG) Serial.print("\n");
          state = CONFIGURING;
        }

        if (strncmp(rbuf, "cur", 3) == 0)
        {
          Serial.print(p_run);
          for (int j = 0; j < NARGUMENTS; ++j)
          {
            Serial.print("\t");
            Serial.print(param[j]);
          }
          Serial.print("\r");
          if (DEBUG) Serial.print("\n");
        }
        if (strncmp(rbuf, "str", 3) == 0){
          state = STARTING;
        }
        if (stop == 1)      {
          state = STOPPING;
          Serial.print("STP\r");
          if (DEBUG) Serial.print("\n");
          stop = 0;
        }
        if (reset == 1) {
          state = RESETING;
          reset = 0;

        }

        command_received = 0;
        }

    }
} expr;

#endif
