#include <iostream>
#include <unistd.h> // for usleep
#include <signal.h> // for SIGINT
#include "PWM.h"
#include "Encoder.h"
using namespace landYacht;
using namespace std;

// 570000 ns = ~-90 degrees
// 1460000 ns = ~0 degrees
// 2350000 ns = ~90 degrees

// encoder default folder is 48304180.eqep/

volatile bool STOP = false;
PWM sail("pwm_test_P9_22.11");
PWM steer("pwm_test_P9_16.12"); // this needs to be calibrated
Encoder vane("48304180.eqep");

void clean_up(int sig) {
   sail.stop();
   steer.stop();
   vane.disable();
   cout << "Exiting...\n";
   STOP = true;
}

int main(void){

   signal(SIGINT, clean_up);

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
   // while (time(NULL) <= end) {
   while(true) {
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
      if(STOP) break;
      usleep(10000); // 10 ms
   }

   clean_up(0);

   /*

   while(true) {
      for(unsigned int i = 57; i < 235; i++) {
         sail.setDutyCycle(i*10000u);
         steer.setDutyCycle(2350000u - i*10000u);
         unsigned int position = vane.getPosition();
         float angle = vane.getAngle();
         cout << "Position: " << position << ", Angle: " << angle << "\n";
         usleep(50000); // 50 ms
         if(STOP) break;
      }
      if(STOP) break;
   }

   */

   return 0;
}

