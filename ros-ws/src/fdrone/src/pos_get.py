#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

names = ['rotor_0_joint', 'rotor_1_joint', 'rotor_2_joint', 'rotor_3_joint']
positions = [0.0, 0.0, 0.0, 0.0]
velocities = [0.0, 0.0, 0.0, 0.0]
efforts = [0.0, 0.0, 0.0, 0.0]

class JointStatePublisher(Node):
    def __init__(self):
        super().__init__('robot_joint_position_publisher')
        self.publisher_ = self.create_publisher(
                JointState, 
                'joint_states', 
                10)

        self.subscription = [ self.create_subscription(
            JointState,
            f'/world/empty/model/fdrone/joint/rotor_{i}_joint/state',
            self.gz_pose_callback,
            10) for i in range(4) ]

    def gz_pose_callback(self, msg):
        global names, positions, velocities, efforts
        joint_state = JointState()
        joint_state.header.stamp = self.get_clock().now().to_msg()
        joint_state.name = names

        match msg.name[0]:
            case 'rotor_0_joint':
                positions[0] = msg.position[0]
                velocities[0] = msg.velocity[0]
                efforts[0] = msg.effort[0]
            case 'rotor_1_joint':
                positions[1] = msg.position[0]
                velocities[1] = msg.velocity[0]
                efforts[1] = msg.effort[0]
            case 'rotor_2_joint':
                positions[2] = msg.position[0]
                velocities[2] = msg.velocity[0]
                efforts[2] = msg.effort[0]
            case 'rotor_3_joint':
                positions[3] = msg.position[0]
                velocities[3] = msg.velocity[0]
                efforts[3] = msg.effort[0]

        joint_state.position = positions
        joint_state.velocity = velocities
        joint_state.effort = efforts
        self.publisher_.publish(joint_state)
        self.get_logger().info(f'{positions}')

def main(args=None):
    rclpy.init(args=args)
    joint_state_publisher = JointStatePublisher()
    rclpy.spin(joint_state_publisher)
    joint_state_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
