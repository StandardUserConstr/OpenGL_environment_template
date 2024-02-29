#include "../include/tools0.h"

TOOLS0::Camera0::Camera0(glm::vec3 start_position,bool flying_or_walking,glm::vec4 default_projection)
{
    this->position = start_position;
    this->rotation = glm::vec3(0.0,0.0,0.0);
    this->up = glm::vec3(0.0,1.0,0.0);
    this->actual_y_angle = 0.0;
    this->actual_x_angle = -90.0;
    this->projection = default_projection;
    this->flying_or_walking = flying_or_walking;
    this->max_scroll = default_projection.x;
    this->mouse_sensitivity_scroll = 1.0;

    this->update_camera();

    return;
}

void TOOLS0::Camera0::process_keyboard(TOOLS0::Camera0::Camera_Movement direction,float movement_speed)
{
    switch(direction)
    {
        case FORWARD:
            {
                if(this->flying_or_walking==0) this->position += this->rotation*movement_speed;
                else
                {
                    this->position.x += this->rotation.x*movement_speed;
                    this->position.z += this->rotation.z*movement_speed;
                }
            }
        break;

        case BACKWARD:
            {
                if(this->flying_or_walking==0) this->position -= this->rotation*movement_speed;
                else
                {
                    this->position.x -= this->rotation.x*movement_speed;
                    this->position.z -= this->rotation.z*movement_speed;
                }
            }
        break;

        case LEFT:
            this->position += this->right*movement_speed;
        break;

        case RIGHT:
            this->position -= this->right*movement_speed;
        break;

    }
    return;
}

void TOOLS0::Camera0::process_mouse_movement(int32_t xoffset_pixels,int32_t yoffset_pixels,float mouse_sensitivity)
{
    this->actual_x_angle += (xoffset_pixels*mouse_sensitivity)*this->mouse_sensitivity_scroll;
    this->actual_y_angle += (yoffset_pixels*mouse_sensitivity)*this->mouse_sensitivity_scroll;

    if(this->actual_y_angle>89.0f) this->actual_y_angle = 89.0f;
    else if(this->actual_y_angle<-89.0f) this->actual_y_angle = -89.0f;

    this->update_camera();

    return;
}

void TOOLS0::Camera0::process_mouse_scroll(float yoffset,float scroll_sensitivity)
{
    this->projection.x -= yoffset*scroll_sensitivity;
    if(this->projection.x<this->min_scroll) this->projection.x = this->min_scroll;
    else if(this->projection.x>this->max_scroll) this->projection.x = this->max_scroll;

    this->mouse_sensitivity_scroll = this->projection.x/this->max_scroll;

    return;
}

glm::vec3 TOOLS0::Camera0::get_camera_position() const
{
    return position;
}

glm::vec3 TOOLS0::Camera0::get_camera_direction() const
{
    return rotation;
}

glm::mat4 TOOLS0::Camera0::get_view_matrix() const
{
    return glm::lookAt(this->position,this->position+this->rotation,this->up);
}

glm::mat4 TOOLS0::Camera0::get_projection_matrix() const
{
    return glm::perspective(glm::radians(this->projection.x),this->projection.y,this->projection.z,this->projection.w);
}

void TOOLS0::Camera0::update_camera()
{
    this->rotation.x = glm::cos(glm::radians(this->actual_x_angle))*glm::cos(glm::radians(this->actual_y_angle));
    this->rotation.y = glm::sin(glm::radians(this->actual_y_angle));
    this->rotation.z = glm::sin(glm::radians(this->actual_x_angle))*glm::cos(glm::radians(this->actual_y_angle));

    //this->rotation = glm::normalize(this->rotation);   //there is no need for "normalized()" func 'cause these operations will gives results that even after "normalization" variables will be the same;

    this->right = glm::normalize(glm::cross(this->rotation,this->const_up_for_calculations));  //or with a logical formula: glm::vec3(-glm::sin(glm::radians(this->actual_x_angle)),0,glm::cos(glm::radians(this->actual_x_angle))); //this variable must be normalize() after func cross() 'cause this func on that variable has "weird" but necessary effect;
    this->up = glm::cross(this->right,this->rotation);   //if u will not create this "up" variable and swap it with simple "const_up_for_calculations" it will work aswell (some examples did it this simple way);

    //normalize operation is:
        //  a.x = a.x/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
        //  a.y = a.y/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
        //  a.z = a.z/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);

    return;
}

