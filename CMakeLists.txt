cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "NVSManager.cpp" INCLUDE_DIRS "." REQUIRES "esp32" nvs_flash)
