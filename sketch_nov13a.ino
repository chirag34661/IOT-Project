#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD and Servo configurations
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address for a 16 chars and 2-line display
Servo myservo;                      // create servo object to control a servo

// Pin definitions
const int IR_PIN_1 = 4;
const int IR_PIN_2 = 5;
const int servo_pin = 6;
const int trigPin = 7;
const int echoPin = 8;

// Variables
int distance;
int pos = 0;        // variable to store the servo position
int IT_State_1;
int IT_State_2;
String Parking_1;
String Parking_2;

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(IR_PIN_1, INPUT);
    pinMode(IR_PIN_2, INPUT);
    
    Serial.begin(9600); // Initialize serial communication

    lcd.init();
    lcd.backlight();
    
    // Servo calibration
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motor Calibration");
    lcd.setCursor(0, 1);
    lcd.print("started");
    delay(1000);
    
    myservo.attach(servo_pin);
    for (pos = 15; pos <= 160; pos += 1) {
        myservo.write(pos);
        delay(15);
    }
    for (pos = 160; pos >= 13; pos -= 1) {
        myservo.write(pos);
        delay(15);
    }
    myservo.detach();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motor Calibration");
    lcd.setCursor(0, 1);
    lcd.print("Done");
    delay(1000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car");
    lcd.setCursor(0, 1);
    lcd.print("Parking System");
    delay(2000);
}

void loop() {
    distance = ultra();
    
    // Check if parking space is available
    if ((distance < 10) && ((Parking_1 == "EMPTY") || (Parking_2 == "EMPTY"))) {
        myservo.attach(servo_pin);
        for (pos = 5; pos <= 200; pos += 1) {
            myservo.write(pos);
            delay(30);
        }
        for (pos = 200; pos >= 5; pos -= 1) {
            myservo.write(pos);
            delay(30);
        }
        myservo.detach();
    }

    // Read IR sensors to determine slot occupancy
    IT_State_1 = digitalRead(IR_PIN_1);
    Parking_1 = (IT_State_1 == LOW) ? "FULL" : "EMPTY";

    IT_State_2 = digitalRead(IR_PIN_2);
    Parking_2 = (IT_State_2 == LOW) ? "FULL" : "EMPTY";

    // Update LCD display with parking slot statuses
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slot One =" + Parking_1);
    lcd.setCursor(0, 1);
    lcd.print("Slot Two =" + Parking_2);
    delay(1000);
}

// Function to get distance from ultrasonic sensor
int ultra() {
    int result = 0;
    unsigned long duration, distanceSum = 0;

    for (int i = 0; i < 3; i++) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        duration = pulseIn(echoPin, HIGH);
        distanceSum += duration / 58.2;
        delay(10);
    }
    result = distanceSum / 3;
    return result;
}
