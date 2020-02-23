#include <Arduino.h>
#include <Servo.h>
#include "Adafruit_TCS34725.h"

#define SERVO1 2
#define SERVO2 3
#define MOTOR_IN1 5
#define MOTOR_IN2 6
#define RGBLED 8

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);

Servo servo1;
Servo servo2;

void setup() {
        Serial.begin(9600);

        if (tcs.begin()) {
                Serial.println("Found sensor");
        } else {
                Serial.println("No TCS34725 found ... check your connections");
                while (1); // halt!
        }

        servo1.attach(SERVO1);
        servo2.attach(SERVO2);
        pinMode( MOTOR_IN1, OUTPUT);
        pinMode( MOTOR_IN2, OUTPUT);

}

void loop() {
        float red, green, blue;
        int count = 0;

        //Home spoon
        servo2.write(35);        // tell servo to go to position in variable 'pos'

        //Dispense Lucky Charms TM
        //Regular Servo
        servo1.write(180);        // tell servo to go to position in variable 'pos'
        delay(500);
        servo1.write(120);        // tell servo to go to position in variable 'pos'
        //Continous Servo
        // servo1.write(0);
        // delay(50);
        // servo1.write(100);

        //Read RGB color
        tcs.setInterrupt(false); // turn on LED
        analogWrite(MOTOR_IN1, LOW); //NEW
        delay(50); // takes 50ms to read
        tcs.getRGB(&red, &green, &blue);
        tcs.setInterrupt(true); // turn off LED
        analogWrite(MOTOR_IN1, 128);

        while (((red>=100&&red<=120)&&(green>=68&&green<=88)&&(blue>=55&&blue<=75)) || ((red>=88&&red<=108)&&(green>=74&&green<=94)&&(blue>=53&&blue<=73))) // || !m3 || !m4 || !m5 || !m6 || !m7 || !m8 )
        {
                //After 10 RGB reads, dispense then reset counter
                if (count == 10)
                {
                        //Dispense Lucky Charms TM
                        //Regular servo
                         servo1.write(180);              // tell servo to go to position in variable 'pos'
                         delay(500);
                         servo1.write(120);              // tell servo to go to position in variable 'pos'
                        //Continous Servo
                        //servo1.write(0);
                        //delay(50);
                        //servo1.write(100);
                        count = 0;
                }
                else
                {
                        count++;
                }

                //Read RGB color
                tcs.setInterrupt(false); // turn on LED
                analogWrite(MOTOR_IN1, LOW);
                delay(50);
                tcs.getRGB(&red, &green, &blue);
                tcs.setInterrupt(true); // turn off LED
                analogWrite(MOTOR_IN1, 128);

        }

        //Color that broke the loop
        Serial.print("R:\t"); Serial.print(int(red));
        Serial.print("\tG:\t"); Serial.print(int(green));
        Serial.print("\tB:\t"); Serial.print(int(blue));
        Serial.print("\n");

        //Move the marshmellow to the yeeter
        // ramp up forward
        digitalWrite(MOTOR_IN1, LOW);
        for (int i=0; i<255; i+=5) {
                analogWrite(MOTOR_IN1, i);
                delay(10);
        }
        // forward full speed for one second
        delay(88);
        // ramp down forward
        for (int i=255; i>=0; i-=5) {
                analogWrite(MOTOR_IN1, i);
                delay(10);
        }

        //Yeet that marshmellow
        servo2.write(70);        // tell servo to go to position in variable 'pos'
        delay(1000);
        servo2.write(35);        // tell servo to go to position in variable 'pos'
        delay(1000);

}
