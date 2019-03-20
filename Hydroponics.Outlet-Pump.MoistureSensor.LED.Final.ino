// Analog is a serial, one-way, reading of information. Doesn't require ground for comparison.
// Digital is a comparison of information, and how it relates to the digital ground pin. From the arduino's perspective try to keep them separate.

// A guide to automation, by OneBadPanda
//WARN Tell the user what you're doing, or atleast log it for diagnostics.
//WAIT for user interaction.
//DO something.
//CHECK check that what you did is done, and default any variables for the next loop just in case.

// ALWAYS ENCLOSE YOUR HIGH-AMPERAGE DC, AND AC POWER! Safety First! If you don't, please video tape it so you can be on the Darwin awards!
//Please visit www.youtube.com/OneBadPanda for other examples like this (If I pursue this).  I (want to) live stream my process for new projects and answer questions about this, or your own project!

//Declare variables.  This is how the arduino remembers things. They are outside of setup() so your other functions can use it. See: Scope
//I (try) to use a pascalCASE variant for program-specific information that needs attention for your project because the real programming standard for writing doesn't account for community projects intented for beginners to change.

//OUTLET / PUMP combo, 'cause I've electrocuted myself on 220V AC and don't want to die, again.  0.3/10, would not recommend.
  int outletPowerPin = 13;  // 5v digital out pin that controls the outlet power.
  int powerDuration = 0; // Just storage for calculation.  Probably doesn't have to be declared here... but it doesn't hurt.
  int fullCYCLE = 30; // Value: Time (in seconds) the pump must run to completely saturate the plant.
  int emptyTANK = 1440; // Value: Time (in minutes) the device should wait before trying again with an empty tank.
  
//Soil Moisture
  int soilVal = 0; // Just storage
  int soilReadPin = A0; // Pin that recieves the signal and records the result. 
  int soilPower = 7; // Pin that activates the power to send to the soil sensor.
  int soilHIGH = 1203; //Value: is read when you have a saturated plant.
  int soilMIN = 0; //Value: the completely dry value.

//LED
  int redLed = 2; // Pin that activates the LED.

//Time
  int Previous = 0; // Storage
  float onSCALE = 1.0/30.0; // 1 / Maximum run time. Coding Consideration: Air-filled tube between the pump and destination. Challenging: without interfering with the maximum.
  float offSCALE = 1.0/360.0; // 1 / Maximum wait time.
// Functions:
// I like functions to be at the very top.
// Alphabetical order is my preference but you may find examples of other coding styles.
// Can't Ctrl+F something spelled wrong down the road.
int calcDuration()
  {
  int value = 0;
  if (soilVal < soilHIGH) value = 1;
  int calcTIME = 0; //Just a storage spot for the calculated time to wait or turn on. SEE: Scope
  switch (value) {

  case 1:
// Return the Time to turn on the outlet if the value is under the soilHIGH but careful with flooding in your project.
// Calculate: PercentTimeScale * (Difference between soilHIGH-soilMIN) * soilVal) = calcTIME
    calcTIME = (onSCALE * (soilHIGH - soilVal)); // High * scale * the Value = Time to turn on

// This is my diagnostic code.  It is not required to run, but provided the variables to me so I could do the math. I'll leave it in as an example.
    Serial.println(onSCALE);
    Serial.println(soilHIGH);
    Serial.println(soilVal);
//  DIAGNOSTIC CODE

    Serial.print("On Duration = ");
    Serial.println(calcTIME);
    delay(calcTIME*1000);
    break;
  case 0:
// Set the time to wait until next check in minutes.
    calcTIME = ((soilVal - soilHIGH) * offSCALE);
    Serial.print("Wait Duration = ");
    Serial.println(calcTIME);
    break;
  }

    return calcTIME;
  }
  
int readSoil()
  {
// Power the sensor and grab a reading
    digitalWrite(soilPower, HIGH);// turn your sensor's power pin on.
    delay(10); //Give the sensor time to receive the power.  10 ms should be fine.
    soilVal = analogRead(soilReadPin); // Set the variable so the display can reference it.
// could add an IF statement to capture it immediately, could save power in the long run.
    digitalWrite(soilPower, LOW); // don't forget to turn your power off.  Water + Metal + Electricity = corrosion.
    return soilVal;

  }
  
unsigned long waitMinute(int x)
  {
// I want to wait in intervals of 60 seconds. so we take 1000 miliseconds and multiply it by 60.  Consider: Where should this variable be if the interval matched the interval of the pump "ON" time.
    unsigned long minute = 60000; // Arduino Unos int values only store values between -32768 to 32677  
    unsigned long minutes = minute*x; //
//WARN
    Serial.print("Minutes to wait: ");
    Serial.println(minutes);
    delay(minutes);
  }
