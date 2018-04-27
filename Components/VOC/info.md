# VOC 
Volatile Organic Compound sensor
(Adafruit CCS811 Air Quality Sensor)

The VOC sensor detects VOCs and outputs through an I2C line value. The conversion to a digital value is done through a arduino library. However, this sensor has some drawbacks. First, it requires a 48 hour break-in period to ensure reliable readings. It also requires baseline correction to ensure accurate readings, as the metal oxide detectors in the sensor change their readings over time. Finally, it requires a 10k thermistor to be soldered onto the board to allow for automatic heat calibration to the final output. However, after setting it up, using the sensor should be rather straightforward.


For more information visit:
https://learn.adafruit.com/adafruit-ccs811-air-quality-sensor/overview
