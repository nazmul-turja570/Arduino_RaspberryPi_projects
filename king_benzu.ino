/*
 * CIRCUIT CONNECTIONS
 * SD card circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 51 on Arduino Mega
 ** MISO - pin 50 on Arduino Mega
 ** SCK - pin 52 on Arduino Mega
 ** CS - pin 53 on Arduino Mega
 * SPEAKER OUT - pin 11 on Ardino Mega
 */
 /*
  * LCD CONNECTIONS:
  * The circuit:
 * LCD RS(4) pin to digital pin 9
 * LCD Enable(6) pin to digital pin 8
 * LCD D4(11) pin to digital pin 6
 * LCD D5(12) pin to digital pin 5
 * LCD D6(13) pin to digital pin 4
 * LCD D7(14) pin to digital pin 3
 * LCD R/W(5) pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * VSS(1) to ground
 * VDD(2) to 5V
 * LED+(15) through a resistor(220 ohm) to 5V
 * LED-(16) to ground
  */
  /*
   * TOUCH SWITCH CONNECTION
   * Swtich(1-6) to digital (30,32,34,36,38,40) respectively
   * SENDER pin connected to 2
   */
#include <CapacitiveSensor.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define THOLD 300         //USE IT TO GIVE THE VALUE OF THRESHOLD
#define COUNTNUMBER 3     //USE IT TO DEFINE THE COUNT NUMBER AFTER WHICH INPUT IS TAKEN INTO ACCOUNT
#define DELAYTIME 20    //USE IT TO DEFINE HOW MANY TIME SHOULD BE DELAYED BETWEEN TWO LOOP ROTATION
#define SENDER 2         //SENDING PIN
#define CHIPSELECT 53       //CHIPSELECT PIN
#define SPEAKERPIN 11       //OUTPUT TO THE SPEAKER
#define SPEAKER_VOLUME 5    //SPEAKER VOLUME
LiquidCrystal lcd(9, 8, 6, 5, 4, 3);
//Byte arrays for Bangla alphabet
byte ko[8] = {0x1F,0x04,0x0F,0x15,0x14,0x14,0x0C,0x04};
byte kho[8] = {0x01,0x15,0x1D,0x05,0x09,0x11,0x11,0x1F};
byte go[8] = {0x0F,0x11,0x1D,0x05,0x09,0x01,0x01,0x01};
byte gho[8] = {0x1F,0x11,0x15,0x1D,0x05,0x09,0x19,0x0F};
byte uo[8] = {0x0E,0x0A,0x1E,0x04,0x17,0x11,0x19,0x0E};
byte co[8] = {0x1F,0x08,0x08,0x0E,0x09,0x09,0x0A,0x0C};
byte cho[8] = {0x1F,0x10,0x1C,0x16,0x1E,0x04,0x08,0x0F};
byte jo[8] = {0x0F,0x12,0x15,0x15,0x13,0x15,0x09,0x02};
byte jho[8] = {0x1D,0x0D,0x15,0x15,0x15,0x15,0x0F,0x05};
byte io[8] = {0x0C,0x14,0x17,0x05,0x07,0x15,0x17,0x1C};



int count = 0;
int row = 0, col = -1;
int start = 0;
int TEST = 0;
int TEST1 = 0;
int mode = 0;
int cur1 = 0, cur2 = 0, cur3 = 0, cur4 = 0, cur5 = 0, cur6 = 0;
int prev1 = 0, prev2 = 0, prev3 = 0, prev4 = 0, prev5 = 0, prev6 = 0;

TMRpcm sound;


CapacitiveSensor   cs_2_1 = CapacitiveSensor(SENDER, 30);       
CapacitiveSensor   cs_2_2 = CapacitiveSensor(SENDER, 32);        
CapacitiveSensor   cs_2_3 = CapacitiveSensor(SENDER, 34);
CapacitiveSensor   cs_2_4 = CapacitiveSensor(SENDER, 36);
CapacitiveSensor   cs_2_5 = CapacitiveSensor(SENDER, 38);
CapacitiveSensor   cs_2_6 = CapacitiveSensor(SENDER, 40);

