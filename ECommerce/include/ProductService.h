#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include <memory>
#include <optional>
#include <shared_mutex>
#include "ICache.h"
#include "IDatabase.h"
#include "Product.h"

class ProductService {
public:
    ProductService(std::shared_ptr<ICache<uint64_t, Product>> cache,
        std::shared_ptr<IDatabase> database);

    std::optional<Product> getProductDetails(uint64_t productId) const;

private:
    std::shared_ptr<ICache<uint64_t, Product>> mCache;
    std::shared_ptr<IDatabase> mDatabase;

    mutable std::shared_mutex mCacheMutex;
};

#endif // PRODUCT_SERVICE_H
