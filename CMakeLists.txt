
cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "NVSManager.cpp" INCLUDE_DIRS "." REQUIRES "esp32" "nvs_flash")

# Include the Cereal library
target_include_directories(${COMPONENT_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/cereal/include")
