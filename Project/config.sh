#config file for last part
#!/bin/bash

# Define GPIO pins
# GPIO_60 = 60
# GPIO_46 = 46

# Export GPIO pins
echo $GPIO_60 > /sys/class/gpio/gpio60/export
echo $GPIO_46 > /sys/class/gpio/gpio46/export

# Set up Pull up resister for button
config-pin p8.16 gpio_pu
# Also did PU for the transister, not sure if this is redundent...
config-pin p9.12 gpio_pu

# Set GPIO directions
echo out > /sys/class/gpio/gpio60/direction
echo in > /sys/class/gpio/gpio46/direction

# Set GPIO values (They both start low)
echo 0 > /sys/class/gpio/gpio60/value
echo 0 > /sys/class/gpio/gpio46/value

echo "GPIO $GPIO_60 and GPIO $GPIO_46 have been configured as outputs and set to out an in respectively."
