#ifndef MCLIB_ENTITY_METADATA_H_
#define MCLIB_ENTITY_METADATA_H_

#include <mclib/common/DataBuffer.h>
#include <mclib/common/Position.h>
#include <mclib/inventory/Slot.h>
#include <mclib/common/Types.h>

#include <array>
#include <memory>

namespace mc {
namespace entity {

class EntityMetadata {
public:
    class Type {
    public:
        virtual ~Type() { }
    };

    struct ByteType : public Type {
        u8 value;

        ByteType() = default;
        ByteType(u8 value) : value(value) { }
    };

    struct VarIntType : public Type {
        VarInt value;

        VarIntType() = default;
        VarIntType(VarInt value) : value(value) { }
    };

    struct FloatType : public Type {
        float value;

        FloatType() = default;
        FloatType(float value) : value(value) { }
    };

    struct StringType : public Type {
        std::wstring value;

        StringType() = default;
        StringType(const std::wstring& value) : value(value) { }
    };

    struct SlotType : public Type {
        inventory::Slot value;

        SlotType() = default;
        SlotType(const inventory::Slot& value) : value(value) { }
    };

    struct BooleanType : public Type {
        bool value;

        BooleanType() = default;
        BooleanType(bool value) : value(value) { }
    };

    struct RotationType : public Type {
        Vector3f value;

        RotationType() = default;
        RotationType(Vector3f value) : value(value) { }
    };

    struct PositionType : public Type {
        bool exists;
        Position value;

        PositionType() = default;
        PositionType(bool exists, Position value) : exists(exists), value(value) { }
    };

    struct UUIDType : public Type {
        bool exists;
        UUID value;

        UUIDType() = default;
        UUIDType(bool exists, UUID value) : exists(exists), value(value) { }
    };

private:
    enum DataType { Byte, VarInt, Float, String, Chat, Slot, Boolean, Rotation, Position, OptPosition, Direction, OptUUID, OptBlockID, None };

    enum { MetadataCount = 36 };
    std::array<std::pair<std::unique_ptr<Type>, DataType>, MetadataCount> m_Metadata;

    void CopyOther(const EntityMetadata& other);
public:
    MCLIB_API EntityMetadata();

    EntityMetadata(const EntityMetadata& rhs);
    EntityMetadata& operator=(const EntityMetadata& rhs);
    EntityMetadata(EntityMetadata&& rhs) = default;
    EntityMetadata& operator=(EntityMetadata&& rhs) = default;

    template <typename T>
    T* GetIndex(std::size_t index) const {
        return dynamic_cast<T*>(m_Metadata[index].first.get());
    }

    friend MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
    friend MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata& metadata);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator<<(DataBuffer& out, const EntityMetadata::UUIDType& value);

MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata& metadata);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::ByteType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::VarIntType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::FloatType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::StringType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::SlotType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::BooleanType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::RotationType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::PositionType& value);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, EntityMetadata::UUIDType& value);

} // ns entity
} // ns mc

#endif
