//functions for controlling arms
#pragma once
#include <mujoco/mujoco.h>
#include "../src/simulation.hpp"
//make set Pose, make preset poses mapped to keyssz
//enum Poses {Zero, Ready, MaxLength, ZigZag};

//movement class
//pose and animation children, pose can be animation but with 1 keyframe, different method to play each thing
//make animation class use IK instead of preset keyframes, or js have both
//let IK make a set of end keyframes based on world position, those positions jd keyframe 
//ADD INVERSE KINEMATICS!!!!!!!!!!!!!!!!! (for kpp)

void moveJoint(int joint, double dir){
    //dir: left -1, right 1
    mjData* data = getData();
    const double step = dir*1; 
    double ctrlMin, ctrlMax, ctrlCurrent;
    ctrlMin = m->actuator_ctrlrange[2*joint];
    ctrlMax = m->actuator_ctrlrange[2*joint+1];
    
    //set state an statename
    setBotState(botState::Teleop, "-");

    const double vmax = 6; // max speed, biar gk pencet sekali tbtb sampe ujung
    d->ctrl[joint] = clamp(
        d->ctrl[joint] += step * vmax * m->opt.timestep,
        ctrlMin, ctrlMax);
}

//POSE
class Pose {
    private:
        //store poses in array
        string name;
        pList orients = {0, 0, 0, 0, 0};
        int key = 0;

    public:
        Pose(string n, int k, pList ori): name(n), key(k), orients(ori) {};

        pList getPose(){ return orients; };

        int getKey(){ return key; };
        string getName(){ return name; };
};

vector<unique_ptr<Pose>> PoseList;

void playPose(Pose pose){
    mjData* data = getData();
    pList orients = pose.getPose();

    //set state and statename
    setBotState(botState::Posed, pose.getName());

    for (int i = 0; i < 5; i++){
        data->ctrl[i] = orients[i];
    };
};

//IK

