#pragma once
#include <mujoco/mujoco.h>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

typedef array<double, 5> pList;

mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

mjvCamera getCam(){ return cam; }
mjvScene getScene(){ return scn; }
mjData* getData(){ return d; }

void print(string s, bool newl = true){
    cout<<s;
    if (newl) {cout<<endl;}
}

void wait(int s){
    this_thread::sleep_for(chrono::milliseconds(s));
}

enum botState {
    Teleop, Posed, Animated, InverseK
};
const string stateNames[] = {
    "Teleop", "Posed", "Animated", "InverseK"
};

//bot states 
string STATE = stateNames[botState::Teleop];
string STATENAME = "-";

void setBotState(botState st, string stn){
    STATE = stateNames[st];
    STATENAME = stn;
};

void displayBotData(){;
    //Da Arm (botname)
    //Current State: (pose, teleop, anim) 
    //Pose/Animation Name: -/posename/animname (- when teleop)

    //<Joint Values>
    //Torso Yaw       : 0.0
    //Shoulder Pitch  : 0.0
    //Upper arm Pitch : 0.0
    //Forearm Pitch   : 0.0
    //Hand Yaw        : 0.0

    print("_____________________________________________________________");
    print("<za hando>");
    print("Current State   : ", false); print(STATE);
    print("State Name      : ", false); print(STATENAME);

    print("<Joint Values>");
    print("Torso Yaw       : ", false); printf("%.2f \n", d->ctrl[0]);
    print("Shoulder Pitch  : ", false); printf("%.2f \n", d->ctrl[1]);
    print("Upper arm Pitch : ", false); printf("%.2f \n", d->ctrl[2]);
    print("Forearm Pitch   : ", false); printf("%.2f \n", d->ctrl[3]);
    print("Hand Yaw        : ", false); printf("%.2f \n", d->ctrl[4]);

    //sensor site pos and vel
    //site is end of obj, utk calc IK
    print("________________________________");
    print("<Site Pos and LinVel>");
    printf("pos = %.2f %.2f %.2f \n", d->sensordata[0], d->sensordata[1], d->sensordata[2]); // x y z site coords
    //printf("vel = %.2f %.2f %.2f \n", d->sensordata[3], d->sensordata[4], d->sensordata[5]); // x y z site velocity components

    //jacobiansz
    double jacp[15] = {0}; // 3*5 jacobian matrix, 5 DoF
    double jacr[15] = {0}; // 3*5 jacobian rotation matrix
    double point[3] = {d->sensordata[0], d->sensordata[1], d->sensordata[2]}; // xyz of site
    int body = 6; // which body endeff site is (hand)
    mj_jac(m, d, jacp, jacr, point, body); // jacobian calculations 

    print("________________________________");
    print("<Jacobian Position>");
    printf("%.2f %.2f %.2f %.2f %.2f \n", jacp[0], jacp[1], jacp[2], jacp[3], jacp[4]); // torso, shoulder, upperarm, forearm, hand
    printf("%.2f %.2f %.2f %.2f %.2f \n", jacp[5], jacp[6], jacp[7], jacp[8], jacp[9]);
    printf("%.2f %.2f %.2f %.2f %.2f \n", jacp[10], jacp[11], jacp[12], jacp[13], jacp[14]);
    //3*5 matrix

    //J only needs everything except hand joint
    double R[10] = {jacr[0], jacr[1], jacr[2], jacr[3], jacr[4], jacr[5], jacr[6], jacr[7], jacr[8], jacr[9]}; //only account for yaw and pitch in final J matrix
    double qdot[5] = {d->qvel[0], d->qvel[1], d->qvel[2], d->qvel[3], d->qvel[5]}; //velocity of each DoF
    double xdot[5] = {0}; //J*qdot
    //J*qdot is jacobian
    //mju_mulMatTVec(xdot, J, qdot, 3, 4); 

    printf("vel jacobi = %.2f %.2f %.2f \n", xdot[0], xdot[2], xdot[1]); // for some reason 2 and 1 ketuker dibanding vel sensor
    printf("vel sensor = %.2f %.2f %.2f \n", d->sensordata[3], d->sensordata[4], d->sensordata[5]);

    //IK
    //dq = Jinv*dx
    //q -> q+dq or q += dq
    //ctrl = q

    //Jinv, better to use in a diff lib, steps:
    //find det of J -> determinant of jacp, 3*5 matrix??

    //SHIT MEM DUMPING HERE IDK WHY!!! CUZ TADI UBAH DR 3x5 to 3x4 MATRIX, cuz ofjacp array size, for some reason min 14
    //ternyata jacobian tetep 15 cuz robot punya 5 joint, jd ya gbs ganti walopun gk dipake


};