//
// Created by lastkgb on 2016-12-06.
//

#include <jni.h>
#include <android/sensor.h>
#include <android/log.h>

#include <cstdint>
#include <cassert>
#include <string>

#include "position.h"

#define  LOG_TAG    "position"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

const int LOOPER_ID_USER = 3;
//const int SENSOR_HISTORY_LENGTH = 100;
const int SENSOR_REFRESH_RATE_HZ = 100;
constexpr int32_t SENSOR_REFRESH_PERIOD_US = int32_t(1000000 / SENSOR_REFRESH_RATE_HZ);
const float SENSOR_FILTER_ALPHA = 0.1f;

Position::Position() { }

void Position::init()
{
    mSensorManager = ASensorManager_getInstance();
    assert(mSensorManager != NULL);
    mGyro = ASensorManager_getDefaultSensor(mSensorManager, ASENSOR_TYPE_ACCELEROMETER);
    assert(mGyro != NULL);
    looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    assert(looper != NULL);
    mGyroEventQ = ASensorManager_createEventQueue(mSensorManager, looper,
                                                  LOOPER_ID_USER, NULL, NULL);
    assert(mGyroEventQ != NULL);
    auto status = ASensorEventQueue_enableSensor(mGyroEventQ, mGyro);
    assert(status >= 0);
    status = ASensorEventQueue_setEventRate(mGyroEventQ, mGyro, SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
    (void) status;   //to silent unused compiler warning

}

void Position::updateSensor()
{
    ALooper_pollAll(0, NULL, NULL, NULL);
    ASensorEvent event;
    float a = SENSOR_FILTER_ALPHA;
    while (ASensorEventQueue_getEvents(mGyroEventQ, &event, 1) > 0) {
        mSensorDataFilter.x = a * event.uncalibrated_gyro.x_uncalib + (1.0f - a) * mSensorDataFilter.x;
        mSensorDataFilter.y = a * event.uncalibrated_gyro.y_uncalib + (1.0f - a) * mSensorDataFilter.y;
        mSensorDataFilter.z = a * event.uncalibrated_gyro.z_uncalib + (1.0f - a) * mSensorDataFilter.z;
        LOGI("Position [%f, %f, %f]", mSensorDataFilter.x, mSensorDataFilter.y, mSensorDataFilter.z);
    }
    //mSensorDatas[mSensorDataIdx] = mSensorDataFilter;
    //mSensorDatas[SENSOR_HISTORY_LENGTH+mSensorDataIdx] = mSensorDataFilter;
    //mSensorDataIdx = (mSensorDataIdx + 1) % SENSOR_HISTORY_LENGTH;
}

void Position::pause() {
    ASensorEventQueue_disableSensor(mGyroEventQ, mGyro);
}

void Position::resume() {
    ASensorEventQueue_enableSensor(mGyroEventQ, mGyro);
    auto status = ASensorEventQueue_setEventRate(mGyroEventQ,
                                                 mGyro,
                                                 SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
}

float Position::getX()
{
    return mSensorDataFilter.x *5;
}

float Position::getY()
{
    return -mSensorDataFilter.y *5;
}
