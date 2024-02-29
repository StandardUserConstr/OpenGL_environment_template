#include "../include/UniversalTools.h"

namespace TOOLS
{
    CameraClass::CameraClass(glm::vec3 start_position,bool flying_or_walking,glm::vec4 default_projection)
    {
        this->priv_var.position = start_position;
        this->priv_var.rotation = glm::vec3(0.0,0.0,0.0);
        this->priv_var.up = glm::vec3(0.0,1.0,0.0);
        this->priv_var.actual_y_angle = 0.0;
        this->priv_var.actual_x_angle = -90.0;
        this->priv_var.projection = default_projection;
        this->priv_var.flying_or_walking = flying_or_walking;
        this->priv_var.max_scroll = default_projection.x;
        this->priv_var.mouse_sensitivity_scroll = 1.0;

        this->update_camera();

        return;
    }

    void CameraClass::process_keyboard(TOOLS::CameraClass::Camera_Movement direction,float movement_speed)
    {
        switch(direction)
        {
            case FORWARD:
                {
                    if(this->priv_var.flying_or_walking==0) this->priv_var.position += this->priv_var.rotation*movement_speed;
                    else
                    {
                        this->priv_var.position.x += this->priv_var.rotation.x*movement_speed;
                        this->priv_var.position.z += this->priv_var.rotation.z*movement_speed;
                    }
                }
            break;

            case BACKWARD:
                {
                    if(this->priv_var.flying_or_walking==0) this->priv_var.position -= this->priv_var.rotation*movement_speed;
                    else
                    {
                        this->priv_var.position.x -= this->priv_var.rotation.x*movement_speed;
                        this->priv_var.position.z -= this->priv_var.rotation.z*movement_speed;
                    }
                }
            break;

            case RIGHT:
                this->priv_var.position += this->priv_var.right*movement_speed;
            break;

            case LEFT:
                this->priv_var.position -= this->priv_var.right*movement_speed;
            break;

        }
        return;
    }

    void CameraClass::process_mouse_movement(int32_t xoffset_pixels,int32_t yoffset_pixels,float mouse_sensitivity)
    {
        this->priv_var.actual_x_angle += (xoffset_pixels*mouse_sensitivity)*this->priv_var.mouse_sensitivity_scroll;
        this->priv_var.actual_y_angle += (yoffset_pixels*mouse_sensitivity)*this->priv_var.mouse_sensitivity_scroll;

        if(this->priv_var.actual_y_angle>89.0f) this->priv_var.actual_y_angle = 89.0f;
        else if(this->priv_var.actual_y_angle<-89.0f) this->priv_var.actual_y_angle = -89.0f;

        this->update_camera();

        return;
    }

    void CameraClass::process_mouse_scroll(float yoffset,float scroll_sensitivity)
    {
        this->priv_var.projection.x -= yoffset*scroll_sensitivity;
        if(this->priv_var.projection.x<this->priv_var.min_scroll) this->priv_var.projection.x = this->priv_var.min_scroll;
        else if(this->priv_var.projection.x>this->priv_var.max_scroll) this->priv_var.projection.x = this->priv_var.max_scroll;

        this->priv_var.mouse_sensitivity_scroll = this->priv_var.projection.x/this->priv_var.max_scroll;

        return;
    }

    glm::vec3 CameraClass::get_camera_position() const
    {
        return this->priv_var.position;
    }

    glm::vec3 CameraClass::get_camera_direction() const
    {
        return this->priv_var.rotation;
    }

    glm::mat4 CameraClass::get_view_matrix() const
    {
        return glm::lookAt(this->priv_var.position,this->priv_var.position+this->priv_var.rotation,this->priv_var.up);
    }

    glm::mat4 CameraClass::get_projection_matrix() const
    {
        return glm::perspective(glm::radians(this->priv_var.projection.x),this->priv_var.projection.y,this->priv_var.projection.z,this->priv_var.projection.w);
    }

