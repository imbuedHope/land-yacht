#include "PWM.h"
#include "Encoder.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

namespace landYacht {

	Encoder enc;
	 //Servo for Arm connected to WindSail

	double angle = enc.getAngle(); //Obtain the Angle of the WindSail

	//REMEMBER
	// 570000 ns = ~-90 degrees
	// 1460000 ns = ~0 degrees
	// 2350000 ns = ~90 degrees

	double timeperdeg = 570000 / 90; //This is an estimation

	int time = angle*timeperdeg;

	PWM pwm("pwm_test_P9_22.11");

	pwm.setPeriod(20000000); // 20 ms
	pwm.setDutyCycle(time); // time calculated above
	pwm.setPolarity(PWM::ACTIVE_HIGH);
	pwm.run();

	while (true) {
		for (unsigned int i = 57; i < 235; i++) {
			pwm.setDutyCycle(i * 10000u);
			usleep(50000); // 50 ms
		}
	}
	return 0;
}