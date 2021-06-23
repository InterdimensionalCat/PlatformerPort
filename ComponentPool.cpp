#include "stdafx.h"
#include "ComponentPool.h"

ComponentPool::ComponentPool() :
    elementSize(0),
    totalSizeElts(0),
    data(nullptr) {}

ComponentPool::ComponentPool(const size_t elementSize, const size_t numEntries) :
    elementSize(elementSize),
    totalSizeElts(numEntries),
    data(new byte[elementSize * totalSizeElts]) {
    if (totalSizeElts == 0) {
        throw std::logic_error("Component pool initalized before any actors were created");
    }
}

bool ComponentPool::isValid() const {
    return data != nullptr;
}

ComponentPool::ComponentPool(const ComponentPool& other)
    : elementSize(other.elementSize),
    totalSizeElts(other.totalSizeElts),
    data(new byte[elementSize * totalSizeElts]) {
    memmove(data, other.data, elementSize * totalSizeElts);
}

ComponentPool& ComponentPool::operator=(ComponentPool rhs) {
    swap(*this, rhs);
    return *this;
}

ComponentPool::~ComponentPool() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
}

void* ComponentPool::at(const size_t& index) {
    if (index < totalSizeElts) {
        return data + index * elementSize;
    } else {
        throw std::out_of_range("ComponentPool index out of range");
    }
}

size_t ComponentPool::resize(const size_t scaleFactor) {
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

void ComponentPool::logData() const {
    for (size_t i = 0; i < totalSizeElts * elementSize; i++) {
        Logger::get() << std::format("{:b}", data[i]) << " ";
    }
    Logger::get() << "\n";
}
