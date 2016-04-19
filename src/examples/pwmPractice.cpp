/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <unistd.h> //for usleep
#include "gpio/PWM.h"
using namespace exploringBB;
using namespace std;

// 570000 ns = ~-90 degrees
// 1460000 ns = ~0 degrees
// 2350000 ns = ~90 degrees

int main(){
   PWM pwm("pwm_test_P9_22.11");
   PWM pwm2("pwm_test_P9_16.12"); // this needs to be calibrated
   pwm.setPeriod(20000000); // 20 ms
   pwm.setDutyCycle(570000u); // 0.57 ms
   pwm.setPolarity(PWM::ACTIVE_HIGH);
   pwm.run();
   pwm2.setPeriod(20000000); // 20 ms
   pwm2.setDutyCycle(570000u); // 0.57 ms
   pwm2.setPolarity(PWM::ACTIVE_HIGH);
   pwm2.run();
   while(true) {
      for(unsigned int i = 57; i < 235; i++) {
         pwm.setDutyCycle(i*10000u);
         pwm2.setDutyCycle(2350000u - i*10000u);
         usleep(50000); // 50 ms
      }
   }
   return 0;
}
