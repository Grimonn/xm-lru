#include "FakeDatabase.h"
#include "Product.h"
#include "Logger.h"

#include <format>
#include <algorithm>
#include <ranges>

constexpr unsigned int PRODUCTS_NBR = 3000;

FakeDatabase::FakeDatabase() {
    Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Initializing FakeDatabase with {} products...", std::to_string(PRODUCTS_NBR)));

    try {
        mProducts.reserve(PRODUCTS_NBR);  // Reserve space to avoid reallocations

        for (uint64_t i = 1; i <= PRODUCTS_NBR; ++i) {
            uint32_t category = (i % 3) + 100;  // Cycles through 100, 101, 102
            auto name = std::format("Product {}", i);
            auto description = std::format("Description of {}", name);

            std::vector<std::byte> thumbnail{
                std::byte { 'A' + (i % 26)},
                std::byte { 'B' + (i % 26)},
                std::byte { 'C' + (i % 26)}
            };

            mProducts.emplace(i, Product{ i, category, name, description, thumbnail });

            if (i % 500 == 0) {
                Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Added Product ID: {} to FakeDatabase", std::to_string(i)));
            }
        }
    }
    catch (const std::exception& e) {
        Logger::log(LogLevel::ERROR, LogCategory::DATABASE, std::format("Failed to initialize FakeDatabase: {}", e.what()));
        throw;
    }

    Logger::log(LogLevel::INFO, LogCategory::DATABASE, "FakeDatabase initialized successfully.");
}

std::optional<Product> FakeDatabase::fetchProductDetails(uint64_t productId) {
    Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Fetching product details for Product ID: {}", std::to_string(productId)));

    if (auto it = mProducts.find(productId); it != mProducts.end()) {
        Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Found Product ID: {} in FakeDatabase", std::to_string(productId)));
        return it->second;
    }

    Logger::log(LogLevel::WARNING, LogCategory::DATABASE, std::format("Product ID: {} not found in FakeDatabase.", std::to_string(productId)));
    return std::nullopt;
}

size_t FakeDatabase::fetchProductCountByCategory(uint32_t categoryId) {
    Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Counting mProducts in category ID: {}", std::to_string(categoryId)));

    auto filteredView = mProducts | std::ranges::views::filter([categoryId](const auto& productPair) {
        return productPair.second.getCategory() == categoryId;
        });

    size_t count = std::ranges::distance(filteredView);

    Logger::log(LogLevel::INFO, LogCategory::DATABASE, std::format("Found {} mProducts in category ID: {}", std::to_string(count), std::to_string(categoryId)));
    return count;
}
