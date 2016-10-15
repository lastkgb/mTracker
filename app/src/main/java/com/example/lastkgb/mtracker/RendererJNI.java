package com.example.lastkgb.mtracker;

import android.content.res.AssetManager;

/**
 * Created by lastkgb on 2016-10-14.
 */

public class RendererJNI {

    static {
        System.loadLibrary("render");
    }

    public static native void glInit(AssetManager assetManager);
    public static native void surfaceCreated();
    public static native void surfaceChanged(int width, int height);
    public static native void drawFrame();
    public static native void pause();
    public static native void resume();
}