void show_in_lcd();
void listen_in_speaker();
void set_cursor();
void set_cursornl();
void do_backspace();

void setup()                    
{
    

  
   cs_2_1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibration on channel 1
   cs_2_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_2_3.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_2_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_2_5.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_2_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
   Serial.begin(9600);
   sound.speakerPin = SPEAKERPIN;
   sound.setVolume(SPEAKER_VOLUME);
   lcd.begin(16, 2);
   lcd.clear();
   Serial.println("SD card initializing . . .");
   lcd.setCursor(0,0);
   lcd.print("INITIALIZING");
   delay(1000);
   if(!SD.begin(CHIPSELECT)){
    Serial.println("Initialization failure");
    lcd.setCursor(0, 1);
    lcd.print("INIT FAILURE");
    while(1);
   }
   lcd.setCursor(0, 1);
   lcd.print("cholo kheli");
   Serial.println("Khela Shuru");
   sound.play("ins.wav");
   delay(3000);
   lcd.clear();
   sound.play("strt.wav");

    lcd.createChar(0, ko);
    lcd.createChar(1, kho); 
    lcd.createChar(2, go);
    lcd.createChar(3, gho);
    lcd.createChar(4, uo);
    lcd.createChar(5, co);
    lcd.createChar(6, cho);
    lcd.createChar(7, jo);
}

void loop()                    
{
    //int TEST;
    //TEST = TST;
    long DEBUGTIME = millis();
    long total1 =  cs_2_1.capacitiveSensor(30);
    long total2 =  cs_2_2.capacitiveSensor(30);
    long total3 =  cs_2_3.capacitiveSensor(30);
    long total4 =  cs_2_4.capacitiveSensor(30);
    long total5 =  cs_2_5.capacitiveSensor(30);
    long total6 =  cs_2_6.capacitiveSensor(30);

    Serial.println(millis() - DEBUGTIME);        // check on performance in milliseconds
    //Serial.print("\t");                    // tab character for debug windown spacing
    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                  // print sensor output 3
    Serial.print("\t");
    Serial.print(total4);                  // print sensor output 4
    Serial.print("\t");
    Serial.print(total5);                  // print sensor output 5
    Serial.print("\t");
    Serial.println(total6);                // print sensor output 6
    
    if(total1 > THOLD){
      cur1 = 1;
    }
    if(total2 > THOLD){
      cur2 = 1;
    }
    if(total3 > THOLD){
      cur3 = 1;
    }
    if(total4 > THOLD){
      cur4 = 1;
    }
    if(total5 > THOLD){
      cur5 = 1;
    }
    if(total6 > THOLD){
      cur6 = 1;
    }

    Serial.println("current");
    Serial.print(cur1);                             //for debugging
    Serial.print("\t");
    Serial.print(cur2);
    Serial.print("\t");
    Serial.print(cur3);
    Serial.print("\t");
    Serial.print(cur4);
    Serial.print("\t");
    Serial.print(cur5);
    Serial.print("\t");
    Serial.println(cur6);
    Serial.println("previous");
    Serial.print(prev1);
    Serial.print("\t");
    Serial.print(prev2);
    Serial.print("\t");
    Serial.print(prev3);
    Serial.print("\t");
    Serial.print(prev4);
    Serial.print("\t");
    Serial.print(prev5);
    Serial.print("\t");
    Serial.println(prev6);                          //for debugging
    
    if((prev1 + prev2 + prev3 + prev4 + prev5 + prev6) == 0){
      start = 1;
    }
    
    if(start)
    {
      if(!count){
        count++;
      }
      else if(cur1==prev1 && cur2==prev2 && cur3==prev3 && cur4==prev4 && cur5==prev5 && cur6==prev6 && (cur1 + cur2 + cur3 + cur4 + cur5 + cur6)){
        count++;
      }
      else{
        count = 0;        //resetting the count if any touch is misplaced
      }

      Serial.println("count");
      Serial.println(count);          //for debugging
      
    }

    if(count == COUNTNUMBER){
      if(cur1){
        TEST = TEST + 40;
      }
      if(cur2){
        TEST = TEST + 20;
      }
      if(cur3){
        TEST = TEST + 10;
      }
      if(cur4){
        TEST = TEST + 4;
      }
      if(cur5){
        TEST = TEST + 2;
      }
      if(cur6){
        TEST = TEST + 1;
      }
      show_in_lcd();
      listen_in_speaker();
      count = 0;                                 //again resetting count for next character read
      TEST1 = TEST;
      start = 0;
      if(TEST == 17){
        sound.play("num.wav");
      }
      if(TEST == 1){
        sound.play("cap.wav");
      }
    }

    prev1 = cur1;
    prev2 = cur2;
    prev3 = cur3;
    prev4 = cur4;
    prev5 = cur5;
    prev6 = cur6;
    cur1 = 0;
    cur2 = 0;
    cur3 = 0;
    cur4 = 0;
    cur5 = 0;
    cur6 = 0;

    Serial.println("TEST1");
    Serial.println(TEST1);
    Serial.println("TEST");
    Serial.println(TEST);
    
    TEST = 0;                              //resetting TEST for next input
    delay(DELAYTIME);                             // arbitrary delay to limit data to serial port 
}

