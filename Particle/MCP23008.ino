// This #include statement was automatically added by the Particle IDE.
#include "MCP23008.h"
MCP23008 relayController;
MCP23008 relayController33;

int relayStatus;
int inputStatus;

void setup() {
    
    relayController.setRelays(4);
    relayController.setInputs(240);
    relayController.init();
    
    relayController33.setAddress(1,0,0);
    relayController33.setRelays(8);
    relayController33.init();
    
    relayController.publishInputEvents = true;
    Particle.function("control32", &MCP23008::relayTalk, &relayController);
    Particle.function("control33", &MCP23008::relayTalk, &relayController33);
    Particle.variable("relayStatus", relayStatus);
    Particle.variable("inputStatus", inputStatus);
    
}
void loop() {
    
    inputStatus = relayController.readAllInputs();
    
    //Tie input to output (4 is input 3 counting from 0)
    if((inputStatus & 4) > 0){
        relayController33.turnOnRelay(2);
    }else{
        relayController33.turnOffRelay(2);
    }
    
    relayStatus = (relayController.readRelayBankStatus() << 8) + relayController33.readRelayBankStatus();
    
}
