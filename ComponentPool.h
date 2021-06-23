#pragma once

typedef char byte;

class ComponentPool
{
public:

    ComponentPool();

    ComponentPool(const size_t elementSize, const size_t numEntries);

    bool isValid() const;

    ComponentPool(const ComponentPool& other);

    friend void swap(ComponentPool& lhs, ComponentPool& rhs) {
        std::swap(lhs.data, rhs.data);
        std::swap(lhs.elementSize, rhs.elementSize);
        std::swap(lhs.totalSizeElts, rhs.totalSizeElts);
    }

    ComponentPool& operator=(ComponentPool rhs);

    ~ComponentPool();

    void* at(const size_t& index);

    size_t resize(const size_t scaleFactor);

    void logData() const;

    size_t size() const {
        return totalSizeElts;
    }

private:

    friend class Scene;

    size_t elementSize;
    size_t totalSizeElts;
    byte* data;
};


