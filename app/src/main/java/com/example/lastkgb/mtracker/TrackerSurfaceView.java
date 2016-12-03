package com.example.lastkgb.mtracker;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

/**
 * Created by lastkgb on 2016-11-22.
 */

public class TrackerSurfaceView extends GLSurfaceView {
    private TrackerRenderer mRenderer;
    private Position mPostion;
    private float mPreviousX;
    private float mPreviousY;
    private final float TOUCH_SCALE_FACTOR = 180.0f/320;

    public TrackerSurfaceView(Context context) {
        super(context);
        mRenderer = new TrackerRenderer();
        setEGLContextClientVersion(2);
        setDebugFlags(DEBUG_LOG_GL_CALLS);
        this.setRenderer(mRenderer);
        this.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public boolean onTouchEvent(final MotionEvent event) {
        float x = event.getX();
        float y = event.getY();
        switch (event.getAction()) {
            case MotionEvent.ACTION_MOVE:
                float dx = x - mPreviousX;
                float dy = y - mPreviousY;
                mRenderer.mAngleX += dx * TOUCH_SCALE_FACTOR;
                mRenderer.mAngleY += dy * TOUCH_SCALE_FACTOR;
                Log.i("TrackerSurfaceView", "calling requestRender by Event");
                requestRender();
                break;
            default:
                // print some information.
                break;
        }
        mPreviousX = x;
        mPreviousY = y;

        return true;
    }

    public void onPositionUpdated(float x, float y, float z) {
        Log.i("TrackerSurfaceView", "onPoitionUpdated called");
        mRenderer.mAngleX += x;
        mRenderer.mAngleX += y;
        Log.i("TrackerSurfaceView", "calling requestRender by position");
        requestRender();

        mPreviousX = x;
        mPreviousY = y;
    }

    public void update(float angleX, float angleY) {
        Log.i("TrackerSurfaceView", "update called");
        mRenderer.mAngleX = angleX;
        mRenderer.mAngleY = angleY;
        Log.i("TrackerSurfaceView", "calling requestRender by update");
        requestRender();
    }

    public TrackerRenderer getRenderer() {
        return mRenderer;
    }

    public void setPosition(Position position) {
        mPostion = position;
    }
}