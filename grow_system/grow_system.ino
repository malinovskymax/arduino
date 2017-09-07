#define LIGHT_PIN 2
#define START_PHASE "day"

long start_offset_s = 0L; // will be applied to start phase on first cycle

long basic_night_duration_s = 36000L; // 0 to 36000 s
long basic_day_duration_s   = 50400L; // 0 to 50400 s

int night_duration_correction_s = 0; // 343
int day_duration_correction_s   = 0; // 480

long night_duration_s = basic_night_duration_s - night_duration_correction_s;
long day_duration_s   = basic_day_duration_s   - day_duration_correction_s;

long day   = 0L;
long night = 0L;

void setup() {
  //Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);

  if (START_PHASE == "day") {
    day = day_duration_s - start_offset_s;
  } else {
    night = night_duration_s - start_offset_s;
  }
}

void loop() {
  // tick
  delay(1000);

  if(night < night_duration_s) {                 // it's day, turn light on
    digitalWrite(LIGHT_PIN, LOW);
    if(day == 0) {                 // it's the end of the twenty-four hours cycle
      day   = day_duration_s;
      night = night_duration_s;
    }
    --day;
  } else {                         // it's night, turn light off
    digitalWrite(LIGHT_PIN, HIGH);
    --night;
  }
}
