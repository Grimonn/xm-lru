#include <gtest/gtest.h>
#include "ProductCache.h"
#include "Logger.h"
#include <fstream>
#include <string>
#include <memory>
#include <thread>
#include <vector>

class ProductCacheTest : public ::testing::Test {
protected:
	void SetUp() override {
		cache = std::make_shared<ProductCache>(3);
	}

	std::shared_ptr<ProductCache> cache;
};

// Test case to verify adding a product to the cache
TEST_F(ProductCacheTest, TestPutProductToCache) {
	uint64_t productId = 1;
	Product product(productId, 101, "Product 1", "Description 1", std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } });

	cache->put(productId, product);

	auto fetchedProduct = cache->get(productId);
	ASSERT_TRUE(fetchedProduct.has_value());
	EXPECT_EQ(fetchedProduct->getId(), productId);
}

// Test case to verify fetching a product from the cache
TEST_F(ProductCacheTest, TestGetProductFromCache) {
	uint64_t productId = 1;
	Product product(productId, 101, "Product 1", "Description 1", std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } });

	cache->put(productId, product);

	// Fetch the product from cache
	auto fetchedProduct = cache->get(productId);

	ASSERT_TRUE(fetchedProduct.has_value());
	EXPECT_EQ(fetchedProduct->getId(), productId);
}

// Test case to verify that the least recently used product is evicted when the cache is full
TEST_F(ProductCacheTest, TestCacheEvictionWhenFull) {
	for (uint64_t i = 1; i <= 3; ++i) {
		Product product(i, 101, "Product " + std::to_string(i), "Description " + std::to_string(i), std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } });
		cache->put(i, product);
	}

	// Trigger eviction (4th product)
	uint64_t newProductId = 4;
	Product newProduct(newProductId, 102, "Product 4", "Description 4", std::vector{ std::byte{ 'D' }, std::byte{ 'E' }, std::byte{ 'F' } });
	cache->put(newProductId, newProduct);

	auto evictedProduct = cache->get(1);
	EXPECT_FALSE(evictedProduct.has_value());
}

// Test case to verify cache respects its capacity
TEST_F(ProductCacheTest, TestCacheCapacity) {
	uint64_t productId = 1;
	Product product(productId, 101, "Product 1", "Description 1", std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } });

	// Initially, the cache should be empty
	EXPECT_EQ(cache->get(productId).has_value(), false);

	cache->put(productId, product);

	auto fetchedProduct = cache->get(productId);
	ASSERT_TRUE(fetchedProduct.has_value());
	EXPECT_EQ(fetchedProduct->getId(), productId);

	for (uint64_t i = 2; i <= 3; ++i) {
		Product product(i, 101, "Product " + std::to_string(i), "Description " + std::to_string(i), std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } });
		cache->put(i, product);
	}

	Product extraProduct(4, 101, "Product 4", "Description 4", std::vector{ std::byte{ 'D' }, std::byte{ 'E' }, std::byte{ 'F' } });
	cache->put(4, extraProduct);

	auto evictedProduct = cache->get(1);
	EXPECT_FALSE(evictedProduct.has_value());
}

// Test case to verify thread safety of ProductCache using jthread
TEST_F(ProductCacheTest, ThreadSafetyWithJThread) {
	constexpr int threadCount = 5;
	constexpr int productsPerThread = 10;

	std::vector<std::jthread> threads;

	auto putProducts = [this](std::stop_token, int threadIndex) {
		for (int i = 0; i < productsPerThread; ++i) {
			uint64_t productId = threadIndex * productsPerThread + i;
			cache->put(productId, Product(productId, 100 + threadIndex,
				"Product " + std::to_string(productId),
				"Description " + std::to_string(productId),
				std::vector{ std::byte{ 'A' }, std::byte{ 'B' }, std::byte{ 'C' } }));
		}
		};

	for (int t = 0; t < threadCount; ++t) {
		threads.emplace_back(putProducts, t);
	}

	EXPECT_FALSE(cache->get(0).has_value()) << "Oldest product should have been evicted.";

	for (int i = (threadCount - 1) * productsPerThread; i < threadCount * productsPerThread; ++i) {
		if (auto product = cache->get(i)) {
			EXPECT_EQ(product->getId(), i);
		}
	}
}