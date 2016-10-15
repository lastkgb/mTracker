package com.example.lastkgb.mtracker;

import android.app.Activity;
//import android.opengl.EGLConfig;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {

    private GLSurfaceView mView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new GLSurfaceView(this);
        mView.setEGLContextClientVersion(2);
        mView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                RendererJNI.surfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                RendererJNI.surfaceChanged(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                RendererJNI.drawFrame();
            }
        });
        // Render the view only when there is a change in the drawing data
        mView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                RendererJNI.glInit(getAssets());
            }
        });
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                RendererJNI.pause();
            }
        });
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                RendererJNI.resume();
            }
        });
    }

}
