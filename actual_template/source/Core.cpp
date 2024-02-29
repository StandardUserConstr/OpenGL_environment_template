#include "../include/Core.h"

CoreClass::CoreClass(bool printf_information)
{
    if(this->initialization()!=0) exit(1);
    if(printf_information==1) this->print_information();
    if(this->shaders_initialization()!=0) exit(1);
    return;
}

CoreClass::~CoreClass()
{
    delete this->camera;
    delete this->sdl_universal_tool;
    glDeleteProgram(this->variables.OpenGL_Static2DManager_ProgramID);
    glDeleteProgram(this->variables.OpenGL_FontManager_ProgramID);
    glDeleteProgram(this->variables.ModelLoading_ProgramID);
    SDL_GL_DeleteContext(this->variables.contextGL);
    SDL_DestroyWindow(this->variables.window);
    SDL_Quit();
    return;
}

bool CoreClass::initialization()
{
    int error;

    error = SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_EVENTS);
    if(error!=0)
    {
        printf_error_args("SDL2 initialization error: %s\n    file: %s | line: %d\n",SDL_GetError(),__FILE__,__LINE__);
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);                                        // to run "SDL_GL_SwapWindow()" (moving the back buffer to front buffer);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    this->sdl_universal_tool = new TOOLS::SDL_UniversalToolsClass();
    this->camera = new TOOLS::CameraClass(MAIN_PROGRAM::DEFAULT_VALUES::VIEW[0],0,MAIN_PROGRAM::DEFAULT_VALUES::PROJECTION);

    this->variables.window = SDL_CreateWindow(MAIN_PROGRAM::DEFAULT_VALUES::WINDOW_NAME,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT,
                                     SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
    if (this->variables.window==NULL)
    {
        printf_error_args("Failed to create SDL window;\n    file: %s | line: %d;\n",__FILE__,__LINE__);
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Failed to create SDL window;","Ok");
        exit(1);
    }
    this->variables.contextGL = SDL_GL_CreateContext(this->variables.window);

// Create the OpenGL context for the window using SDL
// creating context initializing opengl and is required; without it,opengl functions will not work along with glew initialization later and etc;
// function SDL_GL_CreateContext() returns some sort of context id which is required later when u want to destroy context;
    this->variables.contextGL = SDL_GL_CreateContext(this->variables.window);

// Enable glew experimental, this enables some more OpenGL extensions if available;
	glewExperimental = GL_TRUE;

// after create context now i can init glew (first create context,next init glew);
    GLenum err = glewInit();
    if (err!=GLEW_OK)
    {
        printf_error_args("Glew Error: %s\n%s (%d)",glewGetErrorString(err),__FILE__,__LINE__);
        char error_buffor[512];
        sprintf_s(error_buffor,512,"Glew Error: %s;\n%s (%d)",glewGetErrorString(err),__FILE__,__LINE__);
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error_buffor,"Ok");
        exit(1);
    }
    if (!GLEW_VERSION_1_1)
    {
        printf_error("ERROR: glew version is not equal or above GLEW_VERSION_1_1;\n");
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","glew version is not equal or above GLEW_VERSION_1_1;","Ok");
        exit(1);
    }

    //set viewport as base coordinates for vertex (when you try to draw some figures on screen: {0.5,0,-1.0} etc are coordinates that depends on base coordinates from viewport);
    glViewport(0,0,MAIN_PROGRAM::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM::DEFAULT_VALUES::SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

bool CoreClass::shaders_initialization()
{
    const char* VS_OpenGL_Static2DManager_shader = "files/shader_programs/VS_OpenGL_Static2DManager.glsl";
    const char* FS_OpenGL_Static2DManager_shader = "files/shader_programs/FS_OpenGL_Static2DManager.glsl";
    const char* VS_OpenGL_FontManager_shader =     "files/shader_programs/VS_OpenGL_FontManager.glsl";
    const char* FS_OpenGL_FontManager_shader =     "files/shader_programs/FS_OpenGL_FontManager.glsl";
    const char* VS_model_loading =                 "files/shader_programs/VS_ModelLoading.glsl";
    const char* FS_model_loading =                 "files/shader_programs/FS_ModelLoading.glsl";

    #ifdef Core_DebugInfo
        printf("Compiling program shaders: %s & %s... ",VS_OpenGL_Static2DManager_shader,FS_OpenGL_Static2DManager_shader);
    #endif
    this->variables.OpenGL_Static2DManager_ProgramID = OpenGL_UniversalTools::load_and_compile_shader_programs(VS_OpenGL_Static2DManager_shader,FS_OpenGL_Static2DManager_shader);
    if(this->variables.OpenGL_Static2DManager_ProgramID==0)
    {
        printf_error_args("MAIN_PROGRAM::shaders_initialization::load_and_compile_shader_programs: OpenGL_Static2DManager error;\n    file: %s | line: %d\n",__FILE__,__LINE__);
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","OpenGL_Static2DManager error:\nCoreClass::shaders_initialization::load_and_compile_shader_programs;","Ok");
        return 1;
    }
    #ifdef Core_DebugInfo
        printf("DONE\n");
    #endif

    #ifdef Core_DebugInfo
        printf("Compiling program shaders: %s & %s... ",VS_OpenGL_FontManager_shader,FS_OpenGL_FontManager_shader);
    #endif
    this->variables.OpenGL_FontManager_ProgramID = OpenGL_UniversalTools::load_and_compile_shader_programs(VS_OpenGL_FontManager_shader,FS_OpenGL_FontManager_shader);
    if(this->variables.OpenGL_FontManager_ProgramID==0)
    {
        printf_error_args("MAIN_PROGRAM::shaders_initialization::load_and_compile_shader_programs: OpenGL_FontManager error;\n    file: %s | line: %d\n",__FILE__,__LINE__);
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","OpenGL_FontManager error:\nCoreClass::shaders_initialization::load_and_compile_shader_programs;","Ok");
        return 1;
    }
    #ifdef Core_DebugInfo
        printf("DONE\n");
    #endif

    #ifdef Core_DebugInfo
        printf("Compiling program shaders: %s & %s... ",VS_model_loading,FS_model_loading);
    #endif
    this->variables.ModelLoading_ProgramID = OpenGL_UniversalTools::load_and_compile_shader_programs(VS_model_loading,FS_model_loading);
    if(this->variables.ModelLoading_ProgramID==0)
    {
        printf_error_args("CoreClass::shaders_initialization::load_and_compile_shader_programs: shaders_initialization error;\n    file: %s | line: %d\n",__FILE__,__LINE__);
        this->sdl_universal_tool->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","shaders_initialization error:\nCoreClass::shaders_initialization::load_and_compile_shader_programs;","Ok");
        return 1;
    }
    #ifdef Core_DebugInfo
        printf("DONE\n\n");
    #endif

    return 0;
}

void CoreClass::print_information()
{
    printf("version of OpenGL: %s\n",glGetString(GL_VERSION));

    printf("\n");
    return;
}
