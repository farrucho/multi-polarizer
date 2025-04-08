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
    proto *protocol[NPROTOCOLS + 1];


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

      if (new_time > ids_time + 1000) {
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

        if (strncmp(rbuf, "ids", 3) == 0)
        {
          ids();
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
          for (int j = 1; j < NARGUMENTS; ++j)
          {
            if(param[j] > 900){
              param[j] = 900;
            }
          }
	  if(param[8] > 255){
		param[8] = 255;	  
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
          Serial.print("CUR\t");
          Serial.print(p_run);
          for (int j = 0; j < NARGUMENTS; ++j)
          {
            Serial.print("\t");
            Serial.print(param[j]);
          }
          Serial.print("\r");
          if (DEBUG) Serial.print("\n");
        }
        if (strncmp(rbuf, "str", 3) == 0)
          state = STARTING;

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
