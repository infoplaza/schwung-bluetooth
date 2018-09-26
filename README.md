# Schwung Bluetooth protocol

## Schwung
The goal of Schwung app is reducing the time cyclists have to wait at traffic lights by communicating the location of cyclists with traffic light controllers. A movie about the working of Schwung can be found here https://www.youtube.com/watch?v=zPt5rectsWs and here https://www.youtube.com/watch?v=uqj71LEbvZs. Schwung is developed as a colleboration between Infoplaza and Vialis.

This protocol is created to introduce extra functionality, in the first version of the app it was only possible to communicate from the phone to the traffic light controller. In the newest Android version it's also possible to receive feedback. In this way the user knows if a request for green is made at the TLC and on some locations also if green is extended (at this moment only on the locations in Breda). Cyclists shouldn't be substracted by their phones during cycling, therefor a Bluetooth protocol is developed that enables Schwung to communicate with a variety of Bluetooth peripherals. Possible devices could be smart bicycle bells, speakers, blinking leds, but that's up to your creativity! 

## Protocol

The protocol is based on GATT https://www.bluetooth.com/specifications/gatt/generic-attributes-overview. At this moment the protocol contains one service with one characteristic. 



Service | Characteristic | UUID | Values
------------ | -------- | ------------- | --------------
VRI_SERVICE|  | 713D1000-503E-4C75-BA94-3148F18D941E | 
- | REQUEST_STATUS |  713D1003-503E-4C75-BA94-3148F18D941E | 0x00 = UNKNOWN <br> 0x01 = GREEN_REQUESTED <br> 0x02 = GREEN_EXTENDED <br> 0x03 = ERROR

The status GREEN_REQUESTED means that a request for green is made to the traffic light controller. (That is similar to a detection at a detector loop or button). That can be followed by the status GREEN_EXTENDED in the city of Breda, this status means that the traffic light stays a little bit longer green so the cyclist doesn't have to wait.


## Examples

At this moment there is one example implementation available of a Bluetooth peripheral based on the BLE Nano V2 (https://redbear.cc/product/ble-nano-kit-2.html) to get you started or if you want to experiment with the Bluetooth functionality without having to develop your own device. The source code for this example can be found in this repository.


