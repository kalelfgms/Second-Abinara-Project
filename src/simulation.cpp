//main func, put arm classes instantiated in arm.cpp here
#include <iostream>
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

vector<double> q = {};

void mouse_button_callback(GLFWwindow* window, int button, int act, int mods){
    //button_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) = GLFW_PRESS);
}
//make set Pose, make preset poses mapped to keyssz
void startPos(){
    //radians
    d->ctrl[0] = 0;
    d->ctrl[1] = -1;
    d->ctrl[2] = 2;
    d->ctrl[3] = 1.4;
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
    if (act != GLFW_PRESS) return;

    const double step = 0.25;//3.14/20; // sesuai unit dlm range actuator, in this case 3.14
    switch (key){
        //torso yaw
        case GLFW_KEY_N: d->ctrl[0] -= step; cout<<"torso yaw: " << d->ctrl[0] << endl; break;
        case GLFW_KEY_M: d->ctrl[0] += step; cout<<"torso yaw: " << d->ctrl[0] << endl; break;
        //shoulder pitch
        case GLFW_KEY_Z: d->ctrl[1] -= step; cout<<"shoulder pitch yaw: " << d->ctrl[1] << endl; break;
        case GLFW_KEY_X: d->ctrl[1] += step; cout<<"shoulder pitch yaw: " << d->ctrl[1] << endl; break;
        //upper arm pitch
        case GLFW_KEY_A: d->ctrl[2] -= step; cout<<"upper arm yaw: " << d->ctrl[2] << endl; break;
        case GLFW_KEY_S: d->ctrl[2] += step; cout<<"upper arm yaw: " << d->ctrl[2] << endl; break;
        //forearm pitch
        case GLFW_KEY_Q: d->ctrl[3] -= step; cout<<"forearm yaw: " << d->ctrl[3] << endl; break;
        case GLFW_KEY_W: d->ctrl[3] += step; cout<<"forearm yaw: " << d->ctrl[3] << endl; break;
        //reset
        case GLFW_KEY_SPACE:
            cout<<"resetting position"<<endl;
            //mj_resetData(m, d);
            startPos();
            q = {0, 0, 0, 0}; break;
    }
}

void glfw_error_callback(int code, const char* desc){
    printf("error bruh");
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

    GLFWwindow* window = glfwCreateWindow(900, 1200, "arm sim", nullptr, nullptr);
    if (!window){
        fprintf(stderr, "glfwCreateWindow() gagal\n");
        glfwTerminate(); return 1;
    }

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyboard_callback);

    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_100);

    cam.type = mjCAMERA_FREE;
    cam.distance = 1.8;
    cam.azimuth = 45;
    cam.elevation = -25;

    startPos();

    while (!glfwWindowShouldClose(window)){
        mj_step(m, d);
        
        //printf("Joint angles: "); for (double a : q) printf("%.3f ", a*180.0/M_PI); printf("\n");
    


        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        mjv_updateScene(m, d, &opt, nullptr, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mjv_freeScene(&scn);
    mjr_freeContext(&con);
    mj_deleteData(d);
    mj_deleteModel(m);
    glfwTerminate();

    return 0;
}