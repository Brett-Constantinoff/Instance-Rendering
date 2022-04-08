#include "Utils.h"
#include "../libs/stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>

namespace Utils{

    unsigned int genTexture(unsigned int target){
        unsigned int textureID;
        glGenTextures(1, &textureID);
        return textureID;
    }

    unsigned int genBuffer( void ){
        unsigned int bufferID;
        glGenBuffers(1, &bufferID);
        return bufferID;
    }

    unsigned int genVertexArray( void ){
        unsigned int vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        return vertexArrayID;
    }

    unsigned int loadCubeMap( void ){
        std::vector<std::string> files = {
            "src/res/right.jpg",
            "src/res/left.jpg",
            "src/res/top.jpg",
            "src/res/bottom.jpg",
            "src/res/front.jpg",
            "src/res/back.jpg"
        };

        unsigned int textureID = Utils::genTexture(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width;
        int height;
        int channels;
        for(int i = 0; i < files.size(); i++){
            unsigned char* fileData = stbi_load(files[i].c_str(), &width, &height, &channels, 0);
            if(fileData){
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, fileData);
            }
            else{
                std::cout << "File failed to load: " << files[i] << std::endl;
            }
            stbi_image_free(fileData);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}