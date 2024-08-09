// Define pin connection for stepper motor driver STEP & DIR functions 
// and for the two switches (ON/OFF & RWD/ST) 
#define DIR_PIN                     9
#define STEP_PIN                    8
#define STAR_TRACKING_SWITCH_PIN    7
#define STOP_SWITCH_PIN             6

// Define delay (in microseconds) to setup the rotational speed of the gears 
// for star tracking and rewinding back the threaded rod
#define REWIND_SPEED_DELAY                                50

// Uncomment this define if you are using a A4988 stepper motor driver with a 16th step resolution
#define A4988_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY     3100 // TODO: try this value => 3125
// Uncomment this define if you use a DRV8825 stepper motor driver with a 16th step resolution
//#define DRV8825_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY   3100

// Default value for STAR_TRACKING_SPEED_DELAY if none of the above has been defined/uncommented
#define STAR_TRACKING_SPEED_DELAY 3100

#ifdef A4988_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY
  #define STAR_TRACKING_SPEED_DELAY A4988_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY
#endif
#ifdef DRV8825_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY
  #define STAR_TRACKING_SPEED_DELAY DRV8825_16TH_STEP_RES_STAR_TRACKING_SPEED_DELAY
#endif

int starTrackingSpeedDelay;

void setup()
{
	// Pins declaration - Ouput
	pinMode(STEP_PIN, OUTPUT);
	pinMode(DIR_PIN, OUTPUT);
  // Pins declaration - Input
  pinMode(STAR_TRACKING_SWITCH_PIN, INPUT);
  pinMode(STOP_SWITCH_PIN, INPUT);

  starTrackingSpeedDelay = STAR_TRACKING_SPEED_DELAY;
}

bool getSwitchState(uint8_t pin)
{
  return (!digitalRead(pin) == 0) ? false : true;
}

void loop()
{
  bool StarTrackingSwitchState = getSwitchState(STAR_TRACKING_SWITCH_PIN);
  bool StopSwitchState = !getSwitchState(STOP_SWITCH_PIN);

  if(!StopSwitchState)
  {
    // Turn clockwise at 3 rpm to track stars => ST mode
    if(StarTrackingSwitchState)
    {
      // Turn clockwise
      digitalWrite(DIR_PIN, HIGH);

      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(starTrackingSpeedDelay);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(starTrackingSpeedDelay);   

    // Turn counter clockwise with a faster speed to rewind the threaded rod => RWD mode
    } else { 
      // Turn counter clockwise
      digitalWrite(DIR_PIN, LOW);

      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(REWIND_SPEED_DELAY);
      digitalWrite(STEP_PIN, LOW);     
      delayMicroseconds(REWIND_SPEED_DELAY);
    }
  }
}
