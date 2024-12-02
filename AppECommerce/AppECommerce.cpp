#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <format>

#include "ProductService.h"
#include "FakeDatabase.h"
#include "ProductCache.h"
#include "Logger.h"

void simulateProductFetch(const std::shared_ptr<ProductService>& productService, uint64_t productId) {
    auto product = productService->getProductDetails(productId);
    if (product) {
        std::cout << std::format(
            "Product ID: {}\n"
            "Product Name: {}\n"
            "Product Description: {}\n",
            product->getId(),
            product->getName(),
            product->getDescription()
        );
    }
    else {
        std::cout << "Product not found!\n";
    }
}

int main() {
    Logger::initialize("AppOutput.log");
    Logger::setLogLevel(LogLevel::INFO);

    auto cache = std::make_shared<ProductCache>(3);
    auto database = std::make_shared<FakeDatabase>();
    auto productService = std::make_shared<ProductService>(cache, database);

    std::vector<std::jthread> threads;

    // Simulate product fetches in parallel using multiple threads
    for (uint64_t i = 1; i <= 800; ++i) {
        threads.emplace_back(simulateProductFetch, productService, i);
    }

    return 0;
}
