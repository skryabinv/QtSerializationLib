#pragma once

#include "Serializer.h"
#include "SerializationLib_global.h"

namespace qstore {

class SERIALIZATIONLIB_EXPORT SerializerBinary : public Serializer {
    // Serializer interface
public:
    void serialize(const Serializable &serializable, const QString &filename) override;
    void deserialize(Serializable &serializable, const QString &filename) override;
};

} // namespace qstore

