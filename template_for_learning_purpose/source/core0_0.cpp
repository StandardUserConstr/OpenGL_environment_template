#include "../include/core0.h"

Initialization_OpenGL_SDL::Initialization_OpenGL_SDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    this->sdl_universaltool0 = new TOOLS0::SDL_UniversalTools0(60);
    this->opengl_universaltool0 = new TOOLS0::OpenGL_UniversalTools0();
    this->_window = SDL_CreateWindow("Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT,
                                     SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
    if (this->_window == NULL)
    {
        printf_error_args("Failed to create SDL window;\n%s (%d);",__FILE__,__LINE__);
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","Failed to create SDL window;","Ok");
        exit(1);
    }

// Create the OpenGL context for the window using SDL
// creating context initializing opengl and is required; without it,opengl functions will not work along with glew initialization later and etc;
// function SDL_GL_CreateContext() returns some sort of context id which is required later when u want to destroy context;
    this->_glcontext = SDL_GL_CreateContext(_window);

// Enable glew experimental, this enables some more OpenGL extensions if available;
	glewExperimental = GL_TRUE;

// after create context now i can init glew (first create context,next init glew);
    GLenum err = glewInit();
    if (err!=GLEW_OK)
    {
        printf_error_args("Glew Error: %s\n%s (%d)",glewGetErrorString(err),__FILE__,__LINE__);
        char error_buffor[512];
        sprintf_s(error_buffor,512,"Glew Error: %s;\n%s (%d)",glewGetErrorString(err),__FILE__,__LINE__);
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error",error_buffor,"Ok");
        exit(1);
    }
    if (!GLEW_VERSION_1_1)
    {
        printf_error("ERROR: glew version is not equal or above GLEW_VERSION_1_1;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","glew version is not equal or above GLEW_VERSION_1_1;","Ok");
        exit(1);
    }

    this->print_init_information();

    return;
}

void Initialization_OpenGL_SDL::print_init_information() const
{
    int attributes;

    printf("version of OpenGL: %s\n",glGetString(GL_VERSION));

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes);
    printf("maximum nr of vertex attributes supported: %d\n",attributes);

    return;
}

Initialization_OpenGL_SDL::~Initialization_OpenGL_SDL()
{
    delete this->opengl_universaltool0;
    delete this->sdl_universaltool0;
    SDL_GL_DeleteContext(this->_glcontext);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
    return;
}

//===============================================================================================================================================================================================================================================


Data::Data() : Initialization_OpenGL_SDL()
{
    this->setup_objects_and_main_data();
    this->setup_and_load_textures();
    this->setup_VAOs();
    return;
}

