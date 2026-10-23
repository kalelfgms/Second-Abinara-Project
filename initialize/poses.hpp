#include <GLFW/glfw3.h>

#include "../include/controls.hpp"

void addPose(vector<unique_ptr<Pose>>& PoseList, string n, int k, pList ori) {
    PoseList.push_back(make_unique<Pose>(n, k, ori));
}

void initializePoses(){
    addPose(PoseList, "Default", GLFW_KEY_SPACE, {0, -1, 2, 1.5, 0});
    addPose(PoseList, "Zig Zag", GLFW_KEY_C, {0, -1, 2, -2, 0});
    addPose(PoseList, "Zero", GLFW_KEY_V, {0, 0.05, -0.05, 0.05, 0});
    addPose(PoseList, "Max Length", GLFW_KEY_B, {0, -1.5, -1, 1, 0});
    
    addPose(PoseList, "Tri1", GLFW_KEY_1, {0, -0.7, 1.85, 0.4, 0});
    addPose(PoseList, "Tri2", GLFW_KEY_2, {0.3, -0.05, 2.1, 0, -0.5});
    addPose(PoseList, "Tri3", GLFW_KEY_3, {-0.3, -0.05, 2.1, 0, -0.5});
};