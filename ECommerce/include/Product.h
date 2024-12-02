#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <cstdint>

class Product {
public:
    explicit Product(uint64_t id,
        uint32_t category,
        std::string_view name,
        std::string_view description,
        const std::vector<std::byte>& thumbnail);

    [[nodiscard]] uint64_t getId() const noexcept;
    [[nodiscard]] uint32_t getCategory() const noexcept;
    [[nodiscard]] std::string_view getName() const noexcept;
    [[nodiscard]] std::string_view getDescription() const noexcept;
    [[nodiscard]] std::vector<std::byte> getThumbnail() const noexcept;

    bool operator==(const Product& other) const = default;

private:
    uint64_t mID;
    uint32_t mCategory;
    std::string mName;
    std::string mDescription;
    std::vector<std::byte> mThumbnail;
};

#endif // PRODUCT_H
