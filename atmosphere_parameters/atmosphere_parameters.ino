#include <DHT.h>
#include <MQ135.h>

#define DHTPIN 5
#define DHTTYPE DHT11

#define GASPIN A0

#define MAX_NORMAL_CO2_PERCENT 0.1
#define CO2_WARNING_PIN 2

DHT dht(DHTPIN, DHTTYPE);

MQ135 mqSensor(GASPIN);

float ppmToPercent(float ppm) {
  return (ppm / 100000);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(CO2_WARNING_PIN, OUTPUT);
}

void loop() {
  //Serial.print(mqSensor.getRZeroCO(), 5);
  //Serial.println("|co");
  //Serial.print(mqSensor.getRZeroCO2(), 5);
  //Serial.println("|c02");
  //Serial.print(mqSensor.getRZeroEthanol(), 5);
  //Serial.println("|eth");
  //Serial.print(mqSensor.getRZeroNH4(), 5);
  //Serial.println("|nh4");
  //Serial.print(mqSensor.getRZeroToluene(), 5);
  //Serial.println("|tolu");
  //Serial.print(mqSensor.getRZeroAcetone(), 5);
  //Serial.println("|ace");

  
  float resistance = mqSensor.getResistance();

  float co = mqSensor.getCO(resistance);
  float co2 = mqSensor.getCO2(resistance);
  float ethanol = mqSensor.getEthanol(resistance);
  
  Serial.print("CO (%): ");
  Serial.println(ppmToPercent(mqSensor.getCO(resistance)), 5);
  
  Serial.print("CO2 (%): ");
  float co2_percent = ppmToPercent(mqSensor.getCO2(resistance));
  Serial.println(co2_percent, 5);
  
  Serial.print("Ethanol (%): ");
  Serial.println(ppmToPercent(mqSensor.getEthanol(resistance)), 5);

  // temperature and humidity start
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if(isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading from DHT");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %; ");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C;");
  }
  // temperature and humidity end

  if(co2_percent > MAX_NORMAL_CO2_PERCENT) {
    digitalWrite(CO2_WARNING_PIN, LOW);
  } else {
    digitalWrite(CO2_WARNING_PIN, HIGH);
  }

//  digitalWrite(LED_BUILTIN, LOW);
//  delay(1000);
//  digitalWrite(LED_BUILTIN, HIGH);
//  double gas_sensor_level = analogRead(GASPIN);
//  Serial.print(gas_percent, 5);
//  Serial.println("CO2 percent: ");

  delay(5000);
}
