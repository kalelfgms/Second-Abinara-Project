//functions for controlling arms
#pragma once
#include <mujoco/mujoco.h>
#include "../src/simulation.hpp"
//make set Pose, make preset poses mapped to keyssz
//enum Poses {Zero, Ready, MaxLength, ZigZag};

class Pose {
    private:
        //store poses in array
        string name;
        pList orients = {0, 0, 0, 0};
        int key = 0;

    public:
        Pose(string n, int k, pList ori): name(n), key(k), orients(ori) {};

        pList getPose(){ return orients; };

        int getKey(){ return key; };
};

vector<unique_ptr<Pose>> PoseList;

void playPose(Pose pose){
    mjData* data = getData();
    pList orients = pose.getPose();

    for (int i = 0; i < 4; i++){
        data->ctrl[i] = orients[i];
    };
};

void moveJoint(int joint, double dir){
    //dir: left -1, right 1
    mjData* data = getData();
    const double step = dir*1; 
    double ctrlMin, ctrlMax, ctrlCurrent;
    ctrlMin = m->actuator_ctrlrange[2*joint];
    ctrlMax = m->actuator_ctrlrange[2*joint+1];
    //d->ctrl[joint] += step;
    const double vmax = 10;
    d->ctrl[joint] = clamp(
        d->ctrl[joint] += step * vmax * m->opt.timestep,
        ctrlMin, ctrlMax);
}