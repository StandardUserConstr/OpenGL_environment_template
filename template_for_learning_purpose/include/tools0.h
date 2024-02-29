#ifndef TOOLS0_SECURE_H
#define TOOLS0_SECURE_H

#include "headers.h"

namespace TOOLS0
{
//#include <glm/glm.hpp>
    class Camera0
    {
    public:
        enum Camera_Movement{FORWARD = 0,BACKWARD = 1,LEFT = 2,RIGHT = 3};

        bool flying_or_walking;
        float max_scroll;
        float min_scroll = 2.0;

        Camera0(glm::vec3 start_position = glm::vec3(0.0f, 0.0f, -3.0f),bool flying_or_walking = 0,
                glm::vec4 default_projection = glm::vec4(60.0,4.0/3.0,0.1,100.0));

        void process_keyboard(TOOLS0::Camera0::Camera_Movement direction,float movement_speed = 0.2f);
        void process_mouse_movement(int32_t xoffset_pixels,int32_t yoffset_pixels,float mouse_sensitivity = 0.2);
        void process_mouse_scroll(float yoffset,float scroll_sensitivity = 2.0);

        glm::vec3 get_camera_position() const;
        glm::vec3 get_camera_direction() const;
        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;

    protected:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 up;
        glm::vec3 right;
        float actual_x_angle;
        float actual_y_angle;
        glm::vec4 projection;
        float mouse_sensitivity_scroll;

    private:
        const glm::vec3 const_up_for_calculations = glm::vec3(0.0,1.0,0.0);

        void update_camera();

    };

//#include <stdint.h>
//#include <SDL2/SDL.h>
    class SDL_UniversalTimer0
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
    class SDL_UniversalTools0
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

    public:
        SDL_UniversalTools0(uint32_t fps = 60) : FpsControl_fps(fps),actual_fps(fps){return;}

        void FpsControl_EndFrame();     //add at the end of loop;
        void FpsControl_StartFrame();   //add at the the start of loop;
        void change_fps(uint32_t fps);
    //the variable "actual_fps" will update after call of the function "FpsControl_EndFrame()";
        double get_actual_fps() const;
        uint32_t get_expected_fps() const;

        TOOLS0::SDL_UniversalTimer0 timer0;

        //flag:
            //SDL_MESSAGEBOX_ERROR
            //SDL_MESSAGEBOX_WARNING
            //SDL_MESSAGEBOX_INFORMATION
        void ShowSimpleMessageBox0(Uint32 flag,const char* title,const char* message,const char* StringButton);


        #if defined(WINDOWS_CODE)
            int64_t GetTicks()  //this will return nanoseconds instead of seconds but it's only under windows2000+;
            {
                static LARGE_INTEGER ticks;
                if(!QueryPerformanceCounter(&ticks))
                {
                    fprintf(stderr,"function \"TOOLS0::SDL_UniversalTools0::GetTicks()\" has been failed: if(!QueryPerformanceCounter(&ticks))");
                    ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Fatal Error","function \"TOOLS0::SDL_UniversalTools0::GetTicks()\" has been failed:\nif(!QueryPerformanceCounter(&ticks))","ok");
                    exit(-1);
                }
                return ticks.QuadPart;
            }
        #endif
    };

    //<stdint.h>
    //<stdio.h>
    //"stb_image.h"
    //OpenGL include functions like: <GL/glew.h>
    class OpenGL_UniversalTools0
    {
    public:
        //returns -1 if error; else retunrs id of the texture;
        // use func "glActiveTexture" before calling the function;
        int32_t load_texture(const char* const filename);

    };

    class UniversalTools0
    {
    public:
        //adds '\0' at the end of data_out
        void uint32_to_string(unsigned int data_in,char* const data_out);
    };






    //#include <stdlib.h>
    /*template <typename T> class custom_vector0
    {

        T* main_data;
        size_t element_count;
        size_t size_of_obiekt;

    public:
        custom_vector0()
        {
            this->main_data = (T*)malloc(sizeof(T)*4);
            this->element_count = 0;
            this->size_of_obiekt = sizeof(T)*4;
        }

        void clear()
        {
            this->element_count = 0;
            this->size_of_obiekt = sizeof(T)*4;
            free(this->main_data);
            this->main_data = (T*)malloc(sizeof(T)*4);
            return;
        }

        void push_back(T variable)
        {
            this->element_count++;
            if(this->element_count%4==0)
            {
                T* copy_data = (T*)malloc((sizeof(T)*(this->element_count*2)));
                size_t i;
                for(i = 0; i!=this->element_count-1; i++) copy_data[i] = this->main_data[i];


                free(this->main_data);
                this->main_data = (T*)malloc((sizeof(T)*(this->element_count*2)));
                for(i = 0; i!=this->element_count-1; i++) this->main_data[i] = copy_data[i];
                free(copy_data);

                this->size_of_obiekt = (sizeof(T)*(this->element_count*2));
            }
             this->main_data[this->element_count-1] = variable;
        }

        void pop_back()
        {
            if(this->element_count==0) return;
            this->main_data[this->element_count-1] = (T)NULL;
            this->element_count--;
            return;
        }

        size_t size() const
        {
            return this->element_count;
        }

        size_t real_size_in_bytes() const
        {
            return this->size_of_obiekt;
        }

        size_t size_of_elements_in_bytes() const
        {
            return this->element_count*sizeof(T);
        }

        bool empty()
        {
            return 1 ? this->element_count == 0 : 0;
        }

        T& operator[](const size_t iterator) const
        {
            return this->main_data[iterator];
        }

        ~custom_vector0()
        {
            free(this->main_data);
        }

    };
}*/
}



#endif
