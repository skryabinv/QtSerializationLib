#pragma once

#include <QVariant>
#include <tuple>
#include "PolymorphicSerializable.h"

namespace qstore {

namespace details {

template<typename ...Args, size_t ...Indices>
auto fromSequence(const QVariantList& list, std::index_sequence<Indices...>) {    
    return std::tuple{list[Indices].value<Args>()...};
}

}

struct EmptyTransform {
    template<typename T>
    auto operator()(T value) const {
        return value;
    }
};

template<typename T>
struct FromVariantTransform {
    auto operator()(const QVariant& var) const {
        return var.value<T>();
    }
};

template<typename T>
struct DeserializeMakeTransform {
    auto operator()(const QVariant& var) const {
        return qstore::PolymorphicSerializable::make<T>(var.toMap());
    }
};

template<typename ...Args>
QVariantList packToList(Args... args) {
    return QVariantList{QVariant::fromValue(args)...};
}

template<typename ...Args>
auto packTupleToList(const std::tuple<Args...>& tuple) {
    return std::apply([](auto... vals){
        return QVariantList{vals...};
    }, tuple);
}

template<typename ...Args>
auto unpackFromList(const QVariantList& list) {
    if(list.size() == sizeof...(Args)) {
        return details::fromSequence<Args...>(list, std::make_index_sequence<sizeof...(Args)>());
    }
    return std::tuple<Args...>{};
}

template<typename It, typename Callable = EmptyTransform>
QVariantList toVariantList(It begin, It end, Callable callable = {}) {
    QVariantList result;
    std::transform(begin, end,
                   std::back_inserter(result),
                   callable);
    return result;
}

template<typename It, typename Callable>
auto fromVariantList(const QVariantList& list,
              It begin,
              Callable callable) {
    std::transform(list.cbegin(), list.cend(),
                   begin,
                   callable);
}

}
