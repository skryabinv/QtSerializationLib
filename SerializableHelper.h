#pragma once

#include <QStringList>
#include <QVariant>
#include <tuple>
#include <stdexcept>

namespace qstore {

template<typename ...ValueTypes>
class SerializableHelper {
public:
    SerializableHelper(QStringList keys)
        : mKeys{std::move(keys)} {
        if(mKeys.size() != sizeof...(ValueTypes)) {
            throw std::runtime_error{"The size of keys array must be equal "
                                     "to the count of type parameters"};
        }
    }
    auto size() const {
        return mKeys.size();
    }
    auto keys() const {
        return mKeys;
    }

    auto toMap(ValueTypes ...values) const {
        return toMapImpl(std::make_integer_sequence<int, sizeof...(ValueTypes)>(),
                         std::move(values)...);
    }
    auto fromMap(const QVariantMap& map) const {
        return fromMapImpl(std::make_integer_sequence<int, sizeof...(ValueTypes)>(), map);
    }

    template<typename ...Callable>
    QVariantMap saveFromFuncs(Callable ...funcs) const {
        return toMap(funcs()...);
    }

    template<typename ...Callable>
    void passToFuncs(const QVariantMap& map, Callable ...funcs) const {
        std::apply([&funcs...](auto ...values) {
            (funcs(values), ...);
        }, fromMap(map));
    }

private:
    QStringList mKeys;

    template<int ...Indices>
    QVariantMap toMapImpl(std::integer_sequence<int, Indices...>, ValueTypes... values) const {
        return QVariantMap{{mKeys[Indices], QVariant::fromValue(values)}...};
    }
    template<int ...Indices>
    std::tuple<ValueTypes...> fromMapImpl(std::integer_sequence<int, Indices...>,
                                          const QVariantMap& map) const {
        return std::tuple{map[mKeys[Indices]].value<ValueTypes>()...};
    }
};

}
