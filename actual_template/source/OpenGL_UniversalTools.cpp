#include "../include/OpenGL_UniversalTools.h"

namespace OpenGL_UniversalTools
{
    OpenGL_FontManager::OpenGL_FontManager(const char* const filename,uint32_t program_id,uint32_t viewport_width,uint32_t viewport_height,uint32_t font_height,uint32_t style_index)
    {
        if(font_height==0) font_height = 1;

        FT_Error error = FT_Init_FreeType(&this->priv_var.library);
        if(error!=0) ft_fatal_error(error,__FILE__,__LINE__);

        error = FT_New_Face(this->priv_var.library,filename,style_index,&this->priv_var.font);
        if(error!=0) ft_fatal_error(error,__FILE__,__LINE__);

        error = FT_Set_Pixel_Sizes(this->priv_var.font,0,font_height);
        if(error!=0) ft_fatal_error(error,__FILE__,__LINE__);

        FT_Select_Charmap(this->priv_var.font,FT_ENCODING_UNICODE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        this->load_textures_to_map();
        this->priv_var.program_id = program_id;

        this->priv_var.projection = glm::ortho(0.0f,(float)viewport_width,0.0f,(float)viewport_height);
        this->configure_VAO();


        return;
    }

    void OpenGL_FontManager::change_size_of_the_font(uint32_t height)
    {
        if(height==0) height = 1;
        FT_Error error = FT_Set_Pixel_Sizes(this->priv_var.font,0,height);
        if(error!=0) ft_fatal_error(error,__FILE__,__LINE__);
        free_textures_from_map();
        this->priv_var.chars.clear();
        load_textures_to_map();
        return;
    }

    void OpenGL_FontManager::change_viewport(uint32_t width,uint32_t height)
    {
        this->priv_var.projection = glm::ortho(0.0f,(float)width,0.0f,(float)height);
        glUseProgram(this->priv_var.program_id);
        glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->priv_var.projection));
        return;
    }

    //template <typename T> void draw(const T* const text,float x,float y,float scale,glm::vec4 color,glm::vec4 background_color = {0.0,0.0,0.0,0.0},glm::mat4 extra_tranformation = glm::mat4(1.0))

    OpenGL_FontManager::~OpenGL_FontManager()
    {
        free_textures_from_map();
        glDeleteBuffers(1,&this->priv_var.VBO);
        glDeleteVertexArrays(1,&this->priv_var.VAO);
        FT_Done_Face(this->priv_var.font);
        FT_Done_FreeType(this->priv_var.library);
        return;
    }


    void OpenGL_FontManager::configure_VAO()
    {
        glGenBuffers(1,&this->priv_var.VBO);
        glBindBuffer(GL_ARRAY_BUFFER,this->priv_var.VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*4,NULL,GL_DYNAMIC_DRAW);

        glGenVertexArrays(1, &this->priv_var.VAO);
        glBindVertexArray(this->priv_var.VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);


        glUseProgram(this->priv_var.program_id);
        glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->priv_var.projection));
        glUniform1i(glGetUniformLocation(this->priv_var.program_id,"texture0"),0);

        return;
    }

    void OpenGL_FontManager::load_textures_to_map()
    {
        FT_ULong  char_code = 0;
        FT_UInt   index;
        GLuint texture;
        //this->priv_var.count_of_chars = 0;

        char_code = FT_Get_First_Char(this->priv_var.font,&index);
        Character character;

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        while(index!=0)
        {
            //this->priv_var.count_of_chars++;
            FT_Load_Glyph(this->priv_var.font,index,FT_LOAD_RENDER);

            // generate texture
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D,texture);

            if(this->priv_var.font->glyph->bitmap.width==0||this->priv_var.font->glyph->bitmap.rows==0) // font can contains special chars like "space" and it hasn't any bitmap;
            {
                char data0 = 0;
                glTexImage2D(GL_TEXTURE_2D,0,GL_RED,1,1,0,GL_RED,GL_UNSIGNED_BYTE,&data0);
            }
            else glTexImage2D(GL_TEXTURE_2D,0,GL_RED,this->priv_var.font->glyph->bitmap.width,this->priv_var.font->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,this->priv_var.font->glyph->bitmap.buffer);

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            character.texture_id = texture; //here texture id from opengl (u have first load texture,and don't forget to free memory of texture after this);
            character.size[0] = this->priv_var.font->glyph->bitmap.width; character.size[1] = this->priv_var.font->glyph->bitmap.rows;
            character.bearing[0] = this->priv_var.font->glyph->bitmap_left; character.bearing[1] = this->priv_var.font->glyph->bitmap_top;
            character.advance = this->priv_var.font->glyph->advance.x;
            this->priv_var.chars.insert(std::pair<FT_ULong,Character>(char_code,character));

            char_code = FT_Get_Next_Char(this->priv_var.font,char_code,&index);
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT,4); //set to default after all;

        return;
    }

    void OpenGL_FontManager::ft_fatal_error(FT_Error error,const char* const position,uint32_t line)
    {
        fprintf(stderr,"OpenGL_FontManager error: %s | file: %s | line: %u",ft_string_error(error),position,line);
        exit(error);
    }

    void OpenGL_FontManager::free_textures_from_map()
    {
        for (auto& i : this->priv_var.chars) glDeleteTextures(1,&i.second.texture_id);
        return;
    }

    const char* OpenGL_FontManager::ft_string_error(FT_Error error)
    {
        #undef FTERRORS_H_
        #define FT_ERRORDEF( e, v, s )  case e: return s;
        #define FT_ERROR_START_LIST     switch(error){
        #define FT_ERROR_END_LIST       default: return "Unknown error";}
        #include FT_ERRORS_H
    }

