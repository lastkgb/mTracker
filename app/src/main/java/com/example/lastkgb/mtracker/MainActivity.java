package com.example.lastkgb.mtracker;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

    private TrackerSurfaceView mView;
    private TrackerRenderer mRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new TrackerSurfaceView(this);
        mRenderer = mView.getRenderer();
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Log.i("MAIN", "[onCreate] Runnable run called");
                mRenderer.positionInit();
            }
        });
        setContentView(mView);
        Log.i("MAIN", "onCreate called");
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.pause();
            }
        });
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.resume();
            }
        });
    }

    public void update(float angleX, float angleY)
    {
        Log.d("MAIN", "update called");
        mView.update(angleX, angleY);
    }

}
