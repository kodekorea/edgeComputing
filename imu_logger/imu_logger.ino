#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
#define BNO055_SAMPLERATE_DELAY_MS (10)

unsigned long timer = 0;
unsigned long timer1 = 0;
unsigned long timer2 = 0;

double ang_x, ang_y, ang_z;
double gyroX, gyroY;

void setup() {
  // put your setup code here, to run once:
  delay(5000);
  Serial.begin(115200); 

  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - timer) > (BNO055_SAMPLERATE_DELAY_MS - 1)) {
    timer = millis();
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    /* Display calibration status for each sensor. */
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    // 각속도 얻어오는 부분
    imu::Vector<3> gyroVector = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> laccVector = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    ang_x = euler.x();
    ang_y = euler.y();
    ang_z = euler.z();

    Serial.print(gyroVector.x());
    Serial.write(',');
    Serial.print(gyroVector.y());
    Serial.write(',');
    Serial.print(gyroVector.z());
    Serial.write(',');
    Serial.print(laccVector.x());
    Serial.write(',');
    Serial.print(laccVector.y());
    Serial.write(',');
    Serial.print(laccVector.z());    
    Serial.write('\n');
  }
}