void Data::setup_objects_and_main_data()
{

//PROGRAM0
    this->program0_data.sizeof_data.push_back(18*sizeof(float));
    this->program0_data.data_enum.push_back(GL_DYNAMIC_DRAW);

    this->program0_data.data.push_back(new float[this->program0_data.sizeof_data[0]/sizeof(float)]
    {
        -0.5f, -0.5f, 0.0f,  1.0, 0.0, 0.0,   // left
         0.5f, -0.5f, 0.0f,  1.0, 0.0, 0.0,   // right
         0.0f,  0.5f, 0.0f,  1.0, 0.0, 0.0    // top
    });    //coordinates        //color

    this->program0_data.VAO.push_back(0); this->program0_data.VBO.push_back(0);
    glGenVertexArrays(1,&this->program0_data.VAO[0]);
    glGenBuffers(1,&this->program0_data.VBO[0]);

//---------------------------------------------------------------------------------------------------------------------------
//PROGRAM1
    this->program1_data.sizeof_data.push_back(6*32*sizeof(float));
    this->program1_data.sizeof_indices.push_back(6*6*sizeof(uint32_t));
    this->program1_data.data_enum.push_back(GL_STATIC_DRAW);
    this->program1_data.indices_enum.push_back(GL_STATIC_DRAW);
    this->program1_data.model.push_back(glm::mat4(1.0f));

    //this->program1_data.data = (float**)malloc(sizeof(float*)*1);
    this->program1_data.data.push_back(new float[this->program1_data.sizeof_data[0]/sizeof(float)]
    {
            // positions          // colors          // texture coords
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right       //back wall
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left

         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right       //front wall
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left

         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right      //right wall
         0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // bottom left
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f, // top left

         -0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right     //left wall
         -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left

         0.5f,  -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right     //down wall
         -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
         -0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left

         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right     //up wall
         -0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         -0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    });

    this->program1_data.indices.push_back(new uint32_t[this->program1_data.sizeof_indices[0]/sizeof(uint32_t)]
    {
        0, 1, 3,  // first triangle
        1, 2, 3,  // second triangle

        4, 5, 7,  // first triangle
        5, 6, 7,  // second triangle

        8, 9, 11,  // first triangle
        9, 10, 11,  // second triangle

        12, 13, 15,  // first triangle
        13, 14, 15,  // second triangle

        16, 17, 19,  // first triangle
        17, 18, 19,  // second triangle

        20, 21, 23,  // first triangle
        21, 22, 23  // second triangle
    });

    this->program1_data.VAO.push_back(0); this->program1_data.VBO.push_back(0); this->program1_data.EBO.push_back(0);
    glGenVertexArrays(1,&this->program1_data.VAO[0]);
    glGenBuffers(1,&this->program1_data.VBO[0]);
    glGenBuffers(1,&this->program1_data.EBO[0]);                        //needed for program2,program3;

//---------------------------------------------------------------------------------------------------------------------------
//PROGRAM2
    this->program2_data.sizeof_data.push_back(6*32*sizeof(float));
    this->program2_data.data_enum.push_back(GL_STATIC_DRAW);
    this->program2_data.model.push_back(glm::mat4(1.0f));

    //this->program2_data.data = (float**)malloc(sizeof(float*)*1);
    this->program2_data.data.push_back(new float[this->program2_data.sizeof_data[0]/sizeof(float)]
    {
            // positions          //normalization     //texture position
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0, 1.0,  // top right
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0, 0.0,  // bottom right       //back wall
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0, 0.0,  // bottom left
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0, 1.0,  // top left

         0.5f,  0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     1.0, 1.0,  // top right
         0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     1.0, 0.0,  // bottom right       //front wall
        -0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     0.0, 0.0,  // bottom left
        -0.5f,  0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     0.0, 1.0,  // top left

         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0, 1.0,  // top right
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0, 0.0,  // bottom right     //right wall
         0.5f, -0.5f, 0.5f,     1.0f,  0.0f,  0.0f,     0.0, 0.0,  // bottom left
         0.5f,  0.5f, 0.5f,     1.0f,  0.0f,  0.0f,     0.0, 1.0,  // top left

         -0.5f,  0.5f, 0.5f,    -1.0f,  0.0f,  0.0f,    1.0, 1.0,  // top right
         -0.5f, -0.5f, 0.5f,    -1.0f,  0.0f,  0.0f,    1.0, 0.0,  // bottom right    //left wall
         -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0, 0.0,  // bottom left
         -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0, 1.0,  // top left

         0.5f,  -0.5f, 0.5f,    0.0f, -1.0f,  0.0f,     1.0, 1.0,  // top right
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     1.0, 0.0,  // bottom right     //down wall
         -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,     0.0, 0.0,  // bottom left
         -0.5f, -0.5f, 0.5f,    0.0f, -1.0f,  0.0f,     0.0, 1.0,  // top left

         0.5f,  0.5f, -0.5f,    0.0f, 1.0f,  0.0f,      1.0, 1.0,  // top right
         0.5f,  0.5f,  0.5f,    0.0f, 1.0f,  0.0f,      1.0, 0.0,  // bottom right     //up wall
         -0.5f, 0.5f, 0.5f,     0.0f, 1.0f,  0.0f,      0.0, 0.0,  // bottom left
         -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,  0.0f,      0.0, 1.0   // top left
    });

    this->program2_data.VBO.push_back(0); this->program2_data.VAO.push_back(0);
    glGenVertexArrays(1,&this->program2_data.VAO[0]);
    glGenBuffers(1,&this->program2_data.VBO[0]);

//---------------------------------------------------------------------------------------------------------------------------
//PROGRAM3
    this->program3_data.model.push_back(glm::mat4(1.0f));

//---------------------------------------------------------------------------------------------------------------------------
//PROGRAM4
    this->program4_data.model.push_back(glm::mat4(1.0f));


    return;
}

