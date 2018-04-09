#define LIGHT_PIN 2

String start_phase = "night"; // night/morning/evening
String phase       = "";

long start_phase_duration = 780L; // on the first cycle start phase will be reduced for the given ammount of seconds

long basic_night_duration_s   = 28800L; // seconds 22 - 6 = 8
long basic_morning_duration_s = 28800L; // seconds 6 - 14 = 8
long basic_day_duration_s     = 21600L; // seconds 14 - 20 = 6
long basic_evening_duration_s = 7200L;  // seconds 20 - 22 = 2

int night_duration_correction_s   = 0; // seconds
int morning_duration_correction_s = 0; // seconds
int day_duration_correction_s     = 0; // seconds
int evening_duration_correction_s = 0; // seconds

long night_duration_s   = basic_night_duration_s   - night_duration_correction_s;
long morning_duration_s = basic_morning_duration_s - morning_duration_correction_s;
long day_duration_s     = basic_day_duration_s     - day_duration_correction_s;
long evening_duration_s = basic_evening_duration_s - evening_duration_correction_s;

long morning = 0L;
long day     = 0L;
long evening = 0L;
long night   = 0L;

bool phase_changed = true;

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);

  phase = start_phase;

  night   = night_duration_s;
  morning = morning_duration_s;
  day     = day_duration_s;
  evening = evening_duration_s;

  if (start_phase == "morning") {
    morning = start_phase_duration;
  } else if (start_phase == "day") {
    day = start_phase_duration;
  } else if (start_phase == "evening") {
    evening = start_phase_duration;
  } else {
    night = start_phase_duration;
  }
}

void loop() {
  Serial.print(phase);
  Serial.print(" seconds left: ");

  if (phase == "morning") {
    if (morning > 0) {
      Serial.println(morning);
      morning -= 2;
    } else {
      phase_changed = true;
    }
  } else if (phase == "day") {
    if (day > 0) {
      Serial.println(day);
      day -= 2;
    } else {
      phase_changed = true;
    }
  } else if (phase == "evening") {
    if (evening > 0) {
      Serial.println(evening);
      evening -= 2;
    } else {
      phase_changed = true;
    }
  } else { // night
    if (night > 0) {
      Serial.println(night);
      night -= 2;
    } else {
      phase_changed = true;
    }
  }

  if (phase_changed) {
    Serial.print("PHASE_CHANGED ");
    Serial.print("from ");
    Serial.print(phase);
    Serial.print(" to ");
    phase_changed = false;
    if (phase == "morning") {
      digitalWrite(LIGHT_PIN, LOW);
      phase = "day";
      day   = day_duration_s;
    } else if (phase == "day") {
      digitalWrite(LIGHT_PIN, HIGH);
      phase   = "evening";
      evening = evening_duration_s;
    } else if (phase == "evening") {
      digitalWrite(LIGHT_PIN, LOW);
      phase = "night";
      night = night_duration_s;
    } else {
      digitalWrite(LIGHT_PIN, HIGH);
      phase = "morning";
      morning = morning_duration_s;
    }
    Serial.println(phase);
  }
  Serial.println("--------------------");
  delay(2000);
}
