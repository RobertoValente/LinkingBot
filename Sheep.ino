#include <BnrOneA.h>   // Bot'n Roll ONE A library
#include <EEPROM.h>    // EEPROM reading and writing
#include <SPI.h>       // SPI communication library required by BnrOne.cpp
BnrOneA one;           // declaration of object variable to control the Bot'n Roll ONE A

#include <Adafruit_NeoPixel.h>

//constants definition
#define PIN 5
#define NUMPIXELS 2
#define SSPIN  2      // Slave Select (SS) pin for SPI communication

int contador=0;
unsigned long startMillis;  
unsigned long currentMillis;
unsigned long tempodanca = 80000;
unsigned long tempomusica = 36000;

int motL=0, motR=0;
double kL=3; //Linear gain <> Ganho linear
int vel=50;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void ligarLeds() {
  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }  
}

void offLeds() {
  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }  
}
void piscaRed() {
  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
  }  
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

/*void goV1() //function for sheep to run
{
  byte obstacles=one.obstacleSensors(); //Read obstacle sensors
  switch(obstacles){
        case 0:   // obstacle detected on Left sensor
            one.move(40,40);
            one.lcd2("Nenhum");
            break;
        case 1:
            one.move(20,70);     // Move in Circles
            one.lcd2("Esq");
            break;
        case 2:   // obstacle detected on Right sensor
            one.lcd2("Dir");
            one.move(70,20);    // Rotate Left
            break;
        case 3:
            one.move(40,40);    // Rotate Right
            one.lcd2("Deteta 2");
            break;
        
      }   
}
*/
void setup()
{
    //setup routines -> runs only once when program starts
    
   // delay(1500); // wait 1,5 second to stabilize sensors  
    one.spiConnect(SSPIN);   // start the SPI communication module
    one.stop(); // stop motors
    one.obstacleEmitters(ON);// activate obstacles detection
    pixels.begin();     
    
}

void loop()
{
    currentMillis=millis();
    pixels.clear();
    byte rangeL=one.readRangeL(); // read left obstacle sensor range 
    byte rangeR=one.readRangeR(); // read right obstacle sensor range
    one.lcd1("Range Left : ",rangeL);
    one.lcd2("Range Right: ",rangeR);
    // float rangeAll=(rangeR+rangeL)/2; //average sensors
    //ligarLeds();
    //if condition to know if its starting or not,if its starting then it will go straight as a starting point,if not it will go to the "go" function and will follow the main car
    //currentMillis=currentMillis-1000;
      if (contador==0) 
      {
              if ((rangeR<=15) && (rangeL<=15))
              {
                one.move(30,30);
               delay(50);
              //  one.stop();
                contador++;
              }
      }
      else if (contador>0){
            if (currentMillis>115000){
              offLeds();
              one.stop();
              delay(100000);
            }
            else if (currentMillis>tempodanca)
            {
              
              one.move(-40,40);
              piscaRed();
              delay(28000);
              currentMillis=millis();

            }
            else
            {

              if (currentMillis>57000) {
                ligarLeds();
                one.stop();
                delay(7525);
                one.move(30,30);
                delay(1225);
                one.stop();
                delay(4800);
                one.move(-38,-38);
                delay(1225);//14000
                one.stop();
                delay(1500);//15500
                one.move(-40,40);
                delay(2000);//17500
                one.stop();
                delay(500);//18000
                one.move(20,20);
                delay(2000);
                one.move(20,20);
                delay(500);
                one.stop();
                delay(2775);
                offLeds();
                delay(7100);
              }
              else{
                ligarLeds();

               if(abs(rangeL-rangeR)<=2){
                  motR=25;//25
                  motL=25;//25
               }
               else if(rangeL>rangeR)
                {
                  motL=(vel-(rangeL*kL));
                  motR=vel;
                }
                else if(rangeL<rangeR)
                {
                  motL=vel;
                  motR=(vel-(rangeR*kL));      
                }
                one.move(motL,motR);
                delay(50);
              //goV1();
              }
            }
      }
   

}
