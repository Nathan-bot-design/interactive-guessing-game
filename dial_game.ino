#define POT1 A1
#define POT2 A2
#define BUTTON1 A4
#define BUTTON2 A5

#define RED_LED 13
#define YELLOW_LED 12
#define GREEN_LED 11
#define BUZZER 9

int secretCode1, secretCode2;
int attempts = 5;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  randomSeed(analogRead(0));
  secretCode1 = random(0, 100);
  secretCode2 = random(0, 100);

  Serial.println("🔐 Welcome to CODE LOCK BREAKER 🔐");
  Serial.println("Match the 2 hidden codes (0–99) using the potentiometers!");
  Serial.println("➡ Press Button 1 to Submit");
  Serial.println("➡ Press Button 2 for Hints");
  Serial.println("=====================================");
}

void loop() {
  int pot1Value = map(analogRead(POT1), 0, 1023, 0, 99);
  int pot2Value = map(analogRead(POT2), 0, 1023, 0, 99);

  // Inverted logic (HIGH means pressed)
  bool submitPressed = digitalRead(BUTTON1) == HIGH;
  bool hintPressed = digitalRead(BUTTON2) == HIGH;

  Serial.print("\n🔢 Code Guess: ");
  Serial.print(pot1Value);
  Serial.print(" : ");
  Serial.print(pot2Value);
  Serial.println();

  printSlider("Dial 1", pot1Value);
  printSlider("Dial 2", pot2Value);

  if (submitPressed) {
    Serial.println("\n🔘 SUBMITTING...");
    attempts--;

    if (pot1Value == secretCode1 && pot2Value == secretCode2) {
      Serial.println("\n✅ CODE UNLOCKED!");
      digitalWrite(GREEN_LED, HIGH);
      tone(BUZZER, 1000, 1000);
      delay(2000);
      resetGame();
    } else {
      Serial.println("❌ Incorrect Code!");
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER, 200, 500);
      delay(500);
      digitalWrite(RED_LED, LOW);
      if (attempts == 0) {
        Serial.println("💥 GAME OVER! Out of attempts.");
        delay(2000);
        resetGame();
      }
    }
    delay(1000); // debounce
  }

  if (hintPressed) {
    Serial.println("\n💡 HINT:");
    if (pot1Value < secretCode1) Serial.println("➡ Dial 1 is too LOW");
    else if (pot1Value > secretCode1) Serial.println("⬅ Dial 1 is too HIGH");
    else Serial.println("✔ Dial 1 is CORRECT!");

    if (pot2Value < secretCode2) Serial.println("➡ Dial 2 is too LOW");
    else if (pot2Value > secretCode2) Serial.println("⬅ Dial 2 is too HIGH");
    else Serial.println("✔ Dial 2 is CORRECT!");
    digitalWrite(YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
    delay(500);
  }

  delay(300); // small delay for responsiveness
}

void printSlider(String label, int value) {
  Serial.print(label);
  Serial.print(" [");
  int blocks = map(value, 0, 99, 0, 20);
  for (int i = 0; i < 20; i++) {
    if (i < blocks) Serial.print("█");
    else Serial.print(" ");
  }
  Serial.print("] ");
  Serial.print(value);
  Serial.println();
}

void resetGame() {
  Serial.println("\n🔄 Restarting Game...\n");
  secretCode1 = random(0, 100);
  secretCode2 = random(0, 100);
  attempts = 5;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  noTone(BUZZER);
}
