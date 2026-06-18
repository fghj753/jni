#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <link.h>
#include "vendor/Dobby/include/dobby.h"

#define LOG_TAG "Plugin"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

typedef bool (*tConnect)(void*, const char*, uint16_t, uint16_t, uint32_t, int);
tConnect orig_Connect = nullptr;

bool hook_Connect(void* _this, const char* host, uint16_t port, uint16_t cPort, uint32_t dep, int sleep) {
    LOGD("HOOK: Redirecting %s:%d -> 188.127.241.74:5014", host, port);
    return orig_Connect(_this, "80.242.59.112", 4473, cPort, dep, sleep);
}

void InitializeHooks() {
    LOGD("InitializeHooks: STARTED");
    
    uintptr_t base = 0;
    int attempts = 0;
    
    while (!base && attempts < 30) {
        dl_iterate_phdr([](dl_phdr_info* info, size_t, void* data) {
            if (info->dlpi_name && strstr(info->dlpi_name, "libblackrussia-client.so")) {
                *(uintptr_t*)data = info->dlpi_addr;
                LOGD("Found libblackrussia-client.so at base: %p", (void*)info->dlpi_addr);
                return 1;
            }
            return 0;
        }, &base);
        if (!base) {
            usleep(100000);
            attempts++;
        }
    }
    
    if (!base) {
        LOGE("Failed to find libblackrussia-client.so");
        return;
    }

    LOGD("Base address: %p", (void*)base);
    
    uintptr_t rakClient = 0;
    attempts = 0;
    while (!rakClient && attempts < 30) {
        rakClient = *(uintptr_t*)(base + 0x12DB30);
        if (!rakClient) {
            usleep(100000);
            attempts++;
        }
    }
    
    if (!rakClient) {
        LOGE("Failed to find RakClient at offset 0x12DB30");
        return;
    }
    
    LOGD("RakClient found at: %p", (void*)rakClient);

    uintptr_t vtable = *(uintptr_t*)rakClient;
    LOGD("VTable at: %p", (void*)vtable);

    // Пробуем разные смещения в vtable
    void* target = (void*)*(uintptr_t*)(vtable + 32); // индекс 4
    LOGD("Target function at vtable+32: %p", target);
    
    if (!target) {
        LOGE("Target is NULL, trying other offsets...");
        target = (void*)*(uintptr_t*)(vtable + 24);
        LOGD("Target at vtable+24: %p", target);
    }
    
    if (!target) {
        LOGE("All targets are NULL, aborting");
        return;
    }

    LOGD("Calling DobbyHook on target: %p", target);
    int hookResult = DobbyHook(target, (void*)hook_Connect, (void**)&orig_Connect);
    LOGD("DobbyHook returned: %d", hookResult);
    
    if (orig_Connect) {
        LOGD("Connect hook installed successfully!");
    } else {
        LOGE("Failed to hook Connect!");
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGD("JNI_OnLoad: STARTED");
    std::thread(InitializeHooks).detach();
    LOGD("JNI_OnLoad: Thread detached, returning");
    return JNI_VERSION_1_6;
}