void setup() 
  { 
// Setup is where you define pin interactions and single-run boot-up operations.
  Serial.begin(9600); // Show serial information, This can be used for diagnostics, but the same info could be put on a LCD screen for a user-interface!
  
  pinMode(outletPowerPin, OUTPUT);
  digitalWrite(outletPowerPin, LOW); // Let us not electrocute ourselves, OFF by default.
  
  pinMode(soilPower, OUTPUT);// Tell the pin to be output only, as this will power the sensor.
  digitalWrite(soilPower, LOW); //OFF by default

  pinMode(redLed, OUTPUT);
  digitalWrite(redLed, LOW); //OFF by default.

}
void loop() 
  { 
//SYSTEMS CHECK return variables to default in case of unexpected bugs, glitches, or restarts of the device.
//However I want you to come tell me if I'm wrong at youtube.com/OneBadPanda
//so this area of the script is empty for you!

//Loop is where you use pin interactions, with information, to control things. Once this goes,
//You don't want it to stop. (think about optimal control of power.)
//I want to turn off the light before I do a reading should the device reset or my code below has shame in it.

  Serial.print("Soil Moisture = ");
// Sense Soil Moisture
  soilVal = readSoil();
  Serial.println(readSoil());
// When do you want the pump to turn on? When moisture for the plant is low.
// How long do you want to turn the pump on? However long it needs.
// What happens if it doesn't work? 
//Plants can go weeks without water.  I would recommend making it as long as you feel comfortable: water + metal + electricity = corrosion

// the difference between soilMIN and soilHIGH.  I want to wait 1 hour for every %25 of the scale. ~100
// How often do you want it to read the sensor between waterings?

  
  if (soilVal < soilHIGH) // Engage Pump and predict the next reading
  {
//Soil Reading
    Serial.println("Soil reading is below expectation. Engaging Pump.");

//Red LED - I want it ON if outlets are active... FOR SAFETY! 
    digitalWrite(redLed, HIGH);
    Serial.println("Turning red light on.");
// Engage Pump
    digitalWrite(outletPowerPin, HIGH);
    delay(calcDuration()); // I want it to stay on until this time is over.
    
    digitalWrite(outletPowerPin, LOW); // Don't forget to turn it off!
    digitalWrite(redLed, LOW);

//Store previous variable and Test again to check that water reached the sensor.
    Previous = soilVal;
    Serial.print("Checking... soil moisture = ");
// Sense Soil Moisture
    soilVal = readSoil();
    Serial.println(soilVal);
    
// If the tank is low/empty but be careful of a reset short(if that's a thing?)/this isn't the first time running:
    if ((soilVal <= Previous) && (Previous != 0))
    {
//Warn that the resivor is empty, light an LED, and wait for 24 hours.
      digitalWrite(redLed, HIGH);
      Serial.println("Water Tank Empty!");
      Serial.println("Waiting 24H or until next reset.");  // Could add an external push-button.
//Wait 24 hours
      waitMinute(emptyTANK);
    }
  }
    else {
//Wait for a period of time that you think the next water check should be, currently: best effort, not highest yield.

//Warn / USER INTERFACE
    Serial.println("Soil reading is nominal.");
// Let wait cycle run.
    waitMinute(360);
    }
// If the sensor is above 600 but below 1023, then we want the wait time to be dependant on
// The difference between, or 423.  I want to wait 1 hour for every %25 of the scale. ~100
// How often do you want it to read the sensor between waterings?


}
//Recap: 
// I want the device to use a water pump based on soil moisture.  If the soil is adequate.  CHECK
// I want the device to wait an amount of time relative to the soil moisture before the next check.  CHECK
// If the tank is ever empty, I want to notify the user, and wait for interaction for 24 hours. CHECK

// Other uses: The code doesn't need to change should you put your device out-doors, but you will want to consider weather-proofing, protecting electronics and yourself in-case of electric shock. (No Oxfurd comma when using a common verb!), but I'm not an English major.
// Also, if your water tank is receiving (and hopefully cleaning) rain water, this device will work as is, but you can change the code above to account for those variables.

//Improvements:
//Coding challenge: Watering the plant based on previous results so you don't have to hard-code the different times in!
//Design: A better user interface that is more complex than a red light and a serial console.  Let's get FANCY!


// Reminder: This program is for beginners. The code is saturated in data and could be more efficient!  I didn't go to college to learn this.  I am human.  It's reasonable to think I did something wrong
// and you should not blindly trust my code or my build.  I'm catering to the new people, because I AM a novice.
// If you're not a novice and want to give feedback, or if you are and want to tell me how I could be more clear in my code for beginners, I look forward to meeting you at www.youtube.com/OneBadPanda
// Liking and Subscribing will ensure I (start/keep) doing videos to help more people like you!
// This is my first finished project, and the guides out there are hot garbage for beginners without the ability to buy the EXACT parts for the EXACT project.
// If you like this format and wish for me to put more effort into projects like these consider helping keep the light on.