void Data::setup_and_load_textures()
{
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.




//PROGRAM1/PROGRAM2
    size_t count_of_textures = 4;
    const char texture_name[count_of_textures][128] =
    {
        "files/images/container.jpg",
        "files/images/cat.png",
        "files/images/container2.png",
        "files/images/container2_specular.png"
    };

    int32_t texture_error_buffor;
    for(size_t i = 0; i!=count_of_textures; i++)
    {
        glActiveTexture(GL_TEXTURE0+i); // activate the texture unit first before binding textures to tell on what texture unite operations should be done;
        texture_error_buffor = this->opengl_universaltool0->load_texture(texture_name[i]);
        if(texture_error_buffor==-1)
        {
            char error_buffor[256];
            sprintf_s(error_buffor,256,"Failed to load texture: %s;",texture_name[i]);
            this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error",error_buffor,"Ok");
            exit(1);
        }
        this->texture.push_back(texture_error_buffor);
    }

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    return;
}

void Data::setup_VAOs()
{
//program0
    glBindBuffer(GL_ARRAY_BUFFER,this->program0_data.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,this->program0_data.sizeof_data[0],this->program0_data.data[0],this->program0_data.data_enum[0]);

    glBindVertexArray(this->program0_data.VAO[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//program1
    glBindVertexArray(this->program1_data.VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER,this->program1_data.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,this->program1_data.sizeof_data[0],this->program1_data.data[0],this->program1_data.data_enum[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->program1_data.EBO[0]);   //VAO saves bindbuffer EBO operations;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->program1_data.sizeof_indices[0],this->program1_data.indices[0],this->program1_data.indices_enum[0]);

    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//program2
    glBindVertexArray(this->program2_data.VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER,this->program2_data.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,this->program2_data.sizeof_data[0],this->program2_data.data[0],this->program2_data.data_enum[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->program1_data.EBO[0]);   //VAO saves bindbuffer EBO operations;                          //PROGRAM1 VARIABLE;

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture position
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//program3 (is entirely program2 + EBO from program1)

//not necessary but just in case i don't make some operations at the last buffers that has been set here;
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//

    return;
}

Data::~Data()
{
    glDeleteTextures(3,&this->texture[0]);

//PROGRAM0
    for(int i = 0; i!=1; i++) delete[] this->program0_data.data[i];
    glDeleteVertexArrays(1,&this->program0_data.VAO[0]);
    glDeleteBuffers(1,&this->program0_data.VBO[0]);

//PROGRAM1
    for(int i = 0; i!=1; i++) delete[] this->program1_data.data[i];
    for(int i = 0; i!=1; i++) delete[] this->program1_data.indices[i];
    glDeleteVertexArrays(1,&this->program1_data.VAO[0]);
    glDeleteBuffers(1,&this->program1_data.VBO[0]);
    glDeleteBuffers(1,&this->program1_data.EBO[0]);

//PROGRAM2
    for(int i = 0; i!=1; i++) delete[] this->program2_data.data[i];
    glDeleteVertexArrays(1,&this->program2_data.VAO[0]);
    glDeleteBuffers(1,&this->program2_data.VBO[0]);

//PROGRAM3

//PROGRAM4

    return;
}

//===============================================================================================================================================================================================================================================

Shader_Programs::Shader_Programs()
{
    this->program0_data.program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_program0.glsl","files/shader_programs/FS_program0.glsl");

    if(this->program0_data.program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: program0 error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","program0 error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->program1_data.program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_program1.glsl","files/shader_programs/FS_program1.glsl");
    if(this->program1_data.program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: program1 error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","program1 error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->program2_data.program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_program2.glsl","files/shader_programs/FS_program2.glsl");
    if(this->program2_data.program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: program2 error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","program2 error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->program3_data.program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_program3.glsl","files/shader_programs/FS_program3.glsl");
    if(this->program3_data.program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: program3 error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","program3 error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->program4_data.program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_program4.glsl","files/shader_programs/FS_program4.glsl");
    if(this->program4_data.program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: program4 error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","program4 error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->OpenGL_FontManager_program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_OpenGL_FontManager.glsl","files/shader_programs/FS_OpenGL_FontManager.glsl");
    if(this->OpenGL_FontManager_program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: OpenGL_FontManager error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","OpenGL_FontManager error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->OpenGL_Static2DManager_program_id = OpenGL_UniversalTools::load_and_compile_shader_programs("files/shader_programs/VS_OpenGL_Static2DManager.glsl","files/shader_programs/FS_OpenGL_Static2DManager.glsl");
    if(this->OpenGL_Static2DManager_program_id==0)
    {
        printf_error("Shader_Programs::load_and_compile_shader_programs: OpenGL_Static2DManager error;\n");
        this->sdl_universaltool0->ShowSimpleMessageBox0(SDL_MESSAGEBOX_ERROR,"Error","OpenGL_Static2DManager error:\nShader_Programs::load_and_compile_shader_programs;","Ok");
        SDL_Quit();
        exit(1);
    }

    this->setup_uniforms();

    return;
}

void Shader_Programs::setup_uniforms()
{
    //glm::mat4 transformation_default(1.0f);
    //glm::mat4 camera_default = glm::lookAt(MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0],MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0]+MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[1],MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[2]);
    //glm::mat4 projection_default = glm::perspective(glm::radians(MAIN_PROGRAM0::DEFAULT_VALUES::PROJECTION.x),MAIN_PROGRAM0::DEFAULT_VALUES::PROJECTION.y,
    //                                                MAIN_PROGRAM0::DEFAULT_VALUES::PROJECTION.z,MAIN_PROGRAM0::DEFAULT_VALUES::PROJECTION.w);

//PROGRAM1
    glUseProgram(this->program1_data.program_id); // have to bind/activate program before setting uniforms;

    glUniform1i(glGetUniformLocation(this->program1_data.program_id,"texture0"),0); //bind unit (in this case unit 0) to uniform texture0;
    glUniform1i(glGetUniformLocation(this->program1_data.program_id,"texture1"),1);
    //glUniformMatrix4fv(glGetUniformLocation(this->program1_data.program_id,"model"),1,GL_FALSE,glm::value_ptr(transformation_default)); //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program1_data.program_id,"camera"),1,GL_FALSE,glm::value_ptr(camera_default));        //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program1_data.program_id,"projection"),1,GL_FALSE,glm::value_ptr(projection_default));//default matrix uniform;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//PROGRAM2
    glUseProgram(this->program2_data.program_id);

    //glUniform3f(glGetUniformLocation(this->program2_data.program_id,"CameraPos"),MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].x,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].y,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].z);

    // directional light
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"dir_light.direction"),-0.2,-1.0,-0.3);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"dir_light.ambient"),50.03,50.03,50.03);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"dir_light.diffuse"),0.992*0.001,0.984*0.001,0.827*0.001);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"dir_light.specular"),0.5,0.5,0.5);

    // point lights
    int allocate_point_lights = 4;
    glUniform1i(glGetUniformLocation(this->program2_data.program_id,"NR_OF_POINT_LIGHTS"),allocate_point_lights);
    glm::vec3 pointLightPositions[allocate_point_lights] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    char buffor[256];
    for(int i = 0; i!=allocate_point_lights; i++)
    {
        sprintf_s(buffor,256,"point_light[%d].position",i);
        glUniform3f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),pointLightPositions[i].x,pointLightPositions[i].y,pointLightPositions[i].z);
        sprintf_s(buffor,256,"point_light[%d].ambient",i);
        glUniform3f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),0.09,0.09,0.09);
        sprintf_s(buffor,256,"point_light[%d].diffuse",i);
        glUniform3f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),0.984*1.5,0.89*1.5,0.62*1.5);
        sprintf_s(buffor,256,"point_light[%d].specular",i);
        glUniform3f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),1.0,1.0,1.0);
        sprintf_s(buffor,256,"point_light[%d].constant",i);
        glUniform1f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),1.0);
        sprintf_s(buffor,256,"point_light[%d].linear",i);
        glUniform1f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),0.35);
        sprintf_s(buffor,256,"point_light[%d].quadratic",i);
        glUniform1f(glGetUniformLocation(this->program2_data.program_id,(const char*)buffor),0.44);
    }

    // spotLight
    //glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.position"),MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].x,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].y,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0].z);
    //glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.direction"),MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[1].x,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[1].y,MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[1].z);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.ambient"),0.0,0.0,0.0);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.diffuse"),0.988*2.8,0.925*2.8,0.682*2.8);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.specular"),1.0,1.0,1.0);
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"spot_light.constant"),1.0);
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"spot_light.linear"),0.14);
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"spot_light.quadratic"),0.07);
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"spot_light.cutOff"),glm::cos(glm::radians(4.5f)));
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"spot_light.outerCutOff"),glm::cos(glm::radians(20.5f)));

    // material
    //glm::mat3 normalized_model = glm::transpose(glm::inverse(transformation_default));
    //glUniformMatrix3fv(glGetUniformLocation(this->program2_data.program_id,"normal_model"),1,GL_FALSE,glm::value_ptr(normalized_model));
    glUniform1i(glGetUniformLocation(this->program2_data.program_id,"material.diffuse"),2);
    glUniform1i(glGetUniformLocation(this->program2_data.program_id,"material.specular"),3);
    glUniform1f(glGetUniformLocation(this->program2_data.program_id,"material.shininess"),64.0f);
        glActiveTexture(GL_TEXTURE3);                   //bind texture 'cause "draw" functions from OpenGL_FontManager and OpenGL_Static2DManager binds different textures to GL_TEXTURE0;
    glBindTexture(GL_TEXTURE_2D,this->texture[3]); //

    //glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"model"),1,GL_FALSE,glm::value_ptr(transformation_default)); //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"camera"),1,GL_FALSE,glm::value_ptr(camera_default));        //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"projection"),1,GL_FALSE,glm::value_ptr(projection_default));//default matrix uniform;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//PROGRAM3
    glUseProgram(this->program3_data.program_id);

    //glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"model"),1,GL_FALSE,glm::value_ptr(transformation_default)); //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"camera"),1,GL_FALSE,glm::value_ptr(camera_default));        //default matrix uniform;
    //glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"projection"),1,GL_FALSE,glm::value_ptr(projection_default));//default matrix uniform;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//PROGRAM4
    glUseProgram(this->program4_data.program_id);

    // directional light
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"dir_light.direction"),-0.2,-1.0,-0.3);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"dir_light.ambient"),50.03,50.03,50.03);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"dir_light.diffuse"),0.992*0.001,0.984*0.001,0.827*0.001);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"dir_light.specular"),0.5,0.5,0.5);

    // point lights
    allocate_point_lights = 0;
    glUniform1i(glGetUniformLocation(this->program4_data.program_id,"NR_OF_POINT_LIGHTS"),allocate_point_lights);

    // spotLight
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"spot_light.ambient"),0.0,0.0,0.0);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"spot_light.diffuse"),0.988*1.0,0.925*1.0,0.682*1.0);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"spot_light.specular"),0.7,0.7,0.7);
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"spot_light.constant"),1.0);
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"spot_light.linear"),0.07);
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"spot_light.quadratic"),0.017);
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"spot_light.cutOff"),glm::cos(glm::radians(0.0f)));
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"spot_light.outerCutOff"),glm::cos(glm::radians(90.0f)));

    // material
    glUniform1f(glGetUniformLocation(this->program4_data.program_id,"material.shininess"),32.0f);


    glUseProgram(0);
    return;
}

