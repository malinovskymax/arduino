// TODO: implement synchronization with external clock
// (preferable by bluetooth)

#define LIGHT_PIN 2

// Settings
String mode               = "winter"; // Possible modes are: [winter, summer]
String current_time       = "22:16:00";

String morning_start_time = "06:00:00";
String day_start_time     = "14:00:00";
String evening_start_time = "20:00:00";
String night_start_time   = "22:00:00";
// Settings END

long current_time_s = 0L;

long day_end_time_s = 86400L;

String phase = "";

long start_phase_duration = 0L;

long morning = 0L;
long day = 0L;
long evening = 0L;
long night = 0L;

long morning_start_time_s = 0L;
long day_start_time_s = 0L;
long evening_start_time_s = 0L;
long night_start_time_s = 0L;

bool phase_changed = false;

long timeToSeconds(String time) {
  long seconds = time.substring(0, 2).toInt() * 3600;
  seconds     += time.substring(3, 5).toInt() * 60;
  seconds     += time.substring(6, 8).toInt();
  return seconds;
}

void setupCurrentTime() {
  current_time_s = timeToSeconds(current_time);
}

void calculatePhases() {
  morning_start_time_s = timeToSeconds(morning_start_time);
  day_start_time_s = timeToSeconds(day_start_time);
  evening_start_time_s = timeToSeconds(evening_start_time);
  night_start_time_s = timeToSeconds(night_start_time);

  morning = night_start_time_s - morning_start_time_s;
  day = evening_start_time_s - day_start_time_s;
  evening = night_start_time_s - evening_start_time_s;
  night  = day_end_time_s - night_start_time_s + morning_start_time_s;

  if (current_time_s > morning_start_time_s && current_time_s < day_start_time_s) {
    phase = "morning";
    morning = day_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, LOW);
  } else if (current_time_s > day_start_time_s && current_time_s < evening_start_time_s) {
    phase = "day";
    day = evening_start_time_s - current_time_s;
    if (mode == "winter") {
      digitalWrite(LIGHT_PIN, LOW);
    } else if (mode == "summer") {
      digitalWrite(LIGHT_PIN, HIGH);
    }
  } else if (current_time_s > evening_start_time_s && current_time_s < night_start_time_s) {
    phase = "evening";
    evening = night_start_time_s - current_time_s;
    digitalWrite(LIGHT_PIN, LOW);
  } else {
    phase = "night";
    night = day_end_time_s - current_time_s + morning_start_time_s;
    digitalWrite(LIGHT_PIN, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);

  setupCurrentTime();
  calculatePhases();
}

void loop() {
  Serial.print("current_time_s ");
  Serial.println(current_time_s);

  Serial.print(phase);
  Serial.print(" seconds left: ");

  if (current_time_s >= 86400) {
    current_time_s = 0L;
  }

  if (phase == "morning") {
    if (morning > 0) {
      Serial.println(morning);
      morning -= 1;
    } else {
      phase_changed = true;
    }
  } else if (phase == "day") {
    if (day > 0) {
      Serial.println(day);
      day -= 1;
    } else {
      phase_changed = true;
    }
  } else if (phase == "evening") {
    if (evening > 0) {
      Serial.println(evening);
      evening -= 1;
    } else {
      phase_changed = true;
    }
  } else { // night
    if (night > 0) {
      Serial.println(night);
      night -= 1;
    } else {
      phase_changed = true;
    }
  }

  current_time_s += 1;

  if (phase_changed) {
    phase_changed = false;
    Serial.print("PHASE_CHANGED ");
    Serial.print("from ");
    Serial.print(phase);
    Serial.print(" to ");
    calculatePhases();
    Serial.println(phase);
  }
  Serial.println("--------------------");
  delay(1000);
}
