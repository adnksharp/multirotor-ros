#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64

class VelocityPublisher(Node):
    def __init__(self):
        super().__init__('robot_joint_velocity_publisher')

        self.publisher_ = [
                self.create_publisher(
                    Float64, 
                    f'world/empty/model/fdrone/joint/rotor_{i}_joint/cmd_vel', 
                    10) for i in range(4) ]


        timer_period = 0.1
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = Float64()
        try:
            try:
                input_str = input(">> ")
            except KeyboardInterrupt:
                self.get_logger().fatal(f'{"X # " * 20}')
                exit(0)
            vel = list(map(float, input_str.split(',')))
            vel[1] = -vel[1]
            vel[3] = -vel[3]
            if len(vel) == 4:
                for i in range(4):
                    msg.data = vel[i]
                    self.publisher_[i].publish(msg)
                self.get_logger().info(f'{msg.data}')
        except:
            pass

def main(args=None):
    rclpy.init(args=args)
    velocity_publisher = VelocityPublisher()
    rclpy.spin(velocity_publisher)
    velocity_publisher.destroy_node()

if __name__ == '__main__':
    main()
