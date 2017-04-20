#include <mclib/protocol/packets/Packet.h>

#include <mclib/inventory/Slot.h>
#include <mclib/protocol/packets/PacketHandler.h>

#include <iostream>

namespace {

template <typename T>
class FixedPointNumber {
private:
    T m_IntRep;
    
public:
    FixedPointNumber() : m_IntRep(0) { }
    FixedPointNumber(T intRep) : m_IntRep(intRep) { }
    
    float GetFloat() const { 
        return (float)(m_IntRep >> 5) + (m_IntRep & 31) / 32.0f;
    }

    friend mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s8>& fpn);
    friend mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s32>& fpn);
};

mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s8>& fpn) {
    return in >> fpn.m_IntRep;
}

mc::DataBuffer& operator>>(mc::DataBuffer& in, FixedPointNumber<s32>& fpn) {
    return in >> fpn.m_IntRep;
}

}

namespace mc {
namespace protocol {
namespace packets {

namespace in {

// Play packets
SpawnObjectPacket::SpawnObjectPacket() {
    m_Id = 0x00;
}

bool SpawnObjectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    data >> m_UUID;
    data >> m_Type;

    double x, y, z;

    data >> x >> y >> z;

    m_Position = Vector3f((float)x, (float)y, (float)z);

    data >> m_Pitch >> m_Yaw;

    data >> m_Data;
    data >> m_Velocity.x >> m_Velocity.y >> m_Velocity.z;

    return true;
}

void SpawnObjectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


SpawnExperienceOrbPacket::SpawnExperienceOrbPacket() {
    m_Id = 0x01;
}

bool SpawnExperienceOrbPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    double x, y, z;

    data >> eid >> x >> y >> z >> m_Count;

    m_EntityId = eid.GetInt();
    m_Position.x = x;
    m_Position.y = y;
    m_Position.z = z;

    return true;
}

void SpawnExperienceOrbPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnGlobalEntityPacket::SpawnGlobalEntityPacket() {
    m_Id = 0x02;
}

bool SpawnGlobalEntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    double x, y, z;

    data >> eid >> m_Type;

    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    return true;
}

void SpawnGlobalEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


SpawnMobPacket::SpawnMobPacket() {
    m_Id = 0x03;
}

bool SpawnMobPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt entityId, type;

    data >> entityId;
    m_EntityId = entityId.GetInt();
    data >> m_UUID;

    data >> type;
    m_Type = type.GetInt();

    double x, y, z;
    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    data >> m_Yaw;
    data >> m_Pitch;
    data >> m_HeadPitch;

    short vx, vy, vz;
    data >> vx >> vy >> vz;
    m_Velocity = Vector3s(vx, vy, vz);
    data >> m_Metadata;

    return true;
}

void SpawnMobPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPaintingPacket::SpawnPaintingPacket() {
    m_Id = 0x04;
}

bool SpawnPaintingPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    MCString title;
    Position position;
    u8 direction;

    data >> eid >> m_UUID >> title >> position >> direction;

    m_EntityId = eid.GetInt();
    m_Title = title.GetUTF16();
    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    m_Direction = (Direction)direction;

    return true;
}

void SpawnPaintingPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPlayerPacket::SpawnPlayerPacket() {
    m_Id = 0x05;
}

bool SpawnPlayerPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    data >> m_UUID;

    double x, y, z;
    data >> x >> y >> z;

    m_Position = Vector3d(x, y, z);

    data >> m_Yaw;
    data >> m_Pitch;

    data >> m_Metadata;
    return true;
}

void SpawnPlayerPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AnimationPacket::AnimationPacket() {
    m_Id = 0x06;
}

bool AnimationPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    u8 anim;
    data >> anim;

    m_Animation = (Animation)anim;
    return true;
}

void AnimationPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

StatisticsPacket::StatisticsPacket() {
    m_Id = 0x07;
}

bool StatisticsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt count;
    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        MCString name;
        VarInt value;

        data >> name;
        data >> value;

        m_Statistics[name.GetUTF16()] = value.GetInt();
    }

    return true;
}

void StatisticsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockBreakAnimationPacket::BlockBreakAnimationPacket() {
    m_Id = 0x08;
}

bool BlockBreakAnimationPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    Position position;

    data >> eid >> position >> m_DestroyStage;

    m_EntityId = eid.GetInt();
    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();

    return true;
}

void BlockBreakAnimationPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateBlockEntityPacket::UpdateBlockEntityPacket() {
    m_Id = 0x09;
}

bool UpdateBlockEntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position pos;
    u8 action;

    data >> pos;
    data >> action;

    nbt::NBT nbt;
    data >> nbt;

    m_BlockEntity = block::BlockEntity::CreateFromNBT(&nbt);
    m_Position = Vector3i(pos.GetX(), pos.GetY(), pos.GetZ());
    m_Action = (Action)action;

    return true;
}

void UpdateBlockEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockActionPacket::BlockActionPacket() {
    m_Id = 0x0A;
}

bool BlockActionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position position;
    VarInt type;

    data >> position >> m_ActionId >> m_ActionParam >> type;

    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    m_BlockType = type.GetInt();

    return true;
}

void BlockActionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BlockChangePacket::BlockChangePacket() {
    m_Id = 0x0B;
}

bool BlockChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position location;
    VarInt blockId;

    data >> location >> blockId;

    m_Position = Vector3i(location.GetX(), location.GetY(), location.GetZ());
    m_BlockId = blockId.GetInt();

    return true;
}
void BlockChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

BossBarPacket::BossBarPacket() {
    m_Id = 0x0C;
}

bool BossBarPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    static int DivisionCounts[] = { 0, 6, 10, 12, 20 };
    VarInt action;

    data >> m_UUID >> action;
    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case Action::Add:
        {
            MCString title;
            VarInt color, divisions;

            data >> title >> m_Health >> color >> divisions >> m_Flags;

            m_Title = title.GetUTF16();
            m_Color = (Color)color.GetInt();
            m_Divisions = DivisionCounts[divisions.GetInt()];
        }
        break;
        case Action::Remove:
        {
        }
        break;
        case Action::UpdateHealth:
        {
            data >> m_Health;
        }
        break;
        case Action::UpdateTitle:
        {
            MCString title;

            data >> title;
            m_Title = title.GetUTF16();
        }
        break;
        case Action::UpdateStyle:
        {
            VarInt color, divisions;

            data >> color >> divisions;

            m_Color = (Color)color.GetInt();
            m_Divisions = DivisionCounts[divisions.GetInt()];
        }
        break;
        case Action::UpdateFlags:
        {
            data >> m_Flags;
        }
        break;
    }

    return true;
}
void BossBarPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ServerDifficultyPacket::ServerDifficultyPacket() {
    m_Id = 0x0D;
}

