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

class WifiNetwork : public ISerializable {
    //attributes you want to save 
    std::string ssid;
    std::string password;

    std::string serialize() const override {
        //implement how you want to serialize the object here and what attributes need to be serialized
    }

    void deserialize(const std::string& serialized) override {
        //implement how you want to deserialize the obect 

    }
};

```

3. For example if I wanted to serialize the data for the WifiNetwork we would do it by implementing the serialize and deserialize methods like this 

```c++
    std::string serialize() const {
        //create a stream and then append the items with new lines 
        std::ostringstream oss;
        oss << ssid << '\0' << password;
        return oss.str();
    }

    void deserialize(const std::string& serialized) {
        //deseralize the items in the correct order
        std::istringstream iss(serialized);
        //get the values and set them 
        std::getline(iss, this->ssid, '\0');
        std::getline(iss, this->password, '\0');
    }

```





