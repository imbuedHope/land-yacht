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

void* control_thread(void*) {

	steer.setPeriod(20000000); // 20 ms
	steer.setDutyCycle(570000u); // 0.57 ms
	steer.setPolarity(PWM::ACTIVE_HIGH);
	steer.run();
	sail.setPeriod(20000000); // 20 ms
	sail.setDutyCycle(570000u); // 0.57 ms
	sail.setPolarity(PWM::ACTIVE_HIGH);
	sail.run();
	vane.enable();
	// time_t end = time(NULL) + 20; // run for 20 seconds

	unsigned int steerPosition = 1460000u;

	// runs forver until comm thread stops it

	while(halt_system == RUN_STATE) {
		sail.setDutyCycle(1905000u);
		float angle = vane.getAngle();
		if(angle <= 85 || angle >= 95) {
			angle = (int) (angle - 90) % 360;

			if(angle > 180) angle = angle - 360;
			int proportion = 50 * angle;
			steerPosition = steerPosition + proportion;
			if(steerPosition <= 1905000u && steerPosition >= 1015000u)
				steer.setDutyCycle(steerPosition);
			else if(steerPosition > 1905000u)
				steerPosition = 1905000u;
			else if(steerPosition < 1015000u)
				steerPosition = 1015000u;

			cout << "Angle: " << angle << ", Proportion: " << proportion << ", Steer Position: " << steerPosition << endl;
		}

	usleep(10000); // 10 ms
	}

	clean_up();

	return (void*) 0;
}