void show_in_lcd()
{
  //HERE THE CODE FOR SHOWING THE CHARACTER IN LCD IS WRITTEN
  //Number mode
  if(TEST1 == 17){
    if(TEST == 40){
      set_cursor();
      lcd.print("1");
    }
    else if(TEST == 60){
      set_cursor();
      lcd.print("2");
    }
    else if(TEST == 44){
      set_cursor();
      lcd.print("3");
    }
    else if(TEST == 46){
      set_cursor();
      lcd.print("4");
    }
    else if(TEST == 42){
      set_cursor();
      lcd.print("5");
    }
    else if(TEST == 64){
      set_cursor();
      lcd.print("6");
    }
    else if(TEST == 66){
      set_cursor();
      lcd.print("7");
    }
    else if(TEST == 62){
      set_cursor();
      lcd.print("8");
    }
    else if(TEST == 24){
      set_cursor();
      lcd.print("9");
    }
    else if(TEST == 26){
      set_cursor();
      lcd.print("0");
    }
  }
  //Capital mode
  else if(TEST1 == 1){
    if(TEST == 40){
      set_cursor();
      lcd.print("A");
    }
    else if(TEST == 60){
      set_cursor();
      lcd.print("B");
    }
    else if(TEST == 44){
      set_cursor();
      lcd.print("C");
    }
    else if(TEST == 46){
      set_cursor();
      lcd.print("D");
    }
    else if(TEST == 42){
      set_cursor();
      lcd.print("E");
    }
    else if(TEST == 64){
      set_cursor();
      lcd.print("F");
    }
    else if(TEST == 66){
      set_cursor();
      lcd.print("G");
    }
    else if(TEST == 62){
      set_cursor();
      lcd.print("H");
    }
    else if(TEST == 24){
      set_cursor();
      lcd.print("I");
    }
    else if(TEST == 26){
      set_cursor();
      lcd.print("J");
    }
    else if(TEST == 50){
      set_cursor();
      lcd.print("K");
    }
    else if(TEST == 70){
      set_cursor();
      lcd.print("L");
    }
    else if(TEST == 54){
      set_cursor();
      lcd.print("M");
    }
    else if(TEST == 56){
      set_cursor();
      lcd.print("N");
    }
    else if(TEST == 52){
      set_cursor();
      lcd.print("O");
    }
    else if(TEST == 74){
      set_cursor();
      lcd.print("P");
    }
    else if(TEST == 76){
      set_cursor();
      lcd.print("Q");
    }
    else if(TEST == 72){
      set_cursor();
      lcd.print("R");
    }
    else if(TEST == 34){
      set_cursor();
      lcd.print("S");
    }
    else if(TEST == 36){
      set_cursor();
      lcd.print("T");
    }
    else if(TEST == 51){
      set_cursor();
      lcd.print("U");
    }
    else if(TEST == 71){
      set_cursor();
      lcd.print("V");
    }
    else if(TEST == 27){
      set_cursor();
      lcd.print("W");
    }
    else if(TEST == 55){
      set_cursor();
      lcd.print("X");
    }
    else if(TEST == 57){
      set_cursor();
      lcd.print("Y");
    }
    else if(TEST == 53){
      set_cursor();
      lcd.print("Z");
    }
  }
  //General mode
  else{
    if(TEST == 40){
      set_cursor();
      lcd.write(byte(0));
    }
    else if(TEST == 60){
      set_cursor();
      lcd.write(1);
    }
    else if(TEST == 44){
      set_cursor();
      lcd.write(2);
    }
    else if(TEST == 46){
      set_cursor();
      lcd.write(3);
    }
    else if(TEST == 42){
      set_cursor();
      lcd.write(4);
    }
    else if(TEST == 64){
      set_cursor();
      lcd.write(5);
    }
    else if(TEST == 66){
      set_cursor();
      lcd.write(6);
    }
    else if(TEST == 62){
      set_cursor();
      lcd.write(7);
    }
//    else if(TEST == 24){
//      set_cursor();
//      lcd.createChar(1, jho);
//      lcd.write(1);
//    }
//    else if(TEST == 26){
//      set_cursor();
//      lcd.createChar(2, io);
//      lcd.write(2);
//    }
    else if(TEST == 50){
      set_cursor();
      lcd.print("k");
    }
    else if(TEST == 70){
      set_cursor();
      lcd.print("l");
    }
    else if(TEST == 54){
      set_cursor();
      lcd.print("m");
    }
    else if(TEST == 56){
      set_cursor();
      lcd.print("n");
    }
    else if(TEST == 52){
      set_cursor();
      lcd.print("o");
    }
    else if(TEST == 74){
      set_cursor();
      lcd.print("p");
    }
    else if(TEST == 76){
      set_cursor();
      lcd.print("q");
    }
    else if(TEST == 72){
      set_cursor();
      lcd.print("r");
    }
    else if(TEST == 34){
      set_cursor();
      lcd.print("s");
    }
    else if(TEST == 36){
      set_cursor();
      lcd.print("t");
    }
    else if(TEST == 51){
      set_cursor();
      lcd.print("u");
    }
    else if(TEST == 71){
      set_cursor();
      lcd.print("v");
    }
    else if(TEST == 27){
      set_cursor();
      lcd.print("w");
    }
    else if(TEST == 55){
      set_cursor();
      lcd.print("x");
    }
    else if(TEST == 57){
      set_cursor();
      lcd.print("y");
    }
    else if(TEST == 53){
      set_cursor();
      lcd.print("z");
    }
    else if(TEST == 33){
      set_cursor();
      lcd.print(" ");
    }
    else if(TEST == 23){
      set_cursor();
      lcd.print(".");
    }
    else if(TEST == 31){
      set_cursor();
      lcd.print("?");
    }
    else if(TEST == 32){
      set_cursor();
      lcd.print("!");
    }
    else if(TEST == 20){
      set_cursor();
      lcd.print(",");
    }
    else if(TEST == 10){
      set_cursor();
      lcd.print("'");
    }
    //newline character
    else if(TEST == 11){
      set_cursornl();
    }
    //backspace
    else if(TEST == 15){
      do_backspace();
    }
    else if(TEST == 77){
      lcd.clear();
      col = -1;
      row = 0;
    }
  }
}

