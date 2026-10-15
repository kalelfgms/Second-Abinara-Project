//main func, put arm classes instantiated in arm.cpp here
#include <iostream>
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "../include/callbacks.hpp"
#include "simulation.hpp"

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

    opt.flags[mjVIS_CONTACTPOINT] = 1;
    opt.flags[mjVIS_CONTACTFORCE] = 1;

    //callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    
    //scene setup
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_100);

    //cam setup
    cam.orthographic = 1;
    cam.distance = 1.8;
    cam.azimuth = 45;
    cam.elevation = -25;

    //starting pose
    setPose(Pose::Ready);

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