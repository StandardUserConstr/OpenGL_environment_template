#ifndef MAIN_PROGRAM_SECURE_H
#define MAIN_PROGRAM_SECURE_H

#include "Headers.h"
#include "UniversalTools.h"
#include "OpenGL_UniversalTools.h"
#include "ModelLoading.h"


class CoreClass;

namespace MAIN_PROGRAM
{
    namespace DEFAULT_VALUES
    {
        static constexpr char WINDOW_NAME[] = "Window";
        static constexpr uint32_t SCR_WIDTH = 640;
        static constexpr uint32_t SCR_HEIGHT = 480;
        static constexpr double MOUSE_SENSIVITY = 0.2;
        static constexpr uint32_t MOVEMENT_SPEED = 300; //higher == slower;
        static constexpr double SCROLL_SENSITIVITY = 2.0;
        static constexpr float SHIFT_MOVEMENTS = 0.250000;
        static constexpr glm::vec4 PROJECTION = glm::vec4(60.0,4.0/3.0,0.1,100.0);
        static constexpr glm::vec3 VIEW[3] =  {glm::vec3(0.0f, 0.0f, 5.0f),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0)};
    };

    class Events
    {

    public:
        typedef struct __variables
        {
            bool     key_w_pressed, key_s_pressed,
                     key_a_pressed, key_d_pressed,
                     key_x_pressed;

            bool     key_w_released, key_s_released,
                     key_a_released, key_d_released,
                     key_x_released;

            float shift_calculation_variable;
        }variables;

        Events();
    //returns updated variables by one of the function events like: "main_event_loop";
        const variables* const get_variables() const;
    //set variables to default values;
        void reset_variables();

    //  main event loops
//---------------------------------------------------------------------------------
    //returns 1 if program should be closed or error appeared;  //SDL BASED;
        bool main_program_loop(TOOLS::CameraClass* const camera_fps,double actual_fps);

    private:
        __variables _variables;
        void reset_variables_release();

    };

    //#define MainProgram_DebugInfo //for debuf info;
    class MainProgram
    {

    public:
        Events events;
        OpenGL_UniversalTools::OpenGL_Static2DManager* instruction_image;
        OpenGL_UniversalTools::OpenGL_FontManager* font;
        ModelLoading::Model* main_model;

        MainProgram(CoreClass* const core);
        ~MainProgram();

        //  main loops
    //----------------------------------------------------
        void Run_MainProgram(CoreClass* const core);
        void Draw_Models(CoreClass* const core);
        void Draw_StaticImages(CoreClass* const core,const Events::variables* const event);



    };
}

#endif
