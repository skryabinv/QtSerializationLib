#pragma once

#include <QVariant>
#include <QStringList>
#include <tuple>
#include <stdexcept>

namespace qstore {

/*
 * Interface of the serializable object
 * */
class Serializable {
public:
    virtual ~Serializable() = default;
    virtual QVariantMap saveToMap() const = 0;
    virtual void loadFromMap(const QVariantMap& map) = 0;
};

}

