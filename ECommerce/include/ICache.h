#ifndef ICACHE_H
#define ICACHE_H

#include <optional>

template <typename Key, typename Value>
class ICache {
public:
    virtual ~ICache() = default;
    virtual std::optional<Value> get(Key key) = 0;
    virtual void put(Key key, const Value& value) = 0;
};

#endif // ICACHE_H
