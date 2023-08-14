#pragma once

class QString;

namespace qstore {

class Serializable;

class Serializer {
public:
    virtual ~Serializer() = default;
    virtual void serialize(const Serializable& serializable,
                           const QString& filename) = 0;
    virtual void deserialize(Serializable& serializable,
                             const QString& filename) = 0;
};

}
