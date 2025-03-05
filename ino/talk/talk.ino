#include "config.h"
#include "utils.h"
#include "uros.h"
#include "mpu6050.h"

UROS ros;
MPU impu;

void setup() 
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	impu.begin();
	ros.begin();
	//ros.create_publisher(PUB_TOPIC);
	//ros.create_subscriber(SUB_TOPIC);
	//ros.create_publisher(MPU_ACCEL_TOPIC);
	//ros.create_publisher(MPU_GYRO_TOPIC);
	ros.create_f32(MPU_TEMP_TOPIC);
	digitalWrite(LED, LOW);
}

void loop() 
{
	ros.spin_publisher_f32(impu.get_temp());
	//ros.spin_subscriber();
}