    void CameraClass::change_camera_mode(bool flying_or_walking)
    {
        this->priv_var.flying_or_walking = flying_or_walking;
        return;
    }

    void CameraClass::change_max_zoom(float max_zoom)
    {
        if(max_zoom<=this->priv_var.max_scroll) this->priv_var.min_scroll = max_zoom;
        else this->priv_var.min_scroll = this->priv_var.max_scroll;
        return;
    }

    void CameraClass::change_min_zoom(float min_zoom)
    {
        if(min_zoom>=this->priv_var.min_scroll) this->priv_var.max_scroll = min_zoom;
        else this->priv_var.max_scroll = this->priv_var.min_scroll;
        return;
    }

    void CameraClass::change_actual_zoom(float actual_zoom)
    {
        this->priv_var.projection.x = actual_zoom;
        return;
    }

    void CameraClass::update_camera()
    {
        this->priv_var.rotation.x = glm::cos(glm::radians(this->priv_var.actual_x_angle))*glm::cos(glm::radians(this->priv_var.actual_y_angle));
        this->priv_var.rotation.y = glm::sin(glm::radians(this->priv_var.actual_y_angle));
        this->priv_var.rotation.z = glm::sin(glm::radians(this->priv_var.actual_x_angle))*glm::cos(glm::radians(this->priv_var.actual_y_angle));

        //this->rotation = glm::normalize(this->rotation);   //there is no need for "normalized()" func 'cause these operations will gives results that even after "normalization" variables will be the same;

        this->priv_var.right = glm::normalize(glm::cross(this->priv_var.rotation,this->priv_var.const_up_for_calculations));  //or with a logical formula: glm::vec3(-glm::sin(glm::radians(this->actual_x_angle)),0,glm::cos(glm::radians(this->actual_x_angle))); //this variable must be normalize() after func cross() 'cause this func on that variable has "weird" but necessary effect;
        this->priv_var.up = glm::cross(this->priv_var.right,this->priv_var.rotation);   //if u will not create this "up" variable and swap it with simple "const_up_for_calculations" it will work aswell (some examples did it this simple way);

        //normalize operation is:
            //  a.x = a.x/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
            //  a.y = a.y/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
            //  a.z = a.z/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);

        return;
    }

//============================================================================================================================================================================================================================================================================================


    void TOOLS::SDL_UniversalTimerClass::set_internal_timer(uint32_t time_in_miliseconds)
    {
        this->past_time = SDL_GetTicks();
        this->internal_timer = time_in_miliseconds;
        return;
    }

    int32_t TOOLS::SDL_UniversalTimerClass::run_timer()
    {
        if(this->internal_timer==0) return -1;
        this->actual_time_calc_buffor = SDL_GetTicks();
        if((this->actual_time_calc_buffor-this->past_time)>=this->internal_timer)
        {
            this->past_time = this->actual_time_calc_buffor-((this->actual_time_calc_buffor-this->past_time)%this->internal_timer);
            return 1;
        }
        return 0;
    }

//============================================================================================================================================================================================================================================================================================

    SDL_UniversalToolsClass::SDL_UniversalToolsClass(uint32_t fps)
    {
        priv_var.FpsControl_fps = fps;
        priv_var.actual_fps = fps;
        return;
    }

    #if defined(WINDOWS_CODE)
        void SDL_UniversalToolsClass::FpsControl_EndFrame()
        {
            static int64_t actual_time;
            actual_time = this->GetTicks();
            this->priv_var.FpsControl_FpsTimerEnd = (actual_time-this->priv_var.FpsControl_FpsTimerStart);
            if((10000000.0/this->priv_var.FpsControl_fps)>this->priv_var.FpsControl_FpsTimerEnd) {SDL_Delay(((10000000.0/this->priv_var.FpsControl_fps)-this->priv_var.FpsControl_FpsTimerEnd)/10000.0); this->priv_var.actual_fps = this->priv_var.FpsControl_fps;}
            else this->priv_var.actual_fps = (10000000.0/this->priv_var.FpsControl_FpsTimerEnd);
            return;
        }

