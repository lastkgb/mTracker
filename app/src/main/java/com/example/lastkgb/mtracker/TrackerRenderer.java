package com.example.lastkgb.mtracker;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by lastkgb on 2016-11-22.
 */

public class TrackerRenderer implements GLSurfaceView.Renderer {
    public float mAngleX;
    public float mAngleY;


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        Log.i("Renderer", "onSurfaceCreated called");
        naInitGL20();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        Log.i("Renderer", "onDrawFrame called");
        naPositionUpdate();
        //naDrawFrame(mAngleX, mAngleY);
        naDrawFrame2();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        naSurfaceChanged(width, height);
    }

    public void positionInit() {
        Log.i("Renderer", "positionInit called");
        naPositionInit();
    }

    public void pause() {
        naPositionPause();
    }
    public void resume() {
        naPositionResume();
    }

    private static native void naInitGL20();
    private static native void naDrawFrame(float angleX, float angleY);
    private static native void naSurfaceChanged(int width, int height);
    private static native void naPositionInit();
    private static native void naPositionUpdate();
    private static native void naPositionPause();
    private static native void naPositionResume();
    private static native void naDrawFrame2();

    static {
        System.loadLibrary("cube");
    }
}
