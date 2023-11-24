#include <pitches.h>
#include <CuteBuzzerSounds.h>
#include <Sounds.h>
#include <Servo.h>

#define SERVO1_PIN 9
#define SERVO2_PIN 7
#define BUZZER_PIN 4
#define TOUCH_SENSOR_PIN 2
#define MIC_PIN A0 // Analog pin connected to the microphone output
#define VIBRATION_MOTOR_PIN 3
#define YELLOW_LED 10
#define GREEN_LED 11
#define BLUE_LED 12
#define RED_LED 13

const int numLEDs = 4;
int leds[numLEDs] = {10, 11, 12, 13}; // LED pins

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

int tempo = 140;

int threshold = 800;
int volume;

Servo servo1; // Create a servo1 object
Servo servo2; // Create a servo2 object

bool startupSoundPlayed = false; // Flag to track if the startup sound has been played

void setup() {
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(180); // servo 1 DOWN
  servo2.write(30); // servo 2 DOWN
  delay(2000);
  Serial.begin(9600); // Initialize serial communication
  cute.init(BUZZER_PIN); // Set the buzzer pin as output
  pinMode(TOUCH_SENSOR_PIN, INPUT); // Set the touch sensor pin as input
  pinMode(MIC_PIN, INPUT); // Set the microphone pin as input
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT); // Set the vibration motor pin as output
  
  for (int i = 0; i < numLEDs; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Play startup sound melody only when the board is powered up
  if (!startupSoundPlayed) {
    playStartupSound(); // Play startup sound melody only when the board is powered up
    startupSoundPlayed = true; // Update the flag indicating that startup sound has been played
  }
  delay(1500);
}

void loop() {
  if (!Serial) {
    playDisconnectionSound(); // Call the disconnection sound function
    Serial.println("Device disconnected. Please check the connection."); // Print a disconnection message
    delay(500); // Add a small delay to prevent rapid triggering
    return; // Exit the loop or perform other disconnection-related actions
  }
  
  checkSerialCommand(); // Check for serial commands to trigger emotions
  // Check the status of the touch sensor
  if (digitalRead(TOUCH_SENSOR_PIN) == HIGH) {
    happy(); // Call the happy function if the touch sensor is activated (touched)
    delay(1000); // Add a delay to prevent rapid triggering
  }
  // Check for microphone input
  volume = analogRead(A0); // Reads the value from Analog pin A0
  
  if (volume > threshold) { // Adjust this threshold based on your microphone's sensitivity
    activateVibrationMotor(); // Activate vibration motor if a knock or loud sound is detected
    delay(800); // Add a delay to prevent rapid triggering
  }
}

void checkSerialCommand() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read serial input
    command.toLowerCase(); // Convert command to lowercase for case-insensitive comparison

    if (command.equals("dance")) {
      dance(); // Trigger happy function
    } else if (command.equals("happy")) {
      happy(); // Trigger dance function
    } else if (command.equals("angry")) {
      angry(); // Trigger angry function
    } else if (command.equals("sad")) {
      sad(); // Trigger sad function
    } else if (command.equals("surprise")) {
      playSurpriseSound(); // Trigger surprise function
    } else if (command.equals("confused")) {
      playConfusedSound(); // Trigger confused function
    } else if (command.equals("wrong")) {
      playWrongSound(); // Trigger wrong function
    }
  }
}

void activateVibrationMotor() {
  digitalWrite(VIBRATION_MOTOR_PIN, HIGH); // Activate the vibration motor
  delay(700); // Vibration motor duration (adjust as needed)
  digitalWrite(VIBRATION_MOTOR_PIN, LOW); // Turn off the vibration motor
}

void playStartupSound() {
  int duration = 1000;
  digitalWrite(GREEN_LED, HIGH);
  cute.play(S_CONNECTION);
  delay(duration);
  digitalWrite(GREEN_LED, LOW);
  delay(duration);
}

void playDisconnectionSound() {
  cute.play(S_DISCONNECTION);
  delay(200);
}

void playSurpriseSound() {
  cute.play(S_SURPRISE);
  delay(200);
}

void playConfusedSound() {
  cute.play(S_CONFUSED);
  delay(200);
}

void playWrongSound() {
  int duration = 500;
  for (int i = 0; i < 1; i++) {
    digitalWrite(RED_LED, HIGH);
    cute.play(S_OHOOH);
    delay(duration / 1);
    digitalWrite(RED_LED, LOW);
    delay(duration / 1);
  }
}

