### NVSManager 

This component is used to save and load data to NVS by using the concept of templates and serialization. 
In short it allows you to take any object you want and save it directly to nvs and reterive it later as long as you conform to the interface. 


### Usage

1. Create the NVSManager object. The parameter is the name of the storage location.

```c++
NVSManager nvsManager = new NVSManager("storage");
```

2. You need to inheret the I_Serializable interface and then implement both the serialize and deserialize methods in order to save the object. 

```c++

class WifiNetwork : public I_Serializable {
    //attributes you want to save 
    std::string ssid;
    std::string password;

    std::string serialize() override;
    void deserialize(std::string& serialized) override;
    
};

```

3. For example if I wanted to serialize the data for the WifiNetwork we would do it by implementing the serialize and deserialize methods like this 

```c++
        
    //override method used to serialize the object
    std::string WifiNetwork::serialize() {
        //create a stream and then append the items with new lines 
        std::ostringstream oss;
        oss << this->ssid << '\0' << this->password;
        return oss.str();
    }

    void WifiNetwork::deserialize(std::string& serialized) {
        //deseralize the items in the correct order
        std::istringstream iss(serialized);
        //get the values and set them 
        std::getline(iss, this->ssid, '\0');
        std::getline(iss, this->password, '\0');
        //print out the values
        std::cout << "SSID: " << this->ssid << std::endl;
        std::cout << "Password: " << this->password << std::endl;
    }

```

4. Full usage 


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

2. You need to inheret the I_Serializable interface and then implement both the serialize and deserialize methods in order to save the object. Same as previously. 

3. Full Usage 


```c++


    NVSManager* nvsManager1 = new NVSManager("wifiStorage");

    //use with a vector of networks
    std::vector<WifiNetwork*> networks;

    std::cout << "Saving vector of networks" << std::endl;

    //create three networks
    WifiNetwork* network1 = new WifiNetwork("Network1", "pass1");
    WifiNetwork* network2 = new WifiNetwork("Network2", "pass2");
    WifiNetwork* network3 = new WifiNetwork("Network3", "pass3");
    //add them to the vector
    networks.push_back(network1);
    networks.push_back(network2);
    networks.push_back(network3);
    //save the vector to the nvs manager
    err = nvsManager1->saveVector("vector", networks);
    //load the vector from the nvs manager
    std::vector<WifiNetwork*> loadedNetworks;
    err = nvsManager1->loadVector("vector",loadedNetworks);
    //print the loaded networks
    for(int i = 0; i < loadedNetworks.size(); i++){
        std::cout << "Network " << i << ": " << loadedNetworks[i]->getSsid() << std::endl;
    }

```


### Advanced Usage Cont

This library gives you a lot of control over the methods you use when seralizing and deseralizing your data by implementing the methods yourself. There happens to be some popular libraries that you could consider using. 



1. Boost Serialization: Boost is a robust, general-purpose library, and its serialization component is no exception. It provides a simple and flexible API for serialization of complex objects, supports versioning, and handles pointers and other complex types well.

2. Cereal: cereal is a header-only C++11 serialization library that's easy to use and doesn't require a separate build step like protobuf does. It's particularly well-suited to projects where simplicity and integration ease are priorities.

3. protobuf (Protocol Buffers): Protocol Buffers, developed by Google, is a powerful, efficient, and automated mechanism for serializing structured data. It supports several languages, making it a good choice for cross-platform and cross-language projects.
Cap'n Proto: Designed by the original author of protobuf, Cap'n Proto is an extremely efficient protocol that's designed to be very fast. It even allows you to use the serialized data directly without parsing.

4. FlatBuffers: Developed by Google, FlatBuffers is another very efficient serialization library. Like Cap'n Proto, it allows zero-copy reads for maximum speed.


Example 




