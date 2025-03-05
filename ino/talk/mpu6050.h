Adafruit_MPU6050 mpu;

struct MPU 
{
	void begin() 
	{
		mpu.begin();

		while (!mpu.begin()) 
			err(LED);
		
		mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
		mpu.setGyroRange(MPU6050_RANGE_500_DEG);
		mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
	}

	void get_accel(float *accel) 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		accel[0] = a.acceleration.x;
		accel[1] = a.acceleration.y;
		accel[2] = a.acceleration.z;
	}

	void get_gyro(float *gyro) 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		gyro[0] = g.gyro.x;
		gyro[1] = g.gyro.y;
		gyro[2] = g.gyro.z;
	}

	float get_temp() 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		return temp.temperature;
	}
};
