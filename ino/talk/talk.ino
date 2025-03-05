#include "config.h"
#include "utils.h"
#include "uros.h"
#include "mpu6050.h"

// uROS object
UROS ros;
// MPU6050 object
MPU impu;

void setup() 
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	// initialize MPU6050
	impu.begin();
	
	// initialize uROS
	ros.begin();
	//ros.create_publisher(PUB_TOPIC);
	//ros.create_subscriber(SUB_TOPIC);
	// make uROS topics
	ros.create_multi_f32(MPU_ACCEL_TOPIC, 3, ros.mpu_accel);
	ros.create_multi_f32(MPU_GYRO_TOPIC, 3, ros.mpu_gyro);
	ros.create_f32(MPU_TEMP_TOPIC, ros.mpu_temp);
	
	digitalWrite(LED, LOW);
}

void loop() 
{
	// get accel, gyro, temp from MPU6050 and publish them
	float mpu_data[3];
	impu.get_accel(mpu_data);
	ros.spin_publisher_multi_f32(mpu_data, 3, ros.mpu_accel);
	impu.get_gyro(mpu_data);
	ros.spin_publisher_multi_f32(mpu_data, 3, ros.mpu_gyro);
	ros.spin_publisher_f32(impu.get_temp(), ros.mpu_temp);

	//ros.spin_subscriber();
}
