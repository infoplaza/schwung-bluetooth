/*
 * Copyright (c) 2016 RedBear
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

 /*
  * This is a simple example of a bluetooth peripheral that can be used as a test for the Schwung bluetooth protocol.
  */
#include <nRF5x_BLE_API.h>

#ifdef BLE_NANO2
#define DIGITAL_OUT_PIN                   D13
#endif

BLEDevice ble;
Ticker                    ticker;

// Service UUID
static const uint8_t vri_service_uuid[]        = {0x71, 0x3D, 0x10, 0, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};
// Characteristic UUID
static const uint8_t vri_service_status_uuid[]     = {0x71, 0x3D, 0x10, 3, 0x50, 0x3E, 0x4C, 0x75, 0xBA, 0x94, 0x31, 0x48, 0xF1, 0x8D, 0x94, 0x1E};

uint8_t reversedServiceUUID[sizeof(vri_service_uuid)];

static uint8_t vri_status[2]         = {0x00, 0x00};

// Create characteristic and service
GattCharacteristic   vri_service_status(vri_service_status_uuid, vri_status, sizeof(vri_status), sizeof(vri_status), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);
GattCharacteristic   *vriServices[] = {&vri_service_status, };
GattService          vri_service(vri_service_uuid, vriServices, sizeof(vriServices) / sizeof(GattCharacteristic *));

void blink_led() {
  if(digitalRead(DIGITAL_OUT_PIN)) {
    digitalWrite(DIGITAL_OUT_PIN, LOW);
  } else {
    digitalWrite(DIGITAL_OUT_PIN, HIGH);
  }  
}

void disconnectionCallBack(const Gap::DisconnectionCallbackParams_t *params) {
  ble.startAdvertising();
}

// This code is executed when the value of the characterstic is modified
void gattServerWriteCallBack(const GattWriteCallbackParams *Handler) {
  uint8_t buf[1];
  uint16_t bytesRead = 1;
  
  if (Handler->handle == vri_service_status.getValueAttribute().getHandle()) {
      ble.readCharacteristicValue(vri_service_status.getValueAttribute().getHandle(), buf, &bytesRead); 
      ticker.detach();
      // The frequency of the led is modified based on the the value of the Request Status
      // UNKNOWN 
      if (buf[0] == 0x00) {
        digitalWrite(DIGITAL_OUT_PIN, LOW);
      // GREEN_REQUESTED
      } else if (buf[0] == 0x01) {
        ticker.attach(blink_led, 0.60);
      // GREEN_EXTENDED
      } else if (buf[0] == 0x02) {
        ticker.attach(blink_led, 0.15);
      // NO_REQUEST
      } else if (buf[0] == 0x03) {
        digitalWrite(DIGITAL_OUT_PIN, HIGH);
      }
  }

}

void setup() {
  // put your setup code here, to run once
  // close peripheral power
  
  pinMode(DIGITAL_OUT_PIN, OUTPUT);
  digitalWrite(DIGITAL_OUT_PIN, LOW);
  
  ble.init(); 
  ble.onDataWritten(gattServerWriteCallBack);
  ble.onDisconnection(disconnectionCallBack);
  ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                 (const uint8_t *)"Schwung test", sizeof("Schwung") - 1);
  for (unsigned int i = 0; i < sizeof(vri_service_uuid); i++) {
    reversedServiceUUID[i] = vri_service_uuid[sizeof(vri_service_uuid) - i - 1];
  }                              
  ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_128BIT_SERVICE_IDS,(const uint8_t *)reversedServiceUUID, sizeof(reversedServiceUUID));

  ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
  
  ble.addService(vri_service);
  ble.setDeviceName((const uint8_t *)"Test");
  
  
  ble.setTxPower(4);
  ble.setAdvertisingInterval(160);
  ble.setAdvertisingTimeout(0);

  ble.startAdvertising();
}

void loop() {
  // put your main code here, to run repeatedly:
  ble.waitForEvent();
}

