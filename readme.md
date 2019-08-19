# 8 Characters 16 Segment Chainable Alphanumeric LED Display

This project was designed to help connect 8 characters alphanumeric displays to MCUs via I2C

The whole project is designed around [MAX6955](https://datasheets.maximintegrated.com/en/ds/MAX6955.pdf), which is a common-cathode display encoder.

The PCB was designed for 0.8 inch(27.7 x 20mm) LED tubes, the following are compatible:

* LDS-8103AX
* ArkLed (arkch.com) 20803 (red/yellow/amber/orange/blue/green)

Unfortunately, the MAX6955 is pretty expensive ($17), making this board less than ideal.

# MAX6955
The [MAX6955](https://www.maximintegrated.com/en/products/power/display-power-control/MAX6955.html) is a 2-Wire Interfaced, 2.7V to 5.5V LED Display Driver with I/O Expander and Key Scan

## Initializing
The library should be initialized with the MAX6955 ID set:
```
MAX6955 maxdisp(0);
maxdisp.init();
```

The display can also be cleared:
```
maxdisp.clear();
```

And also tested for defects, functionality and power consumption:
```
maxdisp.test(true);
delay(1000);
maxdisp.test(false);
```

## Writing
The MAX6955 has 2 planes for writing characters, the 1st plane is the used for currently displaying characters and the 2nd plane is used to hold the characters that will be used in blinking.

To write to the 1st plane, you can either write single characters:
```
int id = 0;//first character, or 7 for the 8th character
maxdisp.writeReg(PLANE_0 + id, '#');
```

or you can write a whole string:
```
maxdisp.write(PLANE_0, '12345678');
```

## LED Intensity
This board comes with Rset (56k) and Cset (22pf), however, if you decide to use other LED modules than the provided ones, you should be able to calculate the correct Rset and Cset using [Application Note 1148](https://www.maximintegrated.com/en/app-notes/index.mvp/id/1148).

The MAX6955 also includes intensity support in software
it can either be set globally
```
maxdisp.setGlobalIntensity(0);
maxdisp.setGlobalIntensity(15);
```

or per character
```
maxdisp.setIntensity(0,0);
maxdisp.setIntensity(0,15);
```

## Blink
Blinking is done by having two character planes, you'll have to set both of them and when activating blinking, it will switch the display between lanes.

for example, to have the two middle characters switch between "#" and "-", we can write the following:
```
maxdisp.write(PLANE_1, "###--###");
maxdisp.write(PLANE_0, "########");
```

and then activate slow blinking:
```
maxdisp.blink(true,false);
```

or fast blinking:
```
maxdisp.blink(true,true);
```

or disable blinking and use plane 0
```
maxdisp.blink(false,false);
```


## GPIO
The MAX6955 includes 5 GPIOs, which can also be used for 32 keys matrix, though not directly exposed as pinouts in the current version of the PCB
![](https://www.maximintegrated.com/en/images/appnotes/3462/3462Fig01.gif)


## Character Set
![](https://www.maximintegrated.com/en/images/appnotes/3212/3212Fig03.gif)

### References
[https://github.com/pklaus/MAX6955](https://github.com/pklaus/MAX6955)


