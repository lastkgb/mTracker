//
// Created by lastkgb on 2016-10-13.
//
#ifndef MTRACKER_GYROSCOPE_H
#define MTRACKER_GYROSCOPE_H

#include <android/sensor.h>

#include <cassert>

#define SENSOR_HISTORY_LENGTH 100

class gyroscope {

    ASensorManager *sensorManager;
    const ASensor *gyro;
    ASensorEventQueue *gyroEventQueue;
    ALooper *looper;

    struct GyroData {
        float x;
        float y;
        float z;
    };
    GyroData sensorData[SENSOR_HISTORY_LENGTH*2];
    GyroData sensorDataFilter;
    int sensorDataIndex;

public:

    gyroscope() : sensorDataIndex(0) {}

    void init();
    void update();
    void pause();
    void resume();
};

#endif //MTRACKER_GYROSCOPE_H
