package com.example.lastkgb.mtracker;

/**
 * Created by lastkgb on 2016-10-14.
 */

public class SixdofJNI {

    static {
        System.loadLibrary("position");
        System.loadLibrary("orientation");
        System.loadLibrary("accelerometer");
        System.loadLibrary("gyroscope");
    }

    public static native void gyroinit();
//    public static native void gyroupdate();
    public static native void gyropause();
    public static native void gyroresume();

    public static native void accelinit();
//    public static native void accelupdate();
    public static native void accelpause();
    public static native void accelresume();

}
