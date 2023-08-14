#include "PolymorphicSerializable.h"
#include "Serializable.h"

namespace qstore {

namespace details {

SerializableFactory::SerializableFactory() = default;
SerializableFactory::~SerializableFactory() = default;

SerializableFactory& SerializableFactory::getInstance() {
    static SerializableFactory instance;
    return instance;
}

std::unique_ptr<Serializable> SerializableFactory::makeStorable(const QString &className) const {
    auto result = std::unique_ptr<Serializable>{};
    auto it = mCreationMap.find(className);
    if(it != mCreationMap.end()) {
        result = it.value()();
    }
    return result;
}

}

std::unique_ptr<Serializable> PolymorphicSerializable::makeAndDeserialize(const QVariantMap& map) {
    auto className = map[sKeyClassName].toString();
    auto result = details::SerializableFactory::getInstance().makeStorable(className);
    if(result) {
        result->loadFromMap(map);
    }
    return result;
}

QVariantMap PolymorphicSerializable::saveToMap() const {
    auto map = saveToMapImpl();
    if(map.contains(sKeyClassName)) {
        throw std::runtime_error("Invalid use of the classname key");
    }
    auto className = getClassName();
    map[sKeyClassName] = className;
    return map;
}

void PolymorphicSerializable::loadFromMap(const QVariantMap &map) {
    // Compare className with the stored className
    auto className = getClassName();
    if(map[sKeyClassName] != className) {
        throw std::runtime_error("Invalid type of the deserialed object");
    }
    loadFromMapImpl(map);
}

}