//===============================================================================================================================================================================================================================================



    OpenGL_Static2DManager::OpenGL_Static2DManager(uint32_t program_id,const char* const filename,uint32_t viewport_width,uint32_t viewport_height)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        this->priv_var.projection = glm::ortho(0.0f,(float)viewport_width,0.0f,(float)viewport_height);
        this->priv_var.program_id = program_id;
        this->priv_var.texture_id = 0;
        if(this->load_different_image(filename)==1) exit(1);
        this->configure_VAO();
        return;
    }

    void OpenGL_Static2DManager::draw(float x,float y,float width,float height,float visibility,float scale,glm::mat4 extra_tranformation)
    {
        glUseProgram(this->priv_var.program_id);
        glBindVertexArray(this->priv_var.VAO);

        glUniform1f(glGetUniformLocation(this->priv_var.program_id,"visibility"),visibility);
        glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"model"),1,GL_FALSE,glm::value_ptr(extra_tranformation));

        width*=scale;
        height*=scale;

        float vertices[24] =
        {
             x,         y + height,       0.0f, 0.0f ,   //left up;
             x,         y,                0.0f, 1.0f ,   //left down;
             x + width, y,                1.0f, 1.0f ,   //right down;

             x,         y + height,       0.0f, 0.0f ,   //left up;
             x + width, y,                1.0f, 1.0f ,   //right down;
             x + width, y + height,       1.0f, 0.0f     //right up;
        };

        glBindBuffer(GL_ARRAY_BUFFER,this->priv_var.VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,this->priv_var.texture_id);
        glDrawArrays(GL_TRIANGLES,0,6);

        return;
    }

    bool OpenGL_Static2DManager::load_different_image(const char* const filename)
    {
        stbi_set_flip_vertically_on_load(0);
        if(this->priv_var.texture_id!=0) glDeleteTextures(1,&this->priv_var.texture_id);
        int32_t width,height,nr_channels;

        glGenTextures(1,&this->priv_var.texture_id);

        unsigned char* data = stbi_load(filename,&width,&height,&nr_channels,0);

        if(data)
        {
            GLenum format;
            if (nr_channels==3) {format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}
            else if (nr_channels==4) {format = GL_RGBA; glPixelStorei(GL_UNPACK_ALIGNMENT,4);}
            else if (nr_channels==2) {format = GL_RG; glPixelStorei(GL_UNPACK_ALIGNMENT,2);}
            else {format = GL_RED; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}

            glBindTexture(GL_TEXTURE_2D,this->priv_var.texture_id);

            glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            stbi_image_free(data);
            glPixelStorei(GL_UNPACK_ALIGNMENT,4); //set to default after all;
            stbi_set_flip_vertically_on_load(1);
            return 0;
        }
        else
        {
            fprintf(stderr,"OpenGL_Static2DManager: Failed to load texture: %s\n    file: %s | line: %d \n",filename,__FILE__,__LINE__);
            glDeleteTextures(1,&this->priv_var.texture_id);
            glPixelStorei(GL_UNPACK_ALIGNMENT,4); //set to default after all;
            stbi_set_flip_vertically_on_load(1);
            return 1;
        }
    }

    void OpenGL_Static2DManager::change_viewport(uint32_t width,uint32_t height)
    {
        this->priv_var.projection = glm::ortho(0.0f,(float)width,0.0f,(float)height);
        glUseProgram(this->priv_var.program_id);
        glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->priv_var.projection));
        return;
    }

    OpenGL_Static2DManager::~OpenGL_Static2DManager()
    {
        glDeleteTextures(1,&this->priv_var.texture_id);
        glDeleteBuffers(1,&this->priv_var.VBO);
        glDeleteVertexArrays(1,&this->priv_var.VAO);
        return;
    }

    void OpenGL_Static2DManager::configure_VAO()
    {

        glGenBuffers(1,&this->priv_var.VBO);
        glBindBuffer(GL_ARRAY_BUFFER,this->priv_var.VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*4,NULL,GL_DYNAMIC_DRAW);

        glGenVertexArrays(1, &this->priv_var.VAO);
        glBindVertexArray(this->priv_var.VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        glUseProgram(this->priv_var.program_id);
        glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"projection"),1,GL_FALSE,glm::value_ptr(this->priv_var.projection));
        glUniform1i(glGetUniformLocation(this->priv_var.program_id,"texture0"),0);

        return;
    }

