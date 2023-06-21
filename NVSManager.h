#ifndef NVSMANAGER_H
#define NVSMANAGER_H

#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"
#include <string>
#include "I_Serializable.h"
#include <vector>
#include <stdexcept>

class NVSManager {

private:
    nvs_handle_t nvsHandle;
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
        esp_err_t ret;
        // Serialize the object to a string
        std::string serialized = object.serialize();
        // Save the serialized object to NVS
        nvs_set_blob(nvsHandle, key.c_str(), serialized.data(), serialized.size());
        ret = nvs_commit(nvsHandle);
        return ret;
    }

    esp_err_t load(const std::string& key, I_Serializable& object) {
        // Get the size of the saved data
        size_t required_size;
        esp_err_t err = nvs_get_blob(nvsHandle, key.c_str(), NULL, &required_size);
        if (err != ESP_OK) {
            printf("Failed to get the size of saved data. Error code: %d\n", err);
            return err;
        }

        // Load the saved data
        std::vector<uint8_t> buffer(required_size);
        err = nvs_get_blob(nvsHandle, key.c_str(), buffer.data(), &required_size);
        if (err != ESP_OK) {
            printf("Failed to load the saved data. Error code: %d\n", err);
            return err;
        }
        // Deserialize the data to the object
        std::string serialized(buffer.begin(), buffer.end());
        object.deserialize(serialized);
        return ESP_OK;
    }



};




#endif // NVSMANAGER_H
