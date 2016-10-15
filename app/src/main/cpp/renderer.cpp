//
// Created by lastkgb on 2016-10-13.
//

#include <jni.h>
#include <GLES2/gl2.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>

#include "renderer.h"

auto gVertexShader =
        "attribute vec4 vPosition;\n"
                "void main() {\n"
                "  gl_Position = vPosition;\n"
                "}\n";

auto gFragmentShader =
        "precision mediump float;\n"
                "void main() {\n"
                "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                "}\n";

const GLfloat cube[] = {
        -0.5f, 0.5f, 0f,
        -0.5f, -0.5f, 0f,
        0.5f, -0.5f, 0f,
        0.5f, 0.5f, 0f

};

const short drawOrder[] = {0, 1, 2, 3};

//void renderer::glInit(AAssetManager *assetManager) {
void renderer::glInit() {

}

void renderer::surfaceCreated() {
    LOGI("GL_VERSION: %s", glGetString(GL_VERSION));
    LOGI("GL_VENDOR: %s", glGetString(GL_VENDOR));
    LOGI("GL_RENDERER: %s", glGetString(GL_RENDERER));
    LOGI("GL_EXTENSIONS: %s", glGetString(GL_EXTENSIONS));


}

void renderer::render() {

}
renderer gRenderer;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_RendererJNI_init(
            JNIEnv *env, jclass type, jobject assetManager) {
        (void) type;
        AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager);
        gRenderer.init(nativeAssetManager);
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