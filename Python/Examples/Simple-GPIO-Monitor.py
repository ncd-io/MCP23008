#import time to allow some processing sleep. Not Required.
import time
from mcp23008 import mcp23008
import smbus

# Get I2C bus, this is bus 1
bus = smbus.SMBus(1)

#define which GPIOs are to be used as outputs. By default all GPIOs are defined as inputs.
#pass the number of the GPIOs in a set to the object. 0 is the first relay 1 is the second relay etc.
#for this example we have no relays and don't need this variable
# gpio_output_map =  {}

#kwargs is a Python set that contains the address of your device and the output map to be passed to the object for initialization.
#since I am using the default address and have no outputs I don't need to pass this variable
# kwargs = {'address': 0x20, 'gpio_output_map': gpio_output_map}
#create the MCP23008 object from the MCP23008 library
#the object requires that you pass it the bus object so that it can communicate and share the bus with other chips if necessary
mcp23008 = mcp23008(bus)
#to pass in kwargs you should instantiate the mcp23008 object with the following line
#mcp23008 = mcp23008(bus, kwargs)

#by default the inputs are floating. I want to pull up input GPIO 4 so I can hook up a simple switch and read its status.
mcp23008.pull_up_gpio(4)

while True :
    #I want to print out the status of GPIO 4. You can set this up in a conditional logic so it can execute whatever you'd like on change.
    gpio_4_status = mcp23008.get_single_gpio_status(4)
    print gpio_4_status
    #if the status of GPIO 4 is high then I want to print out the status of all GPIOs
    if(gpio_4_status):
        #print out the byte that indicates the status of the GPIOs.
        #255 means all GPIOs are high. 0 means all GPIOs are low.
        #if only GPIO 0 is high then the value will be 1, if only GPIO 3 is high the value will be 8
        #the bits in the byte would look like 00000001 (GPIO 0 is 1 aka high) and 00001000 (GPIO 3 is 1 aka high)
        print mcp23008.get_all_gpio_status()

    
