#pragma once

#include <QMap>
#include <QString>
#include <functional>
#include <memory>
#include <type_traits>
#include "Serializable.h"

namespace qstore {

/*
 * Polymorphic storable types must be derived from this class
 * It create more for interface like hierarchies (with empty
 * */
class PolymorphicSerializable : public Serializable {

public:
    /*
     * Use this method to create and deserialize object of the polymorphic type
     * */
    static std::unique_ptr<Serializable> makeAndDeserialize(const QVariantMap& map);

private:
    /*
     * Do not store anything with this key, when save data in the derived class!
     * */
    inline static auto sKeyClassName = QStringLiteral("PolymorphicStorable_ClassName_Key");

    /*
     * Unique string to define storable type
     * */
    virtual QString getClassName() const = 0;
    /*
     * Save only data of the derived class
     * */
    virtual QVariantMap saveToMapImpl() const = 0;
    /*
     * Load only data of the derived class
     * */
    virtual void loadFromMapImpl(const QVariantMap& map) const = 0;

    // Storable interface
public:
    QVariantMap saveToMap() const override {
        auto map = saveToMapImpl();
        if(map.contains(sKeyClassName)) {
            throw std::runtime_error("Invalid use of the classname key");
        }
        auto className = getClassName();
        map[sKeyClassName] = className;
        return map;
    }
    void loadFromMap(const QVariantMap &map) override {
        // Compare className with the stored className
        auto className = getClassName();
        if(map[sKeyClassName] != className) {
            throw std::runtime_error("Invalid type of the deserialed object");
        }
        loadFromMapImpl(map);
    }
};

#define QSTORE_CAT_1(a, b) a##b
#define QSTORE_CAT_2(a, b) QSTORE_CAT_1(a, b)

/*
 * Use this macros in the class translation unit to add a storable classname key to the object factory
 * */
#define QSTORE_REGISTER_SERIALIZABLE(Type, ClassNameString) \
static qstore::details::SerializableRegistrator<Type> QSTORE_CAT_2(polymorphicsSerializable_, __LINE__)(ClassNameString);

namespace details {

/*
 * Factory to create object by the classname key
 * */
class SerializableFactory {
public:
    SerializableFactory(SerializableFactory&&) = delete;
    ~SerializableFactory();

    static SerializableFactory& getInstance();

    template<typename T>
    void registerStorable(const QString& className) {
        static_assert(std::is_base_of_v<Serializable, T>, "Type of the object must be derived from Storable");
        static_assert(std::is_default_constructible_v<T>, "Type of the object must be default constuctible");
        mCreationMap[className] = std::make_unique<T>;
    }

    std::unique_ptr<Serializable> makeStorable(const QString& className) const;

private:
    SerializableFactory();
    using CreationMethod = std::function<std::unique_ptr<Serializable>()>;
    QMap<QString, CreationMethod> mCreationMap;
};

template<typename T>
class SerializableRegistrator {
public:
    SerializableRegistrator(const QString& className) {
        SerializableFactory::getInstance().registerStorable<T>(className);
    }
    SerializableRegistrator(SerializableRegistrator&&) = delete;
};

}

}