Shader_Programs::~Shader_Programs()
{
    glDeleteProgram(this->program0_data.program_id);
    glDeleteProgram(this->program1_data.program_id);
    glDeleteProgram(this->program2_data.program_id);
    glDeleteProgram(this->program3_data.program_id);
    glDeleteProgram(this->program4_data.program_id);
    glDeleteProgram(this->OpenGL_FontManager_program_id);
    glDeleteProgram(this->OpenGL_Static2DManager_program_id);
    return;
}

//===============================================================================================================================================================================================================================================


Core0::Core0() : camera_fps0(MAIN_PROGRAM0::DEFAULT_VALUES::VIEW[0],0,MAIN_PROGRAM0::DEFAULT_VALUES::PROJECTION)
{

//set viewport as base coordinates for vertex (when you try to draw some figures on screen: {0.5,0,-1.0} etc are coordinates that depends on base coordinates from viewport);
    glViewport(0,0,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    this->static_image0 = new OpenGL_UniversalTools::OpenGL_Static2DManager(this->OpenGL_Static2DManager_program_id,"files/images/instruction.png",MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT);
    this->font0 = new OpenGL_UniversalTools::OpenGL_FontManager("files/fonts/font.ttf",this->OpenGL_FontManager_program_id,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT,32);

    return;
}

void Core0::program0_test0()
{

//bind vertex array and use program for drawing functions like: glDrawArrays(),glDrawElements() etc.;
    glUseProgram(this->program0_data.program_id);

//bind for functions like: glBufferSubData(); glDrawArrays doesn't need to bind buffer cause buffer has been registered throught function glVertexAttribPointer();
    glBindBuffer(GL_ARRAY_BUFFER,this->program0_data.VBO[0]);

    glBindVertexArray(this->program0_data.VAO[0]);

    static float dynamic_variable = 0;
    static float which_direction_calculations = 0;
    if(this->sdl_universaltool0->timer0.run_timer()==1)
    {
        if(which_direction_calculations==0)
        {
            dynamic_variable+=0.02;
            if(dynamic_variable>=2.02)
            {
                dynamic_variable-=0.02;
                which_direction_calculations = 1;
            }
        }
        else
        {
            dynamic_variable-=0.02;
            if(dynamic_variable<=-0.02)
            {
                dynamic_variable+=0.02;
                which_direction_calculations = 0;
            }
        }
    }

//edit vertex buffer;
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(float)*4,sizeof(float),&dynamic_variable);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return;
}

