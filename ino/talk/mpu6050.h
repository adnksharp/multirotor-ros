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

	float* get_accel() 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		float accel[3] = {a.acceleration.x, a.acceleration.y, a.acceleration.z};
		return accel;
	}

	float* get_gyro() 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		float gyro[3] = {g.gyro.x, g.gyro.y, g.gyro.z};
		return gyro;
	}

	float get_temp() 
	{
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);
		return temp.temperature;
	}
};
