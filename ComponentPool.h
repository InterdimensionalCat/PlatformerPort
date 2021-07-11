#pragma once

typedef char byte;

class ComponentPool
{
public:
    virtual ~ComponentPool() {}
    virtual void resize(const size_t newsize) = 0;
    virtual void* at(const size_t index) = 0;
    virtual size_t size() const = 0;
    virtual void removeComponent(const size_t index) = 0;
};

template<typename T>
class TypedComponentPool : public ComponentPool {
public:
    TypedComponentPool(const size_t numEntries) {
        sparsedataindexes.resize(numEntries, std::numeric_limits<size_t>::max());
        freedataspots.reserve(8);
        packeddata.reserve(8);
    }

    ~TypedComponentPool() {
        sparsedataindexes.clear();
        freedataspots.clear();
        packeddata.clear();
    }

    void* at(const size_t index) override {
        if (sparsedataindexes.at(index) == std::numeric_limits<size_t>::max()) {
            throw std::out_of_range("attempting to access a deleted component");
        }
        return (void*)&packeddata.at(sparsedataindexes.at(index));
    }

    template<typename... Args>
    void setIndex(const size_t index, Args... args) {
        if (!freedataspots.empty()) {
            auto sparseind = freedataspots.back();
            packeddata.at(sparseind) = std::move(T{ args... });
            sparsedataindexes.at(index) = sparseind;
            freedataspots.pop_back();
        }
        else {
            packeddata.push_back(std::move(T{ args... }));
            auto sparseind = packeddata.size() - 1;
            sparsedataindexes.at(index) = sparseind;
        }
    }

    void resize(const size_t newsize) {
        sparsedataindexes.resize(newsize, std::numeric_limits<size_t>::max());
    }

    size_t size() const override {
        return sparsedataindexes.size();
    }

    void removeComponent(const size_t index) override {
        if(sparsedataindexes.at(index) == std::numeric_limits<size_t>::max()) return;
        auto sparseind = sparsedataindexes.at(index);
        freedataspots.push_back(sparseind);
        sparsedataindexes.at(index) = std::numeric_limits<size_t>::max();
    }

private:
    std::vector<size_t> sparsedataindexes;
    std::vector<T> packeddata;
    std::vector<size_t> freedataspots;
};


