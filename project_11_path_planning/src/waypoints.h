
/****************************************************
*
* Filename: waipoints.h
*
* Author: feihong -- lhfly520@gmail.com
* Description:---
* Create:        2017-09-01 12:28:00
* Last Modified: 2017-09-02 22:35:27
*****************************************************/

#include "utils.h"

// Choose in the map of highway waypoints the one closest to the car
// 选择离vehicle最近的一个waypoint，注意，最近的，只有一个点
int get_close_waypoint(double x, double y, std::vector<double> maps_x, std::vector<double> maps_y) { 

    // 先初始化最近距离，极大值
    double closest_len = std::numeric_limits<int>::max(); 
    // 最近的waypoint的下标，计数
    int closest_waypoint = 0; 

    // 拿maps向量里面的所有点都和当前点(x, y)进行比较，计算距离
    // maps_x, maps_y都是向量，里面有多个点
    for (int i = 0; i < maps_x.size(); i++) {
        /* code */
        double map_x = maps_x[i];
        double map_y = maps_y[i];

        // 计算点(x, y), 和点(map_x, map_y)两个点之间的距离
        // 调用的是utils里面的distance函数
        double dist = distance(x, y, map_x, map_y); 
        if (dist < closest_len) {
            closest_len      = dist;
            closest_waypoint = i;
        }
    }
    return closest_waypoint; 
}

// Choose in the map of highway waypoints the closest before the car (that is the next).
// 选择地图里面位于车前的最近的waypoints
// The actual closest waypoint could be behind the car.
// 实际的最近的waypoints有可能是在车后面
int get_next_waypoint(double x, double y, double theta, std::vector<double> maps_x, std::vector<double> maps_y) { 

    // 调用上面定义的函数，找到离车最近的waypoint, 注意，还是一个点
    int closest_waypoint = get_close_waypoint(x, y, maps_x, maps_y); 

    // 根据找到的closest_waypoint的下标，来拿到最近的waypoint的坐标
    double map_x = maps_x[closest_waypoint]; 
    double map_y = maps_y[closest_waypoint]; 

    // 利用waypoint和当前点(x, y)的反正切角来得到heading方向
    double heading = atan2(map_y - y, map_x - x); 

    // theta是原来上一个状态的运动方向么？
    // angle是角度的改变量
    double angle = abs(theta - heading); 

    // 如果angle过大，大于45度的话
    if (angle > pi / 4) {
        closest_waypoint++;  // close_waypoint计数 + 1, 选择下一个点，这个点，不符合要求，角度改变量太大了  
    }

    return closest_waypoint; 
}
