#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
// UUID 설정 1
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static int threshold = 1500;   // 터치 임계값
bool touch1detected = false;
BLECharacteristic *pCharacteristic;

// 터치 감지 함수
void gotTouch1() {
  touch1detected = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  Serial.println("\n ESP32 Touch Interrupt Test\n");

  // 터치 인터럽트 설정
  touchAttachInterrupt(T1, gotTouch1, threshold);

  // BLE 초기화
  BLEDevice::init("TOUCH SERVICE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // BLE 특성 설정
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  pCharacteristic->addDescriptor(new BLE2902());
  // 초기 값 설정
  pCharacteristic->setValue("0");
  pService->start();

  // 광고 시작
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  if (touch1detected) {
    touch1detected = false;

    // 터치 상태에 따라 값 전송
    if (touchInterruptGetLastStatus(T1)) {
        Serial.println(" --- T1 Touched");
        pCharacteristic->setValue("1");
    } else {
        Serial.println(" --- T1 Released");
        pCharacteristic->setValue("0");
    }

    // 알림 전송
    pCharacteristic->notify();
  }
  delay(20);
}
