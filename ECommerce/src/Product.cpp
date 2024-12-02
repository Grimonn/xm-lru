#include "Product.h"

Product::Product(uint64_t id, uint32_t category, std::string_view name, std::string_view description, const std::vector<std::byte>& thumbnail)
    : mID{ id }
    , mCategory{ category }
    , mName{ name }
    , mDescription{ description }
    , mThumbnail{ thumbnail } {
}

[[nodiscard]] uint64_t Product::getId() const noexcept { return mID; }
[[nodiscard]] uint32_t Product::getCategory() const noexcept { return mCategory; }
[[nodiscard]] std::string_view Product::getName() const noexcept { return mName; }
[[nodiscard]] std::string_view Product::getDescription() const noexcept { return mDescription; }
[[nodiscard]] std::vector<std::byte> Product::getThumbnail() const noexcept { return mThumbnail; }
