#ifndef I_SERIALIZABLE_H
#define I_SERIALIZABLE_H


#include <string>
// This is the interface that all serializable classes must implement in order to be saved to NVS
class I_Serializable {
public:
    //virtual ~I_Serializable() {}
    // Each serializable class must provide these methods:
    virtual std::string serialize() = 0;
    virtual void deserialize(std::string& serialized) = 0;
};


#endif // I_SERIALIZABLE_H