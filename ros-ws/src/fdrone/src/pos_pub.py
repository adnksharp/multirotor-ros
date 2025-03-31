#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from std_msgs.msg import Float64MultiArray

class JointStatePublisher(Node):
    def __init__(self):
        super().__init__('joint_state_publisher')
        self.publisher_ = self.create_publisher(JointState, 'joint_states', 10)
        self.subscription = self.create_subscription(
            Float64MultiArray,
            'joint_velocities',
            self.velocity_callback,
            10)
        timer_period = 1.0 / 30.0  # 30 Hz
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.positions = [0.0, 0.0, 0.0, 0.0]
        self.velocities = [0.0, 0.0, 0.0, 0.0]

    def velocity_callback(self, msg):
        self.velocities = msg.data

    def timer_callback(self):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.name = ['JX_00', 'JX_01', 'JX_02', 'JX_03']
        msg.position = self.positions
        msg.velocity = self.velocities
        msg.effort = []

        for i in range(4):
            self.positions[i] += self.velocities[i] * (1.0 / 30.0)

        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    joint_state_publisher = JointStatePublisher()
    rclpy.spin(joint_state_publisher)
    joint_state_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
