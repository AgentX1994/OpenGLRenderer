//
//  program.hpp
//  OpenGLThing
//
//  Created by John Asper on 2016/3/84.
//  Copyright © 2016 John Asper. All rights reserved.
//

#ifndef program_hpp
#define program_hpp

#include "main.hpp"

class Program {
private:
    unsigned int id;
    
    unsigned int shaderID;
    unsigned int shaderVP;
    unsigned int shaderFP;
    
    void init(const char *vsFile, const char *fsFile);
    
protected:
    
public:
    // Generic Constructor
    Program();
    
    // Constructs a shader that uses the given shaders
    Program(const char *vsFile, const char *fsFile);
    
    // Copy Constructor. This class cannot be copied so it cannot be defined
    Program(const Program &s);
    
    ~Program();
    
    // Use this shader when rendering
    void use();
    
    // Stop using this shader when rendering
    void unuse();
    
    unsigned int getID();
    
};

#endif /* program_hpp */
