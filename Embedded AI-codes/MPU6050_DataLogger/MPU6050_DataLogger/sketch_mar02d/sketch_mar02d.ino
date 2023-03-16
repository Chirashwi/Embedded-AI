#include <Nokia_LCD.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "NanoEdgeAI.h"
#include "knowledge.h"
#define DATA_INPUT_USER 256
#define AXIS_NUMBER 3

Nokia_LCD lcd(D13 /* CLK */, D11 /* DIN */, D2 /* DC */, D4 /* CE */, D3 /* RST */, D7 /* BL */);

sensors_event_t a, g, temp;

void Button_press(void);
typedef enum
{
  LOG,
  INFERENCE,
} States_t;

#define CONFIRMATIONS_NB         (uint32_t)(3)  /* Number of consecutive confirmations */

float mpu_buffer[DATA_INPUT_USER * AXIS_NUMBER] = { 0 };
float output_class_buffer[CLASS_NUMBER]; // Buffer of class probabilities
const char *id2class[CLASS_NUMBER + 1] = { // Buffer for mapping class id to class name
  "unknown",
  "noangle",
  "roll",
  "pitch",
  "yaw",
};
void fill_mpu_buffer();
void Inference(void);

Adafruit_MPU6050 mpu;

void setup() {
  // put your setup code here, to run once:
    lcd.begin();
  // Turn on the backlight
  lcd.setBacklight(true);
  // Set the contrast
  lcd.setContrast(50);  // Good values are usualy between 40 and 60
  // Clear the screen by filling it with black pixels
  lcd.clear(true);
  delay(2000);
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

enum neai_state error_code = neai_classification_init(knowledge);
  if (error_code!=NEAI_OK)
  {
    Serial.print("Knowledge initialization ERROR:");
    Serial.println(error_code);
  }
  else
  {
    Serial.println("Knowledge initialization DONE:");
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  Inference();  
  
}


void fill_mpu_buffer() {
  
 for (int i = 0; i < DATA_INPUT_USER; i++) {
    mpu.getEvent(&a, &g, &temp);
    mpu_buffer[AXIS_NUMBER * i] = a.acceleration.x;
    mpu_buffer[AXIS_NUMBER * i + 1] = a.acceleration.y;
    mpu_buffer[AXIS_NUMBER * i + 2] = a.acceleration.z;
  }
}



void Inference(void)
{
  uint16_t i,id_class_t0,id_class_tn;

  fill_mpu_buffer();
  neai_classification(mpu_buffer,output_class_buffer,&id_class_t0);
  for (i=0;i<CONFIRMATIONS_NB-1;i++)
  {
    fill_mpu_buffer();
    neai_classification(mpu_buffer,output_class_buffer,&id_class_tn);
    if (id_class_t0 != id_class_tn) { break; }
  }
  if (id_class_t0 == id_class_tn)
  {
    Serial.print("Detected Class:");
    Serial.println(id2class[id_class_t0]);
     lcd.print("Detected Class:");
    lcd.println(id2class[id_class_t0]);
  }
  else
  {
    Serial.println("?");
  }
}
