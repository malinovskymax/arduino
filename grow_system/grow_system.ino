#define LIGHT_PIN 2

long millisecond_since_light_off = 36000000L; // 0 to 36000000
long millisecond_since_light_on  = 0L; // 0 to 50400000

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // tick
  delay(1);

  // timer switcher
  if(millisecond_since_light_on == 50400000 && millisecond_since_light_off == 36000000) {
    millisecond_since_light_on = 0;
    millisecond_since_light_off = 0;
  }

  // light switcher
  if(millisecond_since_light_off == 0) { // it's night, turn light off
    digitalWrite(LIGHT_PIN, HIGH);
  } else {                               // it's day, turn light on
    digitalWrite(LIGHT_PIN, LOW);
  }  

  // ticker
  if(millisecond_since_light_on == 50400000) { // it's night, turn light off and count night time
    ++millisecond_since_light_off;
    //Serial.println("tick");
  } else {                                     // it's day, turn light on and count day time
    ++millisecond_since_light_on;
  }
}