void Core0::program1_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables)
{
    glDisable(GL_BLEND);

    glUseProgram(this->program1_data.program_id);
    glBindVertexArray(this->program1_data.VAO[0]);

    static double dynamic_variable = 0;
    if(this->sdl_universaltool0->timer0.run_timer()==1) dynamic_variable+=0.5;

    static int32_t camera_location = glGetUniformLocation(this->program1_data.program_id,"camera");
    static int32_t model_location = glGetUniformLocation(this->program1_data.program_id,"model");
    static int32_t projection_location = glGetUniformLocation(this->program1_data.program_id,"projection");
    static glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glUniformMatrix4fv(camera_location,1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_view_matrix()));
    glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_projection_matrix()));

    glActiveTexture(GL_TEXTURE0);                   //bind texture 'cause "draw" functions from OpenGL_FontManager and OpenGL_Static2DManager binds different textures to GL_TEXTURE0;
    glBindTexture(GL_TEXTURE_2D,this->texture[0]); //
    double angle;
    for(unsigned int i = 0; i!=10; i++)
    {
        this->program1_data.model[0] = glm::mat4(1.0f);
        this->program1_data.model[0] = glm::translate(this->program1_data.model[0], cubePositions[i]);
        angle = 20.0f * i;
        this->program1_data.model[0] = glm::rotate(this->program1_data.model[0], glm::radians((float)(angle+dynamic_variable)), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(this->program1_data.model[0]));

        glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    static bool disable_image = 0;
    if(disable_image==0)
    {
        if(event_variables.key_x_pressed==1)
        {
            disable_image = 1;
            return;
        }
        this->static_image0->draw(0,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-152,259,152,1.0,1.0);
    }




    char data[11];
    this->universal_tool.uint32_to_string(event_variables.uint32_actual_fps,data);

    glm::vec4 actual_text_color;
    if(event_variables.uint32_actual_fps>=60) actual_text_color = glm::vec4(0.15, 1.0, 0.15, 1.0);
    else if(event_variables.uint32_actual_fps>=30) actual_text_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    else actual_text_color = glm::vec4(1.0, 0.15, 0.15, 1.0);

    font0->draw("FPS: ",MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,glm::vec4(0.0, 0.0, 0.0, 1.0));
    font0->draw(data,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75+32,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,actual_text_color);


    return;
}

