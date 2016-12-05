//
// Created by lastkgb on 2016-12-06.
//

#ifndef MTRACKER_POSITION2_H
#define MTRACKER_POSITION2_H

#include <jni.h>
#include <android/sensor.h>

const int SENSOR_HISTORY_LENGTH = 100;

class Position {
    ASensorManager *mSensorManager;
    const ASensor *mGyro;
    //const ASensor *mAccel;
    ASensorEventQueue *mGyroEventQ;
    //ASensorEventQueue *mAccelEventQ;
    ALooper *looper;

    //float xPos[SENSOR_HISTORY_LENGTH];

    struct SensorData {
        float x;
        float y;
        float z;
    };
   // SensorData mSensorDatas[SENSOR_HISTORY_LENGTH*2];
    SensorData mSensorDataFilter;
    //int mSensorDataIdx;

    //JNIEnv *mJEnv;
    //jmethodID positionUpdate;

public:
    Position();

    void init();
    void updateSensor();
    void pause();
    void resume();
    float getX();
    float getY();

};

#endif //MTRACKER_POSITION2_H
