#include "SerializerBinary.h"
#include "Serializable.h"
#include <QDataStream>
#include <QFile>
#include <QDebug>

namespace qstore {

void SerializerBinary::serialize(const Serializable &serializable,
                                 const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << serializable.saveToMap();        
        file.close();
    } else {
        qWarning() << "Can't write file" << filename;
    }
}

void SerializerBinary::deserialize(Serializable &serializable,
                                   const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {        
        QDataStream stream(&file);
        QVariantMap map;
        stream >> map;
        serializable.loadFromMap(map);
        file.close();
    } else {
        qWarning() << "Can't read file" << filename;
    }
}

} // namespace qstore


