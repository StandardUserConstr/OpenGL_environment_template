#ifndef UNIVERSAL_TOOLS_SECURE_H
#define UNIVERSAL_TOOLS_SECURE_H

#include "Headers.h"

namespace TOOLS
{

//#include <glm/glm.hpp>
    class CameraClass
    {
    public:
        enum Camera_Movement{FORWARD = 0,BACKWARD = 1,LEFT = 2,RIGHT = 3};

        CameraClass(glm::vec3 start_position = glm::vec3(0.0f, 0.0f, -3.0f),bool flying_or_walking = 0,
                glm::vec4 default_projection = glm::vec4(60.0,4.0/3.0,0.1,100.0));

        void process_keyboard(TOOLS::CameraClass::Camera_Movement direction,float movement_speed = 0.2f);
        void process_mouse_movement(int32_t xoffset_pixels,int32_t yoffset_pixels,float mouse_sensitivity = 0.2);
        void process_mouse_scroll(float yoffset,float scroll_sensitivity = 2.0);

        glm::vec3 get_camera_position() const;
        glm::vec3 get_camera_direction() const;
        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;

        void change_camera_mode(bool flying_or_walking);
    // lower number == bigger max zoom;
        void change_max_zoom(float max_zoom);
    // bigger number == bigger min zoom;
        void change_min_zoom(float min_zoom);
        void change_actual_zoom(float actual_zoom);

    protected:
        struct private_variables
        {
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 up;
            glm::vec3 right;
            float actual_x_angle;
            float actual_y_angle;
            glm::vec4 projection;
            float mouse_sensitivity_scroll;

            bool flying_or_walking;
            float max_scroll;
            float min_scroll = 2.0;

            const glm::vec3 const_up_for_calculations = glm::vec3(0.0,1.0,0.0);
        }priv_var;


    private:
        void update_camera();

    };



//#include <stdint.h>
//#include <SDL2/SDL.h>
    class SDL_UniversalTimerClass
    {
        uint32_t internal_timer = 0;
        uint32_t past_time;
        uint32_t actual_time_calc_buffor;
    public:

        void set_internal_timer(uint32_t time_in_miliseconds);
    //returns -1 if error (first u have to run the function "set_internal_timer()" with variable greater than 0);
    //returns 0 if internal timer is still running;
    //returns 1 if internal timer is done and has been restarted after this function call;
        int32_t run_timer();

    };



//#include <stdint.h>
//#include <SDL2/SDL.h>
    class SDL_UniversalToolsClass
    {
        struct private_variables
        {
            #if defined(WINDOWS_CODE)
                int64_t FpsControl_FpsTimerEnd;
                int64_t FpsControl_FpsTimerStart;
            #else
                uint32_t FpsControl_FpsTimerEnd;
                uint32_t FpsControl_FpsTimerStart;
            #endif

            uint32_t FpsControl_fps;
            double actual_fps;
        }priv_var;


    public:
        SDL_UniversalToolsClass(uint32_t fps = 60);

        TOOLS::SDL_UniversalTimerClass timer;

        void FpsControl_EndFrame();     //add at the end of loop;
        void FpsControl_StartFrame();   //add at the the start of loop;
        void change_fps(uint32_t fps);
    //the variable "actual_fps" will update after call of the function "FpsControl_EndFrame()";
        double get_actual_fps() const;
        uint32_t get_expected_fps() const;

        //flag:
            //SDL_MESSAGEBOX_ERROR
            //SDL_MESSAGEBOX_WARNING
            //SDL_MESSAGEBOX_INFORMATION
        void ShowSimpleMessageBox(Uint32 flag,const char* title,const char* message,const char* StringButton);


        #if defined(WINDOWS_CODE)
            int64_t GetTicks();  //this will return nanoseconds instead of seconds but it's only under windows2000+;
        #endif
    };


    //  FUNCTION TOOLS
//===========================================================================================================================================================================

//adds '\0' at the end of data_out
    void uint32_to_string(unsigned int data_in,char* const data_out);
}


#endif
