#include <gtest/gtest.h>
#include <memory>
#include "FakeDatabase.h"
#include "Logger.h"

class FakeDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        fakeDatabase = std::make_shared<FakeDatabase>();
    }

    std::shared_ptr<FakeDatabase> fakeDatabase;
};

// Test case to verify fetching a valid product from the FakeDatabase
TEST_F(FakeDatabaseTest, FetchProductDetails_ValidId) {
    constexpr uint64_t productId = 1;  // Assuming product ID 1 is valid in FakeDatabase
    if (auto product = fakeDatabase->fetchProductDetails(productId)) {
        EXPECT_EQ(product->getId(), productId);
        EXPECT_EQ(product->getCategory(), 101);
        EXPECT_EQ(product->getName(), "Product 1");
    }
    else {
        FAIL() << "Product with ID " << productId << " should exist but was not found.";
    }
}

// Test case to verify fetching a non-existent product (invalid ID)
TEST_F(FakeDatabaseTest, FetchProductDetails_InvalidId) {
    constexpr uint64_t invalidId = 9999;  // Assuming this ID doesn't exist
    EXPECT_FALSE(fakeDatabase->fetchProductDetails(invalidId).has_value())
        << "Product with ID " << invalidId << " should not exist.";
}

// Test case to verify counting products by category (valid category)
TEST_F(FakeDatabaseTest, FetchProductCountByCategory_ValidCategory) {
    constexpr uint32_t categoryId = 101;
    auto count = fakeDatabase->fetchProductCountByCategory(categoryId);
    EXPECT_GT(count, 0) << "Category " << categoryId << " should have products.";
}

// Test case to verify counting products by category (empty or invalid category)
TEST_F(FakeDatabaseTest, FetchProductCountByCategory_InvalidCategory) {
    constexpr uint32_t invalidCategoryId = 999;  // Assuming no products in this category
    auto count = fakeDatabase->fetchProductCountByCategory(invalidCategoryId);
    EXPECT_EQ(count, 0) << "Category " << invalidCategoryId << " should be empty.";
}
