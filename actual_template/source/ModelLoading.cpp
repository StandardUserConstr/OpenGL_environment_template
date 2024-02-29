#include "../include/ModelLoading.h"

namespace ModelLoading
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    void Mesh::Draw(uint32_t program_id)
    {
        static uint8_t resulting_string[32];
        // bind appropriate textures
        uint32_t ambientNr  = 0;
        uint32_t diffuseNr  = 0;
        uint32_t specularNr = 0;
        for(uint32_t i = 0; i!=textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0+i); // active proper texture unit before binding
            glBindTexture(GL_TEXTURE_2D,textures[i].id);

            switch(textures[i].type)
            {
                case aiTextureType_AMBIENT:
                    {
                        sprintf((char*)resulting_string,"material.ambient[%u]",ambientNr);
                        ambientNr++;
                    }
                break;

                case aiTextureType_DIFFUSE:
                    {
                        sprintf((char*)resulting_string,"material.diffuse[%u]",diffuseNr);
                        diffuseNr++;

                    }
                break;

                case aiTextureType_SPECULAR:
                    {
                        sprintf((char*)resulting_string,"material.specular[%u]",specularNr);
                        specularNr++;
                    }
                break;

                default: break;

            }

            // set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(program_id,(const char*)resulting_string),i);

        }

        glUniform1i(glGetUniformLocation(program_id,"NR_OF_MATERIAL_AMBIENT"),ambientNr);
        glUniform1i(glGetUniformLocation(program_id,"NR_OF_MATERIAL_DIFFUSE"),diffuseNr);
        glUniform1i(glGetUniformLocation(program_id,"NR_OF_MATERIAL_SPECULAR"),specularNr);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,(uint32_t)indices.size(),GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        return;
    }

    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        return;
    }

//================================================================================================================================================================================================

    Model::Model(std::string const &path)
    {
        loadModel(path);
        return;
    }

    void Model::Draw(uint32_t program_id)
    {
        for(uint32_t i = 0; i!=meshes.size(); i++) meshes[i].Draw(program_id);
        return;
    }

    void Model::loadModel(std::string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            fprintf(stderr,"ERROR::ASSIMP: %s\n    file: %s | line: %d\n",importer.GetErrorString(),__FILE__,__LINE__);
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0,path.find_last_of('/')+1);

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
        return;
    }

    void Model::processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(uint32_t i = 0; i!=node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh,scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(uint32_t i = 0; i!=node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        return;
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(uint32_t i = 0; i!=mesh->mNumVertices; i++)
        {
            Vertex vertex;

            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            if (mesh->HasNormals())
            {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }

            if(mesh->mTextureCoords[0]) // if contains textures;
            {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(uint32_t i = 0; i!=mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(uint32_t j = 0; j!=face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        loadMaterialTextures(material, aiTextureType_AMBIENT,&textures);
        loadMaterialTextures(material, aiTextureType_DIFFUSE,&textures);
        loadMaterialTextures(material, aiTextureType_SPECULAR,&textures);

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,std::vector<Texture>* textures)
    {
        Texture texture;
        aiString str;
        for(uint32_t i = 0; i!=mat->GetTextureCount(type); i++)
        {
            mat->GetTexture(type,i,&str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = 0;
            for(uint32_t j = 0; j!=textures_loaded.size(); j++)
            {
                if(strcmp(textures_loaded[j].path.data(),str.C_Str())==0)
                {
                    textures->push_back(textures_loaded[j]);
                    skip = 1; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(skip==0)
            {   // if texture hasn't been loaded already, load it
                texture.id = TextureFromFile(str.C_Str(),this->directory);
                texture.type = type;
                texture.path = str.C_Str();
                textures->push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return;
    }

    uint32_t Model::TextureFromFile(const char *path, const std::string &directory)
    {
        stbi_set_flip_vertically_on_load(1);    // tell stb_image.h to flip loaded texture's on the y-axis.

        std::string filename = directory + std::string(path);

        uint32_t textureID;
        glGenTextures(1, &textureID);

        int width, height, nr_channels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nr_channels, 0);

        if (data)
        {
            GLenum format;
            if (nr_channels == 3) {format = GL_RGB; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}
            else if (nr_channels == 4) {format = GL_RGBA; glPixelStorei(GL_UNPACK_ALIGNMENT,4);}
            else if (nr_channels==2) {format = GL_RG; glPixelStorei(GL_UNPACK_ALIGNMENT,2);}
            else {format = GL_RED; glPixelStorei(GL_UNPACK_ALIGNMENT,1);}

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            fprintf(stderr,"failed to load texture: %s\n    file: %s | line: %d\n",filename.c_str(),__FILE__,__LINE__);
        }

        return textureID;
    }

}
