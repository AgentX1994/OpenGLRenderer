//
//  utils.cpp
//  OpenGLThing
//
//  Created by John Asper on 2016/3/84.
//  Copyright © 2016 John Asper. All rights reserved.
//

#include "utils.hpp"

char *readSource(const char *file){
    char *text;
    if(file != NULL){
        FILE *f = fopen(file, "r");
        
        if (f != NULL){
            fseek(f, 0, SEEK_END);
            long len = ftell(f);
            rewind(f);
            
            if (len > 0) {
                text = new char[len+1];
                len = fread(text, sizeof(char), len, f);
                text[len] = 0;
            }
            fclose(f);
        }
    }
    return text;
}

void mat4PrettyPrint(glm::mat4 m){
    std::cout << "[" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "[";
        for (int j = 0; j < 4; j++) {
            std::cout << *(&m[i][j]);
            if (j < 3) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

void vec3PrettyPrint(glm::vec3 v){
    std::cout << "(";
    for (int i = 0; i < 3; i++){
        std::cout << v[i];
        if (i < 2) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}

void glCheckErrors(std::string loc){
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR){
        std::cout << "OPEN GL ERROR at location " << loc << "!" << std::endl;
        switch (err) {
            case GL_INVALID_ENUM:
                std::cout << "\tInvalid Enum";
                break;
            case GL_INVALID_VALUE:
                std::cout << "\tInvalid Value";
                break;
            case GL_INVALID_OPERATION:
                std::cout << "\tInvalid Operation";
                break;
            case GL_OUT_OF_MEMORY:
                std::cout << "\tOut of Memory Error";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cout << "\tInvalid Framebuffer Operation";
                break;
            default:
                std::cout << "\tUnknown Error!";
                break;
        }
        std::cout << std::endl << std::endl;
    }
}