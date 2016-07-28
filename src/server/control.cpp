#include <iostream>
#include <unistd.h> // for usleep
#include <signal.h> // for SIGINT

#include "lib/PWM.h"
#include "lib/Encoder.h"

using namespace landYacht;
using namespace std;

extern "C"
{
#include "server/comn.h"
}

// 570000 ns = ~-90 degrees
// 1460000 ns = ~0 degrees
// 2350000 ns = ~90 degrees

// encoder default folder is 48304180.eqep/

PWM sail("pwm_test_P9_22.11");
PWM steer("pwm_test_P9_16.12"); // this needs to be calibrated
Encoder vane("48304180.eqep");

static void clean_up() {
	// just to be on the safe side...
	halt_system = HALT_STATE;

	sail.stop();
	steer.stop();
	vane.disable();
	cout << "Exiting...\n";
}

//NOTES
//Encoder reads Clockwise as positive angle and CCW as negative angle
//For servos
//ACTIVE_LOW = CCW
//ACTIVE_HIGH = CW

//Working variables
unsigned long lastTime;
double eAngle, steerAngle, SetAngle; //Encoder Angle, Steering Angle (Result), Angle 
double ITerm, lasteAngle;
double kp, ki;
// Time variables
int timeperdeg, timesteer;
// Error Variables
double error, deAngle; 
int SampleTime = 100; // 0.1 seconds

void* control_thread(void*) {
	
	//Set Angle in this case can be 0 or 90 degrees depending on orietation used, as long as wind vane is 90 degrees away from body (perpendicular).
	SetAngle = 0;

	//Call Encoder
	Encoder enc;
	
	unsigned long now = millis();
	int timeChange = (now - lastTime);

	if(timeChange >= SampleTime)
	{
		//Obtain value for Encoder Angle
		eAngle = enc.getAngle(); //encoder measures positive CW 

		//Time per Angle
		timeperdeg = 570000./90; //Estimation 
h
		// Compute all working error variables
			error = eAngle - SetAngle; //For Proportional Controller
			ITerm += (error * ki); ///For Integration Controller
			double deAngle = (eAngle - lasteAngle);

		//Calculate the output
			steerAngle = kp * error + ITerm - kd * deAngle;

		if(steerAngle >= 45 || steerAngle <= -45)
		{
		timesteer = 45*timeperdeg; 
		}
		else
		{
		timesteer = abs(steerAngle)*timeperdeg; //Need absolute of angle otherwise
		//Negative time 
		}
		
		//Additional Variables
		lasteAngle = eAngle;
		lastTime = now;

		//Time to activate the steering
		steer.setPeriod(20000000); // 20 ms
		steer.setDutyCycle(timesteer); // time calculated above
		
		//Set up Direction

		if (steerAngle >= 0)
		{
		steer.setPolarity(PWM::ACTIVE_HIGH);
		}
		else if(steerAngle < 0)
		{
		steer.setPolarity(PWM::ACTIVE_LOW);
		}
		
		steer.run();
	}	

	if (now >= 500)
	{
	clean_up();

	return (void*) 0;
	}	
}

void setTunings(double Kp, double Ki, double Kd)
	{
		double SampleTimeinSec = ((double)SampleTime)/1000;
		kp = Kp * SampleTimeinSec;
		ki = Ki / SampleTimeinSec;
		kd = Kd / SampleTimeInSec;
	}	

void SetSampleTime(int NewSampleTime)
{
	if (NewSampleTimek > 0)
	{
		double ratio = (double)NewSampleTime / (double)SampleTime;
		ki *= ratio;
		kd /= ratio;
		SampleTime = (unsigned long)NewSampleTime;
	}
}

