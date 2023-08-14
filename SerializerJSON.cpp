#include "SerializerJSON.h"
#include "Serializable.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <stdexcept>

namespace qstore {

void qstore::SerializerJSON::serialize(const Serializable &serializable,
                                       const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)) {
        auto doc = QJsonDocument::fromVariant(serializable.saveToMap());
        file.write(doc.toJson());
        file.close();
    }else {
        throw std::runtime_error("Неудалось сохранить файл настроек");
    }
}

void qstore::SerializerJSON::deserialize(Serializable &serializable,
                                         const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(file.readAll(), &err);
        if(err.error == QJsonParseError::NoError) {
            serializable.loadFromMap(doc.toVariant().toMap());
        }
        file.close();
    } else {
        throw std::runtime_error("Неудалось прочитать файл настроек");
    }
}


}
