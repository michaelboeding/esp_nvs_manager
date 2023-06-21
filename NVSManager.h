#ifndef NVS_MANAGER_H
#define NVS_MANAGER_H

#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "cereal/archives/binary.hpp"



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

    template<typename T>
    esp_err_t save(const std::string& key, const T& object) {
        esp_err_t err;
        // Serialize the object to a string
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(object);
        }
        std::string serialized = ss.str();
        // Save the serialized object to NVS
        err = nvs_set_blob(nvsHandle, key.c_str(), serialized.data(), serialized.size());
        err = nvs_commit(nvsHandle);
        return err;
    }

    template<typename T>
    esp_err_t load(const std::string& key, T& object) {
        size_t required_size;
        esp_err_t err = nvs_get_blob(nvsHandle, key.c_str(), NULL, &required_size);
        if (err != ESP_OK) return err;

        std::vector<uint8_t> buffer(required_size);
        err = nvs_get_blob(nvsHandle, key.c_str(), buffer.data(), &required_size);
        if (err != ESP_OK) return err;

        std::string serialized(buffer.begin(), buffer.end());
        std::stringstream ss(serialized);
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(object);
        }
        return ESP_OK;
    }

    template<typename T>
    esp_err_t saveVector(const std::string& key, const std::vector<T>& vec) {
        esp_err_t err;
        // Serialize the vector to a string
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        std::string serializedVec = ss.str();
        // Save the serialized object to NVS
        err = nvs_set_blob(nvsHandle, key.c_str(), serializedVec.data(), serializedVec.size());
        err = nvs_commit(nvsHandle);
        // Error handling.
        if (err == ESP_ERR_NVS_NOT_ENOUGH_SPACE) {
            printf("Not enough space in NVS to save data\n");
            return err;
        } else if (err != ESP_OK) {
            printf("An error occurred while saving data in NVS\n");
            return err;
        }
        return err;
    }

    template<typename T>
    esp_err_t loadVector(const std::string& key, std::vector<T>& vec) {
        size_t required_size;
        esp_err_t err = nvs_get_blob(nvsHandle, key.c_str(), NULL, &required_size);
        if (err != ESP_OK) return err;

        std::vector<uint8_t> buffer(required_size);
        err = nvs_get_blob(nvsHandle, key.c_str(), buffer.data(), &required_size);
        if (err != ESP_OK) return err;

        std::string serializedVec(buffer.begin(), buffer.end());
        std::stringstream ss(serializedVec);
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(vec);
        }
        return ESP_OK;
    }

    //erase the entire flash
    esp_err_t eraseFlash() {
        esp_err_t err = nvs_flash_erase();
        if (err != ESP_OK) {
            // Handle error
            std::cout << "Error erasing NVS flash. Error code: " << err << std::endl;
        }
        return err;
    }

private:
    nvs_handle_t nvsHandle;
};


#endif //NVS_MANAGER_H