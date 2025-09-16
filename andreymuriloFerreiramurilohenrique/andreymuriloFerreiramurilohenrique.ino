#include <Servo.h>
#include <DHT.h>
#define PIN_DHT 3       
#define DHTTYPE DHT11  

const int PIN_SERVO  = 9;   
const int PIN_BUZZER = 8;    
const int PIN_BUTTON = 2;    

const float TEMP_MIN = 22.0;       
const float TEMP_MAX = 30.0;      
const float TEMP_CRIT_HIGH = 35.0; 
const float TEMP_CRIT_LOW  = 8.0;  


const int READ_SAMPLES = 6;
float readings[READ_SAMPLES];
int readIndex = 0;
float total = 0.0;
float averageTemp = 0.0;

Servo ventServo;
DHT dht(PIN_DHT, DHTTYPE);

bool buzzerAllowed = true;
unsigned long lastButtonMillis = 0;
const unsigned long DEBOUNCE_MS = 50;

void beepBuzzer(int times, int onMs, int offMs) {
  if (!buzzerAllowed) return; 
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(onMs);
    digitalWrite(PIN_BUZZER, LOW);
    delay(offMs);
  }
}

float readTempC() {
  float tempC = dht.readTemperature(); 
  if (isnan(tempC)) {
    Serial.println("Falha ao ler do DHT11!");
    return averageTemp; 
  }
  return tempC;
}

void setVentPercentage(float percent) {
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  int angle = map((int)percent, 0, 100, 0, 180);
  ventServo.write(angle);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  ventServo.attach(PIN_SERVO);

  dht.begin(); 

  for (int i = 0; i < READ_SAMPLES; i++) {
    readings[i] = readTempC();
    delay(1000); 
  }
  total = 0;
  for (int i = 0; i < READ_SAMPLES; i++) total += readings[i];
  averageTemp = total / READ_SAMPLES;

  setVentPercentage(0); 

  Serial.println("Sistema iniciado. Botao: toggle buzzer (INPUT_PULLUP, LOW quando pressionado).");
  Serial.print("Buzzer permitido? ");
  Serial.println(buzzerAllowed ? "SIM" : "NAO");
}

// ===== LOOP =====
void loop() {
  // --- leitura suave
  total -= readings[readIndex];
  float newTemp = readTempC();
  readings[readIndex] = newTemp;
  total += readings[readIndex];
  readIndex = (readIndex + 1) % READ_SAMPLES;
  averageTemp = total / READ_SAMPLES;

  int buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == LOW) {
    if (millis() - lastButtonMillis > DEBOUNCE_MS) {
      buzzerAllowed = !buzzerAllowed;
      lastButtonMillis = millis();
      Serial.print("BOTAO PRESSIONADO -> buzzerAllowed = ");
      Serial.println(buzzerAllowed ? "ON" : "OFF");

      if (buzzerAllowed) { 
        digitalWrite(PIN_BUZZER, HIGH);
        delay(120);
        digitalWrite(PIN_BUZZER, LOW);
      }
      delay(200);
    }
  }

  float pct;
  if (averageTemp <= TEMP_MIN) pct = 0.0;
  else if (averageTemp >= TEMP_MAX) pct = 100.0;
  else pct = (averageTemp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN) * 100.0;
  setVentPercentage(pct);

  if (averageTemp >= TEMP_CRIT_HIGH) {
    beepBuzzer(2, 200, 150);
  } else if (averageTemp <= TEMP_CRIT_LOW) {
    beepBuzzer(3, 120, 80);
  }

  Serial.print("Temp média: ");
  Serial.print(averageTemp, 2);
  Serial.print(" C | Vent %: ");
  Serial.print((int)pct);
  Serial.print("% | Buzzer permitido: ");
  Serial.println(buzzerAllowed ? "SIM" : "NAO");

  delay(1000);
}
