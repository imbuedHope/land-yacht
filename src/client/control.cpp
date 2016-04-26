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
PWM pwm("pwm_test_P9_22.11");
PWM pwm2("pwm_test_P9_16.12"); // this needs to be calibrated
Encoder enc("48304180.eqep");

void clean_up(int sig) {
   pwm.stop();
   pwm2.stop();
   enc.disable();
   cout << "Exiting...\n";
   STOP = true;
}

int main(void){

   signal(SIGINT, clean_up);

   pwm.setPeriod(20000000); // 20 ms
   pwm.setDutyCycle(570000u); // 0.57 ms
   pwm.setPolarity(PWM::ACTIVE_HIGH);
   pwm.run();
   pwm2.setPeriod(20000000); // 20 ms
   pwm2.setDutyCycle(570000u); // 0.57 ms
   pwm2.setPolarity(PWM::ACTIVE_HIGH);
   pwm2.run();
   enc.enable();
   while(true) {
      for(unsigned int i = 57; i < 235; i++) {
         pwm.setDutyCycle(i*10000u);
         pwm2.setDutyCycle(2350000u - i*10000u);
         unsigned int position = enc.getPosition();
         float angle = enc.getAngle();
         cout << "Position: " << position << ", Angle: " << angle << "\n";
         usleep(50000); // 50 ms
         if(STOP) break;
      }
      if(STOP) break;
   }
   return 0;
}

