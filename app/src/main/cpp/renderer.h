//
// Created by lastkgb on 2016-10-13.
//

#ifndef MTRACKER_RENDER_H
#define MTRACKER_RENDER_H

#include <jni.h>
#include <GLES2/gl2.h>

#include <cstdint>
#include <string>

class renderer {
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    GLuint shaderProgram;
    GLuint vPositionHandle;
    GLuint vSensorValueHandle;
    GLuint uFragColorHandle;

public:
    renderer() {};

    void init();
    void render();
    void surfaceCreated();
    void surfaceChanged(int w, int h);
    void update();
};


#endif //MTRACKER_RENDER_H
