#define IR 2
#define blade 5
#define count 30
#define delaystab 10
#define ircheck 5
#define iroffset 3
unsigned long rpm = 0;
unsigned long tim=0;
unsigned long timshow=0;
int ir_count = 0;
int prev_ir = 0;
int f = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR, INPUT);
  tim = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!f && !digitalRead(IR)){
    ir_count = ir_count + 1;
    f = 1;
  }
  if(f && digitalRead(IR)){
    f = 0;
  }
  
  if(ir_count >= count*blade){
    rpm = ((ir_count*60000) / (blade*(millis()-tim)));
    //Serial.println(rpm);
    ir_count=0;
    tim = millis();
  }
  if((millis()-timshow)>500){
    Serial.println(rpm);
    timshow = millis();
  }
  /*if(ir_count > ircheck*count*blade){
    ir_count = ir_count - iroffset * count * blade;
    prev_ir = prev_ir - iroffset * count * blade;
  }*/
  if((millis()-tim) > 1000){
    rpm = 0;
    ir_count = 0;
  }
  //Serial.println(rpm);
  //Serial.println(ir_count);
  //Serial.println(prev_ir);
  //Serial.println(millis()-tim);
  //Serial.println("asdf");
  delay(delaystab);
}
