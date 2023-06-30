

#include <Adafruit_NeoPixel.h>
#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOne.cpp
BnrOneA one;           // declarat ion of object variable to control the Bot'n Roll ONE A

//constants definition
#define PIN 10 // Pin Leds
#define NUMPIXELS 33
#define SSPIN  2       // Slave Select (SS) pin for SPI communication

int cont = 0;
unsigned long startMillis;  
unsigned long currentMillis;
unsigned long tempodanca = 80000;
unsigned long tempomusica = 36000;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void apagarLeds() {
  for (int i = 1; i <= NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    //delay(delayCustom);
  }
}

void LedsBrancos() {
  for (int i = 1; i <= NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
    pixels.setBrightness(50);
    //delay(delayCustom);
  }
}

void ledsFullRed() {
  for(int v = 1; v < 6; v++) 
  {
    pixels.setBrightness(50);
    for (int i = 1; i <= NUMPIXELS; i++) 
    {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }
    delay(500);
    for (int i = 1; i <= NUMPIXELS; i++) 
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
    delay(500);
  }
}

void levantabraco() {
  one.servo2(180);
  delay(800);
  one.servo2(0);
  delay(800);
}

void FadeParaBranco() {
  uint16_t i, j;

  for (j = 10; j < 255; j++) {
    for (i = 1; i <= NUMPIXELS; i++) {
      pixels.setPixelColor(i, 255, 255, j);
    }
    pixels.show();
    delay(5);
  } //TEMPO TOTAL: 1225ms
}

void piscaRandom() {
  for(int i = 0; i <= NUMPIXELS; i++) {
    int n1 = (rand() % 255) + 1;
    int n2 = (rand() % 255) + 1;
    int n3 = (rand() % 255) + 1;
    int n4 = (rand() % 255) + 1;
    pixels.setBrightness(n4);
    pixels.setPixelColor(i, pixels.Color(n1, n2, n3));
    pixels.show();
  }
}
void oval(){
  cont=1;
  piscaRandom();
  one.move(32,32);
  delay(4000);
  one.move(15,53);
  delay(3000);
  one.move(32,32);
  delay(2000);
  one.move(15,53);
  delay(3000);
  one.move(32,32);
  delay(1500);
  one.stop();
  delay(500);
  
  /*
  one.move(35,35);
  delay(4000);
  one.move(15,50);
  delay(2500);
  */
  /*
  one.move(35,70);
  delay(2000);
  one.move(40,40);
  delay(3000);
  one.move(35,70);
  delay(2000);
  one.move(40,40);
  delay(3000);
  one.move(35,70);
  delay(2000);*/
}
void setup()
{
    Serial.begin(57600);     // set baud rate to 57600bps for printing values at serial monitor.
    one.spiConnect(SSPIN);   // start SPI communication module
    one.stop();              // stop motors
    apagarLeds();
}

void loop()
{
  currentMillis=millis();
  //necessita de delay de (15(15000)-tempo de inicializacao do bnr)sec;para testes esta 2000 MUDAR !!!!!
  if (currentMillis>44000){

    if (currentMillis>117000)
    {
      apagarLeds();
      one.stop();
      delay(100000);
    }
    else if (currentMillis>tempodanca)
    {
      one.stop();
      delay(6600);
      one.move(-40,40);
      ledsFullRed();
      ledsFullRed();
      ledsFullRed();
      ledsFullRed();
      ledsFullRed();
      one.stop();
      delay(100000);

    }
    else
    {
      switch (cont) {
      case 0:
        oval();
        
        break;      
      case 1:
        //faz algo
        FadeParaBranco();
        levantabraco(); //1600ms
        levantabraco(); //1600ms
        levantabraco(); //1600ms       
        one.move(35,35);
        FadeParaBranco(); //1225ms
        one.stop();
        levantabraco(); //1600ms
        levantabraco(); //1600ms
        levantabraco(); //1600ms
        //26
        one.move(-35,-35);
        FadeParaBranco();//1225
        one.stop();
        FadeParaBranco();
        one.move(50,-50);
        delay(5000);
        one.stop();
        //FadeParaBranco();
        delay(500);
        levantabraco();
        apagarLeds();
        break;
      }

    }
  }
  else{
    one.stop();
  }

  /*
  one.move(40, 40);
  delay(4000);
  one.stop();
  one.move(0, 50);
  delay(700);
  one.move(50, 50);
  delay(500);
  one.stop();
  FadeParaBranco(); //1225ms
  levantabraco(); //1600ms
  levantabraco(); //1600ms
  levantabraco(); //1600ms
  apagarLeds(10); //330ms
  one.move(30, 30);
  delay(1000);
  one.move(14, 14);
  delay(500);
  one.move(-14, -14);
  delay(500);
  one.move(-30, -30);
  delay(1000);
  one.stop();
  one.move(0, 50);
  delay(2800);
  one.stop();
  FadeParaBranco(); //1225ms
  levantabraco(); //1600ms
  levantabraco(); //1600ms
  levantabraco(); //1600ms
  apagarLeds(10); //330ms

  one.move(30, 30);
  delay(1000);
  one.move(14, 14);
  delay(500);
  one.move(-14, -14);
  delay(500);
  one.move(-30, -30);
  delay(1000);
  one.stop();
  one.move(0, 50);
  delay(2800);
  one.stop();
  FadeParaBranco(); //1225ms
  levantabraco(); //1600ms
  levantabraco(); //1600ms
  apagarLeds(10); //330ms

  ledsFullRed(); // Parte do Carro

  FadeParaBranco(); //1225ms
  levantabraco(); //1600ms
  apagarLeds(10); //330ms
  one.move(15, 15);
  delay(900);
  piscaRandom();
  one.move(100,-100);
  delay(2500);
  piscaRandom();
  one.move(-100,100);
  delay(2500);
  one.stop();

  delay(100000);*/
////////////////////////////////////////////////////////
  /*
  one.move(50, 0);
  delay(900); //virar
  one.move(50, 50);
  delay(4000);
  one.move(0, 50);
  delay(900); //virar
  one.move(50, 50);
  delay(2000);
  one.move(0, 50);
  delay(900);//virar
  one.move(50, 50);
  delay(4000);
  one.stop();*/
 
 
    /*one.lcd2("    Forward "); // print data on LCD line 2
    one.move(50,50);          // Forward
    delay(1000);              // wait 1 second
    one.lcd2("     Stop   ");
    one.stop();               // Stop Motors
    delay(500);
    one.lcd2("   Backwards ");
    one.move(-50,-50);        // Backwards
    delay(1000);
    one.lcd2("     Stop   ");
    one.move(0,0);            // Stop Motors
    delay(500);
    one.lcd2("  Rotate Right ");
    one.move(50,-50);         // Rotate Right
    delay(500);
    one.lcd2("     Stop   ");
    one.stop();               // Stop
    delay(500);
    one.lcd2("  Rotate Left ");
    one.move(-50,50);         //Rotate Left
    delay(500);
    one.lcd2("     Stop   ");
    one.stop();               // Stop Motors
    delay(500);    
    one.lcd2("    Forward ");
    one.move(100,100);        // Forward
    delay(1000);
    one.lcd2("     Brake    ");
    one.brake(100,100);       // Stop motors with torque
    delay(800);
    one.lcd2("   Backwards ");
    one.move(-100,-100);      // Backwards
    delay(1000);
    one.lcd2("     Brake    ");
    one.brake(100,100);       // Stop motors with torque
    delay(800);
    one.lcd2("     Stop   ");
    one.stop();               // Stop Motors
    delay(1500);*/
}
