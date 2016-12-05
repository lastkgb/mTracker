//
// Created by lastkgb on 2016-11-23.
//

#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "cube.h"
#include "matrix.h"
#include "mylog.h"
#include "position.h"

auto gVertexShader =
    "attribute vec4 vPosition;\n"
    "uniform mat4 mvp;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mvp * vPosition;\n"
    "}\n";

auto gFragmentShader =
    "void main() \n"
    "{ \n"
    "    gl_FragColor = vec4(0.0,0.5,0.0,1.0); \n"
    "}\n";

GLfloat vertices[108] = {
		-1.0,-1.0,1.0,  1.0,1.0,1.0,   1.0,-1.0,1.0,    //465
		-1.0,-1.0,1.0,  -1.0,1.0,1.0,  1.0,1.0,1.0,     //476
		1.0,-1.0,-1.0,   -1.0,1.0,-1.0,   -1.0,-1.0,-1.0,//130
		1.0,-1.0,-1.0,   1.0,1.0,-1.0,    -1.0,1.0,-1.0, //123
		1.0,-1.0,1.0,   1.0,1.0,-1.0,   1.0,-1.0,-1.0,   //521
		1.0,-1.0,1.0,   1.0,1.0,1.0,    1.0,1.0,-1.0,    //562
		-1.0,-1.0,-1.0,  -1.0,1.0,1.0,  -1.0,-1.0,1.0,   //074
		-1.0,-1.0,-1.0,  -1.0,1.0,-1.0, -1.0,1.0,1.0,    //037
		-1.0,-1.0,-1.0,   1.0,-1.0,1.0,   1.0,-1.0,-1.0, //051
		-1.0,-1.0,-1.0,  -1.0,-1.0,1.0,   1.0,-1.0,1.0,  //045
		-1.0,1.0,1.0,    1.0,1.0,-1.0,  1.0,1.0,1.0,    //726
		-1.0,1.0,1.0,   -1.0,1.0,-1.0,  1.0,1.0,-1.0  //732
};
GLbyte indices[6] = {
	0, 1, 2, 3, 4, 5
};

Cube mCube(vertices, indices);

static void printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI(4, "GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error
													= glGetError()) {
		LOGI(4, "after %s() glError (0x%x)\n", op, error);
	}
}

GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE(1, "Could not compile shader %d:\n%s\n",
						 shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			} else {
				LOGE(1, "infoLen is zero");
			}
		} else {
			LOGE(1, "Shader already compiled");
		}
	} else {
		LOGE(1, "Failed to create GL Shader");
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		LOGE(1, "Could not load Vertex Shader.");
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		LOGE(1, "Could not load Pixel Shader.");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE(1, "Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

int setupShaders() {
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

//	LOGI(4, "setupGraphics(%d, %d)", w, h);
	gProgram = createProgram(gVertexShader, gFragmentShader);
	if (!gProgram) {
		LOGE(1, "Could not create program.");
		return -1;
	}
	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	LOGI(4, "glGetAttribLocation(\"vPosition\") = %d\n",
		 gvPositionHandle);

	//glViewport(0, 0, w, h);
	//checkGlError("glViewport");
	return 0;
}

Position gPosition;

GLuint gmvP;
int gWidth, gHeight;
int iXangle = 0, iYangle = 0, iZangle = 0;
float nAnglex = 0.0, nAngley = 0.0;
float aRotate[16], aScale[16], aTranslate[16], aModelView[16], aPerspective[16], aMVP[16];

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naInitGL20(JNIEnv *env, jclass clazz) {
	glDisable(GL_DITHER);	//disable dither to improve performance with reduced quality
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//set clear value for color buffer as black
//	glEnable(GL_CULL_FACE);		//enabled for better performance
	glClearDepthf(1.0f);	//set clear value [0, 1] for depth buffer as farthest
	glEnable(GL_DEPTH_TEST);	//do depth comparison and update depth buffer
	glDepthFunc(GL_LEQUAL);		//type of depth test

	int res = setupShaders();
	if (res < 0) {
		LOGE(1, "Failed to setup Shader");
		return;
	}

	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	LOGI(1, "glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);

	gmvP = glGetUniformLocation(gProgram, "mvp");
	LOGI(1, "glGetUniformLocation(\"mvp\") = %d\n", gmvP);
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naSurfaceChanged(JNIEnv* env, jclass clazz, int width, int height) {
	glViewport(0, 0, width, height);
	gWidth = width;
	gHeight = height;
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naDrawFrame(JNIEnv* env, jclass clazz, float pAngleX, float pAngleY) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glUseProgram(gProgram);

//	GL1x: glRotatef(pAngleX, 0, 1, 0);	//rotate around y-axis
//	GL1x: glRotatef(pAngleY, 1, 0, 0);	//rotate around x-axis
	//rotate
	rotate_matrix(pAngleX, 0.0, 1.0, 0.0, aRotate);
	rotate_matrix(pAngleY, 1.0, 0.0, 0.0, aModelView);
	multiply_matrix(aRotate, aModelView, aModelView);

//	GL1x: glScalef(0.3f, 0.3f, 0.3f);      // Scale down
	scale_matrix(0.5, 0.5, 0.5, aScale);
	multiply_matrix(aScale, aModelView, aModelView);

// GL1x: glTranslate(0.0f, 0.0f, -3.5f);
	translate_matrix(0.0f, 0.0f, -3.5f, aTranslate);
	multiply_matrix(aTranslate, aModelView, aModelView);

//	gluPerspective(45, aspect, 0.1, 100);
	perspective_matrix(45.0, (float)gWidth/(float)gHeight, 0.1, 100.0, aPerspective);
	multiply_matrix(aPerspective, aModelView, aMVP);

	glUniformMatrix4fv(gmvP, 1, GL_FALSE, aMVP);

	mCube.draw(gvPositionHandle);
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naDrawFrame2(JNIEnv* env, jclass clazz) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glUseProgram(gProgram);

//	GL1x: glRotatef(pAngleX, 0, 1, 0);	//rotate around y-axis
//	GL1x: glRotatef(pAngleY, 1, 0, 0);	//rotate around x-axis
	//rotate
	rotate_matrix(gPosition.getX(), 0.0, 1.0, 0.0, aRotate);
	rotate_matrix(gPosition.getY(), 1.0, 0.0, 0.0, aModelView);
	multiply_matrix(aRotate, aModelView, aModelView);

//	GL1x: glScalef(0.3f, 0.3f, 0.3f);      // Scale down
	scale_matrix(0.5, 0.5, 0.5, aScale);
	multiply_matrix(aScale, aModelView, aModelView);

// GL1x: glTranslate(0.0f, 0.0f, -3.5f);
	translate_matrix(0.0f, 0.0f, -3.5f, aTranslate);
	multiply_matrix(aTranslate, aModelView, aModelView);

//	gluPerspective(45, aspect, 0.1, 100);
	perspective_matrix(45.0, (float)gWidth/(float)gHeight, 0.1, 100.0, aPerspective);
	multiply_matrix(aPerspective, aModelView, aMVP);

	glUniformMatrix4fv(gmvP, 1, GL_FALSE, aMVP);

	mCube.draw(gvPositionHandle);
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naPositionInit(JNIEnv* env, jclass clazz) {
	gPosition.init();
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naPositionUpdate(JNIEnv* env, jclass clazz) {
	gPosition.updateSensor();
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naPositionPause(JNIEnv* env, jclass clazz) {
	gPosition.pause();
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_TrackerRenderer_naPositionResume(JNIEnv* env, jclass clazz) {
	gPosition.resume();
}
}
