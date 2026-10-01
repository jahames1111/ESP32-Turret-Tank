const int ENA = 6;
const int IN1 = 30;
const int IN2 = 31;
const int ENB = 9;
const int IN3 = 33;
const int IN4 = 32;

const int LAUNCH_IN1 = 34;
const int LAUNCH_IN2 = 35;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(LAUNCH_IN1, OUTPUT);
  pinMode(LAUNCH_IN2, OUTPUT);
  
  stopMotors();
  stopLauncher();
  
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  if (Serial2.available() > 0) {
    String packet = Serial2.readStringUntil('\n');
    packet.trim();

    if (packet.startsWith("touno:")) {
      String data = packet.substring(6);
      
      if (data == "FIRE_ON") {
        digitalWrite(LAUNCH_IN1, HIGH);
        digitalWrite(LAUNCH_IN2, LOW);
        Serial.println("MEGA_LOG: Launcher ENGAGED");
      } 
      else if (data == "FIRE_OFF") {
        stopLauncher();
        Serial.println("MEGA_LOG: Launcher STOPPED");
      } 
      else {
        int commaIndex = data.indexOf(',');
        if (commaIndex != -1) {
          int leftVal = data.substring(0, commaIndex).toInt();
          int rightVal = data.substring(commaIndex + 1).toInt();

          driveLeftTrack(leftVal);
          driveRightTrack(rightVal);
        }
      }
    }
  }
}

void driveRightTrack(int speed) {
  if (speed == 0) {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
  } else if (speed > 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, speed);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, abs(speed));
  }
}

void driveLeftTrack(int speed) {
  if (speed == 0) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
  } else if (speed > 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, speed);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, abs(speed));
  }
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void stopLauncher() {
  digitalWrite(LAUNCH_IN1, LOW);
  digitalWrite(LAUNCH_IN2, LOW);
}