//===============================================================================================================================================================================================================================================

    GLuint load_and_compile_shader_programs(const char* const path_glsl_vertex_shader,const char* const path_glsl_fragment_shader)
    {
        //load code from files
    //====================================================================================

        char* vertex_shader_code_data;
        char* fragment_shader_code_data;

        unsigned int file_size;
        unsigned int bytes_readed;
        FILE* file = fopen(path_glsl_vertex_shader,"rb");
        if(file==NULL)
        {
            fprintf(stderr,"failed to load file: %s\n",path_glsl_vertex_shader);
            return 0;
        }
        fseek(file,0,SEEK_END);
        file_size = ftell(file);
        fseek(file,0,SEEK_SET);
        vertex_shader_code_data = (char*)malloc(file_size+1);
        vertex_shader_code_data[file_size] = '\0';
        bytes_readed = fread(vertex_shader_code_data,1,file_size,file);
        if(bytes_readed!=file_size)
        {
            fprintf(stderr,"failed to read: %s\nbytes that should be read: %u\nbytes readed: %u\n",path_glsl_vertex_shader,bytes_readed,file_size);
            free(vertex_shader_code_data);
            fclose(file);
            return 0;
        }
        fclose(file);

        file = fopen(path_glsl_fragment_shader,"rb");
        if(file==NULL)
        {
            fprintf(stderr,"failed to load file: %s\n",path_glsl_fragment_shader);
            return 0;
        }
        fseek(file,0,SEEK_END);
        file_size = ftell(file);
        fseek(file,0,SEEK_SET);
        fragment_shader_code_data = (char*)malloc(file_size+1);
        fragment_shader_code_data[file_size] = '\0';
        bytes_readed = fread(fragment_shader_code_data,1,file_size,file);
        if(bytes_readed!=file_size)
        {
            fprintf(stderr,"failed to read: %s\nbytes that should be read: %u\nbytes readed: %u\n",path_glsl_fragment_shader,bytes_readed,file_size);
            free(vertex_shader_code_data);
            free(fragment_shader_code_data);
            fclose(file);
            return 0;
        }
        fclose(file);


    //====================================================================================


        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,(const char**)&vertex_shader_code_data,NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (success==0)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            fprintf(stderr,"ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n    %s\n",infoLog);
            free(vertex_shader_code_data);
            free(fragment_shader_code_data);
            glDeleteShader(vertexShader);
            return 0;
        }
        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,(const char**)&fragment_shader_code_data,NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (success==0)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            fprintf(stderr,"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n    %s\n",infoLog);
            free(vertex_shader_code_data);
            free(fragment_shader_code_data);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }
        // link shaders
        unsigned int shaderProgram = glCreateProgram(); //returns 0 if not ok;
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success==0)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            fprintf(stderr,"ERROR::SHADER::PROGRAM::LINKING_FAILED\n    %s\n",infoLog);
            free(vertex_shader_code_data);
            free(fragment_shader_code_data);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }
        free(vertex_shader_code_data);
        free(fragment_shader_code_data);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

}
