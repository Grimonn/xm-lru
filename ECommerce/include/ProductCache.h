#ifndef PRODUCT_CACHE_H
#define PRODUCT_CACHE_H

#include <unordered_map>
#include <list>
#include <shared_mutex>
#include <optional>
#include "ICache.h"
#include "Product.h"
#include "Logger.h"

class ProductCache : public ICache<uint64_t, Product> {
public:
    explicit ProductCache(size_t capacity);
    [[nodiscard]] std::optional<Product> get(uint64_t productId) override;
    void put(uint64_t productId, const Product& product) override;

private:
    size_t mCapacity;
    std::list<std::pair<uint64_t, Product>> mCacheList;
    std::unordered_map<uint64_t, std::list<std::pair<uint64_t, Product>>::iterator> mCacheMap;
    mutable std::shared_mutex mCacheMutex;
};

#endif // PRODUCT_CACHE_H
