#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray

class VelocityPublisher(Node):
    def __init__(self):
        super().__init__('velocity_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, 'joint_velocities', 10)
        timer_period = 0.1
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = Float64MultiArray()
        try:
            input_str = input(">> ")
            vel = list(map(float, input_str.split(',')))
            vel[1] = -vel[1]
            vel[3] = -vel[3]
            if len(vel) == 4:
                msg.data = vel
                self.publisher_.publish(msg)
                self.get_logger().info(f'Publishing: {msg.data}')
        except ValueError:
            self.get_logger().warn('Invalid input format <float,float,float,float>')
        except KeyboardInterrupt:
            self.get_logger().info('Exiting...')
            rclpy.shutdown()

def main(args=None):
    rclpy.init(args=args)
    velocity_publisher = VelocityPublisher()
    rclpy.spin(velocity_publisher)
    velocity_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
