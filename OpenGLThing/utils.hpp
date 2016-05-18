//
//  utils.hpp
//  OpenGLThing
//
//  Created by John Asper on 2016/3/84.
//  Copyright © 2016 John Asper. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include "main.hpp"

char *readSource(const char *file);

void mat4PrettyPrint(glm::mat4 m);

void vec3PrettyPrint(glm::vec3 v);

void glCheckErrors(std::string loc);

#endif /* utils_hpp */
