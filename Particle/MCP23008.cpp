#include "MCP23008.h"
#include "Particle.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING
MCP23008::MCP23008() : timer(100, &MCP23008::momentaryOff, *this, true) {}

MCP23008::~MCP23008(){};

void MCP23008::setAddress(int a0, int a1, int a2){
    address |= (a0*1+a1*2+a2*4);
}

void MCP23008::setAddress(int a){
    address = a;
}

void MCP23008::setRelays(int num){
    relayCount = num;
    setOutputs(256 - (1 << num));
}
void MCP23008::setOutputs(int map){
    outputMap = map;
}
void MCP23008::setOutput(int num){
    outputMap = outputMap ^ (1 << num);
}

void MCP23008::setInputs(int map){
    inputMap = map;
}

void MCP23008::init(){
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    sendCommand(0x00, outputMap);
    sendCommand(0x06, inputMap);
    readStatus();
}



void MCP23008::turnOnRelay(int relay){
  relayOp(relay, 1);
}
void MCP23008::turnOffRelay(int relay){
  relayOp(relay, 2);
}
void MCP23008::toggleRelay(int relay){
  relayOp(relay, 3);
}
void MCP23008::momentaryRelay(int relay, int duration){
    _momentaryRelay = relay;
    relayOp(relay, 1);
    timer.changePeriod(duration);
    timer.start();
}
void MCP23008::momentaryRelay(int relay){
    momentaryRelay(relay, 100);
}
void MCP23008::momentaryOff(){
    relayOp(_momentaryRelay, 2);
}
void MCP23008::relayOp(int relay, int op){
    readStatus();
    byte rbit = (1<<(relay-1));
    if((outputMap & rbit) > 0) return;
    setBankStatus(bitop(bankStatus, rbit, op));
}
byte MCP23008::bitop(byte b1, byte b2, int op){
  if(op == 1) return b1 | b2;
  if(op == 2) return b1 & ~b2;
  if(op == 3) return b1 ^ b2;
  return 0;
}

void MCP23008::turnOnAllRelays(){
    setBankStatus((255 ^ outputMap));
}

void MCP23008::turnOffAllRelays(){
    setBankStatus(bankStatus & outputMap);
}

void MCP23008::setBankStatus(int status){
    sendCommand(0x0A, status);
    readStatus();
}

int MCP23008::readRelayStatus(int relay){
    int value = (1<<(relay-1));
    if((outputMap & value) > 0){
        return 256;
    }
    byte bankStatus = readRelayBankStatus();
    if(bankStatus & value){
        return 1;
    }else{
        return 0;
    }
}

int MCP23008::readRelayBankStatus(){
    int status = sendCommand(0x0A);
    return status;
}

void MCP23008::readStatus(){
    byte status = readRelayBankStatus();
    bankStatus = status;
}

int MCP23008::readInputStatus(int input){
    if(!iosetCustom){
        input += relayCount;
    }
    int value = (1<<(input-1));
    if((outputMap & value) == 0){
        return 256;
    }
    byte bankStatus = readAllInputs();
    if(bankStatus & value){
        return 0;
    }else{
        return 1;
 
    }
}
int lastChanges[8];
int MCP23008::readAllInputs(){
    byte status = sendCommand(0x09);
    byte inverted = ~status;
    byte shifted;
    if(!iosetCustom){
        shifted = inverted >> relayCount;
    }else{
        shifted = inverted;
    }
    if(publishInputEvents && !firstInput && shifted!=inputStatus){
      byte diff = shifted ^ inputStatus;
      int now = millis();
      for(int i=0;i<8;i++){
        bool isInput = (outputMap & (1 << (i+(iosetCustom ? 0 : relayCount))));
        if(isInput && (diff & (1 << i)) && (now-lastChanges[i] > 200)){
            lastChanges[i] = now;
          if(shifted & (1 << i)){
            String evon = String(i);
            evon.concat(" on");
            Particle.publish("sa/Input Change", evon);
          }else{
            String evoff = String(i);
            evoff.concat(" off");
            Particle.publish("sa/Input Change", evoff);
          }
        }
      }
    }else if(firstInput){
        firstInput = false;
    }
    inputStatus = shifted;
    return shifted;
}
int MCP23008::sendCommand(int reg){
    return sendCommand(reg, 256);
}

int MCP23008::sendCommand(int reg, int cmd){
    int status = 256;
    while(retrys < 4 && status > 0){
        Wire.beginTransmission(address);
        Wire.write(reg);
        if(cmd < 256) Wire.write(cmd);
        status = Wire.endTransmission();
        retrys++;
    }
    initialized = false;
    int ret = 256;
    if(retrys < 4){
        initialized = true;
        Wire.requestFrom(address, 1);
        ret = Wire.read();
    }
    retrys = 0;
    return ret;
}
int MCP23008::relayTalk(String command){
    int relay=0;
    int op=0;
    int duration=200;
    int dur_multi=1;
    int p=command.indexOf(" ");
    bool all=false;
    bool relay_next=true;
 
    String word;
    if(p > -1){
        while(command.length() > 0){
            if(p > -1){
                word = command.substring(0, p);
                command = command.substring(p+1);
                p=command.indexOf(" ");
            }else{
                word = command;
                command = "";
            }
            if(word.equalsIgnoreCase("on") || word.equalsIgnoreCase("activate")){
                if(op == 0) op = 1;
            }else if(word.equalsIgnoreCase("off") || word.equalsIgnoreCase("deactivate")){
                op = 2;
            }else if(word.equalsIgnoreCase("toggle") || word.equalsIgnoreCase("flip")){
                op = 3;
            }else if(word.equalsIgnoreCase("momentary")){
                op = 4;
            }else if(word.equalsIgnoreCase("relay") || word.equalsIgnoreCase("output")){
                relay_next = true;
            }else if(word.equalsIgnoreCase("all")){
                all = true;
            }else if(op == 1 && word.equalsIgnoreCase("for")){
                op = 4;
            }else{
                int test = word.toInt();
                if(test > 0){
                    if(relay_next){
                        relay = test;
                        relay_next = false;
                    }else{
                        duration = test;
                    }
                }else if(op == 4){
                    if(word.equalsIgnoreCase("seconds") || word.equalsIgnoreCase("second")){
                        dur_multi = 1000; 
                    }else if(word.equalsIgnoreCase("minutes") || word.equalsIgnoreCase("minute")){
                        dur_multi = 60000; 
                    }
                }
            }
        }
    }
    if(op == 4){
        momentaryRelay(relay, (duration*dur_multi));
        return 1;
    }
    if(all){
        int obyte;
        if(op == 1) obyte = bankStatus | (~outputMap & 255);
        else if(op == 2) obyte = bankStatus & outputMap;
        else if(op == 3) obyte = ~bankStatus;
        setBankStatus(obyte);
        return 1;
    }else{
        relayOp(relay, op);
        if(publishInputEvents){
        	String message="Relay ";
        	message.concat(String(relay));
        	if(op==2 && readRelayStatus(relay)==0){
        		message.concat(" off");
        		Particle.publish("Relay Action", message);
        	}
        	if(op==1 && readRelayStatus(relay)==1){
        		message.concat(" on");
        		Particle.publish("Relay Action", message);
        	}
        	if(op==3){
                if(readRelayStatus(relay)==0){
                    message.concat(" off");
                    Particle.publish("Relay Action", message);
                }else{
                    message.concat(" on");
                    Particle.publish("Relay Action", message);
                }
            }
            if(op == 1 && readRelayStatus(relay)!=1){
                return bankStatus;
            }
        	return op;
        }
        return 1;
    }
    return 0;
}
