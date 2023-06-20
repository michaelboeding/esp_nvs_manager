#ifndef NVSMANAGER_H
#define NVSMANAGER_H

#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"
#include <string>


// This is the interface that all serializable classes must implement in order to be saved to NVS
class I_Serializable {
public:
    //virtual ~I_Serializable() {}
    // Each serializable class must provide these methods:
    virtual std::string serialize() = 0;
    virtual void deserialize(std::string& serialized) = 0;
};


class NVSManager {
public:
    //constructor and destructor
    NVSManager(const std::string& namespaceName);
    ~NVSManager();
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
