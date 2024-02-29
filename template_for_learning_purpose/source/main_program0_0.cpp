#include "../include/main_program0.h"

bool MAIN_PROGRAM0::main_event_loop0(TOOLS0::Camera0* const camera_fps0,main_event_loop0_variables* const variables,double actual_fps)
{
    double calculation_variable;
    calculation_variable = 2880.0/(MAIN_PROGRAM0::DEFAULT_VALUES::MOVEMENT_SPEED*actual_fps);

    variables->uint32_actual_fps = actual_fps;

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
                    if(SDL_GetRelativeMouseMode()==1) camera_fps0->process_mouse_movement(event.motion.xrel,-event.motion.yrel,MAIN_PROGRAM0::DEFAULT_VALUES::MOUSE_SENSIVITY);
                }
            break;

            case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            {
                                variables->key_w_pressed = 1;
                            }
                        break;

                        case SDLK_s:
                            {
                                variables->key_s_pressed = 1;
                            }
                        break;

                        case SDLK_d:
                            {
                                variables->key_d_pressed = 1;
                            }
                        break;

                        case SDLK_a:
                            {
                                variables->key_a_pressed = 1;
                            }
                        break;

                        case SDLK_LSHIFT:
                            {
                                variables->shift_calculation_variable = MAIN_PROGRAM0::DEFAULT_VALUES::SHIFT_MOVEMENTS;
                            }
                        break;

                        case SDLK_x:
                            {
                                variables->key_x_pressed = 1;
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


            case SDL_KEYUP:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            {
                                if(variables->key_w_pressed==1) camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::FORWARD,calculation_variable*variables->shift_calculation_variable);
                                variables->key_w_pressed = 0;
                                //this and others "if's" are necessary 'cause there may be a moment where u very fast click and release button and then this solution shines;
                                    //more control over movements will be gained 'cause of this;
                            }
                        break;

                        case SDLK_s:
                            {
                                if(variables->key_s_pressed==1) camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::BACKWARD,calculation_variable*variables->shift_calculation_variable);
                                variables->key_s_pressed = 0;
                            }
                        break;

                        case SDLK_d:
                            {
                                if(variables->key_d_pressed==1) camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::LEFT,calculation_variable*variables->shift_calculation_variable);
                                variables->key_d_pressed = 0;
                            }
                        break;

                        case SDLK_a:
                            {
                                if(variables->key_a_pressed==1) camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::RIGHT,calculation_variable*variables->shift_calculation_variable);
                                variables->key_a_pressed = 0;
                            }
                        break;

                        case SDLK_LSHIFT:
                            {
                                variables->shift_calculation_variable = 1.000000;
                            }
                        break;

                        case SDLK_x:
                            {
                                variables->key_x_pressed = 0;
                            }
                        break;
                    }
                    break;
                }
            break;

            case SDL_MOUSEWHEEL:
                {
                    if(SDL_GetRelativeMouseMode()==1) camera_fps0->process_mouse_scroll(event.wheel.y,MAIN_PROGRAM0::DEFAULT_VALUES::SCROLL_SENSITIVITY);
                }
            break;

        }
    }


    if(variables->key_w_pressed!=0) {camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::FORWARD,calculation_variable*variables->shift_calculation_variable); variables->key_w_pressed = 2;}
    if(variables->key_s_pressed!=0) {camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::BACKWARD,calculation_variable*variables->shift_calculation_variable); variables->key_s_pressed = 2;}
    if(variables->key_d_pressed!=0) {camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::LEFT,calculation_variable*variables->shift_calculation_variable); variables->key_d_pressed = 2;}
    if(variables->key_a_pressed!=0) {camera_fps0->process_keyboard(TOOLS0::Camera0::Camera_Movement::RIGHT,calculation_variable*variables->shift_calculation_variable); variables->key_a_pressed = 2;}
    return 0;
}

