#ifndef CORE0_SECURE_H
#define CORE0_SECURE_H


#include "headers.h"
#include "OpenGL_UniversalTools.h"
#include "tools0.h"
#include "main_program0.h"
#include "model_mesh_test.h"



class Initialization_OpenGL_SDL;
class Data;
class Shader_Programs;
class Core0;

class Initialization_OpenGL_SDL
{
public:
    SDL_Window* _window;
    SDL_GLContext _glcontext;
    TOOLS0::SDL_UniversalTools0* sdl_universaltool0;
    TOOLS0::OpenGL_UniversalTools0* opengl_universaltool0;

    ~Initialization_OpenGL_SDL();

protected:
    Initialization_OpenGL_SDL();

private:
    void print_init_information() const;

};

class Data : public Initialization_OpenGL_SDL
{
public:
    struct Main_Data
    {
        std::vector<float*> data;
        std::vector<uint32_t*> indices;

        std::vector<uint32_t> VBO,VAO,EBO;
        std::vector<uint32_t> sizeof_data,sizeof_indices;
        std::vector<GLenum> data_enum,indices_enum;

        uint32_t program_id;
        std::vector<glm::mat4> model;

    };
    std::vector<uint32_t> texture;

    Main_Data program0_data;
    Main_Data program1_data;
    Main_Data program2_data;
    Main_Data program3_data;
    Main_Data program4_data;

    ~Data();

protected:
    Data();

private:
    void setup_objects_and_main_data();
    void setup_and_load_textures();
    void setup_VAOs();

};

class Shader_Programs : public Data
{
public:
    void setup_uniforms();

    uint32_t OpenGL_FontManager_program_id;
    uint32_t OpenGL_Static2DManager_program_id;

    ~Shader_Programs();

protected:
    Shader_Programs();
};

class Core0 final : public Shader_Programs
{
public:
    Core0();

//test programs:
    void program0_test0();
    void program1_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables);
    void program2_3_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables);
    void program4_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables);

//tools:
    TOOLS0::Camera0 camera_fps0;
    TOOLS0::UniversalTools0 universal_tool;
    OpenGL_UniversalTools::OpenGL_Static2DManager* static_image0;
    OpenGL_UniversalTools::OpenGL_FontManager* font0;

    ~Core0();

private:


};


#endif
