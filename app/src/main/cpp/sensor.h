//
// Created by lastkgb on 2016-10-13.
//
#ifndef MTRACKER_SENSOR_H
#define MTRACKER_SENSOR_H

#include <android/sensor.h>

#include <cassert>

#define SENSOR_HISTORY_LENGTH 100

class sensor {

    ASensorManager *sensorManager;
    const ASensor *sensor;
    ASensorEventQueue *sensorEventQueue;
    ALooper *looper;
    int sensorType;

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

#endif //MTRACKER_SENSOR_H
