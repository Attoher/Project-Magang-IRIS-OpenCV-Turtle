#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include <iostream>

using namespace ros;
using namespace std;
using namespace geometry_msgs;

#include <unistd.h>
#include <termios.h>

double turtle_x = 0.0;
double turtle_y = 0.0;
double turtle_kecepatan_linier = 0.0;
double turtle_kecepatan_sudut = 0.0;

void poseCallback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle_x = msg->x;
    turtle_y = msg->y;
    turtle_kecepatan_linier = msg->linear_velocity;
    turtle_kecepatan_sudut = msg->angular_velocity;

    ROS_INFO("\nKoordinat kura-kura: x=%lf, y=%lf\nKecepatan Linier : %lf \nKecepatan Sudut : %lf", turtle_x, turtle_y, turtle_kecepatan_linier, turtle_kecepatan_sudut);
}

char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

int main(int argc, char **argv)
{

    init(argc, argv, "turtle_controller");
    NodeHandle n;

    Subscriber sub = n.subscribe<turtlesim::Pose>("/turtle1/pose", 1000, poseCallback);
    Publisher pub = n.advertise<Twist>("/turtle1/cmd_vel", 1);

    while (ok())
    {
        Twist move;
        char inp = getch();

        switch (inp)
        {
        case 'a':
            move.angular.z = 0.5;
            break;
        case 'w':
            move.linear.x = 0.5;
            break;
        case 's':
            move.linear.x = -0.5;
            break;
        case 'd':
            move.angular.z = -0.5;
            break;
        default:
            break;
        }

        pub.publish(move);
        spinOnce();
    }

    return 0;
}
