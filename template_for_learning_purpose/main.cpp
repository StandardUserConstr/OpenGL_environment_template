#define STB_IMAGE_IMPLEMENTATION  //define this variable to put implementation of "stb_image" library into single header file "stb_image.h";
#include "include/core0.h"

int start_settings(uint32_t choose,Core0& core);
void program_loop(uint32_t choose,Core0& core,MAIN_PROGRAM0::main_event_loop0_variables& event_variables);

int main(int argc, char *argv[])
{
    uint32_t choose = 0;
    if(argc<2)
    {
        printf_error("give number argument from 0 to ???;\nexample: program_name.exe 1");
        return -1;
    }
    choose = argv[1][0]-47-1;

    Core0 core;
    MAIN_PROGRAM0::main_event_loop0_variables event_variables;

    if(start_settings(choose,core)!=0) return -1;
    program_loop(choose,core,event_variables);

    SDL_Quit();
    return 0;
}







void program_loop(uint32_t choose,Core0& core,MAIN_PROGRAM0::main_event_loop0_variables& event_variables)
{
    while(1)
	{
	    core.sdl_universaltool0->FpsControl_StartFrame();
	    if(MAIN_PROGRAM0::main_event_loop0(&core.camera_fps0,&event_variables,core.sdl_universaltool0->get_actual_fps())==1) break;

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        switch(choose)
        {
            case 0: core.program0_test0();
            break;

            case 1: core.program1_test0(event_variables);
            break;

            case 2: core.program2_3_test0(event_variables);
            break;

            case 3: core.program4_test0(event_variables);
            break;

            default: break;

        }

		SDL_GL_SwapWindow(core._window);

		core.sdl_universaltool0->FpsControl_EndFrame();
	}
    return;
}

int start_settings(uint32_t choose,Core0& core)
{
    switch(choose)
    {
        case 0:
            {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                core.sdl_universaltool0->change_fps(65);
                core.sdl_universaltool0->timer0.set_internal_timer(1000/60); //set timer for program0_test0();
                //uncomment this call to draw in wireframe polygons.
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        break;

        case 1:
            {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                core.sdl_universaltool0->change_fps(0);
                core.sdl_universaltool0->timer0.set_internal_timer(1000/30); //set timer for program1_test0();
            }
        break;

        case 2:
            {
                core.sdl_universaltool0->change_fps(60);
                core.sdl_universaltool0->timer0.set_internal_timer(1000/30); //set timer for program2_3_test0();
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            }
        break;

        case 3:
            {
                core.sdl_universaltool0->change_fps(60);
                glClearColor(0.51f, 0.51f, 0.5f, 1.0f);
            }
        break;

        default: printf("invalid number argument;"); return -1;

    }
    return 0;
}

