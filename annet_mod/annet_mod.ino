#include <Servo.h>

/*
 * This code written by Jeff Glass, October 31 2015. It controls 3 servos with specified behavior for a prodcution at Seatle Children's Theatre
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,    
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License    
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

Servo Truely;  // create servo object to control a servo
Servo Jemma;   // a maximum of eight servo objects can be created
Servo Boy;

//THE FOLLOWING 4 SETS OF VARIBLES CONTROL ALL OF THE BEHAVIOR OF THE SERVOS

//constants which define the minimum and maximum position of each servo
const long POS_T_MIN = 0; 
const long POS_T_MAX = 110;
const long POS_J_MIN = 0;
const long POS_J_MAX = 110;
const long POS_B_MIN = 2;
const long POS_B_MAX = 112;

/* constants which define the time it takes for each servo to travel in one direction (i.e. half
 * the total cycle time. For convenience, these are listed in milliseconds.
 */
const long CHANGE_DURATION_T = 13750;
const long CHANGE_DURATION_J = 13750;
const long CHANGE_DURATION_B = 13750/2;

/* Constants which hold the duration of the pause at the end of each half-cycle, during which 
 *  the servo holds still before reversing direction. For convenience, these are 
 *  listed in milliseconds. 
 */
const long PAUSE_DURATION_T = 100;
const long PAUSE_DURATION_J = 100;
const long PAUSE_DURATION_B = 100;

/*constants which hold the phase offset of each Servo. These are listed in Milliseconds of offset.
 * The servos will behave as if they started this number of milliseconds earlier than the start time.
 *Must be positive.
 */
const long PHASE_OFFSET_T = 0;
const long PHASE_OFFSET_J = CHANGE_DURATION_J + PAUSE_DURATION_J; //(one full half-cycle, to have J be opposite of T)
const long PHASE_OFFSET_B = 0;

////////Other constants, mostly derived from the above. Do not change! //////////

/*Constants to hold the duration of one "half cycle", including a rotation in one directoin and the following
 * pause. Will be useful in the calculations below 
 */

const long HALF_CYCLE_T = CHANGE_DURATION_T + PAUSE_DURATION_T;
const long HALF_CYCLE_J = CHANGE_DURATION_J + PAUSE_DURATION_J;
const long HALF_CYCLE_B = CHANGE_DURATION_B + PAUSE_DURATION_B;

//Constants which hold the extent of the movement for each servo. DO NOT MODIFY, these are calculated from the above values
const long SPAN_T = POS_T_MAX - POS_T_MIN;
const long SPAN_J = POS_J_MAX - POS_J_MIN;
const long SPAN_B = POS_B_MAX - POS_B_MIN;

// variables to store the current servo positions
long posT = POS_T_MIN; //starts at 0    
long posJ = POS_J_MAX; //starts at 110
long posB = POS_B_MIN; //starts at 2

//For debug purposes:
boolean printedThisSecond = false;

long mytime; //time in milliseconds since the program started. Updated every time through loop()
long START_TIME; //time that the setup finished and the loop began running. Only set once.

void setup() {
  Boy.attach(8);
  Truely.attach(9);
  Jemma.attach(10);

  delay(250); //wait 1 second after boot-up to begin the loop, to allow Servo library to fully attach.

  Serial.begin(9600);
  delay(200);

  START_TIME = millis();
}

