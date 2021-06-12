#pragma once

class Logger
{
public:
    class StandardLogger {
    public:
        template<typename T>
        const StandardLogger& operator<<(const T& message) const {
#ifdef _DEBUG
            std::cout << message;
#endif
            return *this;
        }
    };

    static const StandardLogger& get() {
        if (logger.get() == nullptr) {
            logger = std::make_unique<StandardLogger>();
        }
        return *logger;
    }
private:
    inline static std::unique_ptr<StandardLogger> logger;
};

