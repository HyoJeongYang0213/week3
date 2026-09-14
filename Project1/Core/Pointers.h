#pragma once

#include <memory>
#include <utility>
#include <optional>

template <typename T>
using TUniquePtr = std::unique_ptr<T>;

template <typename T, typename ...Args>
TUniquePtr<T> MakeUnique(Args&&... InArgs)
{
	return std::make_unique<T>(std::forward<Args>(InArgs)...);
}

template <typename T>
using TSharedPtr = std::shared_ptr<T>;

template <typename T, typename ...Args>
TSharedPtr<T> MakeShared(Args&&... InArgs)
{
	return std::make_shared<T>(std::forward<Args>(InArgs)...);
}

template <typename T>
using TWeakPtr = std::weak_ptr<T>;

template <typename T>
using TOptional = std::optional<T>;
using FNullopt = std::nullopt_t;
inline constexpr FNullopt Nullopt = std::nullopt;

