
// Rain Touch — Super Mario 

const int rainPin   = 7;
const int ledPin    = 11;
const int buzzerPin = 6;

#define E5  659
#define C5  523
#define G5  784
#define G4  392

//                E    E    -    E    -    C    E    -    G5   --   G4
int melody[]    = {E5,  E5,  0,   E5,  0,   C5,  E5,  0,   G5,  0,   G4};
int durations[] = {120, 120, 120, 120, 120, 120, 120, 120, 250, 250, 250};
int totalNotes  = 11;

int currentNote = 0;
bool melodyDone = false;
unsigned long lastNoteTime = 0;

int brightness = 0;
bool isOn = false;
unsigned long wetStart = 0;
unsigned long dryStart = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rainPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  analogWrite(ledPin, 0);
  noTone(buzzerPin);
  Serial.println("=== Mario Ready ===");
}

void loop() {
  bool wet = (digitalRead(rainPin) == LOW);
  unsigned long now = millis();

  if (wet) {
    dryStart = now;
    if (!isOn && (now - wetStart >= 300)) {
      isOn = true;
      currentNote = 0;
      melodyDone = false;
       lastNoteTime = now;
      Serial.println("ON");
    }
  } else {
    wetStart = now;
    if (isOn && (now - dryStart >= 300)) {
      isOn = false;
      currentNote = 0;
      melodyDone = false;
      noTone(buzzerPin);
      Serial.println("OFF");
    }
  }

  // LED 渐亮
  if (isOn && brightness < 200) {
    brightness += 3;
    analogWrite(ledPin, brightness);
  }

  // LED 渐灭
  if (!isOn && brightness > 0) {
    brightness -= 3;
    analogWrite(ledPin, brightness);
  }

  // 旋律
  if (isOn && !melodyDone) {
    if (now - lastNoteTime >= durations[currentNote]) {
      lastNoteTime = now;
      if (melody[currentNote] == 0) {
        noTone(buzzerPin);
      } else {
        tone(buzzerPin, melody[currentNote], durations[currentNote] * 0.6);
      }
      currentNote++;
      if (currentNote >= totalNotes) {
        melodyDone = true;
      }
    }
  }

  delay(200);
}
