#include "uros.h"

UROS ros;

void setup() 
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	ros.begin();
	ros.create_publisher(PUB_TOPIC);
	ros.create_subscriber(SUB_TOPIC);
	digitalWrite(LED, LOW);
}

void loop() 
{
	ros.spin_publisher();
	ros.spin_subscriber();
}
