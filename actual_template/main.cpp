#define STB_IMAGE_IMPLEMENTATION  // define this variable to put implementation of "stb_image" library into single header file "stb_image.h";
#include "include/Core.h"



int main(int argc,char *argv[])
{
    CoreClass core(true);
    MAIN_PROGRAM::MainProgram main_program(&core);

    main_program.Run_MainProgram(&core);

    return 0;
}
