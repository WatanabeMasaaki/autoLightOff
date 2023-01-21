#include <NimBLEDevice.h>
#include <ESP32Servo.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define LOCAL_NAME "ESP_TEST_DEVICE"
#define COMPLETE_LOCAL_NAME "ESP_TEST_DEVICE_LOCAL_NAME"
#define SERVICE_UUID "ee46ee64-ca0c-4cd3-8b87-bb02d813650a"
#define CHARACTERISTIC_UUID "5d105679-20c4-448d-85e9-fcd75bf2c89d"
#define CHARACTERISTIC_UUID_NOTIFY "6afeb308-af0d-4a1f-9f0d-24b35ec5535d"

NimBLECharacteristic *pNotifyCharacteristic;
NimBLEServer *pServer = NULL;
String stateValue = "0";
Servo servo;
static int LED_PIN = 15;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t data_buff[2];
int RX_PIN = 22;
int TX_PIN = 23;

class ServerCallbacks : public NimBLEServerCallbacks
{
  void onConnect(NimBLEServer *pServer)
  {
//    Serial.println("Client connected");
    deviceConnected = true;
  };
  void onDisconnect(NimBLEServer *pServer)
  {
//    Serial.println("Client disconnected - start advertising");
    deviceConnected = false;
    NimBLEDevice::startAdvertising();
  };
  void onMTUChange(uint16_t MTU, ble_gap_conn_desc *desc)
  {
//    Serial.printf("MTU updated: %u for connection ID: %u\n", MTU, desc->conn_handle);
  };
  uint32_t onPassKeyRequest()
  {
//    Serial.println("Server Passkey Request");
    return 123456;
  };
  bool onConfirmPIN(uint32_t pass_key)
  {
//    Serial.print("The passkey YES/NO number: ");
//    Serial.println(pass_key);
    return true;
  };
  void onAuthenticationComplete(ble_gap_conn_desc *desc)
  {
    if (!desc->sec_state.encrypted)
    {
      NimBLEDevice::getServer()->disconnect(desc->conn_handle);
//      Serial.println("Encrypt connection failed - disconnecting client");
      return;
    }
//    Serial.println("Starting BLE work!");
  };
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      String ledState = value.c_str();
//      Serial.println(ledState);
      

      long timeGet = ledState.toInt();

      Serial.println(timeGet);
      Serial1.println(timeGet);
//      Serial1.println(",");
    }
  }
};

//スマホに送信
void sendMessage() {
  char error = '1';
  Serial.println(error);
  pNotifyCharacteristic->setValue(error);
  pNotifyCharacteristic->notify();
}

// BLE loop
void loopBLE()
{
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);
    pServer->startAdvertising();
//    Serial.println("restartAdvertising");
    oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected)
  {
    oldDeviceConnected = deviceConnected;
  }
  if(Serial1.available() > 0){
    while(Serial1.available() > 0){
      int val = Serial1.read();
      if(val == 49){
        //スマホに通知
        sendMessage();  
      }
    }  
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  servo.attach(LED_PIN);
//  Serial.println("Starting NimBLE Server");
  NimBLEDevice::init(COMPLETE_LOCAL_NAME);
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setSecurityPasskey(123456);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  NimBLEService *pService = pServer->createService(SERVICE_UUID);
  NimBLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::WRITE);
  pNotifyCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_NOTIFY, NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN);
  pRxCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pService->start();
  NimBLEAdvertising *pNimBleAdvertising = NimBLEDevice::getAdvertising();
  pNimBleAdvertising->addServiceUUID(SERVICE_UUID);
  pNimBleAdvertising->addTxPower();
  NimBLEAdvertisementData advertisementData;
  advertisementData.setName(COMPLETE_LOCAL_NAME);
  advertisementData.setManufacturerData("NORA");
  pNimBleAdvertising->setScanResponse(true);
  pNimBleAdvertising->setScanResponseData(advertisementData);
  pNimBleAdvertising->start();
//  Serial.println("first startAdvertising");
}

void loop()
{
  loopBLE();
}
