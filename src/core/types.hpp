// core/types.hpp

#pragma once

#include <cstdint>

#include <string>
#include <string_view>

#include <vector>
#include <unordered_map>

#include <memory>

#include <variant>
#include <optional>
#include <expected>

// Basic types

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using Bool = bool;
using Byte = u8;
using Char = char;
using UInt = u64;
using Int = i64;
using Float = f64;

// Size types

using Size = u64;
using Diff = i64;
using Pos = u32;
using SourceId = u32;

// String types

using String = std::string;
using StringView = std::string_view;

// Container types

template <typename T>
using Vector = std::vector<T>;

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

// Pointer types

template <typename T>
using UniquePtr = std::unique_ptr<T>;

// Variant types

template <typename... Ts>
using Variant = std::variant<Ts...>;
template <typename T>
using Option = std::optional<T>;
template<typename T, typename E>
using Result = std::expected<T, E>;