//===================================================================================================================================================================================================================================

void TOOLS0::SDL_UniversalTimer0::set_internal_timer(uint32_t time_in_miliseconds)
{
    this->past_time = SDL_GetTicks();
    this->internal_timer = time_in_miliseconds;
    return;
}

int32_t TOOLS0::SDL_UniversalTimer0::run_timer()
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

//===================================================================================================================================================================================================================================
#if defined(WINDOWS_CODE)
    void TOOLS0::SDL_UniversalTools0::FpsControl_EndFrame()
    {
        static int64_t actual_time;
        actual_time = this->GetTicks();
        this->FpsControl_FpsTimerEnd = (actual_time-this->FpsControl_FpsTimerStart);
        if((10000000.0/this->FpsControl_fps)>this->FpsControl_FpsTimerEnd) {SDL_Delay(((10000000.0/this->FpsControl_fps)-this->FpsControl_FpsTimerEnd)/10000.0); this->actual_fps = this->FpsControl_fps;}
        else this->actual_fps = (10000000.0/this->FpsControl_FpsTimerEnd);
        //printf("%f\n",this->actual_fps);
        return;
    }

    void TOOLS0::SDL_UniversalTools0::FpsControl_StartFrame()
    {
        this->FpsControl_FpsTimerStart = this->GetTicks();
        return;
    }
#else
    void TOOLS0::SDL_UniversalTools0::FpsControl_EndFrame()
    {
        static uint32_t actual_time;
        actual_time = SDL_GetTicks();
            //if(actual_time<this->FpsControl_FpsTimerStart) this->FpsControl_FpsTimerStart = actual_time; //it's keeping an eye at SDL_GetTicks() 'cause uint32_t will overflow after 47 days from the start of the program and then possibly will frezze without this;
        this->FpsControl_FpsTimerEnd = (actual_time-this->FpsControl_FpsTimerStart);
        //printf("%u %u\n",this->FpsControl_FpsTimerEnd,(1000.0/this->FpsControl_fps)>this->FpsControl_FpsTimerEnd);
        if((1000.0/this->FpsControl_fps)>this->FpsControl_FpsTimerEnd) {SDL_Delay((1000.0/this->FpsControl_fps)-this->FpsControl_FpsTimerEnd); this->actual_fps = this->FpsControl_fps;}
        else this->actual_fps = (1000.0/this->FpsControl_FpsTimerEnd);
        //printf("%f\n",this->actual_fps);
        return;
    }

    void TOOLS0::SDL_UniversalTools0::FpsControl_StartFrame()
    {
        this->FpsControl_FpsTimerStart = SDL_GetTicks();
        return;
    }
#endif
void TOOLS0::SDL_UniversalTools0::change_fps(uint32_t fps)
{
    if(fps==0) fps = 4294967295;
    this->FpsControl_fps = fps;
    this->actual_fps = fps;
    return;
}

double TOOLS0::SDL_UniversalTools0::get_actual_fps() const
{
    return this->actual_fps;
}

uint32_t TOOLS0::SDL_UniversalTools0::get_expected_fps() const
{
    return this->FpsControl_fps;
}

void TOOLS0::SDL_UniversalTools0::ShowSimpleMessageBox0(Uint32 flag,const char* title,const char* message,const char* StringButton)
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
//===================================================================================================================================================================================================================================

int32_t TOOLS0::OpenGL_UniversalTools0::load_texture(const char* const filename)
{
    stbi_set_flip_vertically_on_load(1);

    int32_t width, height, nr_channels;
    uint32_t texture_id;
    glGenTextures(1,&texture_id);

    unsigned char* data = stbi_load(filename,&width,&height,&nr_channels,0);

    if(data)
    {
        GLenum format;
        if (nr_channels == 3) {format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}
        else if (nr_channels == 4) {format = GL_RGBA; glPixelStorei(GL_UNPACK_ALIGNMENT,4);}
        else if (nr_channels==2) {format = GL_RG; glPixelStorei(GL_UNPACK_ALIGNMENT,2);}
        else {format = GL_RED; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}

        glBindTexture(GL_TEXTURE_2D,texture_id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
        return texture_id;
    }
    else
    {
        fprintf(stderr,"Failed to load texture: %s\n",filename);
        glDeleteTextures(1,&texture_id);
        return -1;
    }
}

//===================================================================================================================================================================================================================================



void TOOLS0::UniversalTools0::uint32_to_string(unsigned int data_in,char* const data_out)
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


