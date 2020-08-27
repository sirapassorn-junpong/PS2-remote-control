#include <PS2X_lib.h>

#define PS2_DAT        2  
#define PS2_CMD        3  
#define PS2_SEL        4  
#define PS2_CLK        5  

#define pressures   true
#define rumble      true

PS2X ps2x; // create PS2 Controller Class

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
  Serial.begin(57600);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?)
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring.");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands.");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
    case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
  }
}

void loop () {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
   //DualShock Controller

   ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    
   if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
     Serial.println("Start is being held");
   if(ps2x.Button(PSB_SELECT))
     Serial.println("Select is being held");      

   if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
     Serial.print("Up held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
   }
   if(ps2x.Button(PSB_PAD_RIGHT)){
     Serial.print("Right held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
   }
   if(ps2x.Button(PSB_PAD_LEFT)){
     Serial.print("LEFT held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
   }
   if(ps2x.Button(PSB_PAD_DOWN)){
     Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
   }   
   
   vibrate = 0; //ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
   if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
     if(ps2x.Button(PSB_L3))
       Serial.println("L3 pressed");
     if(ps2x.Button(PSB_R3))
       Serial.println("R3 pressed");
     if(ps2x.Button(PSB_L2))
       Serial.println("L2 pressed");
     if(ps2x.Button(PSB_R2))
       Serial.println("R2 pressed");
     if(ps2x.Button(PSB_TRIANGLE))
       Serial.println("Triangle pressed");        
   }

   if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
     Serial.println("Circle just pressed");
   if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
     Serial.println("X just changed");
   if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
     Serial.println("Square just released");     

   if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
   }  
   delay(50);  
}

/*
   DualShock Controller Codes
   PS2X ps2x; // create PS2 Controller Class
   ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
   ---------------------------------------------
   Hold:              
   ps2x.Button(...)
   ---------------------------------------------
   Pressed:           
   ps2x.ButtonPressed(...)
   ---------------------------------------------
   Released:          
   ps2x.ButtonReleased(...)
   ---------------------------------------------
   ButtonChangeState: 
   ps2x.NewButtonState(...)
   ---------------------------------------------
   Anoter way of Pressed:
   if (ps2x.NewButtonState()) {
      if (ps2x.Button(...)){
      ...
      }
   }
   ---------------------------------------------
   Analog read (how hard the button has been pressed, or the L/Rstick value):
   (ps2x.Analog(...), DEC) //change to DEC
   ---------------------------------------------
   Button Names
   PSB_START      : start button
   PSB_SELECT     : select button
   PSB_PAD_UP     : up
   PSB_PAD_DOWN   : down
   PSB_PAD_LEFT   : left
   PSB_PAD_RIGHT  : right
   PSB_CIRCLE     : circle
   PSB_CROSS      : cross
   PSB_SQUARE     : square
   PSB_TRIANGLE   : triangle
   PSB_L1         : L1
   PSB_R1         : R1
   PSB_L2         : L2
   PSB_R2         : R2
   PSB_L3         : L3 (left stick pressed)
   PSB_R3         : R3 (right stick pressed)
   PSS_LX         : left stick in X axis 
   PSS_LY         : right stick in Y axis
   PSS_RX         : left stick in X axis
   PSS_RY         : right stick in Y axis
   ---------------------------------------------
 */
