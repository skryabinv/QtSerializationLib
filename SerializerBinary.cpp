#include "SerializerBinary.h"
#include <QDataStream>
#include <QFile>

namespace qstore {

void SerializerBinary::serialize(const Serializable &storable,
                                 const QString &filename) {
}

void SerializerBinary::deserialize(Serializable &storable,
                                   const QString &filename) {
}

} // namespace qstore


