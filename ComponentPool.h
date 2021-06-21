#pragma once

typedef char byte;

class ComponentPool
{
public:

    ComponentPool() :
        elementSize(0),
        totalSizeElts(0),
        data(nullptr)
    {}

    ComponentPool(const size_t elementSize, const size_t numEntries) : 
        elementSize(elementSize), 
        totalSizeElts(numEntries), 
        data(new byte[elementSize * totalSizeElts])
    {
        if (totalSizeElts == 0) {
            throw std::logic_error("Component pool initalized before any actors were created");
        }
    }

    bool isValid() const {
        return data != nullptr;
    }

    ComponentPool(const ComponentPool& other) 
        : elementSize(other.elementSize), 
        totalSizeElts(other.totalSizeElts),
        data(new byte[elementSize * totalSizeElts]) {
        memmove(data, other.data, elementSize * totalSizeElts);
    }

    friend void swap(ComponentPool& lhs, ComponentPool& rhs) {
        std::swap(lhs.data, rhs.data);
        std::swap(lhs.elementSize, rhs.elementSize);
        std::swap(lhs.totalSizeElts, rhs.totalSizeElts);
    }

    ComponentPool& operator=(ComponentPool rhs) {
        swap(*this, rhs);
        return *this;
    }

    ~ComponentPool()
    {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    void* at(const size_t& index)
    {
        if (index < totalSizeElts) {
            return data + index * elementSize;
        }
        else {
            throw std::out_of_range("ComponentPool index out of range");
        }
    }

    size_t resize(const size_t scaleFactor) {
        byte* olddata = data;
        data = nullptr;
        data = new byte[elementSize * totalSizeElts * scaleFactor];
        for (size_t i = 0; i < elementSize * totalSizeElts; i++) {
            data[i] = olddata[i];
        }
        delete[] olddata;
        totalSizeElts *= scaleFactor;
        return totalSizeElts;
    }

    void logData() const {
        for (size_t i = 0; i < totalSizeElts * elementSize; i++) {
            Logger::get() << std::format("{:b}", data[i]) << " ";
        }
        Logger::get() << "\n";
    }

private:
    size_t elementSize;
    size_t totalSizeElts;
    byte* data;
};


