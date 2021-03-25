# actionMove_In_SimulationEnvironment-
在Gazebo中的Turtlebot3仿真环境中，基于action向机器人发送目的地

基于ROS教程 http://wiki.ros.org/tf/Tutorials 与 http://wiki.ros.org/actionlib_tutorials/Tutorials 创建的包。

实现功能：
wei_goto_client 向 wei_goto_server发送一个位姿pose，wei_goto_server向topic "/move_base_simple/goal"（即move_base订阅的topic）持续发送该位姿。wei_goto_server会持续监控turtlebot的位置，直到机器人到达目标位置。

节点介绍：
1. fibonacci_client.cpp 与 fibonacci_server.cpp 用于测试action相关功能。
2. tf_boradcaser.cpp 与 tf_listener.cpp 用于测试tf相关功能。
3. wei_goto_client.cpp 与 wei_goto_server.cpp 实现向topic "/move_base_simple/goal"发送位置位置信息的一组client与server。

环境：
1. Ubuntu16、kinetic
2. 构建gazebo，turtlebot3的导航仿真环境。参考 https://www.ncnynl.com/category/Turtlebot3-Simulation/ （进行到单机导航即可）

运行命令：

命令行1：加载gazebo仿真环境

roslaunch turtlebot3_gazebo turtlebot3_world.launch

命令行2：启动turtlebot3的建图与导航

roslaunch turtlebot3_navigation turtlebot3_navigation.launch

命令行3：启动action服务器 wei_goto_server

rosrun wei_goto wei_goto_server

命令行3：启动action clent wei_goto_client

rosrun wei_goto wei_goto_client
