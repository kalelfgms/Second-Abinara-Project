#pragma once
#include <mujoco/mujoco.h>
#include <vector>
#include <array>
#include <string>
#include <memory>

using namespace std;

typedef array<double, 4> pList;

mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

mjvCamera getCam(){ return cam; }
mjvScene getScene(){ return scn; }
mjData* getData(){ return d; }