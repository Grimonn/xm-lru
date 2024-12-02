#include "ProductService.h"
#include "Logger.h"
#include <shared_mutex>

ProductService::ProductService(std::shared_ptr<ICache<uint64_t, Product>> cache, std::shared_ptr<IDatabase> database)
	: mCache(std::move(cache))
	, mDatabase(std::move(database))
{
	if (!this->mCache || !this->mDatabase) {
		Logger::log(LogLevel::ERROR, LogCategory::SERVICE, "ProductService initialization failed: Null cache or database provided.");
		throw std::invalid_argument("Cache and database must not be null.");
	}
	Logger::log(LogLevel::INFO, LogCategory::SERVICE, "ProductService initialized successfully.");
}

std::optional<Product> ProductService::getProductDetails(uint64_t productId) const {
	Logger::log(LogLevel::INFO, LogCategory::SERVICE,
		"Fetching product details for Product ID: " + std::to_string(productId));

	// Shared lock for reading from the cache
	{
		std::shared_lock<std::shared_mutex> readLock(mCacheMutex);
		if (auto cachedProduct = mCache->get(productId); cachedProduct) {
			Logger::log(LogLevel::INFO, LogCategory::SERVICE,
				"Product ID: " + std::to_string(productId) + " found in cache.");
			return cachedProduct;
		}
	}

	Logger::log(LogLevel::INFO, LogCategory::SERVICE,
		"Product ID: " + std::to_string(productId) + " not found in cache. Fetching from database.");

	// Fetch from database outside of the lock
	if (auto dbProduct = mDatabase->fetchProductDetails(productId); dbProduct) {
		Logger::log(LogLevel::INFO, LogCategory::SERVICE,
			"Product ID: " + std::to_string(productId) + " found in database.");

		// Unique lock for writing to the cache
		{
			std::unique_lock<std::shared_mutex> writeLock(mCacheMutex);
			mCache->put(productId, *dbProduct);
			Logger::log(LogLevel::INFO, LogCategory::SERVICE,
				"Product ID: " + std::to_string(productId) + " added to cache.");
		}

		return dbProduct;
	}

	Logger::log(LogLevel::WARNING, LogCategory::SERVICE,
		"Product ID: " + std::to_string(productId) + " not found in cache or database.");
	return std::nullopt;
}

