#define LIGHT_PIN 2

 String start_phase = "night"; // night or day
 String phase = "";

long start_offset_s =   0L; // on the first cycle start phase will be reduced for the given ammount of seconds

long basic_night_duration_s = 36000L; // 0 to 36000 s
long basic_day_duration_s   = 50400L; // 0 to 50400 s

int night_duration_correction_s = 1500;
int day_duration_correction_s   = 2100;

long night_duration_s = basic_night_duration_s - night_duration_correction_s;
long day_duration_s   = basic_day_duration_s   - day_duration_correction_s;

long day   = 0L;
long night = 0L;

bool phase_changed = true;

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);

  phase = start_phase;

  if (start_phase == "day") {
    day = day_duration_s - start_offset_s;
  } else {
    night = night_duration_s - start_offset_s;
  }
}

void loop() {
  // tick
  delay(1000);

  if (phase == "day") {
    if (day > 0) {
      --day;
    } else {
      night = night_duration_s;
      day = day_duration_s;
      phase = "night";
      phase_changed = true;
    }
  } else { // night
    if (night > 0) {
      --night;
    } else {
      night = night_duration_s;
      day = day_duration_s;
      phase = "day";
      phase_changed = true;
    }
  }

  if (phase_changed) {
    phase_changed = false;
    if (phase == "day") {
      digitalWrite(LIGHT_PIN, LOW);
    } else {
      digitalWrite(LIGHT_PIN, HIGH);
    }
  }
}
