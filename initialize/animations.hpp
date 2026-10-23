#include <GLFW/glfw3.h>

#include "../include/controls.hpp"

//same  system as pose, but plays loop
//or anim loop di play dlm sim loop, pake flag, tp hrs dikasih interval dr pose to pose

void addAnimation(vector<unique_ptr<Animation>>& AnimList, string n, int k, vector<pList> keyf) {
    AnimList.push_back(make_unique<Animation>(n, k, keyf));
}

void initializeAnims(){
    addAnimation(AnimList, "Triangle", GLFW_KEY_T, vector<pList>{
        {0, -0.7, 1.85, 0.4, 0},
        {0.3, -0.05, 2.1, 0, -0.5},
        {-0.3, -0.05, 2.1, 0, -0.5}}
    );
};