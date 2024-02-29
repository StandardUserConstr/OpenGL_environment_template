#ifndef MODEL_LOADING_SECURE_H
#define MODEL_LOADING_SECURE_H

#include "Headers.h"

namespace ModelLoading
{

    //#include <glm/glm.hpp>
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    //include <assimp/material.h>
    //include <string.h>
    struct Texture
    {
        unsigned int id;
        aiTextureType type;
        std::string path;
    };


    //include <vector>
    //include <stdio.h>
    class Mesh
    {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<uint32_t>     indices;
        std::vector<Texture>      textures;
        uint32_t VAO;

        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

        // render the mesh
        void Draw(uint32_t program_id);

    private:

        uint32_t VBO, EBO;

        void setupMesh();
    };


//================================================================================================================================================================================================


    //include <assimp/material.h>
    //include <cstring>
    //include <vector>
    //include <assimp/types.h>
    //include <stdint.h>
    //include <stdio.h>
    class Model
    {
    public:
        // model data
        std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh>    meshes;
        std::string directory;

        // constructor, expects a filepath to a 3D model.
        Model(std::string const &path);

        // draws the model, and thus all its meshes
        void Draw(uint32_t program_id);

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const &path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode *node, const aiScene *scene);

        // unpack mesh structure to our custom mesh structure;
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        void loadMaterialTextures(aiMaterial *mat, aiTextureType type,std::vector<Texture>* textures);

        // load texture image to model;
        uint32_t TextureFromFile(const char *path, const std::string &directory);
    };

}


#endif
