#ifndef IDATABASE_H
#define IDATABASE_H

#include "Product.h"

#include <optional>

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual std::optional<Product> fetchProductDetails(uint64_t productId) = 0;
    virtual size_t fetchProductCountByCategory(uint32_t categoryId) = 0;
};

#endif // IDATABASE_H
