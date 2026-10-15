//sim callbacks
#include <mujoco/mujoco.h>
#include "controls.hpp"
#include "../src/simulation.hpp"

static bool  button_left = false, button_middle = false, button_right = false;
static double lastx = 0, lasty = 0;

static void mouse_button_callback(GLFWwindow* window, int button, int act, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)   button_left   = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_left);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) button_middle = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_middle);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)  button_right  = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_right);

    glfwGetCursorPos(window, &lastx, &lasty);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    scn = getScene(); cam = getCam();
    if (!m) return;
    if (!button_left && !button_right && !button_middle) {
        lastx = xpos;
        lasty = ypos;
        return;
    }

    int fbw = 0, fbh = 1;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    if (fbh <= 0) fbh = 1;

    double dx = (xpos - lastx) / (double)fbh;
    double dy = (ypos - lasty) / (double)fbh;
    lastx = xpos;
    lasty = ypos;

    if (button_left) {
        mjv_moveCamera(m, mjMOUSE_ROTATE_H, dx, dy, &scn, &cam);
    } else if (button_right) {
        mjv_moveCamera(m, mjMOUSE_MOVE_H, dx, dy, &scn, &cam);
    } else if (button_middle) {
        mjv_moveCamera(m, mjMOUSE_ZOOM, dx, dy, &scn, &cam);
    }
}

static void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    scn = getScene(); cam = getCam();
    if (!m) return;
    mjv_moveCamera(m, mjMOUSE_ZOOM, 0, -0.05 * yoffset, &scn, &cam);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
    if (act != GLFW_PRESS) return;

    //masalah rn: kalo mentok rangenya (-2 or 2), nambahin value ctrl ga bakal gerakin..  
    switch (key){
        //torso yaw
        case GLFW_KEY_N:
            moveJoint(0, -1); cout<<"torso yaw: " << d->ctrl[0] << endl; break;
        case GLFW_KEY_M: 
            moveJoint(0, 1); cout<<"torso yaw: " << d->ctrl[0] << endl; break;
        //shoulder pitch
        case GLFW_KEY_Z: 
            moveJoint(1, -1); cout<<"shoulder pitch yaw: " << d->ctrl[1] << endl; break;
        case GLFW_KEY_X: 
            moveJoint(1, 1); cout<<"shoulder pitch yaw: " << d->ctrl[1] << endl; break;
        //upper arm pitch
        case GLFW_KEY_A: 
            moveJoint(2, -1); cout<<"upper arm yaw: " << d->ctrl[2] << endl; break;
        case GLFW_KEY_S: 
            moveJoint(2, 1); cout<<"upper arm yaw: " << d->ctrl[2] << endl; break;
        //forearm pitch
        case GLFW_KEY_Q: 
            moveJoint(3, -1); cout<<"forearm yaw: " << d->ctrl[3] << endl; break;
        case GLFW_KEY_W: 
            moveJoint(3, 1); cout<<"forearm yaw: " << d->ctrl[3] << endl; break;

        //posing
        default:
            for (auto& p : PoseList) {
                if (key == p->getKey()) playPose(*p);
            };
            break;
    }
}

void glfw_error_callback(int code, const char* desc){
    printf("error bruh");
}