#pragma once
#include <mujoco/mujoco.h>

mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

mjvCamera getCam(){ return cam; }
mjvScene getScene(){ return scn; }
mjData* getData(){ return d; }