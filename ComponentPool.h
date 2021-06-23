#pragma once

typedef char byte;

class ComponentPool
{
public:
    virtual ~ComponentPool() {}
    virtual void resize(const size_t newsize) = 0;
    virtual void* at(const size_t index) = 0;
    virtual size_t size() const = 0;
};

template<typename T>
class TypedComponentPool : public ComponentPool {
public:
    TypedComponentPool(const size_t numEntries) {
        data.resize(numEntries);
    }

    ~TypedComponentPool() {
        data.clear();
    }

    virtual void* at(const size_t index) override {
        return (void*)&data.at(index);
    }

    virtual void resize(const size_t newsize) {
        data.resize(newsize);
    }

    virtual size_t size() const override {
        return data.size();
    }
private:
    std::vector<T> data;
};


