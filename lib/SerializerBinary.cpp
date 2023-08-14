#include "SerializerBinary.h"
#include <QDataStream>
#include <QFile>

namespace qstore {

void SerializerBinary::serialize(const Serializable &serializable,
                                 const QString &filename) {
}

void SerializerBinary::deserialize(Serializable &serializable,
                                   const QString &filename) {
}

} // namespace qstore


