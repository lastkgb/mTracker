package com.example.lastkgb.mtracker;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by lastkgb on 2016-11-22.
 */

public class TrackerRenderer implements GLSurfaceView.Renderer {
    public float mAngleX;
    public float mAngleY;


/*
    public TrackerRenderer(Context context) {
        super();
        mContext = context;
    }
*/
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        naInitGL20();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {

        naDrawFrame(mAngleX, mAngleY);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        naSurfaceChanged(width, height);
    }

    private static native void naInitGL20();
    private static native void naDrawFrame(float angleX, float angleY);
    private static native void naSurfaceChanged(int width, int height);

    static {
        System.loadLibrary("cube");
    }
}
