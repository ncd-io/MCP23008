
# About

This Library is intended for use with any MCP23008 Relay Controller or GPIO board available from ncd.io

### Developer information
NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit ncd.io

### Requirements
- The Python SMBus Module: https://pypi.python.org/pypi/smbus-cffi/
- An I2C connection to an MCP23008 Chip
- Knowledge base for developing and programming with Python.

### Version
1.0.0

### How to use this library

The libary must be imported into your application. Once it is imported you can create an MCP23008 object and start to communicate to the chip. You can optionally pass in a kwarg to the object at instantiation that acts as an output map.
This output map is a set of GPIO numbers that you would like to set as outputs. For example {0,1,2,3,6} would set channels
0, 1, 2, 3, and 6 as outputs. Any channel with a relay on it should be configured as an output.

When you create an instance of the MCP23008 object be sure to pass it the I2C bus and an array that contains which channels you want to configure as inputs.

Each MCP23008 supports 8 GPIO pins from 0 to 7. We use these GPIOs to drive the relays so if you have 4 relays, then set the first four relays as outputs. You can do this by passing {0, 1, 2, 3} in the kwargs. See the examples for more information.

### Publicly accessible methods
```cpp
set_gpio_high(target_gpio)
```
>This function sets a GPIO to high. If this GPIO is configured as an output and has a relay on it the
>Relay will be turned on. The argument passed is an ingteger from 0-7 with 0 being the first GPIO on the chain.


```cpp
set_gpio_low(target_gpio)
```
>This function sets a GPIO to low. If this GPIO is configured as an output and has a relay on it the
>Relay will be turned off. The argument passed is an integer from 0-7 with 0 being the first GPIO on the chain.


```cpp
toggle_gpio(target_gpio)
```
>This method will toggle the given GPIO. If the GPIO is high then it will be set low. If it is low then it will be set high.
>The argument passed is an ingteger from 0-7 with 0 being the first GPIO on the chain.


```cpp
get_single_gpio_status(target_gpio)
```
>A call to this method will toggle the state of the relay.  If the relay was previously off a call to this method
>turn the relay on and vice versa.


```cpp
get_all_gpio_status()
```
>A call to this method will read the status of the relay and return the current on/off
>status of the relay.  1 will be returned if the relay is on and 0 will be returned if the relay is off. 
>256 will be returned if an error has occured(generally due to lack of communication with the controller).


```cpp
pull_up_gpio(target_gpio)
```
>This method pulls up the target GPIO. This is the most common state needed in GPIO input applications. This state will allow you
>to connect a simple switch or contact closure output sensor to your GPIO and reliably read it.
