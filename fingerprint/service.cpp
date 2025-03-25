/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.biometrics.fingerprint@2.1-service.yogurt"

#include <android/log.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprint.h>
#include <android/hardware/biometrics/fingerprint/2.1/types.h>
#include "BiometricsFingerprint.h"

// Add these headers for property support
#include <cutils/properties.h>

using android::hardware::biometrics::fingerprint::V2_1::IBiometricsFingerprint;
using android::hardware::biometrics::fingerprint::V2_1::implementation::BiometricsFingerprint;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;

// Add this near the beginning of main() in service.cpp
void checkVendorProps() {
    char prop_value[PROPERTY_VALUE_MAX] = {0};
    
    // List of properties that might be relevant for fingerprint
    const char* props[] = {
        "ro.hardware.fingerprint",
        "ro.boot.fpsensor",
        "persist.sys.fp.vendor",
        "ro.vendor.fingerprint.type",
        "ro.vendor.fingerprint.module",
        nullptr
    };
    
    ALOGI("Checking fingerprint-related properties:");
    for (int i = 0; props[i] != nullptr; i++) {
        property_get(props[i], prop_value, "UNKNOWN");
        ALOGI("  %s = %s", props[i], prop_value);
    }
}
int main() {
   
    ALOGI("Fingerprint HAL service is starting up");
    checkVendorProps();
    android::sp<IBiometricsFingerprint> bio = BiometricsFingerprint::getInstance();

    if (bio == nullptr) {
        ALOGE("Failed to get BiometricsFingerprint instance");
        return 1;
    }
    ALOGI("Got BiometricsFingerprint instance, configuring RPC threadpool");

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (bio->registerAsService() != android::OK) {
        ALOGE("Failed to register fingerprint HAL service");
        return 1;
    }
    ALOGI("Fingerprint HAL service registered successfully");

    joinRpcThreadpool();

    ALOGI("Fingerprint HAL service exiting"); // Should never get here
    return 0;
}

