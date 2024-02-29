#ifndef OpenGL_UniversalTools_SECURE_H
#define OpenGL_UniversalTools_SECURE_H

#include "headers.h"

namespace OpenGL_UniversalTools
{
    //class OpenGL_FontManager;
    //class OpenGL_Static2DManager;
    //GLuint load_and_compile_shader_programs(const char* const path_glsl_vertex_shader,const char* const path_glsl_fragment_shader);


    //#include (opengl library)
    //#include <freetype/freetype.h>
    //#include <glm/glm.hpp>
    //#include <glm/gtc/type_ptr.hpp>
    //------------------------------
    //#include <stdlib.h>
    //#include <stdio.h>
    //#include <stdint.h>
    //#include <map>
    //------------------------------
    // class at his constructor will activate blending: "glEnable(GL_BLEND);","glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);" and sets: "glPixelStorei(GL_UNPACK_ALIGNMENT,4);" with func "change_size_of_the_font()";
    class OpenGL_FontManager
    {
        struct Character
        {
            GLuint       texture_id; // ID handle of the glyph texture
            int32_t      size[2];    // Size of glyph
            int32_t      bearing[2]; // Offset from baseline to left/top of glyph
            uint32_t     advance;    // Offset to advance to next glyph
        };

        struct private_variables
        {
            GLuint VBO,VAO;
            FT_Library library;
            FT_Face font;
            GLuint program_id;
            std::map<FT_ULong,Character> chars;
            glm::mat4 projection;
            //uint32_t count_of_chars;
        }priv_var;


    public:
        //defualt encoding is "UNICODE" if available;
        OpenGL_FontManager(const char* const filename,uint32_t program_id,uint32_t viewport_width,uint32_t viewport_height,uint32_t font_height,uint32_t style_index = 0);

        // -- IMPORTANT --
        // heavy function;
        void change_size_of_the_font(uint32_t height);

        void change_viewport(uint32_t width,uint32_t height);

        //at the end of "text" should be 0;
        template <typename T> void draw(const T* const text,float x,float y,float scale,glm::vec4 color,glm::vec4 background_color = {0.0,0.0,0.0,0.0},glm::mat4 extra_tranformation = glm::mat4(1.0))
        {
            glUseProgram(this->priv_var.program_id);
            glUniform4f(glGetUniformLocation(this->priv_var.program_id,"textcolor"),color.x,color.y,color.z,color.w);
            glUniform4f(glGetUniformLocation(this->priv_var.program_id,"background"),background_color.x,background_color.y,background_color.z,background_color.w);
            glUniformMatrix4fv(glGetUniformLocation(this->priv_var.program_id,"model"),1,GL_FALSE,glm::value_ptr(extra_tranformation));

            glBindVertexArray(this->priv_var.VAO);
            glBindBuffer(GL_ARRAY_BUFFER,this->priv_var.VBO);

            OpenGL_FontManager::Character* character;
            float xpos,ypos;
            float w,h;
            std::map<FT_ULong,Character>::iterator map_iterator;
            for (size_t i = 0; text[i]!=0; i++)
            {
                map_iterator = this->priv_var.chars.find((uint32_t)text[i]);
                if(map_iterator==this->priv_var.chars.end()) continue;
                character = &map_iterator->second;

                xpos = x+(character->bearing[0]*scale);
                ypos = y-((character->size[1]-character->bearing[1])*scale);

                w = character->size[0]*scale;
                h = character->size[1]*scale;

                float vertices[24] =
                {
                     xpos,     ypos + h,   0.0f, 0.0f ,   //left up;
                     xpos,     ypos,       0.0f, 1.0f ,   //left down;
                     xpos + w, ypos,       1.0f, 1.0f ,   //right down;

                     xpos,     ypos + h,   0.0f, 0.0f ,   //left up;
                     xpos + w, ypos,       1.0f, 1.0f ,   //right down;
                     xpos + w, ypos + h,   1.0f, 0.0f     //right up;
                };

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,character->texture_id);
                glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);

                glDrawArrays(GL_TRIANGLES,0,6);
                x+=(character->advance/64)*scale; // it's conversion from 26.6 pixel format to normal pixel;
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            return;
        }

        ~OpenGL_FontManager();


    private:
        void configure_VAO();
        void load_textures_to_map();

        void ft_fatal_error(FT_Error error,const char* const position,uint32_t line);
        void free_textures_from_map();
        const char* ft_string_error(FT_Error error);



    };

    //#include (opengl library)
    //#include <freetype/freetype.h>
    //#include <glm/glm.hpp>
    //#include <glm/gtc/type_ptr.hpp>
    //#include <stb_image.h>
    //#include <glm/glm.hpp>
    //------------------------------
    //#include <stdlib.h>
    //#include <stdint.h>
    //#include <stdio.h>
    //----------------------------------------
    //  class at his constructor will activate blending: "glEnable(GL_BLEND);","glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);" and sets: "glPixelStorei(GL_UNPACK_ALIGNMENT,4);" with func "load_different_image()";
                                                                                                                                                //and "stbi_set_flip_vertically_on_load(1);" with func "load_differet_image()";
    class OpenGL_Static2DManager
    {
        struct private_variables
        {
            GLuint VBO,VAO;
            GLuint program_id;
            GLuint texture_id;
            glm::mat4 projection;
        }priv_var;

    public:
        OpenGL_Static2DManager(uint32_t program_id,const char* const filename,uint32_t viewport_width,uint32_t viewport_height);

        void draw(float x,float y,float width,float height,float visibility = 1.0,float scale = 1.0,glm::mat4 extra_tranformation = glm::mat4(1.0));

        //returns 0 if success; else 1;
        bool load_different_image(const char* const filename);

        void change_viewport(uint32_t width,uint32_t height);

        ~OpenGL_Static2DManager();


    private:
        void configure_VAO();

    };

    //returns ID if ok,returns 0 if error;
    GLuint load_and_compile_shader_programs(const char* const path_glsl_vertex_shader,const char* const path_glsl_fragment_shader);

}


#endif

