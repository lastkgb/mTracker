//
// Created by lastkgb on 2016-10-13.
//

#include <jni.h>

#include <android/log.h>
#include <android/sensor.h>

#include <cstdint>
#include <cassert>
#include <string>

#include "accelerometer.h"

#define  LOG_TAG    "accelerometer"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

const int LOOPER_ID_USER = 3;
const int SENSOR_REFRESH_RATE_HZ = 100;
constexpr int32_t SENSOR_REFRESH_PERIOD_US = int32_t(1000000 / SENSOR_REFRESH_RATE_HZ);
const float SENSOR_FILTER_ALPHA = 0.1f;

void accelerometer::init() {
    LOGI("Accelerometer Init");
    sensorManager = ASensorManager_getInstance();
    assert(sensorManager != NULL);
    accel = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    assert(accel != NULL);
    looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    assert(looper != NULL);
    accelEventQueue = ASensorManager_createEventQueue(sensorManager, looper,
                                                     LOOPER_ID_USER, NULL, NULL);
    assert(accelEventQueue != NULL);
    auto status = ASensorEventQueue_enableSensor(accelEventQueue, accel);
    assert(status >= 0);
    status = ASensorEventQueue_setEventRate(accelEventQueue, accel, SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
    (void)status;
}

void accelerometer::update() {
    ALooper_pollAll(0, NULL, NULL, NULL);
    ASensorEvent event;
    float a = SENSOR_FILTER_ALPHA;
    while (ASensorEventQueue_getEvents(accelEventQueue, &event, 1) > 0) {
        sensorDataFilter.x = a * event.acceleration.x + (1.0f - a) * sensorDataFilter.x;
        sensorDataFilter.y = a * event.acceleration.y + (1.0f - a) * sensorDataFilter.y;
        sensorDataFilter.z = a * event.acceleration.z + (1.0f - a) * sensorDataFilter.z;
    }
    sensorData[sensorDataIndex] = sensorDataFilter;
    sensorData[SENSOR_HISTORY_LENGTH+sensorDataIndex] = sensorDataFilter;
    sensorDataIndex = (sensorDataIndex + 1) % SENSOR_HISTORY_LENGTH;
}

void accelerometer::pause() {
    ASensorEventQueue_disableSensor(accelEventQueue, accel);
}

void accelerometer::resume() {
    ASensorEventQueue_enableSensor(accelEventQueue, accel);
    auto status = ASensorEventQueue_setEventRate(accelEventQueue, accel, SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
}

accelerometer gAccelerometer;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_accelinit(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gAccelerometer.init();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_accelpause(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gAccelerometer.pause();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_accelresume(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gAccelerometer.resume();
    }
}