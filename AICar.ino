//The DIY Life
//Michael Klements
//16/04/2021
//HuskyLens Object Tracking Robot

#include <Adafruit_NeoPixel.h>
#include "HUSKYLENS.h"  //Import the required libraries
#include "SoftwareSerial.h"
#include <AFMotor.h>

SoftwareSerial mySerial(2,3);

#define PIN 4
#define NUMPIXELS 9
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long currentMillis;

// High / Low - Frente
// Low / Hight - Trás

//MOTOR A
#define enA 10
#define in1 9
#define in2 8

//MOTOR B
#define enB 5
#define in3 7
#define in4 6

/* CÓDIGO POSSÍVELMENTE INÚTIL
AF_DCMotor rightBack(1);  //Create an object to control each motor
AF_DCMotor rightFront(2);
AF_DCMotor leftFront(3);
AF_DCMotor leftBack(4);*/

byte maxMotorSpeed = 50;  //The normal motor speed
int motorOffset = 25;     //Factor to account for one side being more powerful
int objectWidth = 50;
int turnGain = 12;
int offCentre = 20;
int distGain = 6;

int leftLimit = 160 - offCentre;
int rightLimit = 160 + offCentre;

int leftSp = 0;
int rightSp = 0;

bool isTurning = false;  //Track when turning left and right
bool isTurningLeft = true;

HUSKYLENS huskylens;  //Create a new Huskeylens object
int ID1 = 1;

void printResult(HUSKYLENSResult result);

void setup() {
  Serial.begin(115200);  //Start serial communication
  pixels.begin();
  Wire.begin();          //Connect to Huskylens
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }

  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION);  //Switch the algorithm to object tracking.
}

void frente(int nDelay){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(nDelay);
}

void tras(int nDelay) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(nDelay);
}

void parar(int nDelay){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  delay(nDelay);
}

void rodaRandom() {
  ligarLeds(255, 0, 0);
  int lado = random(1, 3); // retorna: 1 ou 2
  //int trasDelay = random(200, 1001); // retorna: 200 até 1000
  //int giraDelay = random(500, 3001); // retorna: 500 até 3000
  //int frenteDelay = random(400, 2001); // retorna: 400 até 2000

  int trasDelay = 400;
  int giraDelay = 600;
  int frenteDelay = 1000;

  tras(trasDelay);

  switch (lado) {
    case 1:
      // Roda para a direita

      // High / Low - Frente
      // Low / Hight - Trás
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW); //FRENTE
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH); //TRÁS
      delay(giraDelay);
      break;
    case 2:
      // Roda para a esquerda

      // High / Low - Frente
      // Low / Hight - Trás
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH); //TRÁS
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); //FRENTE
      delay(giraDelay);
      break;
  }

  frente(frenteDelay);
  apagarLeds();
}

void ligarLeds(int r, int g, int b) {
  for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.show();
    }
}

void apagarLeds() {
  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void acenderLeds(int r, int g, int b, int vezes, int nDelay) {
  for(int v = 0; v < vezes; v++) {
    pixels.setBrightness(100);
    for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.show();
      delay(nDelay);
    }
    for (int i = NUMPIXELS; i >= 0; i--) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(nDelay);
    }
  }

  for (int i = 1; i <= NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
} 

void acenderLedsRed(int vezes) {
  for(int v = 0; v < vezes; v++) {
    pixels.setBrightness(100);
    for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }
    delay(200);
    for (int i = 0; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
    delay(200);
  }

  delay(400);
  for (int i = 0; i <= NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
  }
}

void rodarEsquerda(int nDelay) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(nDelay);
}

void rodarDireita(int nDelay) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(nDelay);
}

void loop() {
  currentMillis = millis();

  if(currentMillis <= 30000) {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    int32_t error;
    int id;
    if (!huskylens.request()) {
      Serial.println(F("Fail to request objects from HUSKYLENS!"));
    } else if (!huskylens.request()) {
      Serial.println(F("Fail to request objects from HUSKYLENS!"));
    } else if (huskylens.isLearned()) {
      HUSKYLENSResult result = huskylens.read();
      id = result.ID;
      if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.println(String() + F("ID=") + result.ID);
        switch (id) {
          case 2:
            rodaRandom();
            parar(500);
            break;
          case 3:
            ligarLeds(random(0, 255), random(0, 255), random(0, 255));
            frente(1000);
            apagarLeds();
            parar(500);
            break;
          case 1:
            parar(100);
            break;
        }
      }
    }
  }

  if((currentMillis >= 50000) && (currentMillis < 86000)) {
    frente(1000);
    rodarEsquerda(1200);
    parar(500);
    tras(1000);
    rodarDireita(1200);
    parar(500);
    frente(1000);
    parar(600);

    // 7.5 SEGUNDOS
    acenderLeds(0, 255, 0, 1, 200); //3600ms
    apagarLeds();
    tras(200);
    parar(100);
    acenderLeds(0, 0, 255, 1, 200); //3600ms
    apagarLeds();
    //////////////////////

    frente(1000);
    parar(0);
    rodarEsquerda(1200);
    parar(500);
    tras(1000);
    rodarDireita(1200);
    parar(500);
    frente(1000);
    parar(600);

    // 7.5 SEGUNDOS
    acenderLeds(0, 255, 0, 1, 200); //3600ms
    apagarLeds();
    tras(200);
    parar(100);
    acenderLeds(0, 0, 255, 1, 200); //3600ms
    apagarLeds();
    //////////////////////

    frente(1000);
    rodarEsquerda(1200);
    parar(500);
    tras(1000);
    rodarDireita(1200);
    parar(500);
    frente(1000);
    parar(600);

    parar(500);
  }

  if((currentMillis >= 86000) && (currentMillis <= 118000)) {
    acenderLedsRed(4);
    rodarDireita(7000);
    rodarEsquerda(7000);
    apagarLeds();
    delay(500);
    acenderLedsRed(4);
    rodarDireita(7000);
    rodarEsquerda(7000);
    apagarLeds();
  }

  if((currentMillis >= 116000) && (currentMillis <= 124000)) {
    rodarDireita(2000);
    rodarEsquerda(2000);
    parar(100);
    delay(999999999);
  }
}

/* CÓDIGO POSSÍVELMENTE INÚTIL
void moveForward()  //Set all motors to run forward
{
  rightBack.run(FORWARD);
  rightFront.run(FORWARD);
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
}

void stopMove()  //Set all motors to stop
{
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
}

void setMotorSpeed(int leftTempSp, int rightTempSp) {
  rightBack.setSpeed(rightTempSp);  //Set the motors to the motor speed
  rightFront.setSpeed(rightTempSp);
  leftFront.setSpeed(leftTempSp + motorOffset);
  leftBack.setSpeed(leftTempSp + motorOffset);
}*/

void printResult(HUSKYLENSResult result)  //Display the results on the serial monitor
{
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  } else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  } else {
    Serial.println("Object unknown!");
  }
}
