#define pin A0
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;
float esp_voltage=0;
int valu=0;
void isr() //interrupt service routine
{
rev++;
}

void setup()
{
attachInterrupt(digitalPinToInterrupt(2),isr,RISING);  //attaching the interrupt
pinMode(pin,INPUT);
Serial.begin(9600);
}

void loop()
{
delay(1000);
detachInterrupt(digitalPinToInterrupt(2));           //detaches the interrupt
time=millis()-oldtime;        //finds the time 
rpm=(rev/time)*60000/5;         //calculates rpm
oldtime=millis();             //saves the current time
rev=0;
esp_voltage = ((float)analogRead(pin))/1023*5/4.44*5;
if(esp_voltage > 5) esp_voltage = 5.0;
//esp_voltage = map((float)analogRead(pin), 0, 1023, 0, 5);
valu = analogRead(pin);
Serial.print("RPM: ");
Serial.print(rpm);
Serial.print("\tESP Voltage: ");
Serial.println(valu);

attachInterrupt(digitalPinToInterrupt(2),isr,RISING);

}
