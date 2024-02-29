#include "../include/MainProgram.h"
#include "../include/Core.h"

namespace MAIN_PROGRAM
{
    Events::Events()
    {
        this->reset_variables();
        return;
    }
    const MAIN_PROGRAM::Events::variables* const Events::get_variables() const
    {
        return &this->_variables;
    }

    void Events::reset_variables()
    {
        this->_variables.key_w_pressed = 0; this->_variables.key_s_pressed = 0;
        this->_variables.key_a_pressed = 0; this->_variables.key_d_pressed = 0;
        this->_variables.key_x_pressed = 0;

        this->_variables.key_w_released = 0; this->_variables.key_s_released = 0;
        this->_variables.key_a_released = 0; this->_variables.key_d_released = 0;
        this->_variables.key_x_released = 0;

        this->_variables.shift_calculation_variable = 1.00000;
        return;
    }

    bool Events::main_program_loop(TOOLS::CameraClass* const camera_fps,double actual_fps)
    {
        double calculation_variable;
        calculation_variable = 2880.0/(MAIN_PROGRAM::DEFAULT_VALUES::MOVEMENT_SPEED*actual_fps);

        this->reset_variables_release();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
                case SDL_QUIT:
                    {

                        return 1;
                    }
                break;

                case SDL_WINDOWEVENT:
                    if(event.window.event==SDL_WINDOWEVENT_RESIZED)
                    {
                        glViewport(0,0,event.window.data1,event.window.data2);
                    }
                break;

                case SDL_MOUSEMOTION:
                    {
                        if(SDL_GetRelativeMouseMode()==1) camera_fps->process_mouse_movement(event.motion.xrel,-event.motion.yrel,MAIN_PROGRAM::DEFAULT_VALUES::MOUSE_SENSIVITY);
                    }
                break;

                case SDL_KEYDOWN:
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_w:
                                {
                                    this->_variables.key_w_pressed = 1;
                                }
                            break;

                            case SDLK_s:
                                {
                                    this->_variables.key_s_pressed = 1;
                                }
                            break;

                            case SDLK_d:
                                {
                                    this->_variables.key_d_pressed = 1;
                                }
                            break;

                            case SDLK_a:
                                {
                                    this->_variables.key_a_pressed = 1;
                                }
                            break;

                            case SDLK_LSHIFT:
                                {
                                    this->_variables.shift_calculation_variable = MAIN_PROGRAM::DEFAULT_VALUES::SHIFT_MOVEMENTS;
                                }
                            break;

                            case SDLK_x:
                                {
                                    this->_variables.key_x_pressed = 1;
                                }
                            break;

                            case SDLK_ESCAPE:
                                {
                                    if(SDL_GetRelativeMouseMode()==1) SDL_SetRelativeMouseMode(SDL_FALSE);
                                }
                            break;

                        }
                        break;
                    }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    {
                        if(SDL_GetRelativeMouseMode()==0) SDL_SetRelativeMouseMode(SDL_TRUE);
                    }
                break;

                case SDL_MOUSEBUTTONUP:
                    {
                        if(SDL_GetRelativeMouseMode()==0) SDL_SetRelativeMouseMode(SDL_TRUE);
                    }
                break;


                case SDL_KEYUP:
                    {
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_w:
                                {
                                    if(this->_variables.key_w_pressed==0) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::FORWARD,calculation_variable*this->_variables.shift_calculation_variable);
                                    //this and others "if's" are necessary 'cause there may be a moment where u very fast click and release button and then this solution shines;
                                        //more control over movements will be gained 'cause of this;
                                    this->_variables.key_w_released = 1;
                                    this->_variables.key_w_pressed = 0;
                                }
                            break;

                            case SDLK_s:
                                {
                                    if(this->_variables.key_s_pressed==0) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::BACKWARD,calculation_variable*this->_variables.shift_calculation_variable);
                                    this->_variables.key_s_released = 1;
                                    this->_variables.key_s_pressed = 0;
                                }
                            break;

                            case SDLK_d:
                                {
                                    if(this->_variables.key_d_pressed==0) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::RIGHT,calculation_variable*this->_variables.shift_calculation_variable);
                                    this->_variables.key_d_released = 1;
                                    this->_variables.key_d_pressed = 0;
                                }
                            break;

                            case SDLK_a:
                                {
                                    if(this->_variables.key_a_pressed==0) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::LEFT,calculation_variable*this->_variables.shift_calculation_variable);
                                    this->_variables.key_a_released = 1;
                                    this->_variables.key_a_pressed = 0;
                                }
                            break;

                            case SDLK_LSHIFT:
                                {
                                    this->_variables.shift_calculation_variable = 1.000000;
                                }
                            break;

                            case SDLK_x:
                                {
                                    this->_variables.key_x_pressed = 0;
                                    this->_variables.key_x_released = 1;
                                }
                            break;

                            case SDLK_ESCAPE:
                                {
                                    if(SDL_GetRelativeMouseMode()==1) SDL_SetRelativeMouseMode(SDL_FALSE);
                                }
                            break;
                        }
                        break;
                    }
                break;

                case SDL_MOUSEWHEEL:
                    {
                        if(SDL_GetRelativeMouseMode()==1) camera_fps->process_mouse_scroll(event.wheel.y,MAIN_PROGRAM::DEFAULT_VALUES::SCROLL_SENSITIVITY);
                    }
                break;

            }
        }

        if(this->_variables.key_w_pressed==1) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::FORWARD,calculation_variable*this->_variables.shift_calculation_variable);
        if(this->_variables.key_s_pressed==1) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::BACKWARD,calculation_variable*this->_variables.shift_calculation_variable);
        if(this->_variables.key_d_pressed==1) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::RIGHT,calculation_variable*this->_variables.shift_calculation_variable);
        if(this->_variables.key_a_pressed==1) camera_fps->process_keyboard(TOOLS::CameraClass::Camera_Movement::LEFT,calculation_variable*this->_variables.shift_calculation_variable);

        return 0;
    }

    void Events::reset_variables_release()
    {
        this->_variables.key_w_released = 0; this->_variables.key_s_released = 0;
        this->_variables.key_a_released = 0; this->_variables.key_d_released = 0;
        this->_variables.key_x_released = 0;
        return;
    }

    //============================================================================================================================================================================================================================================================================================


    MainProgram::MainProgram(CoreClass* const core)
    {
        const char* instruction_img =                  "files/images/instruction.png";
        const char* font_path =                        "files/fonts/font.ttf";
        const char* model_path =                       "files/models/model0/Winged_Victory_model.obj";



        #ifdef MainProgram_DebugInfo
            printf("loading image: %s... ",instruction_img);
        #endif
        this->instruction_image = new OpenGL_UniversalTools::OpenGL_Static2DManager(core->variables.OpenGL_Static2DManager_ProgramID,instruction_img,MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT);
        #ifdef MainProgram_DebugInfo
            printf("DONE\n");
        #endif

        #ifdef MainProgram_DebugInfo
            printf("loading font: %s... ",font_path);
        #endif
        this->font = new OpenGL_UniversalTools::OpenGL_FontManager(font_path,core->variables.OpenGL_FontManager_ProgramID,MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT,32);
        #ifdef MainProgram_DebugInfo
            printf("DONE\n");
        #endif

        #ifdef MainProgram_DebugInfo
            printf("loading model: %s... ",model_path);
        #endif
        this->main_model = new ModelLoading::Model(model_path);
        #ifdef MainProgram_DebugInfo
            printf("DONE\n");
        #endif


        // shader options for model_loading;
//===================================================================================================================================================
        glUseProgram(core->variables.ModelLoading_ProgramID);

        // directional light
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"dir_light.direction"),-0.2,-1.0,-0.3);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"dir_light.ambient"),50.03,50.03,50.03);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"dir_light.diffuse"),0.992*0.001,0.984*0.001,0.827*0.001);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"dir_light.specular"),0.5,0.5,0.5);

        // point lights
        uint32_t allocate_point_lights = 0;
        glUniform1i(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"NR_OF_POINT_LIGHTS"),allocate_point_lights);

        // spotLight
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.ambient"),0.0,0.0,0.0);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.diffuse"),0.988*1.25,0.925*1.25,0.682*1.25);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.specular"),0.7,0.7,0.7);
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.constant"),1.0);
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.linear"),0.027);
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.quadratic"),0.0028);
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.cutOff"),glm::cos(glm::radians(0.0f)));
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.outerCutOff"),glm::cos(glm::radians(90.0f)));

        // material
        glUniform1f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"material.shininess"),32.0f);


        core->sdl_universal_tool->change_fps(144);
        glClearColor(0.50f, 0.50f, 0.5f, 1.0f);
        core->sdl_universal_tool->timer.set_internal_timer(1000/60); // per 16.6 second do something;

        #ifdef MainProgram_DebugInfo
            printf("MainProgram is running;\n\n");
        #endif

        return;
    }

    MainProgram::~MainProgram()
    {
        delete this->font;
        delete this->instruction_image;
        delete this->main_model;
        return;
    }

    void MainProgram::Run_MainProgram(CoreClass* const core)
    {
        for(;;)
        {
            core->sdl_universal_tool->FpsControl_StartFrame();                                                                  // START FRAME;

            bool exit_program = this->events.main_program_loop(core->camera,core->sdl_universal_tool->get_actual_fps());        // EVENTS
            if(exit_program==1) break;                                                                                          //
            const Events::variables* event = this->events.get_variables();                                                      //

            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);                                                                   // CLEAR DRAW
            Draw_Models(core);                                                                                                  // DRAW
            Draw_StaticImages(core,event);                                                                                      //
            SDL_GL_SwapWindow(core->variables.window);                                                                          // DISPLAY DRAW

            core->sdl_universal_tool->FpsControl_EndFrame();                                                                    // END FRAME;
        }
        #ifdef MainProgram_DebugInfo
            printf("exited from the MainProgram;\n");
        #endif
        return;
    }

    void MainProgram::Draw_Models(CoreClass* const core)
    {
        glUseProgram(core->variables.ModelLoading_ProgramID); // for settings uniforms;

        static int32_t camera_location = glGetUniformLocation(core->variables.ModelLoading_ProgramID,"camera");
        static int32_t model_location = glGetUniformLocation(core->variables.ModelLoading_ProgramID,"model");
        static int32_t projection_location = glGetUniformLocation(core->variables.ModelLoading_ProgramID,"projection");

        glm::vec3 camera_pos = core->camera->get_camera_position();
        glm::vec3 camera_dir = core->camera->get_camera_direction();
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"CameraPos"),camera_pos.x,camera_pos.y,camera_pos.z);

        glUniformMatrix4fv(camera_location,1,GL_FALSE,glm::value_ptr(core->camera->get_view_matrix()));
        glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(core->camera->get_projection_matrix()));

        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.position"),camera_pos.x,camera_pos.y,camera_pos.z);
        glUniform3f(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"spot_light.direction"),camera_dir.x,camera_dir.y,camera_dir.z);

        // animation
    //----------------------------------------------------------------------------------------------------------------------------------------------------------
        glm::mat4 model = glm::mat4(1.0);
        model = glm::scale(model,glm::vec3(0.065));
        model = glm::translate(model,glm::vec3(50.0,-105.0,-405.0));
        static float angle = 0.0;
        if(core->sdl_universal_tool->timer.run_timer()==1) angle+=0.5; // per 16.6 seconds (1000/60) do angle+=0.5;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 normalized_model = glm::transpose(glm::inverse(model));
        glUniformMatrix3fv(glGetUniformLocation(core->variables.ModelLoading_ProgramID,"normal_model"),1,GL_FALSE,glm::value_ptr(normalized_model));
        glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(model));
    //----------------------------------------------------------------------------------------------------------------------------------------------------------

        this->main_model->Draw(core->variables.ModelLoading_ProgramID);

        return;
    }

    void MainProgram::Draw_StaticImages(CoreClass* const core,const Events::variables* const event)
    {

        static bool disable_image = 0;
        if(disable_image==0)
        {
            if(event->key_x_pressed==1||event->key_x_released==1)
            {
                disable_image = 1;
                return;
            }
            this->instruction_image->draw(0,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT-152,259,152,1.0,1.0);
        }


        static char data[11];
        uint32_t actual_fps = (uint32_t)core->sdl_universal_tool->get_actual_fps();
        TOOLS::uint32_to_string(actual_fps,data);

        glm::vec4 actual_text_color;
        if(actual_fps>=60) actual_text_color = glm::vec4(0.15, 1.0, 0.15, 1.0);
        else if(actual_fps>=30) actual_text_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        else actual_text_color = glm::vec4(1.0, 0.15, 0.15, 1.0);

        this->font->draw("FPS: ",MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH-75,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,glm::vec4(0.0, 0.0, 0.0, 1.0));
        this->font->draw(data,MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH-75+32,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,actual_text_color);

        return;
    }
}

