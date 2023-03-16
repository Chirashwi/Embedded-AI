#include"NanoEdgeAI.h"
#include "knowledge.h"
#include <Nokia_LCD.h>

#define CONFORMATIONS_NB (uint32_t)(3)
#define SIZE 512
#define AXIS 1


float mic_x = 0.0f;
float mic_buffer[SIZE * AXIS] = {0.0};


float output_class_buffer[CLASS_NUMBER]; // Buffer of class probabilities
const char *id2class[CLASS_NUMBER + 1] = { // Buffer for mapping class id to class name
    "unknown",
    "AMBULANCE",
    "CRYBABY",
    "noinput",
};
Nokia_LCD lcd(D13 /* CLK */, D11 /* DIN */, D2 /* DC */, D4 /* CE */, D3 /* RST */, D7 /* BL */);

void fill_mic_buffer(void);
void log_samples(void);
void inference(void);


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    neai_classification_init(knowledge);
  lcd.begin();
  // Turn on the backlight
  lcd.setBacklight(true);
  // Set the contrast
  lcd.setContrast(60);  // Good values are usualy between 40 and 60
  // Clear the screen by filling it with black pixels
  lcd.clear(true);
  delay(2000);
}


void loop() {
    // put your main code here, to run repeatedly:
    inference();
}
void fill_mic_buffer(void)
{
    for (int i = 0; i < SIZE; i++)
      {
        mic_x = analogRead(A0);
        mic_buffer[i] = mic_x;
        delay(2);
     
  }
}
void log_samples(void)
{
    fill_mic_buffer();
    for (int j = 0; j < SIZE; j++)
      {
        Serial.print(mic_buffer[j]);
        Serial.print(" ");
     
  }
    Serial.println();
}
void inference(void)
{
    uint16_t i, id_class_t0, id_class_tn;
    fill_mic_buffer();
    neai_classification(mic_buffer, output_class_buffer, &id_class_t0);
    for (int i = 0; i < CONFORMATIONS_NB - 1; i++)
      {
        fill_mic_buffer();
        neai_classification(mic_buffer, output_class_buffer, &id_class_tn);
        if (id_class_t0 != id_class_tn)
          {
            break;
         
    }
        if (id_class_t0 == id_class_tn)
          {
            Serial.print("detected class is:");
            Serial.println(id2class[id_class_t0]);
      lcd.setCursor( 0 , 3);
      lcd.clear(false);
      lcd.print("Class: ");
      lcd.print(id2class[id_class_t0]);
         
    }
        else
          {
            Serial.println("?");
            delay(30);
      lcd.setCursor( 0 , 3);
      lcd.clear(false);
      lcd.print("?");
    }

       
  }
   
}
}
