#define LIGHT_PIN 2

// Settings
String current_time       = "22:30:00";

String morning_start_time = "06:00:00";
String day_start_time     = "14:00:00";
String evening_start_time = "20:00:00";
String night_start_time   = "22:00:00";
// Settings END

long current_time_s = 0L;

String phase = "";

long start_phase_duration = 0L;

long morning = 0L;
long day = 0L;
long evening = 0L;
long night = 0L;

bool phase_changed = false;

long timeToSeconds(String time) {
  long seconds = time.substring(0, 2).toInt() * 3600;
  seconds     += time.substring(3, 5).toInt() * 60;
  seconds     += time.substring(6, 8).toInt();
  return seconds;
}

void calculatePhases() {
  if (current_time_s == 0 && !phase_changed) {
    current_time_s = timeToSeconds(current_time);
  }
  long morning_start_time_s = timeToSeconds(morning_start_time);
  long day_start_time_s = timeToSeconds(day_start_time);
  long evening_start_time_s = timeToSeconds(evening_start_time);
  long night_start_time_s = timeToSeconds(night_start_time);

  morning = night_start_time_s - morning_start_time_s;
  day = evening_start_time_s - day_start_time_s;
  evening = night_start_time_s - evening_start_time_s;
  night = morning_start_time_s - night_start_time_s;

  if (current_time_s > morning_start_time_s && current_time_s < day_start_time_s) {
    phase = "morning";
    morning = day_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, LOW);
  } else if (current_time_s > day_start_time_s && current_time_s < evening_start_time_s) {
    phase = "day";
    day = evening_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, HIGH);
  } else if (current_time_s > evening_start_time_s && current_time_s < night_start_time_s) {
    phase = "evening";
    evening = night_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, LOW);
  } else {
    phase = "night";
    night = morning_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, HIGH);
  }
}

void swithPhase() {
  Serial.print("PHASE_CHANGED ");
  Serial.print("from ");
  Serial.print(phase);
  Serial.print(" to ");
  calculatePhases();
  phase_changed = false;
  Serial.println(phase);
}

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);

  calculatePhases();
}

void loop() {
  Serial.print(phase);
  Serial.print(" seconds left: ");

  if (current_time_s >= 86400) {
    current_time_s = 0L;
  }

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

  current_time_s += 2;

  if (phase_changed) {

  }
  Serial.println("--------------------");
  delay(2000);
}
