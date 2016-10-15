//
// Created by lastkgb on 2016-10-13.
//

#include <jni.h>
#include <android/log.h>

#include <cstdint>
#include <cassert>
#include <string>

#include "gyroscope.h"

#define  LOG_TAG    "gyroscope"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

const int LOOPER_ID_USER = 3;
const int SENSOR_REFRESH_RATE_HZ = 100;
constexpr int32_t SENSOR_REFRESH_PERIOD_US = int32_t(1000000 / SENSOR_REFRESH_RATE_HZ);
const float SENSOR_FILTER_ALPHA = 0.1f;

void gyroscope::init() {
    LOGI("Gyroscope init");
    sensorManager = ASensorManager_getInstance();
    assert(sensorManager != NULL);
    gyro = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
    assert(gyro != NULL);
    looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    assert(looper != NULL);
    gyroEventQueue = ASensorManager_createEventQueue(sensorManager, looper,
                                                     LOOPER_ID_USER, NULL, NULL);
    assert(gyroEventQueue != NULL);
    auto status = ASensorEventQueue_enableSensor(gyroEventQueue, gyro);
    assert(status >= 0);
    status = ASensorEventQueue_setEventRate(gyroEventQueue, gyro, SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
    (void)status;
}

void gyroscope::update() {
    ALooper_pollAll(0, NULL, NULL, NULL);
    ASensorEvent event;
    float a = SENSOR_FILTER_ALPHA;
    while (ASensorEventQueue_getEvents(gyroEventQueue, &event, 1) > 0) {
        sensorDataFilter.x = a * event.uncalibrated_gyro.x_uncalib + (1.0f - a) * sensorDataFilter.x;
        sensorDataFilter.y = a * event.uncalibrated_gyro.y_uncalib + (1.0f - a) * sensorDataFilter.y;
        sensorDataFilter.z = a * event.uncalibrated_gyro.z_uncalib + (1.0f - a) * sensorDataFilter.z;
    }
    sensorData[sensorDataIndex] = sensorDataFilter;
    sensorData[SENSOR_HISTORY_LENGTH+sensorDataIndex] = sensorDataFilter;
    sensorDataIndex = (sensorDataIndex + 1) % SENSOR_HISTORY_LENGTH;
}

void gyroscope::pause() {
    ASensorEventQueue_disableSensor(gyroEventQueue, gyro);
}

void gyroscope::resume() {
    ASensorEventQueue_enableSensor(gyroEventQueue, gyro);
    auto status = ASensorEventQueue_setEventRate(gyroEventQueue, gyro, SENSOR_REFRESH_PERIOD_US);
    assert(status >= 0);
}

gyroscope gGyroscope;

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_gyroinit(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gGyroscope.init();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_gyropause(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gGyroscope.pause();
    }

    JNIEXPORT void JNICALL
    Java_com_example_lastkgb_mtracker_SixdofJNI_gyroresume(JNIEnv *env, jclass type) {
        (void) env;
        (void) type;
        gGyroscope.resume();
    }
}