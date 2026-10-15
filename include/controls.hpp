//functions for controlling arms
#include <mujoco/mujoco.h>
#include "../src/simulation.hpp"

//make set Pose, make preset poses mapped to keyssz
enum Pose {Zero, Ready, MaxLength, ZigZag};

void setPose(Pose pose){
    //in radians
    mjData* data = getData();
    switch (pose){
        case Pose::Ready: // Space
            data->ctrl[0] = 0;
            data->ctrl[1] = -1;
            data->ctrl[2] = 2;
            data->ctrl[3] = 1.4;
            break;
        case Pose::MaxLength: // B
            data->ctrl[0] = 0;
            data->ctrl[1] = -1.5;
            data->ctrl[2] = -1;
            data->ctrl[3] = 1;
            break;
        case Pose::ZigZag: // C
            data->ctrl[0] = 0;
            data->ctrl[1] = -1;
            data->ctrl[2] = 2;
            data->ctrl[3] = -2;
            break;
        case Pose::Zero: // V
            data->ctrl[0] = 0.05;
            data->ctrl[1] = 0.05;
            data->ctrl[2] = 0.05;
            data->ctrl[3] = 0.05;
            break;
        default: //jaga jaga
            data->ctrl[0] = 0.05;
            data->ctrl[1] = 0.05;
            data->ctrl[2] = 0.05;
            data->ctrl[3] = 0.05;
            break;
    }
}

void moveJoint(int joint, double dir){
    //joints from bottom to top, 0, 1, 2, 3
    //dir: left -1, right 1
    mjData* data = getData();
    const double step = dir*0.25; //3.14/20; // sesuai unit dlm range actuator, in this case 3.14
    d->ctrl[joint] += step;
}