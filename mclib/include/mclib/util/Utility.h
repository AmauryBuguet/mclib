#ifndef MCLIB_UTIL_UTILITY_H
#define MCLIB_UTIL_UTILITY_H

#include <mclib/common/AABB.h>
#include <mclib/common/Vector.h>
#include <mclib/core/Connection.h>
#include <mclib/core/PlayerManager.h>
#include <mclib/world/World.h>

#include <json/json.h>
#include <fstream>
#include <string>
#include <queue>

namespace mc {

inline std::string to_string(const std::string& str) {
    return str;
}

inline std::string to_string(const std::wstring& str) {
    return std::string(str.begin(), str.end());
}

namespace util {

MCLIB_API std::string ParseChatNode(Json::Value node);
MCLIB_API std::string StripChatMessage(const std::string& message);
MCLIB_API s64 GetTime();

class PlayerController : public core::PlayerListener {
private:
    core::PlayerManager& m_PlayerManager;
    core::Connection* m_Connection;
    world::World& m_World;
    Vector3d m_Position;
    float m_Yaw;
    float m_Pitch;
    AABB m_BoundingBox;
    EntityId m_EntityId;
    u64 m_LastUpdate;
    Vector3d m_TargetPos;
    bool m_Sprinting;
    bool m_LoadedIn;
    bool m_HandleFall;

    double m_MoveSpeed;

    std::queue<Vector3d> m_DigQueue;

    // todo: gravity
    const float FallSpeed = 8.3f * (50.0f / 1000.0f);

public:
    MCLIB_API PlayerController(core::Connection* connection, world::World& world, core::PlayerManager& playerManager);
    MCLIB_API ~PlayerController();

    void MCLIB_API OnClientSpawn(core::PlayerPtr player);
    bool MCLIB_API ClearPath(Vector3d target);

    void MCLIB_API Dig(Vector3d target);
    void MCLIB_API Attack(EntityId id);
    void MCLIB_API Move(Vector3d delta);

    bool MCLIB_API HandleJump();
    bool MCLIB_API HandleFall();
    void MCLIB_API UpdateDigging();
    void MCLIB_API UpdatePosition();
    void MCLIB_API Update();

    bool MCLIB_API InLoadedChunk() const;
    Vector3d MCLIB_API GetPosition() const;
    Vector3d MCLIB_API GetHeading() const;
    float MCLIB_API GetYaw() const;
    float MCLIB_API GetPitch() const;
    AABB MCLIB_API GetBoundingBox() const;

    void MCLIB_API SetYaw(float yaw);
    void MCLIB_API SetPitch(float pitch);
    void MCLIB_API LookAt(Vector3d target);
    void MCLIB_API SetMoveSpeed(double speed);
    void MCLIB_API SetTargetPosition(Vector3d target);
    void MCLIB_API SetHandleFall(bool handle);
};

class IConsole {
public:
    virtual void Output(const std::string& str) = 0;
    virtual void Output(const std::wstring& str) = 0;

    virtual IConsole& operator<<(const std::string& str) = 0;
    virtual IConsole& operator<<(const std::wstring& str) = 0;

    template <typename T>
    IConsole& operator<<(T data) {
        using std::to_string;
        using mc::to_string;

        Output(to_string(data));
        return *this;
    }

    IConsole& operator<<(const char* data) {
        Output(std::string(data));
        return *this;
    }

    IConsole& operator<<(const wchar_t* data) {
        Output(std::wstring(data));
        return *this;
    }
};


class Console {
private:
    IConsole* m_Impl;

public:
    Console() : m_Impl(nullptr) { }

    // Doesn't take control of impl
    void SetImpl(IConsole* impl) {
        m_Impl = impl;
    }

    IConsole* GetImpl() const {
        return m_Impl;
    }

    void Output(const std::string& str) {
        if (m_Impl)
            m_Impl->Output(str);
    }

    void Output(const std::wstring& str) {
        if (m_Impl)
            m_Impl->Output(str);
    }

    template <typename T>
    Console& operator<<(const T& data) {
        using std::to_string;
        using mc::to_string;
        Output(to_string(data));
        return *this;
    }

    template <typename T>
    Console& operator<<(const std::string& str) {
        Output(str);
        return *this;
    }

    template <typename T>
    Console& operator<<(const std::wstring& str) {
        Output(str);
        return *this;
    }

    Console& operator<<(const char* str) {
        Output(std::string(str));
        return *this;
    }

    Console& operator<<(const wchar_t* str) {
        Output(std::wstring(str));
        return *this;
    }
};

class LoggerConsole : public IConsole {
private:
    std::ofstream m_Out;
    std::string m_Filename;

public:
    LoggerConsole(const std::string& filename) {
        m_Out.open(filename.c_str(), std::ios::out);
    }

    ~LoggerConsole() {
        m_Out.close();
    }

    void Output(const std::string& str) {
        m_Out << str << std::endl;
    }

    void Output(const std::wstring& str) {
        m_Out << std::string(str.begin(), str.end()) << std::endl;
    }

    IConsole& operator<<(const std::string& str) {
        Output(str);
        return *this;
    }
    IConsole& operator<<(const std::wstring& str) {
        Output(str);
        return *this;
    }
};

} // ns util
} // ns mc

#endif // UTILITY_H
