#include "Encoder.h"
#include "util.h"
#include <cstdlib>

namespace landYacht {

	Encoder::Encoder(string folderName) {
		this->name = folderName;
		this->path = ENC_PATH + this->name + "/";
	}

	unsigned int Encoder::getPosition() {
		return atoi(read(this->path, ENC_POSITION).c_str());
	}

	float Encoder::getAngle() {
		return (getPosition() % ENC_COUNT) * 360.0 / ENC_COUNT;
	}

	int Encoder::enable() {
		return write(this->path, ENC_ENABLED, 1);
	}

	int Encoder::disable() {
		return write(this->path, ENC_ENABLED, 0);
	}

	Encoder::~Encoder() {

	}

} /* namespace exploringBB */
