#pragma once

#include "Serializer.h"

namespace qstore {

class SerializerJSON : public Serializer {
    // Serializer interface
public:
    void serialize(const Serializable &storable, const QString &filename) override;
    void deserialize(Serializable &storable, const QString &filename) override;
};

}
