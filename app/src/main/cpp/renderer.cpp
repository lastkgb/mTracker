//
// Created by lastkgb on 2016-10-13.
//

#include <jni.h>
#include <GLES2/gl2.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>

#include <cassert>

#include "renderer.h"

const GLfloat cube[] = {
        -0.5f, 0.5f, 0f,
        -0.5f, -0.5f, 0f,
        0.5f, -0.5f, 0f,
        0.5f, 0.5f, 0f

};

const short drawOrder[] = {0, 1, 2, 3};


GLuint loadShader(GLenum shaderType, const std::string& pSource) {
    GLuint shader = glCreateShader(shaderType);
    assert(shader != 0);
    const char *sourceBuf = pSource.c_str();
    glShaderSource(shader, 1, &sourceBuf, NULL);
    glCompileShader(shader);
    GLint shaderCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
    assert(shaderCompiled != 0);
    return shader;
}

GLuint createProgram(const std::string& pVertexSource, const std::string& pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    GLuint program = glCreateProgram();
    assert(program != 0);
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
    glLinkProgram(program);
    GLint programLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
    assert(programLinked != 0);
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
    return program;
}

void renderer::init() {
    vertexShaderSource =
            "attribute vec4 vPosition;\n"
                    "void main() {\n"
                    "  gl_Position = vPosition;\n"
                    "}\n";
    fragmentShaderSource =
            "precision mediump float;\n"
                    "void main() {\n"
                    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                    "}\n";
}

void renderer::surfaceCreated() {
    LOGI("GL_VERSION: %s", glGetString(GL_VERSION));
    LOGI("GL_VENDOR: %s", glGetString(GL_VENDOR));
    LOGI("GL_RENDERER: %s", glGetString(GL_RENDERER));
    LOGI("GL_EXTENSIONS: %s", glGetString(GL_EXTENSIONS));

    shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    assert(shaderProgram != 0);
}

void renderer::render() {

}

void renderer::surfaceChanged(int w, int h) {
    glViewport(0, 0, w, h);
}

void renderer::update() {

}

renderer gRenderer;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_RendererJNI_init(JNIEnv *env, jclass type) {
        (void) type;
        gRenderer.init();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_RendererJNI_surfaceCreated(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gRenderer.surfaceCreated();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_RendererJNI_surfaceChanged(
            JNIEnv *env, jclass type, jint width, jint height) {
        (void) env;
        (void) type;
        gRenderer.surfaceChanged(width, height);
    }


    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_RendererJNI_drawFrame(
            JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gRenderer.update();
        gRenderer.render();
    }
}