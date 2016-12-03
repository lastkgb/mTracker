//
// Created by lastkgb on 2016-10-13.
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
const int SENSOR_HISTORY_LENGTH = 100;
const int SENSOR_REFRESH_RATE_HZ = 100;
constexpr int32_t SENSOR_REFRESH_PERIOD_US = int32_t(1000000 / SENSOR_REFRESH_RATE_HZ);
const float SENSOR_FILTER_ALPHA = 0.1f;

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
    SensorData mSensorDatas[SENSOR_HISTORY_LENGTH*2];
    SensorData mSensorDataFilter;
    int mSensorDataIdx;

    pthread_t th;

    JNIEnv *mJEnv;
    jclass mJavaClass;

    jmethodID positionUpdate;

public:
    Position()
    {

    }

    void setJniEnv(JNIEnv *env)
    {
         mJEnv = env;
    }

/*
    void getJavaMethods()
    {
        jclass cls = mJEnv->GetObjectClass(mJEnv, obj);
        onPositionUpdated = mJEnv->GetMethodID(mJEnv, cls, "onPositionUpdated", "([Ljava/nio/Buffer;I)V");

        if (onPositionUpdated == 0) {
            return;
        }
    }
*/

    void init(jclass clazz)
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

        mJavaClass = (*mJEnv).FindClass("com/example/lastkgb/mtracker/TrackerSurfaceView");

        positionUpdate = mJEnv->GetMethodID(mJavaClass, "onPositionUpdated", "(FFF)V");
        if (positionUpdate == 0) {
            return;
        }

    }

    void updateSensor()
    {
        //ALooper_pollAll(0, NULL, NULL, NULL);
        ASensorEvent event;
        float a = SENSOR_FILTER_ALPHA;
        while (ASensorEventQueue_getEvents(mGyroEventQ, &event, 1) > 0) {
            mSensorDataFilter.x = a * event.uncalibrated_gyro.x_uncalib + (1.0f - a) * mSensorDataFilter.x;
            mSensorDataFilter.y = a * event.uncalibrated_gyro.y_uncalib + (1.0f - a) * mSensorDataFilter.y;
            mSensorDataFilter.z = a * event.uncalibrated_gyro.z_uncalib + (1.0f - a) * mSensorDataFilter.z;
            LOGI("Position [%f, %f, %f]", mSensorDataFilter.x, mSensorDataFilter.y, mSensorDataFilter.z);
        }
        mSensorDatas[mSensorDataIdx] = mSensorDataFilter;
        mSensorDatas[SENSOR_HISTORY_LENGTH+mSensorDataIdx] = mSensorDataFilter;
        mSensorDataIdx = (mSensorDataIdx + 1) % SENSOR_HISTORY_LENGTH;

        mJEnv->CallVoidMethod(mJavaClass, positionUpdate, mSensorDataFilter.x, mSensorDataFilter.y, mSensorDataFilter.z);
        LOGI("Position Updated");
    }

};

Position gPosition;

jint JNI_OnLoad(JavaVM* jVm, void* reserved)
{
    JNIEnv* env;
    LOGI("JNI_OnLoad called");
    if (jVm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    gPosition.setJniEnv(env);
    return JNI_VERSION_1_6;
}

void *updatePosition(void *arg)
{
    gPosition.updateSensor();
}

void start()
{
    int res;
    void *status;
    pthread_t th;

    LOGI("Thread start called");
    res = pthread_create(&th, NULL, updatePosition, (void*) 0);
    LOGI("Thread creation return %d(Tid:%d)", res, (int) th);
    if (!res) {
        res = pthread_join(th, &status);
        LOGI("Thread started");
    } else {
        LOGI("Failed to create thread");
    };
}


extern "C" {

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_Position_naInit(JNIEnv *env, jclass clazz) {
    LOGI("Position Init called");
    gPosition.init(clazz);
}

JNIEXPORT void JNICALL
Java_com_example_lastkgb_mtracker_Position_naStart(JNIEnv *env, jclass clazz) {
    LOGI("Position Update called");
    start();
}

JNIEXPORT void JNICALL
        Java_com_example_lastkgb_mtracker_Position_naStop(JNIEnv *env, jclass clazz)
{

}

}