bool ServerDifficultyPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Difficulty;
    return true;
}

void ServerDifficultyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TabCompletePacket::TabCompletePacket() {
    m_Id = 0x0E;
}

bool TabCompletePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt count;

    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        MCString match;

        data >> match;

        m_Matches.push_back(match.GetUTF16());
    }

    return true;
}

void TabCompletePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChatPacket::ChatPacket() {
    m_Id = 0x0F;
}

bool ChatPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString chatData;
    u8 position;

    data >> chatData;
    data >> position;

    m_Position = (ChatPosition)position;

    Json::Reader reader;

    reader.parse(chatData.GetUTF8(), m_ChatData);

    return true;
}

void ChatPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MultiBlockChangePacket::MultiBlockChangePacket() {
    m_Id = 0x10;
}

bool MultiBlockChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_ChunkX >> m_ChunkZ;
    VarInt count;
    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        u8 horizontal;
        u8 y;
        VarInt blockID;

        data >> horizontal >> y >> blockID;

        BlockChange change;
        change.x = horizontal >> 4;
        change.z = horizontal & 15;
        change.y = y;
        change.blockData = blockID.GetShort();

        m_BlockChanges.push_back(change);
    }
    return true;
}

void MultiBlockChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ConfirmTransactionPacket::ConfirmTransactionPacket() {
    m_Id = 0x11;
}

bool ConfirmTransactionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId >> m_Action >> m_Accepted;
    return true;
}

void ConfirmTransactionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CloseWindowPacket::CloseWindowPacket() {
    m_Id = 0x12;
}

bool CloseWindowPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    return true;
}

void CloseWindowPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

OpenWindowPacket::OpenWindowPacket() {
    m_Id = 0x13;
}

bool OpenWindowPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString type, title;

    data >> m_WindowId >> type >> title >> m_SlotCount;
    
    m_WindowType = type.GetUTF16();
    m_WindowTitle = title.GetUTF16();

    m_EntityId = 0;
    if (m_WindowType.compare(L"EntityHorse") == 0)
        data >> m_EntityId;

    return true;
}

void OpenWindowPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WindowItemsPacket::WindowItemsPacket() {
    m_Id = 0x14;
}

bool WindowItemsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    s16 count;
    data >> count;

    for (s16 i = 0; i < count; ++i) {
        inventory::Slot slot;
        data >> slot;

        m_Slots.push_back(slot);
    }

    return true;
}

void WindowItemsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WindowPropertyPacket::WindowPropertyPacket() {
    m_Id = 0x15;
}

bool WindowPropertyPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId >> m_Property >> m_Value;
    return true;
}

void WindowPropertyPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetSlotPacket::SetSlotPacket() {
    m_Id = 0x16;
}

bool SetSlotPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WindowId;
    data >> m_SlotIndex;
    data >> m_Slot;
    return true;
}

void SetSlotPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetCooldownPacket::SetCooldownPacket() {
    m_Id = 0x17;
}

bool SetCooldownPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt item, ticks;
    data >> item >> ticks;

    m_ItemId = item.GetInt();
    m_Ticks = ticks.GetInt();

    return true;
}

void SetCooldownPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PluginMessagePacket::PluginMessagePacket() {
    m_Id = 0x18;
}

bool PluginMessagePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    std::size_t begin = data.GetReadOffset();

    data >> m_Channel;

    data.ReadSome(m_Data, data.GetSize() - (data.GetReadOffset() - begin) - 1);

    return true;
}

void PluginMessagePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

NamedSoundEffectPacket::NamedSoundEffectPacket() {
    m_Id = 0x19;
}

bool NamedSoundEffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString name;
    VarInt category;

    data >> name >> category;

    m_Name = name.GetUTF16();
    m_Category = (SoundCategory)category.GetInt();

    FixedPointNumber<u32> x, y, z;
    
    data >> x >> y >> z;
    m_Position.x = (double)x.GetFloat();
    m_Position.y = (double)y.GetFloat();
    m_Position.z = (double)z.GetFloat();

    data >> m_Volume >> m_Pitch;

    return true;
}

void NamedSoundEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityStatusPacket::EntityStatusPacket() {
    m_Id = 0x1B;
}

bool EntityStatusPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_Status;
    return true;
}

void EntityStatusPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ExplosionPacket::ExplosionPacket() {
    m_Id = 0x1C;
}

bool ExplosionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    float posX, posY, posZ;
    s32 count;

    data >> posX >> posY >> posZ;

    m_Position = Vector3d(posX, posY, posZ);

    data >> m_Radius;
    data >> count;

    for (s32 i = 0; i < count; ++i) {
        s8 x, y, z;
        data >> x >> y >> z;
        m_AffectedBlocks.push_back(Vector3s(x, y, z));
    }

    float motionX, motionY, motionZ;

    data >> motionX >> motionY >> motionZ;

    m_PlayerMotion = Vector3d(motionX, motionY, motionZ);

    return true;
}

void ExplosionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UnloadChunkPacket::UnloadChunkPacket() {
    m_Id = 0x1D;
}

bool UnloadChunkPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_ChunkX >> m_ChunkZ;
    return true;
}

void UnloadChunkPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChangeGameStatePacket::ChangeGameStatePacket() {
    m_Id = 0x1E;
}

bool ChangeGameStatePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    u8 reason;
    data >> reason;

    m_Reason = (Reason)reason;

    data >> m_Value;

    return true;
}

void ChangeGameStatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

KeepAlivePacket::KeepAlivePacket() {
    m_Id = 0x1F;
}

bool KeepAlivePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt aliveId;

    data >> aliveId;

    m_AliveId = aliveId.GetLong();

    return true;
}

void KeepAlivePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ChunkDataPacket::ChunkDataPacket() {
    m_Id = 0x20;
}

