#define trigPin D5
#define echoPin D6
#define multi (10)

float empty=6200;
float duration;
float distance;
float percent;

void setup() {
  Serial.begin(9600);
  //Set pins
 // pinMode(D0, INPUT);
  //for Sonar
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

float get_data_from_sonar(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration* multi;
  percent=((empty-distance)*100)/empty;
  if(percent<0){
    percent=100;
  }
  //Prints the distance on the Serial Monitor
   Serial.print("Distance from Sonar: ");
   Serial.println(distance);
   Serial.print("% Water Full: ");
   Serial.println(percent);
   delay(500);
}

void loop(){
  get_data_from_sonar();
}
