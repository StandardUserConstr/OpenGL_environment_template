#ifndef MAIN_PROGRAM0_SECURE_H
#define MAIN_PROGRAM0_SECURE_H

#include "headers.h"
#include "tools0.h"

namespace MAIN_PROGRAM0
{
    namespace DEFAULT_VALUES
    {
        constexpr uint32_t SCR_WIDTH = 640;
        constexpr uint32_t SCR_HEIGHT = 480;
        constexpr double MOUSE_SENSIVITY = 0.2;
        constexpr uint32_t MOVEMENT_SPEED = 300; //higher == slower;
        constexpr double SCROLL_SENSITIVITY = 2.0;
        constexpr float SHIFT_MOVEMENTS = 0.250000;
        constexpr glm::vec4 PROJECTION = glm::vec4(60.0,4.0/3.0,0.1,100.0);
        constexpr glm::vec3 VIEW[3] =  {glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0)};
    };

    struct main_event_loop0_variables
    {
        uint32_t key_w_pressed = 0,key_s_pressed = 0,
                 key_a_pressed = 0,key_d_pressed = 0,
                 key_x_pressed = 0,uint32_actual_fps = 0;

        float shift_calculation_variable = 1.00000;
    };

//returns 1 if program should be closed or error appeared;  //SDL BASED;
    bool main_event_loop0(TOOLS0::Camera0* const camera_fps0,main_event_loop0_variables* const variables,double actual_fps);
}



#endif
