#pragma once
#include <compare>
#include <utility>
#include <cstddef>
#include <functional>

namespace util {

/*
  Вспомогательный шаблонный класс "Маркированный тип".
  С его помощью можно описать строгий тип на основе другого типа.
 */

template <typename Value, typename Tag>
class Tagged {
public:
    using ValueType = Value;
    using TagType = Tag;

    explicit Tagged(Value&& v) : value_(std::move(v)) {}

    explicit Tagged(const Value& v) : value_(v) {}

    const Value& operator*() const {
        return value_;
    }

    Value& operator*() {
        return value_;
    }

    // Так в C++20 можно объявить оператор сравнения Tagged-типов (будет просто вызван соответствующий оператор для поля value_)
    auto operator<=>(const Tagged<Value, Tag>&) const = default;

private:
    Value value_;
};

// хэшер для Tagged-типа, чтобы Tagged-объекты можно было хранить в unordered-контейнерах
template <typename TaggedValue>
struct TaggedHasher {
    size_t operator()(const TaggedValue& value) const {
        // Возвращает хеш значения, хранящегося внутри value
        return std::hash<typename TaggedValue::ValueType>{}(*value);
    }
};

}  // namespace util