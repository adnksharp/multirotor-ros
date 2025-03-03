#include "uros.h"

UROS ros;

void setup() 
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	ros.init();
	digitalWrite(LED, LOW);
}

void loop() 
{
	ros.spin();
	digitalWrite(LED, ros.ping());
}