void loop() {
  mytime = millis() + START_TIME;

  //TRUELY///////////////////////////

  long time_T = mytime + PHASE_OFFSET_T; //the "effective time" for this servo, taking into account the startup delay and phase offset
  long cycleNumber_T = int(time_T/HALF_CYCLE_T);
  long timeSinceLastDirectionChange_T = time_T - (cycleNumber_T * HALF_CYCLE_T);
  long dir = ((int(cycleNumber_T) % 2) - 0.5) * -2; //Should be 1 for an increasing rotation, and -1 for a decreasing rotation

  float percentThroughChange_T;
  
  if (timeSinceLastDirectionChange_T > CHANGE_DURATION_T){ //If we are at the end of a half-cycle, and in the pause-period, just write the maximum or minimum rotation value as appropriate
    if (dir > 0) {Truely.write(POS_T_MAX); posT = POS_T_MAX;}
    else {Truely.write(POS_T_MIN); posT = POS_T_MIN;}
  }
  else {
    percentThroughChange_T = float(timeSinceLastDirectionChange_T)/float(CHANGE_DURATION_T);
    if (dir > 0) posT = (SPAN_T * (percentThroughChange_T)) + POS_T_MIN;
    else posT = (POS_T_MAX - (SPAN_T * (percentThroughChange_T)));
    Truely.write(int(posT));
  }

  //JEMMA////////

  long time_J = mytime + PHASE_OFFSET_J; //the "effective time" for this servo, taking into account the startup delay and phase offset
  long cycleNumber_J = int(time_J/HALF_CYCLE_J);
  long timeSinceLastDirectionChange_J = time_J - (cycleNumber_J * HALF_CYCLE_J);
  dir = ((int(cycleNumber_J) % 2) - 0.5) * -2; //Should be 1 for an increasing rotation, and -1 for a decreasing rotation

  float percentThroughChange_J;
  
  if (timeSinceLastDirectionChange_J > CHANGE_DURATION_J){ //If we are at the end of a half-cycle, and in the pause-period, just write the maximum or minimum rotation value as appropriate
    if (dir > 0) {Jemma.write(POS_J_MAX); posJ = POS_J_MAX;}
    else {Jemma.write(POS_J_MIN); posJ = POS_J_MIN;}
  }
  else {
    percentThroughChange_J = float(timeSinceLastDirectionChange_J)/float(CHANGE_DURATION_J);
    if (dir > 0) posJ = (SPAN_J * (percentThroughChange_J)) + POS_J_MIN;
    else posJ = (POS_J_MAX - (SPAN_J * (percentThroughChange_J)));
    Jemma.write(int(posJ));
  }

  //BOY////////

  long time_B = mytime + PHASE_OFFSET_B; //the "effective time" for this servo, taking into account the startup delay and phase offset
  long cycleNumber_B = int(time_B/HALF_CYCLE_B);
  long timeSinceLastDirectionChange_B = time_B - (cycleNumber_B * HALF_CYCLE_B);
  dir = ((int(cycleNumber_B) % 2) - 0.5) * -2; //Should be 1 for an increasing rotation, and -1 for a decreasing rotation

  float percentThroughChange_B;
  
  if (timeSinceLastDirectionChange_B > CHANGE_DURATION_B){ //If we are at the end of a half-cycle, and in the pause-period, just write the maximum or minimum rotation value as appropriate
    if (dir > 0) {Jemma.write(POS_B_MAX); posB = POS_B_MAX;}
    else {Jemma.write(POS_B_MIN); posB = POS_B_MIN;}
  }
  else {
    percentThroughChange_B = float(timeSinceLastDirectionChange_B)/float(CHANGE_DURATION_B);
    if (dir > 0) posB = (SPAN_B * (percentThroughChange_B)) + POS_B_MIN;
    else posB = (POS_B_MAX - (SPAN_B * (percentThroughChange_B)));
    Boy.write(int(posB));
  }

  //END BOY//////////////////////////////

  /*
   * The following sends serial lines containing the current milliseconds since start and the position of each servo.
   * Useful for debugging. Adjust the sendEvery parameter below to control how often the arduino
   * sends a Serial message
   */

  int sendEvery = 250; //A debug message will be sent every this number of milliseconds.

  if ((mytime % sendEvery) > 0){
    printedThisSecond = true;
  }

  if ((mytime % sendEvery) == 0 && printedThisSecond == true){
    //Serial strings are fomatted as "Millisconds,Truely's Position,Jemma's Position,Boy's Position"
    Serial.print(time_T);
    Serial.print(',');
    Serial.print(posT);
    Serial.print(',');
    Serial.print(posJ);
    Serial.print(',');
    Serial.println(posB);
    
    printedThisSecond = false;
  }
  
}









