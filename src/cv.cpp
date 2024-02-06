#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "turtlesim/TeleportAbsolute.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace ros;

Mat image;
ServiceClient teleport_client;

void resetTurtle()
{
    turtlesim::TeleportAbsolute teleport_srv;
    teleport_srv.request.x = 5.545;
    teleport_srv.request.y = 5.545;
    teleport_srv.request.theta = 0.0;
}

void poseCallback(const turtlesim::Pose::ConstPtr &msg)
{
    double turtle_x = static_cast<double>(msg->x);
    double turtle_y = static_cast<double>(msg->y);
    double minX1 = 60 / 500.0 * 11.09;
    double maxX1 = 220 / 500.0 * 11.09;
    double minY1 = 60 / 500.0 * 11.09;
    double maxY1 = 220 / 500.0 * 11.09;

    double minX2 = 60 / 500.0 * 11.09;
    double maxX2 = 440 / 500.0 * 11.09;
    double minY2 = 280 / 500.0 * 11.09;
    double maxY2 = 440 / 500.0 * 11.09;
    if (turtle_x >= 11.07 || turtle_x <= 0.1 || turtle_y >= 11.07 || turtle_y <= 0.1 || (minX1 <= turtle_x && turtle_x <= maxX1 && minY1 <= turtle_y && turtle_y <= maxY1) ||
    (minX2 <= turtle_x && turtle_x <= maxX2 && minY2 <= turtle_y && turtle_y <= maxY2))
    {
        resetTurtle();
        ROS_INFO("ERROR");
    }

    circle(image, Point(turtle_x / 11.09 * 500, 500 - turtle_y / 11.09 * 500), 3, Scalar(0, 255, 0), 1, LINE_AA);
    imshow("Turtle Track", image);
    waitKey(1);
}

int main(int argc, char **argv)
{
    init(argc, argv, "turtle_secondary");
    NodeHandle n;

    teleport_client = n.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");

    Subscriber sub = n.subscribe<turtlesim::Pose>("/turtle1/pose", 1000, poseCallback);

    image = imread("/home/ubuntu/iris/src/magang/src/arena_robot.jpg");

    spin();

    return 0;
}
