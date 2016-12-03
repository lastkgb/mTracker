package com.example.lastkgb.mtracker;

import android.content.Context;

/**
 * Created by lastkgb on 2016-11-26.
 */

public class Position {

    private TrackerSurfaceView mView;

    public Position(Context context)
    {
        naInit();
    }

/*
    public void onPositionUpdated(float x, float y, float z)
    {
        mView.getRenderer().mAngleX = x;
        mView.getRenderer().mAngleY = y;
        mView.requestRender();
    }
*/

    public void init(TrackerSurfaceView view)
    {
        mView = view;
        naInit();
    }



    public static native void naInit();
    public static native void naStart();

    static {
        System.loadLibrary("position");
    }
}
