import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32, Float32MultiArray

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
        self.get_logger().info('temp: %f' % arg.data)
    
    def accel_reader(self, arg):
        self.get_logger().info('accel: %s' % arg.data)
    
    def gyro_reader(self, arg):
        self.get_logger().info('guro: %s' % arg.data)

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
