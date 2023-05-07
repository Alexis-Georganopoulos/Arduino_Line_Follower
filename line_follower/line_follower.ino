
#include <prismino.h>

#define IR_EN 4


// the analog value is between 0 and 1023 so it needs a 16-bit variable
// can use "unsigned int" or "uint16_t" for short
                            //ir_left, ir_right
  unsigned int ir_current[2] = {0,0};

  unsigned int ir_max[2] = {0,0};

  unsigned char button_value;

  unsigned int go = false;

  const float ir_cuttoff = 0.3;
  const int steady_state_speed = -40; //motors are wired in reverse unfortunately

// Function prototypes
void calibration(void);
void button(void);


void setup()
{
  // set pin output mode (sources current)
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(IR_EN, OUTPUT);

  //button callback
  buttonCallback(button);

  Serial.begin(57600);
}

void loop()
{
  if(go)
  {
    
    // turn on the emitter and read the IR sensor
    digitalWrite(IR_EN, HIGH); 
    delayMicroseconds(100);
    ir_current[0] = analogRead(A3);
    ir_current[1] = analogRead(A2);
    digitalWrite(IR_EN, LOW); // turn off IR, no need to keep it on all the time
    if(ir_current[0] >= ir_max[0] && ir_current[1] >= ir_max[1])
    {
      setSpeed(-steady_state_speed, -steady_state_speed);
      delay(100);
    }
    else if (ir_current[0] >= ir_max[0])
    {
      setSpeed(-steady_state_speed, steady_state_speed);
      delay(100);
    }
    else if (ir_current[1] >= ir_max[1])
    {
      setSpeed(steady_state_speed, -steady_state_speed);
      delay(100);
    }
    else
    {
      setSpeed(steady_state_speed, steady_state_speed);
    }

  }
  
  if(!go)
  {
    setSpeed(0, 0);
  }
  button_value = !digitalRead(A1);
  if(button_value && !go)
  {
    calibration();
  }
}

void button(void)
{
  go = !go;
  digitalWrite(LED, !digitalRead(LED));
 
}

//I dont wanna deal with Arduino ide import issues(already had some comflicts with <prismino.h> for some reason) so im dumping it all in one file
//***********************************************************************************************************************************************
// the ".c/.h" of the functions

void irSweep(unsigned int span);

void calibration(void)
{
  Serial.print("Calibrating infrared sensors");
  Serial.print("\n");
  unsigned int motor_speed = 15;
  unsigned int span = 10;

  setSpeed(-motor_speed, motor_speed);
  irSweep(span);
  setSpeed(motor_speed, -motor_speed);
  irSweep(2*span);
  setSpeed(-motor_speed, motor_speed);
  irSweep(span);
  setSpeed(0, 0);
  Serial.print("calibration detected: ");
  Serial.print(ir_max[0]);
  Serial.print(" , ");
  Serial.print(ir_max[1]);
  Serial.print("\n");

  ir_max[0] = int(ir_cuttoff*ir_max[0]);
  ir_max[1] = int(ir_cuttoff*ir_max[1]);
  Serial.print("calibration regulated: ");
  Serial.print(ir_max[0]);
  Serial.print(" , ");
  Serial.print(ir_max[1]);
  Serial.print("\n");
}
void irSweep(unsigned int span)
{
  int i = 0;
  for(i = 0; i< span ;i++)
  {
    digitalWrite(IR_EN, HIGH); 
    delayMicroseconds(100);
    ir_max[0] = (analogRead(A3) > ir_max[0]) ? analogRead(A3) : ir_max[0];
    ir_max[1] = (analogRead(A2) > ir_max[1]) ? analogRead(A2) : ir_max[1];
    digitalWrite(IR_EN, LOW);
    delay(100);
  }

}