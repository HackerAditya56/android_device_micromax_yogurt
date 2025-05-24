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

#ifndef ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_V2_1_BIOMETRICSFINGERPRINT_H
#define ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_V2_1_BIOMETRICSFINGERPRINT_H

#include <hardware/fingerprint.h>
#include <android/hardware/biometrics/fingerprint/2.1/IBiometricsFingerprint.h>

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <mutex>

namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {
namespace V2_1 {
namespace implementation {

using ::android::hardware::biometrics::fingerprint::V2_1::RequestStatus;
using ::android::hardware::biometrics::fingerprint::V2_1::FingerprintError;
using ::android::hardware::biometrics::fingerprint::V2_1::FingerprintAcquiredInfo;
using ::android::hardware::Return;
using ::android::hardware::hidl_vec;
using ::android::sp;

struct BiometricsFingerprint : public IBiometricsFingerprint {
public:
    BiometricsFingerprint(); // Constructor
    ~BiometricsFingerprint(); // Destructor

    // Fingerprint HAL getters/setters
    static IBiometricsFingerprint* getInstance(); // For singleton usage

    // Overridden methods from IBiometricsFingerprint:
    Return<uint64_t> setNotify(
        const sp<IBiometricsFingerprintClientCallback>& clientCallback) override;
    Return<uint64_t> preEnroll() override;
    Return<RequestStatus> postEnroll() override;
    Return<RequestStatus> enumerate() override;
    Return<RequestStatus> cancel() override;
    Return<RequestStatus> enroll(const hidl_array<uint8_t, 69>& hat, uint32_t gid, uint32_t timeoutSec) override;
    Return<RequestStatus> remove(uint32_t gid, uint32_t fid) override;
    Return<RequestStatus> setActiveGroup(uint32_t gid, const hidl_string& storePath) override;
    Return<RequestStatus> authenticate(uint64_t operationId, uint32_t gid) override;
    Return<uint64_t> getAuthenticatorId() override;
    Return<uint64_t> getDeviceId(); // Added method to return a fixed device ID in case of platform-specific bugs

private:
    static fingerprint_device_t* openHal(); // Open HAL handles device initialization
    static void notify(const fingerprint_msg_t *msg); // Callback for fingerprint HAL
    fingerprint_device_t *mDevice = nullptr; // Fingerprint device handle
    sp<IBiometricsFingerprintClientCallback> mClientCallback;
    mutable std::mutex mClientCallbackMutex;

    static BiometricsFingerprint* sInstance;
    static Return<RequestStatus> ErrorFilter(int32_t error);
    static FingerprintError VendorErrorFilter(int32_t error, int32_t* vendorCode);
    static FingerprintAcquiredInfo VendorAcquiredFilter(int32_t info, int32_t* vendorCode);
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android

#endif // ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_V2_1_BIOMETRICSFINGERPRINT_H
