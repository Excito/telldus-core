#include "ProtocolFineoffset.h"
#include "Strings.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>

std::string ProtocolFineoffset::decodeData(ControllerMessage &dataMsg)
{
	std::string data = dataMsg.getParameter("data");
	if (data.length() < 8) {
		return "";
	}

	uint8_t checksum = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2));
	data = data.substr(0, data.length()-2);

	uint8_t humidity = (uint8_t)TelldusCore::hexTo64l(data.substr(data.length()-2));
	data = data.substr(0, data.length()-2);

	uint16_t value = (uint16_t)TelldusCore::hexTo64l(data.substr(data.length()-3));
	double temperature = (value & 0x7FF)/10.0;

	value >>= 11;
	if (value & 1) {
		temperature = -temperature;
	}
	data = data.substr(0, data.length()-3);

	uint16_t id = (uint16_t)TelldusCore::hexTo64l(data) & 0xFF;

	std::stringstream retString;
	retString << "class:sensor;protocol:fineoffset;id:" << id << ";model:";

	if (humidity <= 100) {
		retString << "temperaturehumidity;humidity:" << (int)humidity << ";";
	} else if (humidity == 0xFF) {
		retString << "temperature;";
	} else {
		return "";
	}

	retString << "temp:" << std::fixed << std::setprecision(1) << temperature << ";";

	return retString.str();
}
