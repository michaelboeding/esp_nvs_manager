#include "NVSManager.h"

NVSManager::NVSManager(const std::string& namespaceName) {
    // Initialize the NVS flash storage
    ESP_ERROR_CHECK(nvs_flash_init());
    // Open the NVS namespace
    ESP_ERROR_CHECK(nvs_open(namespaceName.c_str(), NVS_READWRITE, &nvsHandle));
}

NVSManager::~NVSManager() {
    // Close the NVS handle
    nvs_close(nvsHandle);
}