bool ChunkDataPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    world::ChunkColumnMetadata metadata;

    data >> metadata.x;
    data >> metadata.z;
    data >> metadata.continuous;
    VarInt mask;
    data >> mask;
    metadata.sectionmask = mask.GetInt();

    VarInt size;

    data >> size;

    m_ChunkColumn = std::make_shared<world::ChunkColumn>(metadata);

    data >> *m_ChunkColumn;

    // Skip biome information
    if (metadata.continuous)
        data.SetReadOffset(data.GetReadOffset() + 256);

    VarInt entities;
    data >> entities;

    s32 entityCount = entities.GetInt();

    for (s32 i = 0; i < entities.GetInt(); ++i) {
        nbt::NBT nbt;

        data >> nbt;

        block::BlockEntityPtr blockEntity = block::BlockEntity::CreateFromNBT(&nbt);

        if (blockEntity == nullptr) continue;

        m_BlockEntities.push_back(blockEntity);
        m_ChunkColumn->AddBlockEntity(blockEntity);
    }

    return true;
}

void ChunkDataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EffectPacket::EffectPacket() {
    m_Id = 0x21;
}

bool EffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EffectId;
    Position pos;
    data >> pos;

    m_Position.x = pos.GetX();
    m_Position.y = pos.GetY();
    m_Position.z = pos.GetZ();

    data >> m_Data;
    data >> m_DisableRelativeVolume;

    return true;
}

void EffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ParticlePacket::ParticlePacket() {
    m_Id = 0x22;
}

bool ParticlePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_ParticleId >> m_LongDistance;

    float x, y, z;
    data >> x >> y >> z;
    m_Position = Vector3d(x, y, z);

    float offsetX, offsetY, offsetZ;
    data >> offsetX >> offsetY >> offsetZ;
    m_MaxOffset = Vector3d(offsetX, offsetY, offsetZ);

    data >> m_ParticleData >> m_Count;

    if (m_ParticleId == 36) { // iconcrack
        for (s32 i = 0; i < 2; ++i) {
            VarInt varData;
            data >> varData;
            m_Data.push_back(varData.GetInt());
        }
    } else if (m_ParticleId == 37 || m_ParticleId == 38) { // blockcrack || blockdust
        VarInt varData;
        data >> varData;
        m_Data.push_back(varData.GetInt());
    }

    return true;
}

void ParticlePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

JoinGamePacket::JoinGamePacket() {
    m_Id = 0x23;
}

bool JoinGamePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_Gamemode;
    data >> m_Dimension;
    data >> m_Difficulty;
    data >> m_MaxPlayers;
    data >> m_LevelType;
    data >> m_ReducedDebug;
    return true;
}

void JoinGamePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

MapPacket::MapPacket() {
    m_Id = 0x24;
}

bool MapPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt damage, count;

    data >> damage >> m_Scale >> m_TrackPosition >> count;
    m_MapId = damage.GetInt();

    for (s32 i = 0; i < count.GetInt(); ++i) {
        Icon icon;
        data >> icon.direction >> icon.x >> icon.z;

        icon.type = icon.direction & 0x0F;
        icon.direction = (icon.direction & 0xF0) >> 4;

        m_Icons.push_back(icon);
    }

    data >> m_Columns;

    if (m_Columns > 0) {
        VarInt length;

        data >> m_Rows >> m_X >> m_Z >> length;
        data.ReadSome(m_Data, length.GetInt());
    }

    return true;
}

void MapPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


EntityRelativeMovePacket::EntityRelativeMovePacket() {
    m_Id = 0x25;
}

bool EntityRelativeMovePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    data >> m_Delta.x >> m_Delta.y >> m_Delta.z;
    data >> m_OnGround;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityLookAndRelativeMovePacket::EntityLookAndRelativeMovePacket() {
    m_Id = 0x26;
}

bool EntityLookAndRelativeMovePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid >> m_Delta.x >> m_Delta.y >> m_Delta.z;
    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityLookAndRelativeMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityLookPacket::EntityLookPacket() {
    m_Id = 0x27;
}

bool EntityLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;
    return true;
}

void EntityLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityPacket::EntityPacket() {
    m_Id = 0x28;
}

bool EntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

VehicleMovePacket::VehicleMovePacket() {
    m_Id = 0x29;
}

bool VehicleMovePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Position.x >> m_Position.y >> m_Position.z >> m_Yaw >> m_Pitch;
    return true;
}

void VehicleMovePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

OpenSignEditorPacket::OpenSignEditorPacket() {
    m_Id = 0x2A;
}

bool OpenSignEditorPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    Position position;

    data >> position;

    m_Position.x = position.GetX();
    m_Position.y = position.GetY();
    m_Position.z = position.GetZ();
    return true;
}

void OpenSignEditorPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerAbilitiesPacket::PlayerAbilitiesPacket() {
    m_Id = 0x2B;
}

bool PlayerAbilitiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Flags;
    data >> m_FlyingSpeed;
    data >> m_FOVModifier;

    return true;
}

void PlayerAbilitiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CombatEventPacket::CombatEventPacket() {
    m_Id = 0x2C;
}

bool CombatEventPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt event;

    data >> event;

    m_Event = (Event)event.GetInt();

    if (m_Event == Event::EndCombat) {
        VarInt duration;

        data >> duration;

        m_Duration = duration.GetInt();
        data >> m_EntityId;
    } else if (m_Event == Event::EntityDead) {
        VarInt pid;
        data >> pid;

        m_PlayerId = pid.GetInt();
        data >> m_EntityId;

        MCString message;
        data >> message;

        m_Message = message.GetUTF16();
    }

    return true;
}

void CombatEventPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerListItemPacket::PlayerListItemPacket() {
    m_Id = 0x2D;
}

