// This #include statement was automatically added by the Particle IDE.
#include "MCP23008.h"
MCP23008 relayController;

String relayStatus;
String inputStatus;

void setup() {
    relayController.setOutputs(4);
    relayController.publishInputEvents = true;
    Particle.function("relayControl", &MCP23008::relayTalk, &relayController);
    Particle.variable("relayStatus", relayStatus);
    Particle.variable("inputStatus", inputStatus);
    
}
void loop() {
    relayStatus = String(relayController.readRelayBankStatus());
    
    inputStatus = String(relayController.readAllInputs());
}