void listen_in_speaker()
{
  //HERE THE CODE FOR LISTENING IN SPEAKER WILL BE WRITTEN
  //Number mode
  if(TEST1 == 17){
    if(TEST == 40){
      sound.play("1.wav");
    }
    else if(TEST == 60){
      sound.play("2.wav");
    }
    else if(TEST == 44){
      sound.play("3.wav");
    }
    else if(TEST == 46){
      sound.play("4.wav");
    }
    else if(TEST == 42){
      sound.play("5.wav");
    }
    else if(TEST == 64){
      sound.play("6.wav");
    }
    else if(TEST == 66){
      sound.play("7.wav");
    }
    else if(TEST == 62){
      sound.play("8.wav");
    }
    else if(TEST == 24){
      sound.play("9.wav");
    }
    else if(TEST == 26){
      sound.play("0.wav");
    }
  }
  //General mode
  else{
    if(TEST == 40){
      sound.play("a.wav");
    }
    else if(TEST == 60){
      sound.play("b.wav");
    }
    else if(TEST == 44){
      sound.play("c.wav");
    }
    else if(TEST == 46){
      sound.play("d.wav");
    }
    else if(TEST == 42){
      sound.play("e.wav");
    }
    else if(TEST == 64){
      sound.play("f.wav");
    }
    else if(TEST == 66){
      sound.play("g.wav");
    }
    else if(TEST == 62){
      sound.play("h.wav");
    }
    else if(TEST == 24){
      sound.play("i.wav");
    }
    else if(TEST == 26){
      sound.play("j.wav");
    }
    else if(TEST == 50){
      sound.play("k.wav");
    }
    else if(TEST == 70){
      sound.play("l.wav");
    }
    else if(TEST == 54){
      sound.play("m.wav");
    }
    else if(TEST == 56){
      sound.play("n.wav");
    }
    else if(TEST == 52){
      sound.play("o.wav");
    }
    else if(TEST == 74){
      sound.play("p.wav");
    }
    else if(TEST == 76){
      sound.play("q.wav");
    }
    else if(TEST == 72){
      sound.play("r.wav");
    }
    else if(TEST == 34){
      sound.play("s.wav");
    }
    else if(TEST == 36){
      sound.play("t.wav");
    }
    else if(TEST == 51){
      sound.play("u.wav");
    }
    else if(TEST == 71){
      sound.play("v.wav");
    }
    else if(TEST == 27){
      sound.play("w.wav");
    }
    else if(TEST == 55){
      sound.play("x.wav");
    }
    else if(TEST == 57){
      sound.play("y.wav");
    }
    else if(TEST == 53){
      sound.play("z.wav");
    }
    else if(TEST == 33){
      sound.play("space.wav");
    }
    else if(TEST == 23){
      sound.play("fst.wav");
    }
    else if(TEST == 31){
      sound.play("ques.wav");
    }
    else if(TEST == 32){
      sound.play("excl.wav");
    }
    else if(TEST == 20){
      sound.play("cma.wav");
    }
    else if(TEST == 10){
      sound.play("apost.wav");
    }
    //newline character
    else if(TEST == 11){
      sound.play("nlin.wav");
    }
    //backspace
    else if(TEST == 15){
      sound.play("bspac.wav");
    }
    else if(TEST == 77){
      sound.play("clear.wav");   
    }
  }
}

void set_cursor()
{
  col++;
  if(col > 15){
    row = row + 1;
    col = (col % 16);
  }
  if(row > 1){
    row = (row % 2);
  }
  lcd.setCursor(col, row);          //in setCursor coloumn comes first in syntax. So setCursor(col, row)
}

void set_cursornl()
{
  row++;
  if(row > 1){
    row = row % 2;
  }
  col = -1;
}

void do_backspace()
{
  lcd.setCursor(col, row);
  lcd.print(" ");
  col--;
  if(col < 0){
    col = 15;
    row = 1 - row;                   //if previous row is 0, new row is 1. if previous row is 1, new row is 0
  }
}