bool PlayerListItemPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;
    VarInt numPlayers;

    data >> action;
    data >> numPlayers;

    m_Action = (Action)action.GetInt();

    for (s32 i = 0; i < numPlayers.GetInt(); ++i) {
        UUID uuid;
        data >> uuid;

        ActionDataPtr actionData = std::make_shared<ActionData>();
        actionData->uuid = uuid;

        

        switch (m_Action) {
        case Action::AddPlayer:
        {
            MCString name;
            VarInt numProperties;

            data >> name;
            data >> numProperties;

            actionData->name = name.GetUTF16();

            for (s32 j = 0; j < numProperties.GetInt(); ++j) {
                MCString propertyName;
                MCString propertyValue;
                u8 isSigned;
                MCString signature;

                data >> propertyName;
                data >> propertyValue;
                data >> isSigned;
                if (isSigned)
                    data >> signature;

                actionData->properties[propertyName.GetUTF16()] = propertyValue.GetUTF16();
            }

            VarInt gameMode, ping;
            data >> gameMode;
            data >> ping;

            u8 hasDisplayName;
            MCString displayName;

            data >> hasDisplayName;
            if (hasDisplayName)
                data >> displayName;

            actionData->gamemode = gameMode.GetInt();
            actionData->ping = ping.GetInt();
            actionData->displayName = displayName.GetUTF16();
        }
        break;
        case Action::UpdateGamemode:
        {
            VarInt gameMode;
            data >> gameMode;

            actionData->gamemode = gameMode.GetInt();
        }
        break;
        case Action::UpdateLatency:
        {
            VarInt ping;
            data >> ping;

            actionData->ping = ping.GetInt();
        }
        break;
        case Action::UpdateDisplay:
        {
            u8 hasDisplayName;
            MCString displayName;

            data >> hasDisplayName;
            if (hasDisplayName)
                data >> displayName;

            actionData->displayName = displayName.GetUTF16();
        }
        break;
        case Action::RemovePlayer:
        {
            std::shared_ptr<ActionData> actionData = std::make_shared<ActionData>();
            actionData->uuid = uuid;
        }
        break;
        }

        m_Data.push_back(actionData);
    }

    return true;
}

void PlayerListItemPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket() {
    m_Id = 0x2E;
}

bool PlayerPositionAndLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Position.x >> m_Position.y >> m_Position.z;
    data >> m_Yaw >> m_Pitch;
    data >> m_Flags;
    VarInt tid;
    data >> tid;

    m_TeleportId = tid.GetInt();
    return true;
}

void PlayerPositionAndLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UseBedPacket::UseBedPacket() {
    m_Id = 0x2F;
}

bool UseBedPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    Position location;

    data >> eid >> location;

    m_EntityId = eid.GetInt();
    m_Position.x = location.GetX();
    m_Position.y = location.GetY();
    m_Position.z = location.GetZ();
    return true;
}

void UseBedPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

DestroyEntitiesPacket::DestroyEntitiesPacket() {
    m_Id = 0x30;
}

bool DestroyEntitiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt count;

    data >> count;

    for (s32 i = 0; i < count.GetInt(); ++i) {
        VarInt eid;

        data >> eid;

        m_EntityIds.push_back(eid.GetInt());
    }
    return true;
}

void DestroyEntitiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

RemoveEntityEffectPacket::RemoveEntityEffectPacket() {
    m_Id = 0x31;
}

bool RemoveEntityEffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid >> m_EffectId;
    m_EntityId = eid.GetInt();
    return true;
}

void RemoveEntityEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ResourcePackSendPacket::ResourcePackSendPacket() {
    m_Id = 0x32;
}

bool ResourcePackSendPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString url, hash;

    data >> url >> hash;

    m_Url = url.GetUTF16();
    m_Hash = hash.GetUTF8();

    return true;
}

void ResourcePackSendPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

RespawnPacket::RespawnPacket() {
    m_Id = 0x33;
}

bool RespawnPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Dimension;
    data >> m_Difficulty;
    data >> m_Gamemode;

    MCString level;
    data >> level;

    m_Level = level.GetUTF16();

    return true;
}

void RespawnPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityHeadLookPacket::EntityHeadLookPacket() {
    m_Id = 0x34;
}

bool EntityHeadLookPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    data >> m_Yaw;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityHeadLookPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

WorldBorderPacket::WorldBorderPacket() {
    m_Id = 0x35;
}

bool WorldBorderPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;

    data >> action;

    m_Action = (Action)action.GetInt();

    switch (m_Action) {
    case Action::SetSize:
    {
        data >> m_Diameter;
    }
    break;
    case Action::LerpSize:
    {
        data >> m_OldDiameter;
        data >> m_Diameter;
        VarLong speed;

        data >> speed;
        m_Speed = speed.GetLong();
    }
    break;
    case Action::SetCenter:
    {
        data >> m_X >> m_Z;
    }
    break;
    case Action::Initialize:
    {
        data >> m_X >> m_Z;

        data >> m_OldDiameter >> m_Diameter;

        VarLong speed;
        data >> speed;
        m_Speed = speed.GetLong();

        VarInt portalTeleportBoundary, warningTime, warningBlocks;

        data >> portalTeleportBoundary >> warningTime >> warningBlocks;

        m_PortalTeleportBoundary = portalTeleportBoundary.GetInt();
        m_WarningTime = warningTime.GetInt();
        m_WarningBlocks = warningBlocks.GetInt();
    }
    break;
    case Action::SetWarningTime:
    {
        VarInt warningTime;
        data >> warningTime;
        m_WarningTime = warningTime.GetInt();
    }
    break;
    case Action::SetWarningBlocks:
    {
        VarInt warningBlocks;
        data >> warningBlocks;
        m_WarningBlocks = warningBlocks.GetInt();
    }
    break;
    }
    return true;
}

void WorldBorderPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CameraPacket::CameraPacket() {
    m_Id = 0x36;
}

bool CameraPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt id;
    data >> id;
    m_EntityId = id.GetInt();
    return true;
}

void CameraPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

HeldItemChangePacket::HeldItemChangePacket() {
    m_Id = 0x37;
}

bool HeldItemChangePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Slot;
    return true;
}

void HeldItemChangePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

DisplayScoreboardPacket::DisplayScoreboardPacket() {
    m_Id = 0x38;
}

bool DisplayScoreboardPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    u8 pos;
    MCString name;

    data >> pos >> name;

    m_Position = (ScoreboardPosition)pos;
    m_Name = name.GetUTF16();

    return true;
}

void DisplayScoreboardPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


EntityMetadataPacket::EntityMetadataPacket() {
    m_Id = 0x39;
}

bool EntityMetadataPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    data >> m_Metadata;

    m_EntityId = eid.GetInt();

    return true;
}

void EntityMetadataPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

AttachEntityPacket::AttachEntityPacket() {
    m_Id = 0x3A;
}

bool AttachEntityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_EntityId;
    data >> m_VehicleId;

    return true;
}

void AttachEntityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityVelocityPacket::EntityVelocityPacket() {
    m_Id = 0x3B;
}

bool EntityVelocityPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    data >> eid;
    data >> m_Velocity.x;
    data >> m_Velocity.y;
    data >> m_Velocity.z;
    m_EntityId = eid.GetInt();
    return true;
}

void EntityVelocityPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityEquipmentPacket::EntityEquipmentPacket() {
    m_Id = 0x3C;
}

bool EntityEquipmentPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;
    VarInt equipmentSlot;

    data >> eid;
    data >> equipmentSlot;
    data >> m_Item;

    m_EntityId = eid.GetInt();
    m_EquipmentSlot = (EquipmentSlot)equipmentSlot.GetInt();

    return true;
}

void EntityEquipmentPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetExperiencePacket::SetExperiencePacket() {
    m_Id = 0x3D;
}

bool SetExperiencePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt level, total;
    data >> m_ExperienceBar >> level >> total;
    m_Level = level.GetInt();
    m_TotalExperience = level.GetInt();
    return true;
}

void SetExperiencePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateHealthPacket::UpdateHealthPacket() {
    m_Id = 0x3E;
}

bool UpdateHealthPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt food;
    data >> m_Health >> food >> m_Saturation;
    m_Food = food.GetInt();
    return true;
}

void UpdateHealthPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

ScoreboardObjectivePacket::ScoreboardObjectivePacket() {
    m_Id = 0x3F;
}

bool ScoreboardObjectivePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString objective, value, type;
    u8 mode;

    data >> objective >> mode >> value >> type;

    m_Objective = objective.GetUTF16();
    m_Mode = (Mode)mode;
    m_Value = value.GetUTF16();
    m_Type = type.GetUTF16();

    return true;
}

void ScoreboardObjectivePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetPassengersPacket::SetPassengersPacket() {
    m_Id = 0x40;
}

bool SetPassengersPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid, count;

    data >> eid >> count;
    m_EntityId = eid.GetInt();

    for (s32 i = 0; i < count.GetInt(); ++i) {
        VarInt peid;

        data >> peid;
        m_Passengers.push_back(peid.GetInt());
    }

    return true;
}

void SetPassengersPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TeamsPacket::TeamsPacket() {
    m_Id = 0x41;
}

bool TeamsPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString name;
    u8 mode;

    data >> name >> mode;

    m_TeamName = name.GetUTF16();
    m_Mode = (Mode)mode;

    switch (m_Mode) {
        case Mode::Create:
        {
            MCString display, prefix, suffix, visbility, collision;
            VarInt count;

            data >> display >> prefix >> suffix >> m_FriendlyFlags >> visbility >> collision >> m_Color >> count;

            m_TeamDisplayName = display.GetUTF16();
            m_TeamPrefix = prefix.GetUTF16();
            m_TeamSuffix = suffix.GetUTF16();
            m_TagVisbility = visbility.GetUTF16();
            m_CollisionRule = collision.GetUTF16();

            for (s32 i = 0; i < count.GetInt(); ++i) {
                MCString player;
                data >> player;

                m_Players.push_back(player.GetUTF16());
            }
        }
        break;
        case Mode::Remove:
        {

        }
        break;
        case Mode::Update:
        {
            MCString display, prefix, suffix, visbility, collision;

            data >> display >> prefix >> suffix >> m_FriendlyFlags >> visbility >> collision >> m_Color;

            m_TeamDisplayName = display.GetUTF16();
            m_TeamPrefix = prefix.GetUTF16();
            m_TeamSuffix = suffix.GetUTF16();
            m_TagVisbility = visbility.GetUTF16();
            m_CollisionRule = collision.GetUTF16();
        }
        break;
        case Mode::AddPlayer:
        case Mode::RemovePlayer:
        {
            VarInt count;

            data >> count;

            for (s32 i = 0; i < count.GetInt(); ++i) {
                MCString player;
                data >> player;

                m_Players.push_back(player.GetUTF16());
            }
        }
        break;
    }

    return true;
}

void TeamsPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

UpdateScorePacket::UpdateScorePacket() {
    m_Id = 0x42;
}

bool UpdateScorePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString name, objective;
    u8 action;

    data >> name >> action >> objective;

    m_ScoreName = name.GetUTF16();
    m_Action = (Action)action;
    m_Objective = objective.GetUTF16();
    
    if (m_Action != Action::Remove) {
        VarInt value;
        data >> value;
        m_Value = value.GetInt();
    }
    return true;
}

void UpdateScorePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SpawnPositionPacket::SpawnPositionPacket() {
    m_Id = 0x43;
}

bool SpawnPositionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Location;
    return true;
}

void SpawnPositionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TimeUpdatePacket::TimeUpdatePacket() {
    m_Id = 0x44;
}

bool TimeUpdatePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_WorldAge;
    data >> m_Time;
    return true;
}

void TimeUpdatePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

TitlePacket::TitlePacket() {
    m_Id = 0x45;
}

bool TitlePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt action;

    data >> action;

    m_Action = (Action)action.GetInt();

    switch (m_Action) {
        case Action::SetTitle:
        case Action::SetSubtitle:
        case Action::SetActionBar:
        {
            MCString text;
            data >> text;

            m_Text = text.GetUTF16();
        }
        break;
        case Action::SetDisplay:
        {
            data >> m_FadeIn >> m_Stay >> m_FadeOut;
        }
        break;
        default:
        break;
    }
    
    return true;
}

void TitlePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SoundEffectPacket::SoundEffectPacket() {
    m_Id = 0x46;
}

bool SoundEffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt id, category;
    
    data >> id >> category;

    m_SoundId = id.GetInt();
    m_Category = (SoundCategory)category.GetInt();

    FixedPointNumber<int> x, y, z;
    data >> x >> y >> z;

    m_Position.x = (double)x.GetFloat();
    m_Position.y = (double)y.GetFloat();
    m_Position.z = (double)z.GetFloat();
    data >> m_Volume;
    data >> m_Pitch;

    return true;
}

void SoundEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PlayerListHeaderAndFooterPacket::PlayerListHeaderAndFooterPacket() {
    m_Id = 0x47;
}

bool PlayerListHeaderAndFooterPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString header, footer;

    data >> header >> footer;

    m_Header = header.GetUTF16();
    m_Footer = footer.GetUTF16();

    return true;
}

void PlayerListHeaderAndFooterPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

CollectItemPacket::CollectItemPacket() {
    m_Id = 0x48;
}

bool CollectItemPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt collected, collector, count;

    data >> collected >> collector >> count;

    m_Collected = collected.GetInt();
    m_Collector = collector.GetInt();
    m_PickupCount = count.GetInt();
    
    return true;
}

void CollectItemPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityTeleportPacket::EntityTeleportPacket() {
    m_Id = 0x49;
}

bool EntityTeleportPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;

    m_EntityId = eid.GetInt();

    data >> m_Position.x >> m_Position.y >> m_Position.z;

    data >> m_Yaw >> m_Pitch;
    data >> m_OnGround;

    return true;
}

void EntityTeleportPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityPropertiesPacket::EntityPropertiesPacket() {
    m_Id = 0x4A;
}

bool EntityPropertiesPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid;

    data >> eid;
    m_EntityId = eid.GetInt();

    s32 propertyCount;
    data >> propertyCount;

    for (s32 i = 0; i < propertyCount; ++i) {
        MCString key;
        double value;

        data >> key;
        data >> value;

        mc::entity::Attribute attribute(key.GetUTF16(), value);

        VarInt modifierCount;
        data >> modifierCount;

        for (s32 j = 0; j < modifierCount.GetInt(); ++j) {
            UUID uuid;
            double amount;
            u8 operation;

            data >> uuid;
            data >> amount;
            data >> operation;

            mc::entity::Modifier modifier(uuid, amount, (mc::entity::ModifierOperation)operation);

            attribute.AddModifier(modifier);
        }

        m_Properties.insert(std::make_pair(attribute.GetKey(), attribute));
    }
    return true;
}

void EntityPropertiesPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EntityEffectPacket::EntityEffectPacket() {
    m_Id = 0x4B;
}

bool EntityEffectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt eid, duration;
    data >> eid >> m_EffectId >> m_Amplifier >> duration >> m_Flags;
    m_EntityId = eid.GetInt();
    m_Duration = duration.GetInt();
    return true;
}

void EntityEffectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}


// Login packets
DisconnectPacket::DisconnectPacket() {
    m_Id = 0x00;
    m_ProtocolState = protocol::State::Login;
}

bool DisconnectPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Reason;
    return true;
}

void DisconnectPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

EncryptionRequestPacket::EncryptionRequestPacket() {
    m_Id = 0x01;
    m_ProtocolState = protocol::State::Login;
}

bool EncryptionRequestPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    VarInt pubKeyLen;
    VarInt verifyTokenLen;

    data >> m_ServerId;

    data >> pubKeyLen;
    data.ReadSome(m_PublicKey, pubKeyLen.GetInt());

    data >> verifyTokenLen;
    data.ReadSome(m_VerifyToken, verifyTokenLen.GetInt());

    return true;
}

void EncryptionRequestPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

LoginSuccessPacket::LoginSuccessPacket() {
    m_Id = 0x02;
    m_ProtocolState = protocol::State::Login;
}

bool LoginSuccessPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_UUID;
    data >> m_Username;
    return true;
}

void LoginSuccessPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

SetCompressionPacket::SetCompressionPacket() {
    m_Id = 0x03;
    m_ProtocolState = protocol::State::Login;
}

bool SetCompressionPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_MaxPacketSize;
    return true;
}

void SetCompressionPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

namespace status {

ResponsePacket::ResponsePacket() {
    m_Id = 0x00;
    m_ProtocolState = protocol::State::Status;
}

bool ResponsePacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    MCString response;

    data >> response;

    m_Response = response.GetUTF16();

    return true;
}

void ResponsePacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

PongPacket::PongPacket() {
    m_Id = 0x01;
    m_ProtocolState = protocol::State::Status;
}

bool PongPacket::Deserialize(DataBuffer& data, std::size_t packetLength) {
    data >> m_Payload;

    return true;
}

void PongPacket::Dispatch(PacketHandler* handler) {
    handler->HandlePacket(this);
}

} // ns status

} // ns in

namespace out {

// Handshake packets
HandshakePacket::HandshakePacket(s32 protocol, std::string server, u16 port, protocol::State state) 
    : m_ProtocolVersion(protocol), m_Server(server), m_Port(port), m_NewState((s32)state)
{
    m_Id = 0x00;
}

DataBuffer HandshakePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_ProtocolVersion << m_Server << m_Port << m_NewState;

    return buffer;
}

// Login packets

LoginStartPacket::LoginStartPacket(const std::string& name) 
    : m_Name(name)
{
    m_Id = 0x00;
}

DataBuffer LoginStartPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Name;
    return buffer;
}

EncryptionResponsePacket::EncryptionResponsePacket(const std::string& sharedSecret, const std::string& verifyToken)
    : m_SharedSecret(sharedSecret), m_VerifyToken(verifyToken)
{
    m_Id = 0x01;
}

DataBuffer EncryptionResponsePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    VarInt sharedLength = (s32)m_SharedSecret.length();
    VarInt verifyLength = (s32)m_VerifyToken.length();
    buffer << sharedLength;
    buffer << m_SharedSecret;
    buffer << verifyLength;
    buffer << m_VerifyToken;
    return buffer;
}

// Play packets

TeleportConfirmPacket::TeleportConfirmPacket(s32 teleportId) : m_TeleportId(teleportId) {
    m_Id = 0x00;
}

DataBuffer TeleportConfirmPacket::Serialize() const {
    DataBuffer buffer;
    VarInt teleportId(m_TeleportId);

    buffer << m_Id;
    buffer << teleportId;

    return buffer;
}

TabCompletePacket::TabCompletePacket(const std::wstring& text, bool assumeCommand) 
    : m_Text(text), m_AssumeCommand(assumeCommand), m_HasPosition(false)
{
    m_Id = 0x01;
}

TabCompletePacket::TabCompletePacket(const std::wstring& text, bool assumeCommand, bool hasPosition, Position lookingAt)
    : m_Text(text), m_AssumeCommand(assumeCommand), m_HasPosition(hasPosition), m_LookingAt(lookingAt)
{
    m_Id = 0x01;
}

