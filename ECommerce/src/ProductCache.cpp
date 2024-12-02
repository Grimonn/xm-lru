#include "ProductCache.h"
#include <stdexcept>
#include <string>

ProductCache::ProductCache(size_t capacity) 
    : mCapacity{ capacity } 
{
    if (mCapacity == 0) {
        Logger::log(LogLevel::ERROR, LogCategory::CACHE, "ProductCache initialized with zero capacity.");
        throw std::invalid_argument("Cache capacity must be greater than zero.");
    }
    Logger::log(LogLevel::INFO, LogCategory::CACHE, "ProductCache initialized with capacity: " + std::to_string(mCapacity));
}

[[nodiscard]] std::optional<Product> ProductCache::get(uint64_t productId) {
    std::shared_lock lock(mCacheMutex);

    Logger::log(LogLevel::INFO, LogCategory::CACHE, "Getting Product ID: " + std::to_string(productId));

    if (auto it = mCacheMap.find(productId); it != mCacheMap.end()) {
        mCacheList.splice(mCacheList.begin(), mCacheList, it->second);
        Logger::log(LogLevel::INFO, LogCategory::CACHE, "Product ID: " + std::to_string(productId) + " found.");
        return it->second->second;
    }

    Logger::log(LogLevel::INFO, LogCategory::CACHE, "Product ID: " + std::to_string(productId) + " not found.");
    return std::nullopt;
}

void ProductCache::put(uint64_t productId, const Product& product) {
    std::unique_lock lock(mCacheMutex);

    Logger::log(LogLevel::INFO, LogCategory::CACHE, "Putting Product ID: " + std::to_string(productId));

    if (auto it = mCacheMap.find(productId); it != mCacheMap.end()) {
        mCacheList.erase(it->second);
        mCacheMap.erase(it);
    }

    mCacheList.emplace_front(productId, product);
    mCacheMap[productId] = mCacheList.begin();

    if (mCacheMap.size() > mCapacity) {
        const auto& [oldId, _] = mCacheList.back();
        Logger::log(LogLevel::WARNING, LogCategory::CACHE, "Evicting Product ID: " + std::to_string(oldId));
        mCacheMap.erase(oldId);
        mCacheList.pop_back();
    }
}
