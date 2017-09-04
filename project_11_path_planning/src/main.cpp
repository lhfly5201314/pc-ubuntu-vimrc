//C++ file
/****************************************************
*
* Filename: main.cpp
*
* Author: feihong -- lhfly520@gmail.com
* Description:---
* Create:        2017-09-01 15:13:51
* Last Modified: 2017-09-03 19:25:17
*****************************************************/

#include <fstream>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "spline.h"
#include "coords_transform.h"

using namespace std;
using json = nlohmann::json; 

// Checks if the SocketIO event has JSON data.
string hasData(string s) { 
    auto found_null = s.find("null"); 
    auto b1 = s.find_first_of("["); 
    auto b2 = s.find_first_of("}"); 
    if (found_null != string::npos) {
        return ""; 
    } else if (b1 != string::npos && b2 != string::npos) { 
        return s.substr(b1, b2 - b1 + 2); 
    }
    return ""; 
}

int main()
{
    uWS::Hub h; 

    // Load up map values for waypoint's 'x', 'y', 's' and 'd' normalized normal vectors
    // 加载waypoint的地图值，也就是两种类型的坐标
    // 定义几个向量，每个向量装一个坐标的信息，因为
    // map_waypoints可以有多个
    vector<double> map_waypoints_x; 
    vector<double> map_waypoints_y; 
    vector<double> map_waypoints_s; 
    vector<double> map_waypoints_dx; 
    vector<double> map_waypoints_dy; 

    // Waypoints map to read from
    string map_file_ = "../data/highway_map.csv"; 
    std::cout << "数据读取成功" << std::endl; 

    // The max_s value before wrapping around the track back to 0
    // 赛道的总长度
    double max_s= 6945.554; 

    // 文件输入流
    ifstream in_map_(map_file_.c_str(), ifstream::in); 

    string line; 
    // 将文件流读入到string对象里面
    while (getline(in_map_, line)) { 
        istringstream iss(line); 
        double x; 
        double y; 
        float s; 
        float d_x; 
        float d_y; 

        iss >> x; 
        iss >> y; 
        iss >> s; 
        iss >> d_x; 
        iss >> d_y; 

        map_waypoints_x.push_back(x); 
        map_waypoints_y.push_back(y); 
        map_waypoints_s.push_back(s); 
        map_waypoints_dx.push_back(d_x); 
        map_waypoints_dy.push_back(d_y); 
    }

    // Start on lane 1
    // 从lane 1 开始
    int lane = 1; 

    // Reference velocity (mph)
    // 推荐的速度
    double ref_vel = 0.0; 

    // 将这几个向量已经lane，vel的信息以引用(内存地址)的形式返回给模拟器
    h.onMessage([&map_waypoints_x, &map_waypoints_y, &map_waypoints_s, &map_waypoints_dx, &map_waypoints_dy, &ref_vel, &lane](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) { 
            if (length && length > 2 && data[0] == '4' && data[1] == '2') { 

                auto s = hasData(data); 
                // cout << "s: " << s <<endl; 

                // 返回的数据s不为""
                if (s != "") {
                    json j = json::parse(s); 

                    // cout << "j: " << j <<endl; 

                    string event = j[0].get<string>(); 

                    if (event == "telemetry") {
                        // j[1] is the data JSON object
                        //
                        // Main car's localization data
                        // 主车的位置信息
                        double car_x     = j[1]["x"];
                        double car_y     = j[1]["y"];
                        double car_s     = j[1]["s"];
                        double car_d     = j[1]["d"];
                        double car_yaw   = j[1]["yaw"];
                        double car_speed = j[1]["speed"];

                        // Previous path data given to the Planner
                        // 上一个路径的数据给到planner里面来
                        // 这些数据也是可以从json字符串里面来取到的
                        auto previous_path_x = j[1]["previous_path_x"]; 
                        auto previous_path_y = j[1]["previous_path_y"]; 

                        // Previous path's end 's' and 'd' values
                        // 之前路径的s和d数据，也是坐标
                        double end_path_s = j[1]["end_path_s"]; 
                        double end_path_d = j[1]["end_path_d"]; 


                        // Sensor Fusion data  - a list of all other cars on the same side of the road.
                        // 传感器聚合数据，在路同一层的其他车的列表
                        auto sensor_fusion = j[1]["sensor_fusion"]; 

                        int prev_size = previous_path_x.size(); 

                        // 如果之前一个path的x列表不为空的话
                        // 说面当前主车已经到达终点离 
                        if (prev_size > 0) {
                            car_s = end_path_s;  // 终点的时候的s坐标
                        }

                        bool is_too_close            = false;
                        bool prepare_for_lane_change = false;
                        bool ready_for_lane_change   = false;
                        bool is_left_lane_free       = true;
                        bool is_right_lane_free      = true;

                        for (size_t i = 0; i < sensor_fusion.size(); ++i) {

                            // 上面拿到了sensor_fusion数据，就可以开始构造出vehicle对象了
                            Vehicle vehicle(sensor_fusion[i]); 

                            // 如果vehicle实在lane之内的话
                            if (is_in_lane(vehicle.d, lane)) {

                                // 用之前的点来投影s的值
                                vehicle.s += (double)prev_size * 0.02 * vehicle.speed;  // use previous points to project s value onward  

                                // 判断，然后赋值，居然可以这样用
                                // vechile的s大于我的主车的当前的s，那当然就是位于我的主车的前面了
                                bool is_in_front_of_us = vehicle.s > car_s; 
                                // 判断两辆车之间是否离得足够远，是否够安全
                                bool is_closer_than_safety_margin = vehicle.s - car_s < safety_margin; 

                                // 如果在我们的车前面，而且距离比安全margin还近的话
                                if (is_in_front_of_us && is_closer_than_safety_margin) {
                                    is_too_close = true;  // flag, 标识为太近了 
                                    prepare_for_lane_change = true;  // 如果太近了，那么就准备变道 
                                }

                            }
                        }

                        // 如果状态是准备变道的话
                        if (prepare_for_lane_change) {
                            int num_vehicles_left  = 0;
                            int num_vehicles_right = 0;
                            // Check if left and right lanes are free
                            // 检查左右lane是否可用
                            // 这里遍历每一个sensor_fusion data其实就是在遍历每一个当前主车两侧的其他car了
                            for (size_t i = 0; i < sensor_fusion.size(); ++i) {
                                // 还是要先把sensor_fusion.data给转为vehicle对象
                                Vehicle vehicle(sensor_fusion[i]); 
                                // Check left lane
                                // 先检查左lane
                                // 如果当前的vehicle i在lane - 1这个lane里面的话
                                // 做lane编号，就是当前lane编号 - 1个
                                // 判断在左lane中的车并且比较其和我们主车的距离，看是否足够安全
                                if (is_in_lane(vehicle.d, lane - 1)) {
                                     ++num_vehicles_left;  // 左边的车的计数 + 1  
                                     // 计算vehicle.s的公式
                                     vehicle.s += (double)prev_size * 0.02 * vehicle.speed; 
                                     // 检查距离距离是否过近，看这个变量的命名
                                     // 距离太近了就不能变道
                                     // 太近的标准就是车的s大小位于car_s+-0.5个安全距离之间，说面离得太近了 
                                     bool too_close_to_change = (vehicle.s > car_s - safety_margin / 2) && (vehicle.s < car_s + safety_margin / 2); 
                                     if (too_close_to_change)
                                         is_left_lane_free = false;  // 如果太近，把左lane可用置为false 
                                }
                                // 同理，需要check right lane
                                // 右lane编号就是当前lane编号 + 1
                                // 如果当前车实在右边的lane里面的话
                                else if (is_in_lane(vehicle.d, lane + 1)) { 
                                     ++num_vehicles_right;  // 右车个数 + 1  
                                     vehicle.s += (double)prev_size * 0.02 * vehicle.speed; 
                                     bool too_close_to_change = (vehicle.s > car_s - safety_margin / 2) && (vehicle.s < car_s + safety_margin / 2); 
                                     if (too_close_to_change)
                                         is_right_lane_free = false;  // 太近了，无法向右边变道 
                            }

                                if (is_left_lane_free || is_right_lane_free)
                                    // 只要有任意一边的lane可用的话，都可以准备变道了
                                    ready_for_lane_change = true; 
                        }

                            // 打印左右两边vehicle的数量
                            cout << "LEFT " << num_vehicles_left << " RIGHT " << num_vehicles_right << endl; 
                    }
                        // Actually perform lane change
                        // 实际的变道表现
                        // 如果准备变道，而且left lane可用,而且当前lane不是最左边的lane的话，那就向左变道
                        // 也就是lane编号 - 1
                        // 看来是优先向左边变道了
                        if (ready_for_lane_change && is_left_lane_free && lane > 0) {
                            lane -= 1; 
                        }
                        else if (ready_for_lane_change && is_right_lane_free && lane < 2) {  // 向右边变道也是同样的道理，总共就3条lane，所以向右变道时，当前lane不能是位于最右边 
                            lane += 1;  
                }

                        // Eventually slow down if too close the car before
                        // 如果离前面的车太近的话，就需要进行减速了
                        if (is_too_close) {
                            // 速度进行递减
                            ref_vel -= 0.224;  // deceleration around 5m / s^2 
                        }
                        else if (ref_vel < max_safe_speed) { 
                            ref_vel += 0.224;  // 如果离前面的车不近，而且当前速度还没有达到限制的最大速度，那么需要继续加速   
                        }

                        // List of widely spaced (x, y) waypoints.These will be later interpolated
                        // with a spline, filling it with more points which control speed
                        vector<double> pts_x; 
                        vector<double> pts_y; 

                        // Reference x, y, yaw state
                        // 参考坐标值第一次是初始为主car的坐标值
                        // yaw也是用的主car的yaw
                        double ref_x = car_x; 
                        double ref_y = car_y; 
                        // 参照系yaw是一个弧度？这里要把degree转成rad
                        double ref_yaw = deg2rad(car_yaw); 

                        // cout << "ref_x: " << ref_x << endl; 
                        // cout << "ref_y: " << ref_y << endl; 
                        // cout << "ref_y:yaw " << ref_yaw << endl; 

                        // If previous size it almost empty, use the car as a starting reference
                        // 如果之前的size大小基本上是空的，那么把我们自己的car做为开始的reference
                        if (prev_size < 2) { // size小于2就是说面只有一辆车啊，almost empty了 
                            // 算prev_car的坐标，需要用到yaw
                            // 这里yaw的作用，很像orientation啊
                            // 这里当prev_size的个数小于2的时候
                            // 直接用的是我们主car的car_yaw来计算prev_car
                            // 的坐标, 而不是像else里面的情况，else里面是
                            // 直接上标再向前推一个，直接用下标来取
                            double prev_car_x = car_x - cos(car_yaw); 
                            double prev_car_y = car_y - sin(car_yaw); 

                            // 直接把prev_car_x, 和我们的主car的坐标给放到向量里面来
                            pts_x.push_back(prev_car_x); pts_x.push_back(car_x); 
                            pts_y.push_back(prev_car_y); pts_y.push_back(car_y); 
                        }
                        // Use the previous path's end points as starting reference
                        // 用前一个path的end来作为参照系
                        else { 
                            // ref_x, y的下标是 - 1
                            // 下面要用到很多ref_x, ref_y
                            ref_x = previous_path_x[prev_size - 1]; 
                            ref_y = previous_path_y[prev_size - 1]; 

                            // ref_x, y的previous下标就是 - 2了
                            double ref_x_prev = previous_path_x[prev_size - 2]; 
                            double ref_y_prev = previous_path_y[prev_size - 2]; 
                            // yaw居然是这么来计算的
                            // y坐标，x坐标上的值的差值的商就是yaw
                            // 当前值减去前一个值的坐标值之商
                            ref_yaw = atan2(ref_y - ref_y_prev, ref_x - ref_x_prev); 
                            pts_x.push_back(ref_x_prev); pts_x.push_back(ref_x); 
                            pts_y.push_back(ref_y_prev); pts_y.push_back(ref_y); 
                        }
                        
                        // In Frent coordiantes, add evenly 30m spaced points ahead of the starting reference
                        // 在Frent坐标系里面，在开始参照系的前面加上30m的空间点
                        // 现在开始就要找waypoints了
                        vector<double> next_wp0 = frenet_to_cartesian(car_s + 30, (lane_width * lane + lane_width / 2), map_waypoints_s, map_waypoints_x, map_waypoints_y); 
                        vector<double> next_wp1 = frenet_to_cartesian(car_s + 60, (lane_width * lane + lane_width / 2), map_waypoints_s, map_waypoints_x, map_waypoints_y); 
                        vector<double> next_wp2 = frenet_to_cartesian(car_s + 90, (lane_width * lane + lane_width / 2), map_waypoints_s, map_waypoints_x, map_waypoints_y); 

                        pts_x.push_back(next_wp0[0]); pts_x.push_back(next_wp1[0]);pts_x.push_back(next_wp2[0]);  
                        // 我靠，这里有一个bug，这里pts_y.push_back，push的位置不对啊，应该是next_wp0[1]，我写成了, next_wp1[1], 靠，这种错误，真的是难找哦，结果就是导致下一个车子的位置不对了
                        // pts_y.push_back(next_wp1[1]); pts_y.push_back(next_wp1[1]);pts_y.push_back(next_wp2[1]);  
                        pts_y.push_back(next_wp0[1]); pts_y.push_back(next_wp1[1]);pts_y.push_back(next_wp2[1]);  

                        // Rototranslate into car's reference system to make the math easier
                        // 旋转翻译car的参照系统，这样数学原理会更加容易一些
                        for (size_t i = 0; i < pts_x.size(); ++i) {
                            double shift_x = pts_x[i] - ref_x; 
                            double shift_y = pts_y[i] - ref_y; 
                            pts_x[i] = shift_x * cos(0 - ref_yaw) - shift_y * sin(0 - ref_yaw); 
                            pts_y[i] = shift_x * sin(0 - ref_yaw) + shift_y * cos(0 - ref_yaw); 
                        }

                        // Create a spline
                        // 创建一个spline对象
                        tk::spline s; 
                        // 这个s可以产生一个平滑的曲线，变道的时候可以使用得上
                        s.set_points(pts_x, pts_y); 

                        // Define the actual (x, y) points will be used for the planner
                        // 定义实际要用于planner的点
                        vector<double> next_x_vals; 
                        vector<double> next_y_vals; 

                        // Start with all previous points from last time
                        // 从上一个time的全部previous points开始
                        for (size_t i = 0; i < previous_path_x.size(); ++i) {
                            // previous_path_x, y立马的数据也要给撞到next_x_vals, next_y_vals里面来
                            next_x_vals.push_back(previous_path_x[i]); 
                            next_y_vals.push_back(previous_path_y[i]); 
                        }

                        // Calculate how to break up spline points to travel at reference velocity
                        double target_x = 30.0; 
                        // 现在s是一个spline对象可以用（）进行构造
                        // 上面并没有再定义target_y而是这里直接用s()来生成的
                        // 奇怪的用法, 这里s()里面确实用的是target_y，而不是
                        // target_x
                        double target_y = s(target_y); 
                        // double target_y = s(target_x); 
                        // 计算target_x, target_y之间的dist
                        double target_dist = sqrt(target_x * target_x + target_y * target_y); 
                        double x_add_on = 0.0; 

                        // 原来这里也有一个bug，这里计数i是从1开始不是从0开始
                        // 难怪车子老是跑歪了
                        // for (size_t i = 0; i <= 50 - previous_path_x.size(); ++i) {
                        for (size_t i = 1; i <= 50 - previous_path_x.size(); ++i) {
                            double N       = target_dist / (0.02 * ref_vel / 2.24);
                            double x_point = x_add_on + target_x / N;
                            double y_point = s(x_point);  // 又是s进行构造

                            // 这个x_add_on要更新为x_point的新的值，所以x_add_on在不断的更新中
                            x_add_on       = x_point;

                            // x_ref, y_ref也要不断的进行更新
                            double x_ref   = x_point;
                            double y_ref   = y_point;

                            // Rotate back into previous coordinate system
                            x_point = x_ref * cos(ref_yaw) - y_ref * sin(ref_yaw); 
                            y_point = x_ref * sin(ref_yaw) + y_ref * cos(ref_yaw); 

                            // x_point, y_point要加上ref_x, ref_y, 而不是x_ref, y_ref
                            x_point += ref_x; 
                            y_point += ref_y; 
                            
                            // 把x_point, y_point给装到next_x_vals, next_y_vals的向量里面
                            // 所以，最后next_x_vals，next_y_vals这两个向量一次是装入两个值
                            // 一个是previous_path_x[i], 一次是x_point
                            next_x_vals.push_back(x_point); 
                            next_y_vals.push_back(y_point); 
                        }

                        // Prepare message and send it to the simulator
                        // 把上面算出来的信息给组织起来，并且发回模拟器里面去
                        // 就是要预测得到next的x，y坐标，把这些next值给传回到
                        // 模拟器里面，模拟器再模拟出车子的移动来
                        json msgJson; 
                        msgJson["next_x"] = next_x_vals; 
                        msgJson["next_y"] = next_y_vals; 
                        auto msg = "42[\"control\"," + msgJson.dump() + "]"; 
                        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT); }
                } else { 
                    // Manual driving
                    std::string msg = "42[\"manual\",{}]"; 
                    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT); 
                }
            }
    }); 


    // We don't need this since we're not using HTTP but if it's removed the program doesn't compile :-(
    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) { 
            const std::string s= "<h1>Hello world!/<h1>"; 
            if (req.getUrl().valueLength == 1) {
                res->end(s.data(), s.length()); 
            } else { 
                res->end(nullptr, 0); 
            }
        }); 


    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) { 
            std::cout << "Connected!!!" << std::endl; 
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) { 
            ws.close(); 
            std::cout << "Disconnected" << std::endl; 
    }); 

    int port = 4567; 
    if (h.listen(port)) {
        std::cout << "Listening to port " << port << std::endl; 
    } else { 
        std::cerr << "Failed to listen to port " << std::endl; 
        return  -1; 
    }
    h.run(); 
}
