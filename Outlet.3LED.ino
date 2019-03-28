// Arduino Base Project.  Split-wire Outlet with 3 LED display
  // Global Variables:
  // Pin Control
    int redLED = 2;
    int yellowLED = 3;
    int greenLED =  4;
    int powerSwitch = 13;

//Functions
//lightOn() -  Turns light on.  Resistor Minimums(Red 68, Yellow 150, Green 150)(common High = 150) Round up = 330 or (Orange/Orange/Brown)
//lightOff() -
//switchOn() - 
//switchOff() - 
void setup() {
  // Enable serial "console" messages
  Serial.begin(9600);
  // put your setup code here, to run once:
  // Our pins are all "OUTPUT" since they are supplying voltage, not reading information.
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(powerSwitch, OUTPUT);
}
void loop() {

  // put your main code here, to run repeatedly:
  //Testing the switch and redLED light, with a basic 5 second delay.
lightOn(redLED);
switchOn(powerSwitch);
delay(5000);
switchOff(powerSwitch);
lightOff(redLED);

// Test the other lights
lightOn(yellowLED);
lightOn(greenLED);
delay(5000);
lightOff(yellowLED);
lightOff(greenLED);

}
void lightOn(int LED) {
  // Send console message
  Serial.print("LED on:");
  Serial.println(LED);
  digitalWrite(LED, HIGH);
}
void lightOff(int LED)
{
  // Send console message
  Serial.print("LED on:");
  Serial.println(LED);
  digitalWrite(LED, LOW);
}
  void switchOn(int pin) {
  // Send console message
  Serial.print("switch on:");
  Serial.println(pin);
  digitalWrite(pin, HIGH);
}
void switchOff(int pin)
{
  // Send console message
  Serial.print("switch on:");
  Serial.println(pin);
  digitalWrite(pin, LOW);
}
  
