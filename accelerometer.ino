/*
ARDUINO DODECATIMER

* Controls the Bean board
* senses orientation up and down
* sends signal if accelerometer is still for 2 secs or is moving

*/

int getOrientation(int x, int y, int z);
boolean isStill(int x, int y, int z);


//initial accel values
int xRotation = 0;
int yRotation = 0;
int zRotation = 0;

//change in accel
int old_x = 0;
int old_y = 0;
int old_z = 0;

boolean stillFlag;

//set up our AccelerationReading object
AccelerationReading accel;

//variables to check for changes in time
unsigned long timeSinceStartMS = 0;
//hold time since the cube becomes stationary
unsigned long timeSinceStillMS = 0;

void setup() {
  // Optional: Use Bean.setAccelerationRange() to set the sensitivity
  // to something other than the default of ±2g.

  accel = Bean.getAcceleration();

  timeSinceStartMS = millis();
  timeSinceStillMS = millis();

  stillFlag = false;

  Bean.setLed(0,0,0);

  //set our initial accel values
  xRotation = (accel.xAxis);
  yRotation = (accel.yAxis),
  zRotation = (accel.zAxis);
  old_x = xRotation;
  old_y = yRotation;
  old_z = zRotation;

  pinMode( 1, OUTPUT);
}

// the loop routine runs over and over again forever w/ a 100ms sleep
void loop() {

  //get our accelerometer info
  accel = Bean.getAcceleration();

  //update our position
  xRotation = (accel.xAxis);
  yRotation = (accel.yAxis),
  zRotation = (accel.zAxis);

  //update the time since we started the program
  //(Resets after 50 days according to arduino docs)
  timeSinceStartMS = millis();

  //if the accelerometer isn't still, reset our Still timer
  if(!isStill(xRotation, yRotation, zRotation)) {
    timeSinceStillMS = timeSinceStartMS;
  }

  //if 1000ms has past since we last moved, send signal to start timer
  if(timeSinceStartMS - timeSinceStillMS >= 1000)
  {
    //if we sat still for 2 sec, turn led on and send signal for still and orientation
    Bean.setLed(255,255,255);
    getOrientation(xRotation, yRotation, zRotation);
    digitalWrite(3, HIGH);
  } else {
    //if we've moved, set our led to off and stop signal
    Bean.setLed(0,0,0);
    digitalWrite(3, LOW);
  }
  //sleep for half a second to conserve power
  Bean.sleep(500);
}

int getOrientation(int x, int y, int z) {

  //check x is up
  if(x > 166)
  {
    digitalWrite(1, HIGH);
  }
  //x is up at an angle
  else if(x <= 166 && x > 0) {
    //Bean.setLed(255,0,0);
  }
  //x is down at an angle
  else if(x <= 0 && x > -166) {
    //Bean.setLed(0,255,0);
  }
  //x is down
  else if(x <= -166){
    digitalWrite(1, LOW);
  }
}

//checks the state of the cube's accelerometer
boolean isStill(int x, int y, int z) {

  //find the change in each orientation value
  int delta_x = x - old_x;
  int delta_y = y - old_y;
  int delta_z = z - old_z;

  //check for movement with a 40 threshold
  if(delta_x > 40 || delta_x < -40 || delta_y > 40 || delta_y < -40 || delta_z > 40 || delta_z < -40) {

    //if we are moving, grab the new values to compare
    old_x = x;
    old_y = y;
    old_z = z;

    return false;
  }

  //if change in each variable isn't above the threshold we aren't moving
  return true;

}