DataBuffer TabCompletePacket::Serialize() const {
    DataBuffer buffer;
    MCString text(m_Text);

    buffer << m_Id << text << m_AssumeCommand << m_HasPosition;

    if (m_HasPosition)
        buffer << m_LookingAt;

    return buffer;
}

ChatPacket::ChatPacket(const std::wstring& message) : m_Message(message) {
    m_Id = 0x02;
}

ChatPacket::ChatPacket(const std::string& message) : m_Message(message.begin(), message.end()) {
    m_Id = 0x02;
}

DataBuffer ChatPacket::Serialize() const {
    MCString out(m_Message);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << out;

    return buffer;
}

ClientStatusPacket::ClientStatusPacket(Action action) : m_Action(action) {
    m_Id = 0x03;
}

DataBuffer ClientStatusPacket::Serialize() const {
    VarInt action(m_Action);
    DataBuffer buffer;

    buffer << m_Id;
    buffer << action;

    return buffer;
}

ClientSettingsPacket::ClientSettingsPacket(const std::wstring& locale, u8 viewDistance, ChatMode chatMode, bool chatColors, u8 skinFlags, MainHand hand)
    : m_Locale(locale), m_ViewDistance(viewDistance), m_ChatMode(chatMode),
    m_ChatColors(chatColors), m_SkinFlags(skinFlags), m_MainHand(hand)
{
    m_Id = 0x04;
}

DataBuffer ClientSettingsPacket::Serialize() const {
    MCString locale(m_Locale);
    DataBuffer buffer;
    VarInt chatMode((int)m_ChatMode);
    VarInt hand((int)m_MainHand);

    buffer << m_Id;
    buffer << locale;
    buffer << m_ViewDistance;
    buffer << chatMode;
    buffer << m_ChatColors;
    buffer << m_SkinFlags;
    buffer << hand;

    return buffer;
}


ConfirmTransactionPacket::ConfirmTransactionPacket(u8 windowId, s16 action, bool accepted) 
    : m_WindowId(windowId), m_Action(action), m_Accepted(accepted)
{
    m_Id = 0x05;
}

DataBuffer ConfirmTransactionPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;
    buffer << m_Action;
    buffer << m_Accepted;

    return buffer;
}

EnchantItemPacket::EnchantItemPacket(u8 windowId, u8 enchantmentIndex) {
    m_Id = 0x06;
}

DataBuffer EnchantItemPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_WindowId << m_EnchantmentIndex;

    return buffer;
}

ClickWindowPacket::ClickWindowPacket(u8 windowId, u16 slotIndex, u8 button, u16 action, s32 mode, inventory::Slot clickedItem) 
    : m_WindowId(windowId), m_SlotIndex(slotIndex), m_Button(button), m_Action(action), m_Mode(mode), m_ClickedItem(clickedItem)
{
    m_Id = 0x07;
}

DataBuffer ClickWindowPacket::Serialize() const {
    DataBuffer buffer;
    
    buffer << m_Id;
    buffer << m_WindowId << m_SlotIndex << m_Button << m_Action;
    VarInt mode(m_Mode);
    buffer << mode << m_ClickedItem;
    
    return buffer;
}

CloseWindowPacket::CloseWindowPacket(u8 windowId)
    : m_WindowId(windowId)
{
    m_Id = 0x08;
}

DataBuffer CloseWindowPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_WindowId;

    return buffer;
}

PluginMessagePacket::PluginMessagePacket(const std::wstring& channel, const std::string& data) 
    : m_Channel(channel), m_Data(data)
{
    m_Id = 0x09;
}

DataBuffer PluginMessagePacket::Serialize() const {
    DataBuffer buffer;
    MCString channel(m_Channel);

    buffer << m_Id << channel;
    buffer << m_Data;

    return buffer;
}

UseEntityPacket::UseEntityPacket(EntityId target, Action action, Hand hand, Vector3f position)
    : m_Target(target), m_Action(action), m_Hand(hand), m_Position(position)
{
    m_Id = 0x0A;
}

DataBuffer UseEntityPacket::Serialize() const {
    DataBuffer buffer;
    VarInt target(m_Target);
    VarInt type((int)m_Action);

    buffer << m_Id;
    buffer << target;
    buffer << type;

    if (m_Action == Action::InteractAt) {
        buffer << m_Position.x;
        buffer << m_Position.y;
        buffer << m_Position.z;
    }

    if (m_Action == Action::Interact || m_Action == Action::InteractAt) {
        VarInt hand((int)m_Hand);
        buffer << hand;
    }

    return buffer;
}

KeepAlivePacket::KeepAlivePacket(s64 id) : m_KeepAliveId(id) {
    m_Id = 0x0B;
}

DataBuffer KeepAlivePacket::Serialize() const {
    DataBuffer buffer;
    VarInt aliveId(m_KeepAliveId);

    buffer << m_Id;
    buffer << aliveId;

    return buffer;
}

PlayerPositionPacket::PlayerPositionPacket(Vector3d position, bool onGround)
    : m_Position(position), m_OnGround(onGround)
{
    m_Id = 0x0C;
}

DataBuffer PlayerPositionPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_OnGround;
    return buffer;
}

PlayerPositionAndLookPacket::PlayerPositionAndLookPacket(Vector3d position, float yaw, float pitch, bool onGround)
    : m_Position(position), m_Yaw(yaw), m_Pitch(pitch), m_OnGround(onGround)
{
    m_Id = 0x0D;
}

DataBuffer PlayerPositionAndLookPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_Yaw << m_Pitch;
    buffer << m_OnGround;

    return buffer;
}

PlayerLookPacket::PlayerLookPacket(float yaw, float pitch, bool onGround)
    : m_Yaw(yaw), m_Pitch(pitch), m_OnGround(onGround)
{
    m_Id = 0x0E;
}

DataBuffer PlayerLookPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_Yaw << m_Pitch;
    buffer << m_OnGround;
    return buffer;
}


PlayerPacket::PlayerPacket(bool onGround)
    : m_OnGround(onGround)
{
    m_Id = 0x0F;
}

DataBuffer PlayerPacket::Serialize() const {
    DataBuffer buffer;
    buffer << m_Id;
    buffer << m_OnGround;
    return buffer;
}

VehicleMovePacket::VehicleMovePacket(Vector3d position, float yaw, float pitch)
    : m_Position(position), m_Yaw(yaw), m_Pitch(pitch)
{
    m_Id = 0x10;
}

