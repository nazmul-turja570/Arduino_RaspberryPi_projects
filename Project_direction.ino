#include <IRremote.h>
#include <Keypad.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <String.h>
#define CS 53
#define AUDIO_OUT  46 //5,6,11 or 46 on Mega, 9 on Uno, Nano, 
#define AUDIO_VOLUME 5

#include <newGSM.h>
byte rx = 2;
byte tx = 3;
char* number = "+8801558965159";
char* msg = "HELP AT ROOM NO. ";
newGSM myGSM(rx,tx);



TMRpcm sound;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS]={4,5,6,7};
byte colPins[COLS]={8,9,10,11};

Keypad keypad = Keypad (makeKeymap(keys),rowPins, colPins, ROWS, COLS);

int RECV_PIN = 12;  //IR Receiver Pin
unsigned long state; //Stores the last recieved room state code(HEX)
unsigned long state_values[] = {300,400,500,600}; //room state code preset values. state cannot take any value outisde of these 4.
//IR receiver often reads garbage values if the sender is far away. the current room state should not be set to a garabge value.
//THE VALUES NEED TO BE CHEKCED BEFORE ASSIGNING TO STATE
int inRoom; //Finds out if we are inside a room or not
unsigned long currentmillis; //time in milliseconds from epoch
unsigned long prevmillis;
unsigned long soundstart;
unsigned long soundlength = 8000;
unsigned long restTime = 2000; //Delay between checking for the next signal in milliseconds. Keep it a multiple of 100 ms
unsigned long graceTime = 4000; //Delay between detecting getting into a room and getting out.
int grace = 0;
int transition = 0;
int stop_keypad = 0;
char last_played[80] = "intro.wav";
char temp[10];
char temp1[80];
char butt[2] = "r";
IRrecv irrecv(RECV_PIN);  //Library Object irrecv

decode_results results;

void play_sound(int room, char button){
  if (room != 0){
    butt[0] = button;
    itoa((room-200)/10,temp,10);
    strcpy(last_played,temp);
    strcat(last_played,butt);
    Serial.println(last_played);
    strcat(last_played,".wav");
    Serial.println(last_played);
    sound.play(last_played);
    stop_keypad = 1;
    soundstart = millis();
  }
}
//&& stop_keypad != 1

void entrance_sound(int room){
  if (room!=0){
    itoa((room-200),temp,10);
    strcpy(last_played,temp);
    strcat(last_played,".wav");
    Serial.println(last_played);
    sound.play(last_played);
    stop_keypad = 1;
    soundstart = millis();
  }
}


void repeat_sound(){
    Serial.println(last_played);
    sound.play(last_played);
    stop_keypad = 1;
    soundstart = millis();
}


void setup()
{
  Serial.begin(9600);
  sound.speakerPin = AUDIO_OUT;
  sound.setVolume(AUDIO_VOLUME);
  if(!SD.begin(CS)){
    Serial.println("Initialization failure");
    while(1);
    //You can use an LED (RED preferably) to show initialization failed.
  }
  Serial.println("Initialization successful");

  
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  state = 0;
  inRoom = 0;
  grace = 0;
  currentmillis = millis();
  prevmillis = millis();
  myGSM.begin(9600);
  myGSM.ATtest(); //it'll print OK in Serial Monitor, if everything works fine. Else it'll print Error.
  delay(2000);
}

void loop() {
  /*==============================================CODE FOF KEYPAD ==================================
   * /
   */
  if (millis() -  soundstart > soundlength){
    stop_keypad = 0;
  }
  char key = keypad.getKey();
  if(key && !stop_keypad){
    Serial.println(key);
    //Code for what to do when getting a keypad pressed goes here
    switch(key){
      case '1':play_sound(state,key);
              break;
      case '2':play_sound(state,key);
              break;
      case '3':play_sound(state,key);
              break;
      case '4':play_sound(state,key);
              break;
      case '*':repeat_sound();
              break;
      case '#':itoa((state-200)/100,temp,10);
              strcpy(temp1,msg);
              strcat(temp1, temp);
              myGSM.SendTextMessage(temp1,number);
              break;
      default:
              break;
    }
    
  }

  /*==============================================CODE FOR IR RECEIVER ==================================
   * /
   */
  if (grace){
      currentmillis = millis();
      if (currentmillis - prevmillis > graceTime){
        grace = 0;
      }
  }
  else{
  if (transition){
    currentmillis = millis();
    if ((currentmillis - prevmillis > restTime) && inRoom == 0){ //Timer exceeded and no valid sensor data (Entering Room Loop)
      inRoom = 1;
      transition = 0;
      Serial.println("We are in the Room");
      prevmillis = millis();
      grace = 1;
    }
    else if ((currentmillis - prevmillis > restTime) && inRoom == 1){ //Timer exceeded and no valid sensor data (Exiting Room Loop)
      inRoom = 0;
      transition = 0;
      Serial.println("We are out of the Room");
      state = 0;    //In Corridor 
    }
    
  }
  
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);   //Received room state code stored in results.value
    if (state != results.value && inRoom==0){  //Room Change! Entering Room Loop Starts
      if (results.value == state_values[0] || results.value == state_values[1] || results.value == state_values[2] || results.value == state_values[3]){
       state = results.value;
       Serial.print("Entering Room : ");
       Serial.println(state);
       entrance_sound(state);
       prevmillis = millis();
       transition = 1;
      }
    }
    else if (state == results.value && inRoom==0){ //Reset entering room loop timer
      prevmillis = millis();  //Reset the timer 
      Serial.println("Timer Reset");
    }
    else if (state == results.value && inRoom==1){ //Start exiting room loop timer
      prevmillis = millis();  //Start Timer
      transition = 1;
      Serial.println("We are leaving the room");
    }
    
    irrecv.resume(); // Receive the next value
  }
  }
  /*=====================================DELAY==================================
   * /
   */
  delay(100);
}