void Core0::program2_3_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables)
{
    glBindVertexArray(this->program2_data.VAO[0]);
    glUseProgram(this->program2_data.program_id);

    static const uint32_t count_of_point_lights = 4;
    static double dynamic_var0 = 0;
    static glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    static glm::vec3 pointLightPositions[count_of_point_lights] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    if(this->sdl_universaltool0->timer0.run_timer()==1)
    {
        dynamic_var0+=0.65;
        for(uint32_t i = 0; i!=count_of_point_lights; i++)
        {
            pointLightPositions[i].x+=glm::sin(glm::radians(dynamic_var0))*0.035;
            pointLightPositions[i].y-=glm::cos(glm::radians(dynamic_var0))*0.015;
            pointLightPositions[i].z+=glm::cos(glm::radians(dynamic_var0))*0.045;
        }
    }
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"point_light[0].position"),pointLightPositions[0].x,pointLightPositions[0].y,pointLightPositions[0].z);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"point_light[1].position"),pointLightPositions[1].x,pointLightPositions[1].y,pointLightPositions[1].z);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"point_light[2].position"),pointLightPositions[2].x,pointLightPositions[2].y,pointLightPositions[2].z);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"point_light[3].position"),pointLightPositions[3].x,pointLightPositions[3].y,pointLightPositions[3].z);


    glm::vec3 camera_pos = this->camera_fps0.get_camera_position();
    glm::vec3 camera_dir = this->camera_fps0.get_camera_direction();
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"CameraPos"),camera_pos.x,camera_pos.y,camera_pos.z);

    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.position"),camera_pos.x,camera_pos.y,camera_pos.z);
    glUniform3f(glGetUniformLocation(this->program2_data.program_id,"spot_light.direction"),camera_dir.x,camera_dir.y,camera_dir.z);

    glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"camera"),1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_projection_matrix()));

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,this->texture[2]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,this->texture[3]);

    for (uint32_t i = 0; i!=10; i++)
    {
        this->program2_data.model[0] = glm::mat4(1.0f);
        this->program2_data.model[0] = glm::translate(this->program2_data.model[0], cubePositions[i]);
        this->program2_data.model[0] = glm::scale(this->program2_data.model[0],glm::vec3(1.2f)); //a bigger cube
        float angle = 20.0f * i;
        this->program2_data.model[0] = glm::rotate(this->program2_data.model[0], glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(this->program2_data.program_id,"model"),1,GL_FALSE,glm::value_ptr(this->program2_data.model[0]));
        glm::mat3 normalized_model = glm::transpose(glm::inverse(this->program2_data.model[0]));
        glUniformMatrix3fv(glGetUniformLocation(this->program2_data.program_id,"normal_model"),1,GL_FALSE,glm::value_ptr(normalized_model));

        glDrawElements(GL_TRIANGLES,6*6,GL_UNSIGNED_INT,0);
    }


    glUseProgram(this->program3_data.program_id);

    glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"camera"),1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_projection_matrix()));

    for (uint32_t i = 0; i!=count_of_point_lights; i++)
    {
        this->program3_data.model[0] = glm::mat4(1.0f);
        this->program3_data.model[0] = glm::translate(this->program3_data.model[0],pointLightPositions[i]);
        this->program3_data.model[0] = glm::scale(this->program3_data.model[0],glm::vec3(0.2f)); //a smaller cube
        glUniformMatrix4fv(glGetUniformLocation(this->program3_data.program_id,"model"),1,GL_FALSE,glm::value_ptr(this->program3_data.model[0]));
        glDrawElements(GL_TRIANGLES,6*6,GL_UNSIGNED_INT,0);
    }




    static bool disable_image = 0;
    if(disable_image==0)
    {
        if(event_variables.key_x_pressed==1)
        {
            disable_image = 1;
            return;
        }
        this->static_image0->draw(0,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-152,259,152,1.0,1.0);
    }

    char data[11];
    this->universal_tool.uint32_to_string(event_variables.uint32_actual_fps,data);

    glm::vec4 actual_text_color;
    if(event_variables.uint32_actual_fps>=60) actual_text_color = glm::vec4(0.15, 1.0, 0.15, 1.0);
    else if(event_variables.uint32_actual_fps>=30) actual_text_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    else actual_text_color = glm::vec4(1.0, 0.15, 0.15, 1.0);

    font0->draw("FPS: ",MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,glm::vec4(0.0, 0.0, 0.0, 1.0));
    font0->draw(data,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75+32,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,actual_text_color);


    return;
}

