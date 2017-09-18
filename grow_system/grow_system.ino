#define LIGHT_PIN 2

 String start_phase = "day"; // night or day
 String phase = "";

long start_phase_duration =   1200L; // on the first cycle start phase will be reduced for the given ammount of seconds

long basic_night_duration_s = 36000L; // 0 to 36000 s
long basic_day_duration_s   = 50400L; // 0 to 50400 s

int night_duration_correction_s = 75; //75 s
int day_duration_correction_s   = 53; //53 s

long night_duration_s = basic_night_duration_s - night_duration_correction_s;
long day_duration_s   = basic_day_duration_s   - day_duration_correction_s;

long day   = 0L;
long night = 0L;

bool phase_changed = true;

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);

  phase = start_phase;

  if (start_phase == "day") {
    day = start_phase_duration;
  } else {
    night = start_phase_duration;
  }
}

void loop() {
  Serial.print(phase);
  Serial.print(" seconds left: ");

  if (phase == "day") {
    if (day > 0) {
      Serial.println(day);
      day -= 2;
    } else {
      night = night_duration_s;
      day = day_duration_s;
      phase = "night";
      phase_changed = true;
    }
  } else { // night
    if (night > 0) {
      Serial.println(night);
      night -= 2;
    } else {
      night = night_duration_s;
      day = day_duration_s;
      phase = "day";
      phase_changed = true;
    }
  }

  if (phase_changed) {
    Serial.println("PHASE_CHANGED");
    phase_changed = false;
    if (phase == "day") {
      digitalWrite(LIGHT_PIN, LOW);
    } else {
      digitalWrite(LIGHT_PIN, HIGH);
    }
  }
  Serial.println("--------------------");
    // tick
  delay(2000);
}
