//Default functions
//If state functions are not defined for individual protocols, this ones will be used.

void proto::stopping() {
    if (DEBUG) Serial.println("default_stopping");
  }
  
  void proto::configuring() {
    if (DEBUG) Serial.println("default_configuring");
    
  }
  
  void proto::starting() {
    if (DEBUG) Serial.println("default_starting");
    
  }
  
  void proto::started() {
    if (DEBUG) Serial.println("default_started");
    
  }