void dance() {
  shuffleLEDs(); // Shuffle LED sequence

  int melody[] = {
    // Your melody notes here...
    // For example:
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8,
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8,
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8,
  
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8,
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8,
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
 
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8,
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  };

  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int LEDIndex = 0;
  int wholenote = (60000 * 4) / 120;
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    unsigned long currentMillis = millis();
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
 
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);

    // Servo 1 movements
    if (currentMillis % 1000 >= 0 && currentMillis % 1000 <= 499) {
      servo1.write(20);
    } else if (currentMillis % 1000 >= 500 && currentMillis % 1000 <= 999) {
      servo1.write(180);
    }

    // Servo 2 movements
    if (currentMillis % 1000 >= 0 && currentMillis % 1000 <= 499) {
      servo2.write(180);
    } else if (currentMillis % 1000 >= 500 && currentMillis % 1000 <= 999) {
      servo2.write(30);
    }

    // Blink LEDs
    if (currentMillis % 100 >= 0 && currentMillis % 100 <= 49) {
      digitalWrite(leds[LEDIndex], HIGH);
    } else if (currentMillis % 100 >= 50 && currentMillis % 100 <= 99) {
      digitalWrite(leds[LEDIndex], LOW);
      LEDIndex = (LEDIndex + 1) % numLEDs;
    }
    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // Switch off all LEDs
    for (int i = 0; i < numLEDs; i++) {
      digitalWrite(leds[i], LOW);
    }
 
    // stop the waveform generation before the next note.
    noTone(BUZZER_PIN);

    servo1.write(180); // servo 1 DOWN
    servo2.write(30); // servo 2 DOWN
  }
}

void shuffleLEDs() {
  for (int i = numLEDs - 1; i > 0; i--) {
    int randomIndex = random(i + 1);
    if (randomIndex != i) {
      // swap leds
      int temp = leds[i];
      leds[i] = leds[randomIndex];
      leds[randomIndex] = temp;
    }
  }
}

void happy() {
  unsigned long servoStartTime = millis();
  unsigned long ledInterval = 100;
  unsigned long servoDuration = 1800;

  cute.play(S_HAPPY);
  delay(200);

  while (millis() - servoStartTime < servoDuration) {
    unsigned long currentMillis = millis();

    if (currentMillis - servoStartTime < 1800) {
      servo1.write(20);
      servo2.write(180);
    } else if (currentMillis - servoStartTime >= 1800 && currentMillis - servoStartTime < 2800) {
      servo1.write(180);
      servo2.write(30);
    }

    if ((currentMillis - servoStartTime) % (2 * ledInterval) < ledInterval) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
    }
  }

  servo1.write(180);
  servo2.write(30);

  // Turn off the LEDs
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  delay(1500);
}

void angry() {
  // Define initial and final positions
  int initialPos = 20; // Initial position for angry emotion
  int finalPos = 40; // Final position for angry emotion
  int returnPos = 180; // Return position

  // // Define an aggressive sound melody and its duration
  // int angryMelody[] = {400, 500, 600, 700};
  // int angryNoteDurations[] = {100, 100, 100, 100};

  // // Play aggressive sound
  // int size = sizeof(angryMelody) / sizeof(int);
  // for (int i = 0; i < size; i++) {
  //   tone(BUZZER_PIN, angryMelody[i], angryNoteDurations[i]);
  //   delay(angryNoteDurations[i]); // Add a small delay between notes
  //   noTone(BUZZER_PIN);
  // }

  cute.play(S_CONFUSED);
  delay(200);

  // Move back and forth between 20 and 40 degrees for 3 seconds
  unsigned long startTime = millis();
  unsigned long duration = 1000; // 1 second

  while (millis() - startTime < duration) {
    // Move from 20 to 40 degrees
    for (int angle = initialPos; angle <= finalPos; angle++) {
      servo1.write(angle); // Move the servo to the current angle
      //servo2.write(angle);
      delay(15); // Adjust delay for smoother movement
    }

    // Move from 40 to 20 degrees (backwards)
    for (int angle = finalPos; angle >= initialPos; angle--) {
      servo1.write(angle); // Move the servo to the current angle
      //servo2.write(angle);
      delay(15); // Adjust delay for smoother movement
    }
  }

  // Return to 180 degrees after completing the back-and-forth motion
  servo1.write(returnPos); // Move the servo to the return position (180 degrees)
  //servo2.write(returnPos);
  delay(1100); // Delay for stability or other operations
}

void sad() {
  cute.play(S_SAD);
  delay(200);

  // Define movements for the sad emotion
  int sadPosServo1 = 180; // Position for sad emotion
  int sadPosServo2 = 30; // down
  servo1.write(sadPosServo1); // Move the servo to the sad position
  servo2.write(sadPosServo2);
  delay(1100); // Delay for stability or other operations
}
