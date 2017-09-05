#import time to allow some processing sleep. Not Required.
import time
from mcp23008 import mcp23008
import smbus

# Get I2C bus, this is bus 1
bus = smbus.SMBus(1)

#define which GPIOs are to be used as outputs. By default all GPIOs are defined as inputs.
#pass the number of the GPIOs in a set to the object. 0 is the first relay 1 is the second etc.
gpio_output_map =  {0,1,2,3}
#kwargs is a Python set that contains the address of your device and the output map to be passed to the object for initialization.
kwargs = {'address': 0x20, 'gpio_output_map': gpio_output_map}
#create the MCP23008 object from the MCP23008 library and pass it the GPIO output map and address defined above
#the object requires that you pass it the bus object so that it can communicate and share the bus with other chips if necessary
mcp23008 = mcp23008(bus, kwargs)


while True :
    #turn on Relay at GPIO 0 aka the first relay
    mcp23008.set_gpio_high(0)
    time.sleep(.2)
    #turn off Relay at GPIO 0 aka the first relay
    mcp23008.set_gpio_low(0)
    time.sleep(.2)
    #toggle Relay at GPIO 0 aka the first relay
    mcp23008.toggle_relay(1)
    time.sleep(.2)
    mcp23008.toggle_relay(2)
    time.sleep(.2)
    mcp23008.toggle_relay(2)
    #print out the byte that indicates the status of the GPIOs.
    #255 means all GPIOs are high. 0 means all GPIOs are low.
    #if only GPIO 0 is high then the value will be 1, if only GPIO 3 is high the value will be 8
    #the bits in the byte would look like 00000001 (GPIO 0 is 1 aka high) and 00001000 (GPIO 0 is 1 aka high)
    print mcp23008.get_all_gpio_status()
    
