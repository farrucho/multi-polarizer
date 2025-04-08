//Protocol 0 class
//This protocol shall be used for help and/or debug purposes

//Defining every function is opcional, as they can be defined as common for all protocols
//Defining them here will override the default only for this protocol



extern class P0: public proto {
    public:

      void stopping() {
        if (DEBUG) Serial.println("0_stopping");
      }
  
      void configuring() {
        if (DEBUG) Serial.println("0_configuring");
        // for (int j = 0; j < NARGUMENTS; ++j)
        // {
        //   Serial.println(expr.param[j]);
        // }
      }
  
      void starting() {
        if (DEBUG) Serial.println("0_starting");
          // Inicializar componentes e ligar led
          
          // switch_E0.enable();
          // switch_E1.enable();
          // switch_X.enable();
          // switch_Y.enable();
          // switch_Z.enable();
            
          // stepper_E0.enable();
          // stepper_E1.enable();
          // stepper_X.enable();
          // stepper_Y.enable();
          // stepper_Z.enable();
            
          // photodiode.enable();
            
          // led.enable();
          // led.turnOn();

      }
  
      void started() {
        if (DEBUG) Serial.println("0_started");
      }
  } PP0;