#ifndef NVSMANAGER_H
#define NVSMANAGER_H

#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"
#include <string>
#include "I_Serializable.h"

class NVSManager {
public:
    //constructor and destructor
    NVSManager(const std::string namespaceName) {
        // Initialize the NVS flash storage
        ESP_ERROR_CHECK(nvs_flash_init());
        // Open the NVS namespace
        ESP_ERROR_CHECK(nvs_open(namespaceName.c_str(), NVS_READWRITE, &nvsHandle));
    }

    ~NVSManager() {
        // Close the NVS handle
        nvs_close(nvsHandle);
    }

    //template functions - note these have to be in the header file not the cpp file due to the way templates work

    esp_err_t save(const std::string& key, I_Serializable& object) {
        // Serialize the object to a string
        std::string serialized = object.serialize();
        // Save the serialized object to NVS
        return nvs_set_blob(nvsHandle, key.c_str(), serialized.data(), serialized.size());
    }

    esp_err_t load(const std::string& key, I_Serializable& object) {
         // Get the size of the saved data
        size_t required_size;
        esp_err_t err = nvs_get_blob(nvsHandle, key.c_str(), NULL, &required_size);
        if (err != ESP_OK) return err;

        // Load the saved data
        std::vector<uint8_t> buffer(required_size);
        err = nvs_get_blob(nvsHandle, key.c_str(), buffer.data(), &required_size);
        if (err != ESP_OK) return err;

        // Deserialize the data to the object
        std::string serialized(buffer.begin(), buffer.end());
        object.deserialize(serialized);
        return ESP_OK;
    }

private:
    nvs_handle_t nvsHandle;
};




#endif // NVSMANAGER_H
