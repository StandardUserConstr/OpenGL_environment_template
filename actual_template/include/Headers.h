#ifndef HEADERS_SECURE_H
#define HEADERS_SECURE_H

#include <SDL2/SDL.h>               //SDL2
#include <GL/glew.h>                //Glew
#include <glm/glm.hpp>              //GLM
#include <glm/gtc/type_ptr.hpp>     //
#include <assimp/Importer.hpp>      //Assimp
#include <assimp/scene.h>           //
#include <assimp/postprocess.h>     //
#include <freetype/freetype.h>      //FreeType

#if defined(_WIN32)
    #define WINDOWS_CODE
    #include <profileapi.h>
#endif

#include <stdio.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <map>

#include "external_libs/stb_image.h"

#define printf_error(string) fprintf(stderr,string)
#define printf_error_args(string,args...) fprintf(stderr,string,args)

#define Core_DebugInfo            // debug option (can be commented);
#define MainProgram_DebugInfo     // debug option (can be commented);

#endif // HEADERS_SECURE_H
