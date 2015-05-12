
/*  Heart beat monitor 1.0    by 
Jonathan Williams, Teresa Doan
Joel Murphy and Yury Gitman   http://www.pulsesensor.com

----------------------  Notes ----------------------  ---------------------- 
This code:
1) Blinks an LED to User's Live Heartbeat   PIN 13
2) Determines BPM
3) 
4) Prints All of the Above to Serial

Read Me:
https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md   

Wire RGB led like this:
http://fritzing.org/media/fritzing-repo/projects/s/simple-rgb-led-on-an-arduino-uno/images/RGBLed.png

Read this to decide on Heart rate values BPM = Beats per Minute
http://www.heart.org/HEARTORG/Conditions/More/MyHeartandStrokeNews/All-About-Heart-Rate-Pulse_UCM_438850_Article.jsp

 ----------------------       ----------------------  ----------------------
*/

//  Variables
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

const int iRed = 8;               // Red segment of RGB Led wired to pin 8
const int iGreen = 9;             // Green segment of RGB Led wired to pin 9
const int iBlue = 10;             // Blue segment of RGB Led wired to pin 10

// ON / OFF
const byte ON = 0;                 // ON and OFF are defined, the above are wired to ground.
const byte OFF = 255;

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
   // AND APPLY THAT VOLTAGE TO THE A-REF PIN
//   analogReference(EXTERNAL);   
}

//////////////////////////////////////////////////////
//
// RGBOut
//
void RGBOut(byte bRed, byte bGreen, byte bBlue) {
  analogWrite(iRed, bRed);
  analogWrite(iGreen, bGreen);
  analogWrite(iBlue, bBlue);
}

//  Where the Magic Happens
void loop(){
  
    serialOutput() ;       
    
  if (QS == true){     //  A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        digitalWrite(blinkPin,HIGH);     // Blink LED, we got a beat. 
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.     
        QS = false;                      // reset the Quantified Self flag for next time    
       } 
      else { 

      digitalWrite(blinkPin,LOW);            // There is not beat, turn off pin 13 LED
      }
        ledFadeToBeat();                      // Makes the LED Fade Effect Happen 
        
      // Start JHW mods
      
       if ((BPM>0) && (BPM<50) ) 
       {   // Heart-beat per minute is more than 0 but less than 50
        
        RGBOut(OFF, ON, OFF); // Light up blue                         
 }
 else {
          RGBOut(OFF, OFF, OFF);            // light is off
 }
      
       if ((BPM>50) && (BPM<100) ) 
       {   // Heart-beat per minute is more than 0 but less than 50
        
        RGBOut(OFF, OFF, ON) ;  // Light up green                         
 }
 else {
          RGBOut(OFF, OFF, OFF);            // light is off
 }
 if (BPM>100)
       {   // Heart-beat per minute is more than 0 but less than 50
        
        RGBOut(ON, OFF, OFF) ;  // Light up red                        
 }
 else {
          RGBOut(OFF, OFF, OFF);            // light is off
 }
      
  delay(20);                             //  take a break
}









