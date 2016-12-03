package com.example.lastkgb.mtracker;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

    private TrackerSurfaceView mView;
    private Position mPosition;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new TrackerSurfaceView(this);
        mPosition = new Position(this);
        mPosition.init(mView);

        mView.setPosition(mPosition);
        mPosition.naStart();

        setContentView(mView);
        Log.i("MAIN", "onCreate called");
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
/*
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                RendererJNI.pause();
            }
        });
*/
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
/*
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                RendererJNI.resume();
            }
        });
*/
    }

    public void update(float angleX, float angleY)
    {
        Log.d("MAIN", "update called");
        mView.update(angleX, angleY);
    }

}
