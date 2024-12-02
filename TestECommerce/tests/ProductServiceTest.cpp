#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ProductService.h"
#include "ProductCache.h"
#include "Product.h"
#include "ICache.h"
#include "IDatabase.h"
#include <vector>

// Mock the ICache interface
class MockCache : public ICache<uint64_t, Product> {
public:
    MOCK_METHOD(std::optional<Product>, get, (uint64_t productId), (override));
    MOCK_METHOD(void, put, (uint64_t productId, const Product& product), (override));
};

// Mock the IDatabase interface
class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(std::optional<Product>, fetchProductDetails, (uint64_t productId), (override));
    MOCK_METHOD(size_t, fetchProductCountByCategory, (uint32_t categoryId), (override));
};

// Test fixture for ProductService
class ProductServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<MockCache> mockCache = std::make_shared<MockCache>();
    std::shared_ptr<MockDatabase> mockDatabase = std::make_shared<MockDatabase>();

    ProductService productService{ mockCache, mockDatabase };
};

// Test case 1: Cache hit - Product found in cache
TEST_F(ProductServiceTest, TestCacheHit) {
    uint64_t productId = 1;
    Product expectedProduct(productId, 100, "Product 1", "Description of Product 1", {});

    EXPECT_CALL(*mockCache, get(productId))
        .WillOnce(::testing::Return(expectedProduct));

    auto result = productService.getProductDetails(productId);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), expectedProduct.getId());
    EXPECT_EQ(result->getName(), expectedProduct.getName());
}

// Test case 2: Cache miss, database hit - Product found in database
TEST_F(ProductServiceTest, TestCacheMissDatabaseHit) {
    uint64_t productId = 2;
    Product expectedProduct(productId, 101, "Product 2", "Description of Product 2", {});

    EXPECT_CALL(*mockCache, get(productId))
        .WillOnce(::testing::Return(std::nullopt));

    EXPECT_CALL(*mockDatabase, fetchProductDetails(productId))
        .WillOnce(::testing::Return(expectedProduct));

    EXPECT_CALL(*mockCache, put(productId, expectedProduct))
        .Times(1);

    auto result = productService.getProductDetails(productId);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), expectedProduct.getId());
    EXPECT_EQ(result->getName(), expectedProduct.getName());
}

// Test case 3: Cache miss, database miss - Product not found in cache or database
TEST_F(ProductServiceTest, TestCacheMissDatabaseMiss) {
    uint64_t productId = 3;

    EXPECT_CALL(*mockCache, get(productId))
        .WillOnce(::testing::Return(std::nullopt));

    EXPECT_CALL(*mockDatabase, fetchProductDetails(productId))
        .WillOnce(::testing::Return(std::nullopt));

    auto result = productService.getProductDetails(productId);

    ASSERT_FALSE(result.has_value());
}

//Test case 4: Database call without cache hit
TEST_F(ProductServiceTest, TestCacheMissOnlyDatabaseHit) {
    uint64_t productId = 4;
    Product expectedProduct(productId, 102, "Product 4", "Description of Product 4", {});

    EXPECT_CALL(*mockCache, get(productId))
        .WillOnce(::testing::Return(std::nullopt));

    EXPECT_CALL(*mockDatabase, fetchProductDetails(productId))
        .WillOnce(::testing::Return(expectedProduct));

    EXPECT_CALL(*mockCache, put(productId, expectedProduct))
        .Times(1);

    auto result = productService.getProductDetails(productId);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), expectedProduct.getId());
    EXPECT_EQ(result->getName(), expectedProduct.getName());
}

// Test case 5: Cache miss, empty database, no product found
TEST_F(ProductServiceTest, TestEmptyDatabase) {
    uint64_t productId = 5;

    EXPECT_CALL(*mockCache, get(productId))
        .WillOnce(::testing::Return(std::nullopt));

    EXPECT_CALL(*mockDatabase, fetchProductDetails(productId))
        .WillOnce(::testing::Return(std::nullopt));

    auto result = productService.getProductDetails(productId);

    ASSERT_FALSE(result.has_value());
}

// Test case 6: Cache eviction when full
TEST_F(ProductServiceTest, TestCacheEviction) {
    uint64_t productId = 6;
    Product expectedProduct(productId, 103, "Product 6", "Description of Product 6", {});

    ProductCache cache(2);

    cache.put(1, Product(1, 100, "Product 1", "Description of Product 1", {}));
    cache.put(2, Product(2, 101, "Product 2", "Description of Product 2", {}));

    EXPECT_CALL(*mockCache, put(productId, expectedProduct))
        .Times(1);

    cache.put(productId, expectedProduct);

    auto result = cache.get(productId);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), expectedProduct.getId());
}