#ifndef ENCODER_H_
#define ENCODER_H_
#include<string>
using std::string;

#define ENC_PATH "/sys/devices/ocp.3/48304000.epwmss/"
#define ENC_POSITION "position"
#define ENC_COUNT 4100
#define ENC_ENABLED "enabled"

namespace landYacht {

class Encoder {

private:
	string name, path;

public:
	Encoder(string folderName); // default folder is 48304180.eqep/

	virtual unsigned int getPosition();
	virtual float getAngle();
	virtual int enable();
	virtual int disable();

	virtual ~Encoder();
};

} /* namespace landYacht */

#endif /* ENCODER_H_ */