void Core0::program4_test0(MAIN_PROGRAM0::main_event_loop0_variables event_variables)
{
    static Model model0("files/models/model1/obj0.obj");
    glUseProgram(this->program4_data.program_id);

    static int32_t camera_location = glGetUniformLocation(this->program4_data.program_id,"camera");
    static int32_t model_location = glGetUniformLocation(this->program4_data.program_id,"model");
    static int32_t projection_location = glGetUniformLocation(this->program4_data.program_id,"projection");

    glm::vec3 camera_pos = this->camera_fps0.get_camera_position();
    glm::vec3 camera_dir = this->camera_fps0.get_camera_direction();
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"CameraPos"),camera_pos.x,camera_pos.y,camera_pos.z);

    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"spot_light.position"),camera_pos.x,camera_pos.y,camera_pos.z);
    glUniform3f(glGetUniformLocation(this->program4_data.program_id,"spot_light.direction"),camera_dir.x,camera_dir.y,camera_dir.z);

    glUniformMatrix4fv(camera_location,1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_view_matrix()));
    glUniformMatrix4fv(projection_location,1,GL_FALSE,glm::value_ptr(this->camera_fps0.get_projection_matrix()));

    this->program4_data.model[0] = glm::mat4(1.0f);
    this->program4_data.model[0] = glm::scale(this->program4_data.model[0],glm::vec3(0.08f));
    glm::mat3 normalized_model = glm::transpose(glm::inverse(this->program4_data.model[0]));
    glUniformMatrix3fv(glGetUniformLocation(this->program4_data.program_id,"normal_model"),1,GL_FALSE,glm::value_ptr(normalized_model));
    glUniformMatrix4fv(model_location,1,GL_FALSE,glm::value_ptr(this->program4_data.model[0]));
    model0.Draw(this->program4_data.program_id);



    static bool disable_image = 0;
    if(disable_image==0)
    {
        if(event_variables.key_x_pressed==1)
        {
            disable_image = 1;
            return;
        }
        this->static_image0->draw(0,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-152,259,152,1.0,1.0);
    }

    char data[11];
    this->universal_tool.uint32_to_string(event_variables.uint32_actual_fps,data);

    glm::vec4 actual_text_color;
    if(event_variables.uint32_actual_fps>=60) actual_text_color = glm::vec4(0.15, 1.0, 0.15, 1.0);
    else if(event_variables.uint32_actual_fps>=30) actual_text_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    else actual_text_color = glm::vec4(1.0, 0.15, 0.15, 1.0);

    font0->draw("FPS: ",MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,glm::vec4(0.0, 0.0, 0.0, 1.0));
    font0->draw(data,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_WIDTH-75+32,MAIN_PROGRAM0::DEFAULT_VALUES::SCR_HEIGHT-20,0.45,actual_text_color);


    return;
}

Core0::~Core0()
{
    delete this->static_image0;
    delete this->font0;
    return;
}

