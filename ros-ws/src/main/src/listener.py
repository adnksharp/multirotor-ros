#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32, Float32MultiArray

std_out: dict = {
    'temp': 0.0,
    'accel': [0.0, 0.0, 0.0],
    'gyro': [0.0, 0.0, 0.0]
}

class Listener(Node):
    def __init__(self):
        super().__init__('listener')

        self.temp = self.create_subscription(
            Float32,
            'driver/mpu/temp',
            self.temp_reader,
            10
        )
        self.accel = self.create_subscription(
            Float32MultiArray,
            'driver/mpu/accel',
            self.accel_reader,
            10
        )
        self.gyro = self.create_subscription(
            Float32MultiArray,
            'driver/mpu/gyro',
            self.gyro_reader,
            10
        )

        self.temp
        self.accel
        self.gyro

    def temp_reader(self, arg):
        std_out['temp'] = arg.data
        self.get_logger().info('%s' % std_out)
    
    def accel_reader(self, arg):
        for i in range(3):
            std_out['accel'][i] = arg.data[i]
        self.get_logger().info('%s' % std_out)
    
    def gyro_reader(self, arg):
        for i in range(3):
            std_out['gyro'][i] = arg.data[i]
        self.get_logger().info('%s' % std_out)

def main(args = None):
    rclpy.init(args = args)
    echo = Listener()
    
    try:
        rclpy.spin(echo)
        echo.destroy_node()
        rclpy.shutdown()
    except:
        pass

if __name__ == '__main__':
    main()