        void SDL_UniversalToolsClass::FpsControl_StartFrame()
        {
            this->priv_var.FpsControl_FpsTimerStart = this->GetTicks();
            return;
        }
    #else
        void SDL_UniversalToolsClass::FpsControl_EndFrame()
        {
            static uint32_t actual_time;
            actual_time = SDL_GetTicks();
                //if(actual_time<this->priv_var.FpsControl_FpsTimerStart) this->priv_var.FpsControl_FpsTimerStart = actual_time; //it's keeping an eye at SDL_GetTicks() 'cause uint32_t will overflow after 47 days from the start of the program and then possibly will frezze without this;
            this->priv_var.FpsControl_FpsTimerEnd = (actual_time-this->priv_var.FpsControl_FpsTimerStart);
            if((1000.0/this->priv_var.FpsControl_fps)>this->priv_var.FpsControl_FpsTimerEnd) {SDL_Delay((1000.0/this->priv_var.FpsControl_fps)-this->priv_var.FpsControl_FpsTimerEnd); this->priv_var.actual_fps = this->priv_var.FpsControl_fps;}
            else this->priv_var.actual_fps = (1000.0/this->priv_var.FpsControl_FpsTimerEnd);
            return;
        }

        void SDL_UniversalToolsClass::FpsControl_StartFrame()
        {
            this->priv_var.FpsControl_FpsTimerStart = SDL_GetTicks();
            return;
        }
    #endif

    void SDL_UniversalToolsClass::change_fps(uint32_t fps)
    {
        if(fps==0) fps = 4294967295;
        this->priv_var.FpsControl_fps = fps;
        this->priv_var.actual_fps = fps;
        return;
    }

    double SDL_UniversalToolsClass::get_actual_fps() const
    {
        return this->priv_var.actual_fps;
    }

    uint32_t SDL_UniversalToolsClass::get_expected_fps() const
    {
        return this->priv_var.FpsControl_fps;
    }

    void SDL_UniversalToolsClass::ShowSimpleMessageBox(Uint32 flag,const char* title,const char* message,const char* StringButton)
    {
        SDL_MessageBoxButtonData button = {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,0,StringButton};
        SDL_MessageBoxData MsgData =
        {
            flag,
            NULL,
            title,
            message,
            1,
            &button,
            NULL
        };
        int buttonid;
        SDL_ShowMessageBox(&MsgData, &buttonid);
        return;
    }

#if defined(WINDOWS_CODE)
    int64_t SDL_UniversalToolsClass::GetTicks()
    {
        static LARGE_INTEGER ticks;
        if(!QueryPerformanceCounter(&ticks))
        {
            fprintf(stderr,"function \"TOOLS::SDL_UniversalToolsClass::GetTicks()\" has been failed: if(!QueryPerformanceCounter(&ticks))");
            ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Fatal Error","function \"TOOLS::SDL_UniversalToolsClass::GetTicks()\" has been failed:\nif(!QueryPerformanceCounter(&ticks))","ok");
            exit(-1);
        }
        return ticks.QuadPart;
    }
#endif


    //  FUNCTION TOOLS
//============================================================================================================================================================================================================================================================================================


    void uint32_to_string(unsigned int data_in,char* const data_out)
    {
        char data_rev[10];
        for(int i = 0;; i++)
        {
            if(data_in < 10)
            {
                data_rev[i] = data_in+48;
                data_out[i+1] = '\0';
                for(int j = 0; j!=i+1; j++) data_out[j] = data_rev[i-j];
                return;
            }
            data_rev[i] = (data_in%10)+48;
            data_in = (data_in-(data_in%10))/10;
        }
        return;
    }
}
