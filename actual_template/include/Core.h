#ifndef CORE_SECURE_H
#define CORE_SECURE_H

#include "Headers.h"
#include "MainProgram.h"
#include "UniversalTools.h"

//#define Core_DebugInfo //for debug info;
class CoreClass
{
    struct core_variables
    {
        SDL_Window* window;
        SDL_GLContext contextGL;
        GLuint OpenGL_Static2DManager_ProgramID,
                OpenGL_FontManager_ProgramID,
                ModelLoading_ProgramID;
    };

public:
    CoreClass(bool printf_information);
    ~CoreClass();

    core_variables variables;

//tools:
    TOOLS::CameraClass* camera;
    TOOLS::SDL_UniversalToolsClass* sdl_universal_tool;

private:
//returns 0 if no error has been occured; returns 1 of is an error;
    bool initialization();
//returns 0 if no error has been occured; returns 1 of is an error;
    bool shaders_initialization();
    void print_information();

};

#endif
