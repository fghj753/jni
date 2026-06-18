#include <jni.h>
#include <android/log.h>

#define LOG_TAG "Plugin"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGD("================== PLUGIN LOADED SUCCESSFULLY ==================");
    return JNI_VERSION_1_6;
}
