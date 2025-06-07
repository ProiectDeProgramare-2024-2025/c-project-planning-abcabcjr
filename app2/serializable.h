#pragma once

#include <fstream>

class Serializable {
public:
    virtual ~Serializable() = default;

    virtual void serialize(std::ofstream& file) const = 0;
    virtual void deserialize(std::ifstream& file) = 0;
};