DataBuffer VehicleMovePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Position.x << m_Position.y << m_Position.z;
    buffer << m_Yaw << m_Pitch;

    return buffer;
}

SteerBoatPacket::SteerBoatPacket(bool rightPaddle, bool leftPaddle)
    : m_RightPaddle(rightPaddle), m_LeftPaddle(leftPaddle)
{
    m_Id = 0x11;
}

DataBuffer SteerBoatPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_RightPaddle << m_LeftPaddle;

    return buffer;
}

PlayerAbilitiesPacket::PlayerAbilitiesPacket(bool isFlying)
    : m_IsFlying(isFlying)
{
    m_Id = 0x12;
}

DataBuffer PlayerAbilitiesPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    u8 flags = (u8)m_IsFlying << 1;
    float flyingSpeed = 0.0f;
    float walkingSpeed = 0.0f;

    buffer << flags << flyingSpeed << walkingSpeed;

    return buffer;
}

PlayerDiggingPacket::PlayerDiggingPacket(Status status, Vector3i position, Face face)
    : m_Status(status), m_Position(position), m_Face(face)
{
    m_Id = 0x13;
}

DataBuffer PlayerDiggingPacket::Serialize() const {
    DataBuffer buffer;
    Position location((s32)m_Position.x, (s32)m_Position.y, (s32)m_Position.z);

    buffer << m_Id;
    buffer << (u8)m_Status;
    buffer << location;
    buffer << (u8)m_Face;

    return buffer;
}

EntityActionPacket::EntityActionPacket(EntityId eid, Action action, s32 actionData)
    : m_EntityId(eid), m_Action(action), m_ActionData(actionData)
{
    m_Id = 0x14;
}

DataBuffer EntityActionPacket::Serialize() const {
    DataBuffer buffer;
    VarInt eid(m_EntityId);
    VarInt action((s32)m_Action);
    VarInt actionData(m_ActionData);

    buffer << m_Id;
    buffer << eid << action << actionData;

    return buffer;
}

SteerVehiclePacket::SteerVehiclePacket(float sideways, float forward, u8 flags)
    : m_Sideways(sideways), m_Forward(forward), m_Flags(flags)
{
    m_Id = 0x15;
}

DataBuffer SteerVehiclePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Sideways << m_Forward << m_Flags;

    return buffer;
}

ResourcePackStatusPacket::ResourcePackStatusPacket(Result result) 
    : m_Result(result)
{
    m_Id = 0x16;
}

DataBuffer ResourcePackStatusPacket::Serialize() const {
    DataBuffer buffer;
    VarInt result((int)m_Result);

    buffer << m_Id << result;

    return buffer;
}

HeldItemChangePacket::HeldItemChangePacket(u16 slot)
    : m_Slot(slot)
{
    m_Id = 0x17;
}

DataBuffer HeldItemChangePacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Slot;

    return buffer;
}

CreativeInventoryActionPacket::CreativeInventoryActionPacket(s16 slot, inventory::Slot item)
    : m_Slot(slot),
    m_Item(item)
{
    m_Id = 0x18;
}

DataBuffer CreativeInventoryActionPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;
    buffer << m_Slot;
    buffer << m_Item;

    return buffer;
}

UpdateSignPacket::UpdateSignPacket(Vector3d position, const std::wstring& line1, const std::wstring& line2, 
        const std::wstring& line3, const std::wstring& line4) 
    : m_Line1(line1), m_Line2(line2), m_Line3(line3), m_Line4(line4)
{
    m_Id = 0x19;
    m_Position = Position((s32)position.x, (s32)position.y, (s32)position.z);
}

DataBuffer UpdateSignPacket::Serialize() const {
    DataBuffer buffer;
    MCString line1(m_Line1);
    MCString line2(m_Line2);
    MCString line3(m_Line3);
    MCString line4(m_Line4);

    buffer << m_Id;
    buffer << m_Position;
    buffer << line1 << line2;
    buffer << line3 << line4;

    return buffer;
}

AnimationPacket::AnimationPacket(Hand hand) 
    : m_Hand(hand) 
{
    m_Id = 0x1A;
}

DataBuffer AnimationPacket::Serialize() const {
    DataBuffer buffer;
    VarInt hand((int)m_Hand);

    buffer << m_Id << hand;

    return buffer;
}

SpectatePacket::SpectatePacket(UUID uuid)
    : m_UUID(uuid)
{
    m_Id = 0x1B;
}

DataBuffer SpectatePacket::Serialize() const {
    DataBuffer buffer;
    
    buffer << m_Id << m_UUID;

    return buffer;
}

PlayerBlockPlacementPacket::PlayerBlockPlacementPacket(Vector3i position, Face face, Hand hand, Vector3f cursorPos) 
    : m_Position(position), m_Face(face), m_Hand(hand), m_CursorPos(cursorPos)
{
    m_Id = 0x1C;
}

DataBuffer PlayerBlockPlacementPacket::Serialize() const {
    DataBuffer buffer;
    Position location((s32)m_Position.x, (s32)m_Position.y, (s32)m_Position.z);
    VarInt face((u8)m_Face), hand((int)m_Hand);

    buffer << m_Id;
    buffer << location;
    buffer << face;
    buffer << hand;
    buffer << m_CursorPos.x;
    buffer << m_CursorPos.y;
    buffer << m_CursorPos.z;

    return buffer;
}

UseItemPacket::UseItemPacket(Hand hand)
    : m_Hand(hand)
{
    m_Id = 0x1D;
}

DataBuffer UseItemPacket::Serialize() const {
    DataBuffer buffer;
    VarInt hand((int)m_Hand);

    buffer << m_Id << hand;

    return buffer;
}

namespace status {

RequestPacket::RequestPacket() {
    m_Id = 0x00;
    m_ProtocolState = protocol::State::Status;
}

DataBuffer RequestPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id;

    return buffer;
}

PingPacket::PingPacket(s64 payload) : m_Payload(payload) {
    m_Id = 0x01;
    m_ProtocolState = protocol::State::Status;
}

DataBuffer PingPacket::Serialize() const {
    DataBuffer buffer;

    buffer << m_Id << m_Payload;

    return buffer;
}

} // ns status

} // ns out
} // ns packets
} // ns protocol
} // ns mc
