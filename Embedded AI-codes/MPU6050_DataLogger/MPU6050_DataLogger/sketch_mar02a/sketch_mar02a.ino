
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define DATA_INPUT_USER 256
#define AXIS_NUMBER 3

Adafruit_MPU6050 mpu;
/* Get new sensor events with the readings */
  sensors_event_t a, g, temp;

float mpu_buffer[DATA_INPUT_USER * AXIS_NUMBER] = { 0 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(50);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
}

}

void loop() {
  // put your main code here, to run repeatedly:
  fill_mpu_buffer();
  for (int i = 0; i < DATA_INPUT_USER; i++) {
    Serial.print(mpu_buffer[AXIS_NUMBER * i]);
    Serial.print(" ");
    Serial.print(mpu_buffer[AXIS_NUMBER * i + 1]);
    Serial.print(" ");
    Serial.print(mpu_buffer[AXIS_NUMBER * i + 2]);
    Serial.print(" ");
  }
    Serial.println();
    delay(100);
}

 void fill_mpu_buffer() {
    for (int i = 0; i < DATA_INPUT_USER; i++) {
      mpu.getEvent(&a, &g, &temp);
       mpu_buffer[AXIS_NUMBER * i] = a.acceleration.x;
      mpu_buffer[AXIS_NUMBER * i + 1] = a.acceleration.y;
      mpu_buffer[AXIS_NUMBER * i + 2] = a.acceleration.z;
  }
}
