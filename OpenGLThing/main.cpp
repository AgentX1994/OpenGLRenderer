//
//  main.cpp
//  OpenGLThing
//
//  Created by John Asper on 2016/3/84.
//  Copyright © 2016 John Asper. All rights reserved.
//

#include "main.hpp"
#include "program.hpp"

void error_callback(int error, const char* description){
    std::cerr << "ERROR: " << description << std::endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    switch(key){
            case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
    }
}

int main(int argc, const char * argv[]) {
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(800,600, "OpenGL Testing", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwSwapInterval(1);
    
    Program shader(
                   "/Users/john/Documents/Xcode Projects/OpenGLThing/OpenGLThing/default.vert",
                   "/Users/john/Documents/Xcode Projects/OpenGLThing/OpenGLThing/default.frag");
    glBindFragDataLocation(shader.getID(), 0, "outColor");
    
    Program mot_blur(
                     "/Users/john/Documents/Xcode Projects/OpenGLThing/OpenGLThing/post_process.vert",
                     "/Users/john/Documents/Xcode Projects/OpenGLThing/OpenGLThing/motion_blur.frag");
    
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
     
    float colors[] = {
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,2,1,3
    };
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(float), vertices, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(shader.getID(), "vert_position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(float), colors, GL_STATIC_DRAW);
    GLint colAttrib = glGetAttribLocation(shader.getID(), "in_color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    GLuint texCoordBuffer;
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2*4*sizeof(float), texCoords, GL_STATIC_DRAW);
    GLint texAttrib = glGetAttribLocation(shader.getID(), "texCoords");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    glCheckErrors("Before Loop");
    
    glClearColor(0.25f, 0.25f, 0.25f, 1);
    
    GLuint frames[3];
    
    glGenTextures(3, frames);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glCheckErrors("before viewport");
    glViewport(0, 0, width, height);
    
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, *(frames+i));
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    
    double last_time = glfwGetTime();
    int nFrames = 0;
    int i = 0;
    while(!glfwWindowShouldClose(window)){
        // Keep Running
        glfwPollEvents();
        
        double current_time = glfwGetTime();
        nFrames++;
        if (current_time - last_time >= 1.0) {
            std::cout << 1000.0/double(nFrames) << " ms per frame, " << nFrames << " fps" << std::endl;
            nFrames = 0;
            last_time = current_time;
        }
        
        glfwGetFramebufferSize(window, &width, &height);
        glCheckErrors("before viewport");
        glViewport(0, 0, width, height);
        
        float ratio = width / (float)height;
        
        glCheckErrors("before shader use");
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frames[i%3], 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: FRAMEBUFFER NOT COMPLETE" << std::endl;
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.use();
        
        glm::mat4 proj = glm::perspective(60.f, ratio, 0.1f, 100.f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f,0.f,-1.f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(glfwGetTime()*5.0f)*5.0f, sin(glfwGetTime()*5.0f)*5.0f, 0.0f)) *
                        glm::rotate(glm::mat4(1.0f), (float)glfwGetTime()*2.0f, glm::vec3(0,0,1)) *
                        glm::scale(glm::mat4(1),glm::vec3(2));
        
        int projectionMatrixLocation = glGetUniformLocation(shader.getID(), "mProj"); // Get the location of our projection matrix in the shader
        int viewMatrixLocation = glGetUniformLocation(shader.getID(), "mView"); // Get the location of our view matrix in the shader
        int worldMatrixLocation = glGetUniformLocation(shader.getID(), "mWorld"); // Get the location of our world matrix in the shader
        
        glCheckErrors("After uniform location");
        
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &proj[0][0]); // Send our projection matrix to the shader
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &view[0][0]); // Send our view matrix to the shader
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &model[0][0]); // Send our model matrix to the shader
        
        glCheckErrors("After setting Uniforms");
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        
        glCheckErrors("After Binding");
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        shader.unuse();
        
        glCheckErrors("After unuse");
        
        mot_blur.use();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glCheckErrors("After clear2");
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frames[i%3]);
        glUniform1i(glGetUniformLocation(mot_blur.getID(), "frame1"), 0);
        
        glCheckErrors("After frame1");
        
        if(i > 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, frames[(i-1)%3]);
            glUniform1i(glGetUniformLocation(mot_blur.getID(), "frame2"), 1);
        } else {
            glUniform1i(glGetUniformLocation(mot_blur.getID(), "frame2"), 0);
            glCheckErrors("in else statement");
        }
        glCheckErrors("After frame2");
        if(i > 1) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, frames[(i-2)%3]);
            glUniform1i(glGetUniformLocation(mot_blur.getID(), "frame3"), 2);
        } else {
            glUniform1i(glGetUniformLocation(mot_blur.getID(), "frame3"), 0);
        }
        glCheckErrors("After frame3");
        
        glUniform1f(glGetUniformLocation(mot_blur.getID(), "w1"), 0.7f);
        glUniform1f(glGetUniformLocation(mot_blur.getID(), "w2"), 0.3f);
        glUniform1f(glGetUniformLocation(mot_blur.getID(), "w3"), 0.2f);
        
        glCheckErrors("After weights");
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        mot_blur.unuse();
        
        glBindVertexArray(0);
        glCheckErrors("After Draw");
        
        i++;
        
        glfwSwapBuffers(window);
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
