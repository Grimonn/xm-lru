#ifndef FAKE_DATABASE_H
#define FAKE_DATABASE_H

#include "IDatabase.h"

#include <unordered_map>
#include <optional>

class Product;

class FakeDatabase : public IDatabase {
public:
    FakeDatabase();
    std::optional<Product> fetchProductDetails(uint64_t productId) override;
    size_t fetchProductCountByCategory(uint32_t categoryId) override;

private:
    std::unordered_map<uint64_t, Product> mProducts;
};

#endif // FAKE_DATABASE_H
