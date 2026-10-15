//main func, put arm classes instantiated in arm.cpp here
#include <iostream>
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../include/controls.hpp"
#include "../include/callbacks.hpp"
#include "simulation.hpp"

void addPose(vector<unique_ptr<Pose>>& PoseList, string n, int k, pList ori) {
    PoseList.push_back(make_unique<Pose>(n, k, ori));
}

int main(){
    char error[1000] = "Couldn't load model";
    m = mj_loadXML("../robot/arm.xml", nullptr, error, sizeof(error));
    if (!m) {
        fprintf(stderr, "%s\n", error); return 1;
    }
    d = mj_makeData(m);

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "glfwInit() gagal\n");
        return 1;
    }

    //biar nyala windownya
    GLFWwindow* window = glfwCreateWindow(900, 1200, "arm sim", nullptr, nullptr);
    if (!window){
        fprintf(stderr, "glfwCreateWindow() gagal\n");
        glfwTerminate(); return 1;
    }
    glfwMakeContextCurrent(window);

    //callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    
    //scene setup
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_100);

    //cam setup
    mjv_defaultCamera(&cam);
    cam.lookat[0] = 0.0;
    cam.lookat[1] = 0.0;
    cam.lookat[2] = 0.2;    
    cam.orthographic = 1;
    cam.distance = 1.8;
    cam.azimuth = 45;
    cam.elevation = -25;

    //create poses
    addPose(PoseList, "Default", GLFW_KEY_SPACE, pList{0, -1, 2, 1.5});
    addPose(PoseList, "ZigZag", GLFW_KEY_C, {0, -1, 2, -2});
    addPose(PoseList, "Zero", GLFW_KEY_V, {0, 0.05, -0.05, 0.05});
    addPose(PoseList, "MaxLength", GLFW_KEY_B, {0, -1.5, -1, 1});
    //harus inget index tiap pose Class di file lain... kalo mau manually set position sih

    //starting pose
    playPose(*PoseList[0]);

    //sim loop
    while (!glfwWindowShouldClose(window)){
        mj_step(m, d);

        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        mjv_updateScene(m, d, &opt, nullptr, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //beres beres
    mjv_freeScene(&scn);
    mjr_freeContext(&con);
    mj_deleteData(d);
    mj_deleteModel(m);
    glfwTerminate();

    return 0;
}