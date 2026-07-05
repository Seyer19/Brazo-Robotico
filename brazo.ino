/* Brazo robótico con 3 servos + 1 motor paso a paso
Controlado por 3 joysticks
Joystick 1 -> Servo base (A0)
Joystick 2 -> Servo brazo (A1) + Servo pinza (A2)
Joystick 3 -> Motor paso a paso (A3)
*/
#include <Servo.h>
#include <Stepper.h>
#define STEPS 32 // pasos internos del 28BYJ-48
#define DEADZONE 40
#define DELAY_MS 15
// === Asignación de pines ===
const int JOY_BASE = A0;
const int JOY_BRAZO = A1;
const int JOY_PINZA = A2;
const int JOY_MOTOR = A3;
// Servos
const int SERVO_BASE = 11;
const int SERVO_BRAZO = 12;
const int SERVO_PINZA = 13;

10

// Stepper (28BYJ-48)
const int IN1 = 4;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 7;
Stepper stepperMotor(STEPS, IN1, IN2, IN3, IN4);
// Variables de control
Servo servoBase, servoBrazo, servoPinza;
int angBase = 90, angBrazo = 90, angPinza = 90;
void setup() {
Serial.begin(9600);
// Servos
servoBase.attach(SERVO_BASE);
servoBrazo.attach(SERVO_BRAZO);
servoPinza.attach(SERVO_PINZA);
servoBase.write(angBase);
servoBrazo.write(angBrazo);
servoPinza.write(angPinza);
// Stepper
stepperMotor.setSpeed(1000);
// Pines joystick
pinMode(JOY_BASE, INPUT);
pinMode(JOY_BRAZO, INPUT);
pinMode(JOY_PINZA, INPUT);
pinMode(JOY_MOTOR, INPUT);
Serial.println(F("Brazo robótico listo con 3 joysticks y motor paso a
paso."));
}
void loop() {
// === Joysticks servos ===
int valBase = analogRead(JOY_BASE);
int valBrazo = analogRead(JOY_BRAZO);
int valPinza = analogRead(JOY_PINZA);
int valMotor = analogRead(JOY_MOTOR);

11

// --- Servo Base ---
if (valBase > (512 + DEADZONE)) angBase++;
else if (valBase < (512 - DEADZONE)) angBase--;
angBase = constrain(angBase, 0, 180);
servoBase.write(angBase);
// --- Servo Brazo ---
if (valBrazo > (512 + DEADZONE)) angBrazo++;
else if (valBrazo < (512 - DEADZONE)) angBrazo--;
angBrazo = constrain(angBrazo, 0, 180);
servoBrazo.write(angBrazo);
// --- Servo Pinza ---
if (valPinza > (512 + DEADZONE)) angPinza++;
else if (valPinza < (512 - DEADZONE)) angPinza--;
angPinza = constrain(angPinza, 0, 180);
servoPinza.write(angPinza);
// === Motor paso a paso controlado por Joystick 3 ===
if (valMotor > (512 + DEADZONE)) {
stepperMotor.step(5); // derecha
} else if (valMotor < (512 - DEADZONE)) {
stepperMotor.step(-5); // izquierda
} else {
// detener motor (apagar bobinas)
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
// Pequeño delay para suavizar movimiento
delay(DELAY_MS);
// Debug opcional
static unsigned long t0 = 0;
if (millis() - t0 > 750) {
t0 = millis();
Serial.print("Base: "); Serial.print(angBase);
Serial.print(" Brazo: "); Serial.print(angBrazo);
Serial.print(" Pinza: "); Serial.print(angPinza);
Serial.print(" Motor: "); Serial.println(valMotor);
}

}
