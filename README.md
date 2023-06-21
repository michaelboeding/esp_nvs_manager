### NVSManager 

This component is used to save and load data to NVS by using the concept of templates and serialization. 
In short it allows you to take any object you want and save it directly to nvs and reterive it later using cereal to serialize and deserialize it. 


### Setup 

1. Clone with submodules. This is using the cereal lib for serilization which is added as a submodule. 

```c++
git clone --recurse-submodules <repository_url>
```

2. Make sure that the path to the cereal module is defined in your CMakeLists.txt in the main folder. 

```txt
idf_component_register(SRCS "main.cpp" INCLUDE_DIRS "./components/esp_nvs_manager/libs/cereal/include/" ".")

```

3. You may need to disable certain compiler flagss

```c++ 
idf.py menuconfig 

Once in the options menu go to:

*  Compiler Options -> Enable C++ exceptions 

* Compiler Options -> Enable C++ run-time type info (RTTI)

```

### Usage

1. Create the NVSManager object. The parameter is the name of the storage location.

```c++
NVSManager nvsManager = new NVSManager("storage");
```

2. You need to make your class serializable with Cereal. This is done by adding a serialize method in your class that takes an Archive object. The serialize method should use the Archive object to serialize all data members that need to be saved.

```c++

#include <cereal/types/string.hpp>

class WifiNetwork {
public:
    // The data members you want to save.
    std::string ssid;
    std::string password;

    template <class Archive>
    void serialize(Archive &ar) {
        ar(ssid, password);
    }
};

```

3. The NVSManager's save and load methods can be used to save and load objects.


```c++
void app_main() {
    // Create the NVS manager with the namespace "storage" referring to the NVS partition.
    NVSManager nvsManager("storage");
    
    // Create a WifiNetwork object to save.
    WifiNetwork network{"MySSID", "MyPassword"};
    
    // Save the network to NVS.
    esp_err_t err = nvsManager.save("wifiNetwork", network);
    if (err != ESP_OK) {
        // Handle error...
    }
    
    // Load the network from NVS.
    WifiNetwork loadedNetwork;
    err = nvsManager.load("wifiNetwork", loadedNetwork);
    if (err != ESP_OK) {
        // Handle error...
    }
    
    // The loadedNetwork object now contains the loaded data.
    std::cout << "SSID: " << loadedNetwork.ssid << std::endl;
    std::cout << "Password: " << loadedNetwork.password << std::endl;
}

```

### Advanced Usage


NVSManager can also save and load vectors of objects. The usage is similar to saving and loading single objects:

```c++
void app_main(){
    //MARK: SAVE THE NETWORK
    esp_err_t err;
    //create the nvs manager with the namespace storage refering to the nvs partition
    NVSManager* nvsManager = new NVSManager("storage");
    //create the wifi network object to save 
    WifiNetwork* network = new WifiNetwork("MichaelsWifi", "Password123");
    //save the network to the nvs manager
    err = nvsManager->save("wifiNetwork", *network);

    //MARK: LOAD THE NETWORK
    WifiNetwork wifiNetwork;
    //load the object 
    err = nvsManager->load("wifiNetwork", wifiNetwork);
}

```


### Advanced Usage 

If you would like to save vectors of objects this component allows you to do that also.

1. Create the NVSManager object. The parameter is the name of the storage location.

```c++
NVSManager nvsManager = new NVSManager("networkStorage");
```

2. Full Usage 

```c++

    esp_err_t err;
    //create the nvs manager
    NVSManager* nvsManager1 = new NVSManager("wifiStorage");
    //use with a vector of networks
    std::vector<WifiNetwork> networks;
    std::cout << "Saving vector of networks" << std::endl;
    //create three networks
    WifiNetwork network1 = WifiNetwork("Network1", "pass1");
    WifiNetwork network2 = WifiNetwork("Network2", "pass2");
    WifiNetwork network3 = WifiNetwork("Network3", "pass3");
    //add them to the vector
    networks.push_back(network1);
    networks.push_back(network2);
    networks.push_back(network3);
    //save the vector to the nvs manager
    err = nvsManager1->saveVector("vector", networks);
    //load the vector from the nvs manager
    std::vector<WifiNetwork> loadedNetworks;
    err = nvsManager1->loadVector("vector",loadedNetworks);
    //print the loaded networks
    for(int i = 0; i < loadedNetworks.size(); i++){
        std::cout << "Network " << i << ": " << loadedNetworks[i].getSsid() << std::endl;
    }

```


