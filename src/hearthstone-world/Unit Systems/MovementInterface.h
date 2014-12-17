/***
 * Demonstrike Core
 */

#pragma once

enum MovementFlagsA : uint8 // First bytes of movement are client set | Actual movement identifiers
{
    MOVEMENTFLAG_MOVE_FORWARD           = 0x01,
    MOVEMENTFLAG_MOVE_BACKWARD          = 0x02,
    MOVEMENTFLAG_MOVE_STRAFE_LEFT       = 0x04,
    MOVEMENTFLAG_MOVE_STRAFE_RIGHT      = 0x08,
    MOVEMENTFLAG_MOVE_TURN_LEFT         = 0x10,
    MOVEMENTFLAG_MOVE_TURN_RIGHT        = 0x20,
    MOVEMENTFLAG_MOVE_PITCH_UP          = 0x40,
    MOVEMENTFLAG_MOVE_PITCH_DOWN        = 0x80,

    MOVEMENTFLAG_MASK_MOVING            = 0x0F,
    MOVEMENTFLAG_MASK_TURNING           = 0x30,
    MOVEMENTFLAG_MASK_PITCHING          = 0xC0
};

enum MovementFlagsB : uint8
{
    MOVEMENTFLAG_TOGGLE_WALK            = 0x01,
    MOVEMENTFLAG_TOGGLE_NO_GRAVITY      = 0x02,
    MOVEMENTFLAG_TOGGLE_ROOT            = 0x04,
    MOVEMENTFLAG_TOGGLE_FALLING         = 0x08,
    MOVEMENTFLAG_TOGGLE_FALLING_FAR     = 0x10,
    MOVEMENTFLAG_PENDING_STOP           = 0x20,
    MOVEMENTFLAG_PENDING_STRAFE_STOP    = 0x40,
    MOVEMENTFLAG_PENDING_FORWARD        = 0x80,
};

enum MovementFlagsC : uint8
{
    MOVEMENTFLAG_PENDING_BACKWARD       = 0x01,
    MOVEMENTFLAG_PENDING_STRAFE_LEFT    = 0x02,
    MOVEMENTFLAG_PENDING_STRAFE_RIGHT   = 0x04,
    MOVEMENTFLAG_PENDING_ROOT           = 0x08,
    MOVEMENTFLAG_SWIMMING               = 0x10,
    MOVEMENTFLAG_ASCENDING              = 0x20,
    MOVEMENTFLAG_DESCENDING             = 0x40,
    MOVEMENTFLAG_CAN_FLY                = 0x80,
};

enum MovementFlagsD : uint8
{
    MOVEMENTFLAG_FLYING                 = 0x01,
    MOVEMENTFLAG_SPLINE_ELEVATION       = 0x02,
    MOVEMENTFLAG_WATERWALKING           = 0x04,
    MOVEMENTFLAG_FEATHERFALLING         = 0x08,
    MOVEMENTFLAG_HOVER                  = 0x10,
    MOVEMENTFLAG_DISABLE_COLLISION      = 0x20,
};

enum MovementFlagsE : uint8
{
    MOVEMENTFLAG_NO_STRAFE              = 0x01,
    MOVEMENTFLAG_NO_JUMPING             = 0x02,
    MOVEMENTFLAG_FULL_SPEED_TURNING     = 0x04,
    MOVEMENTFLAG_FULL_SPEED_PITCHING    = 0x08,
    MOVEMENTFLAG_ALWAYS_ALLOW_PITCHING  = 0x10,
};

enum MovementFlagsF : uint8
{
    MOVEMENTFLAG_CAN_SWIM_TO_FLY_TRANSITION = 0x04,
    MOVEMENTFLAG_INTERPOLATED_MOVEMENT      = 0x40,
    MOVEMENTFLAG_INTERPOLATED_TURNING       = 0x80
};

enum MovementCodes : uint16
{
    // Movement codes
    MOVEMENT_CODE_PLAYER_MOVE = 0,
    MOVEMENT_CODE_HEARTBEAT,
    MOVEMENT_CODE_JUMP,
    MOVEMENT_CODE_FALL_LAND,
    MOVEMENT_CODE_START_FORWARD,
    MOVEMENT_CODE_START_BACKWARD,
    MOVEMENT_CODE_START_STRAFE_LEFT,
    MOVEMENT_CODE_START_STRAFE_RIGHT,
    MOVEMENT_CODE_START_TURN_LEFT,
    MOVEMENT_CODE_START_TURN_RIGHT,
    MOVEMENT_CODE_START_PITCH_DOWN,
    MOVEMENT_CODE_START_PITCH_UP,
    MOVEMENT_CODE_START_ASCEND,
    MOVEMENT_CODE_START_DESCEND,
    MOVEMENT_CODE_START_SWIM,
    MOVEMENT_CODE_STOP,
    MOVEMENT_CODE_STOP_STRAFE,
    MOVEMENT_CODE_STOP_TURN,
    MOVEMENT_CODE_STOP_PITCH,
    MOVEMENT_CODE_STOP_ASCEND,
    MOVEMENT_CODE_STOP_SWIM,
    MOVEMENT_CODE_SET_FACING,
    MOVEMENT_CODE_SET_PITCH,
    MOVEMENT_CODE_FALL_RESET,
    MOVEMENT_CODE_SET_RUN_MODE,
    MOVEMENT_CODE_SET_WALK_MODE,
    MOVEMENT_CODE_SET_PITCH_RATE,
    MOVEMENT_CODE_SET_CAN_FLY,
    MOVEMENT_CODE_UNSET_CAN_FLY,
    MOVEMENT_CODE_SET_HOVER,
    MOVEMENT_CODE_UNSET_HOVER,
    MOVEMENT_CODE_WATER_WALK,
    MOVEMENT_CODE_LAND_WALK,
    MOVEMENT_CODE_FEATHER_FALL,
    MOVEMENT_CODE_NORMAL_FALL,
    MOVEMENT_CODE_ROOT,
    MOVEMENT_CODE_UNROOT,
    MOVEMENT_CODE_UPDATE_KNOCK_BACK,
    MOVEMENT_CODE_UPDATE_TELEPORT,
    MOVEMENT_CODE_CHANGE_TRANSPORT,
    MOVEMENT_CODE_NOT_ACTIVE_MOVER,
    MOVEMENT_CODE_SET_COLLISION_HEIGHT,
    MOVEMENT_CODE_UPDATE_COLLISION_HEIGHT,

    // Speed codes
    MOVEMENT_CODE_UPDATE_WALK_SPEED,
    MOVEMENT_CODE_UPDATE_RUN_SPEED,
    MOVEMENT_CODE_UPDATE_RUN_BACK_SPEED,
    MOVEMENT_CODE_UPDATE_SWIM_SPEED,
    MOVEMENT_CODE_UPDATE_FLIGHT_SPEED,
    MOVEMENT_CODE_SET_WALK_SPEED,
    MOVEMENT_CODE_SET_RUN_SPEED,
    MOVEMENT_CODE_SET_RUN_BACK_SPEED,
    MOVEMENT_CODE_SET_SWIM_SPEED,
    MOVEMENT_CODE_SET_SWIM_BACK_SPEED,
    MOVEMENT_CODE_SET_FLIGHT_SPEED,
    MOVEMENT_CODE_SET_FLIGHT_BACK_SPEED,
    MOVEMENT_CODE_SET_TURN_RATE,

    // Acknowledgement codes
    MOVEMENT_CODE_ACK_ROOT,
    MOVEMENT_CODE_ACK_UNROOT,
    MOVEMENT_CODE_ACK_FEATHER_FALL,
    MOVEMENT_CODE_ACK_GRAVITY_ENABLE,
    MOVEMENT_CODE_ACK_GRAVITY_DISABLE,
    MOVEMENT_CODE_ACK_HOVER,
    MOVEMENT_CODE_ACK_WATER_WALK,
    MOVEMENT_CODE_ACK_KNOCK_BACK,
    MOVEMENT_CODE_ACK_SET_CAN_FLY,
    MOVEMENT_CODE_ACK_SET_COLLISION_HEIGHT,
    MOVEMENT_CODE_ACK_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY,
    MOVEMENT_CODE_ACK_FORCE_WALK_SPEED_CHANGE,
    MOVEMENT_CODE_ACK_FORCE_RUN_SPEED_CHANGE,
    MOVEMENT_CODE_ACK_FORCE_RUN_BACK_SPEED_CHANGE,
    MOVEMENT_CODE_ACK_FORCE_SWIM_SPEED_CHANGE,
    MOVEMENT_CODE_ACK_FORCE_FLIGHT_SPEED_CHANGE,

    // Spline codes
    MOVEMENT_CODE_SPLINE_DONE,
    MOVEMENT_CODE_SPLINE_SET_WALK_SPEED,
    MOVEMENT_CODE_SPLINE_SET_RUN_SPEED,
    MOVEMENT_CODE_SPLINE_SET_RUN_BACK_SPEED,
    MOVEMENT_CODE_SPLINE_SET_SWIM_SPEED,
    MOVEMENT_CODE_SPLINE_SET_SWIM_BACK_SPEED,
    MOVEMENT_CODE_SPLINE_SET_FLIGHT_SPEED,
    MOVEMENT_CODE_SPLINE_SET_FLIGHT_BACK_SPEED,
    MOVEMENT_CODE_SPLINE_SET_PITCH_RATE,
    MOVEMENT_CODE_SPLINE_SET_TURN_RATE,
    MOVEMENT_CODE_SPLINE_SET_WALK_MODE,
    MOVEMENT_CODE_SPLINE_SET_RUN_MODE,
    MOVEMENT_CODE_SPLINE_GRAVITY_ENABLE,
    MOVEMENT_CODE_SPLINE_GRAVITY_DISABLE,
    MOVEMENT_CODE_SPLINE_SET_HOVER,
    MOVEMENT_CODE_SPLINE_UNSET_HOVER,
    MOVEMENT_CODE_SPLINE_START_SWIM,
    MOVEMENT_CODE_SPLINE_STOP_SWIM,
    MOVEMENT_CODE_SPLINE_SET_FLYING,
    MOVEMENT_CODE_SPLINE_UNSET_FLYING,
    MOVEMENT_CODE_SPLINE_SET_WATER_WALK,
    MOVEMENT_CODE_SPLINE_SET_LAND_WALK,
    MOVEMENT_CODE_SPLINE_SET_FEATHER_FALL,
    MOVEMENT_CODE_SPLINE_SET_NORMAL_FALL,
    MOVEMENT_CODE_SPLINE_ROOT,
    MOVEMENT_CODE_SPLINE_UNROOT,

    // Vehicle codes
    MOVEMENT_CODE_DISMISS_CONTROLLED_VEHICLE,
    MOVEMENT_CODE_CHANGE_SEATS_ON_CONTROLLED_VEHICLE,

    // Embedded codes
    MOVEMENT_EMBEDDED_CAST_SPELL,
    MOVEMENT_EMBEDDED_PET_CAST_SPELL,
    MOVEMENT_EMBEDDED_USE_ITEM,

    MAX_MOVEMENT_CODE
};

enum MovementSpeedTypes : uint8
{
    MOVE_SPEED_WALK,
    MOVE_SPEED_RUN,
    MOVE_SPEED_RUN_BACK,
    MOVE_SPEED_SWIM,
    MOVE_SPEED_SWIM_BACK,
    MOVE_SPEED_FLY,
    MOVE_SPEED_FLY_BACK,
    MOVE_SPEED_TURNRATE,
    MOVE_SPEED_PITCHRATE,
    MOVE_SPEED_MAX
};

enum MovementStatusTypes : uint8
{
    MOVEMENT_STATUS_ROOT            = 0,
    MOVEMENT_STATUS_UNROOT          = 1,
    MOVEMENT_STATUS_FEATHERFALLING  = 2,
    MOVEMENT_STATUS_ENABLEGRAVITY   = 3,
    MOVEMENT_STATUS_DISABLEGRAVITY  = 4,
    MOVEMENT_STATUS_HOVERING        = 5,
    MOVEMENT_STATUS_WATERWALKING    = 6,
    MOVEMENT_STATUS_KNOCKBACKED     = 7,
    MOVEMENT_STATUS_CANFLY          = 8,
    MOVEMENT_STATUS_COLLISION_HEIGHT= 9,
    MOVEMENT_STATUS_CANSFTRANSITION = 10,
    MOVEMENT_STATUS_MAX
};

enum UnderwaterStates : uint8
{
    UNDERWATERSTATE_NONE            = 0x00,
    UNDERWATERSTATE_UNDERWATER      = 0x02,
    UNDERWATERSTATE_FATIGUE         = 0x04,
    UNDERWATERSTATE_LAVA            = 0x08,
    UNDERWATERSTATE_SLIME           = 0x10,
    UNDERWATERSTATE_TIMERS_PRESENT  = 0x20
};

class MovementInterface;
struct PacketHandler { void (MovementInterface::*function)(bool read, ByteBuffer &buffer); };

class MovementInterface
{
public:
    MovementInterface(Unit *_unit);
    ~MovementInterface();

    static void InitializeMovementHandlers();
    static uint16 GetInternalMovementCode(uint16 opcode);

    void Update(uint32 diff);
    void UpdatePreWrite(uint16 opcode, uint16 moveCode);
    bool UpdatePostRead(uint16 opcode, uint16 moveCode, ByteBuffer *source);

    void HandleBreathing();
    void HandleMovementFlags(bool read, ByteBuffer *buffer);
    void HandleMovementFlags2(bool read, ByteBuffer *buffer);

    void OnDeath();
    void OnRepop();
    void OnRessurect();
    void OnRevive();
    void OnDismount();
    void OnTaxiEnd();
    void OnRelocate(LocationVector destination);

    // Aura related functions
    bool isRooted() { return false; }
    bool isAsleep() { return false; }
    bool isStunned() { return false; }
    bool canFly() { return false; }

    void setRooted(bool root) {}
    void setAsleep(bool sleep) {}
    void setStunned(bool stun) {}
    void setCanFly(bool canfly) {}

    // Movement flags
    uint8 GetMovementMask() { return m_movementFlagMask; }
    uint8 GetFlags(uint8 index) { return m_movementFlags[index]|m_serverFlags[index]; }
    bool hasFlag(MovementFlagsA flag) { return GetFlags(0)&flag; };
    bool hasFlag(MovementFlagsB flag) { return GetFlags(1)&flag; };
    bool hasFlag(MovementFlagsC flag) { return GetFlags(2)&flag; };
    bool hasFlag(MovementFlagsD flag) { return GetFlags(3)&flag; };
    bool hasFlag(MovementFlagsE flag) { return GetFlags(4)&flag; };
    bool hasFlag(MovementFlagsF flag) { return GetFlags(5)&flag; };

    void setServerFlag(MovementFlagsA flag) { m_serverFlags[0] |= flag; }
    void setServerFlag(MovementFlagsB flag) { m_serverFlags[1] |= flag; }
    void setServerFlag(MovementFlagsC flag) { m_serverFlags[2] |= flag; }
    void setServerFlag(MovementFlagsD flag) { m_serverFlags[3] |= flag; }
    void setServerFlag(MovementFlagsE flag) { m_serverFlags[4] |= flag; }
    void setServerFlag(MovementFlagsF flag) { m_serverFlags[5] |= flag; }

    void removeServerFlag(MovementFlagsA flag) { m_serverFlags[0] &= ~flag; }
    void removeServerFlag(MovementFlagsB flag) { m_serverFlags[1] &= ~flag; }
    void removeServerFlag(MovementFlagsC flag) { m_serverFlags[2] &= ~flag; }
    void removeServerFlag(MovementFlagsD flag) { m_serverFlags[3] &= ~flag; }
    void removeServerFlag(MovementFlagsE flag) { m_serverFlags[4] &= ~flag; }
    void removeServerFlag(MovementFlagsF flag) { m_serverFlags[5] &= ~flag; }

    bool isMoving() { return hasFlag(MOVEMENTFLAG_MASK_MOVING); }
    bool isTurning() { return hasFlag(MOVEMENTFLAG_MASK_TURNING); }
    bool isPitching() { return hasFlag(MOVEMENTFLAG_MASK_PITCHING); }

    // Spline moving
    bool isSplineMovingActive() { return false; }

    // Underwater state
    uint8 GetUnderwaterState() { return m_underwaterState; }
    void AddUnderwaterStateTimerPresent() { m_underwaterState |= UNDERWATERSTATE_TIMERS_PRESENT; }

    // Packet handlers
    bool ReadFromClient(uint16 opcode, ByteBuffer *buffer);
    void WriteFromServer(uint16 opcode, ByteBuffer *buffer, WoWGuid extra_guid = 0, float extra_float = 0.f, uint8 extra_byte = 0);

    void DoExtraData(uint16 moveCode, bool read, ByteBuffer *buffer, uint8 sequence = 0);

    // Alternate packet handlers
    void SetActiveMover(WorldPacket *packet);
    void MoveTimeSkipped(WorldPacket *packet);
    void MoveSplineComplete(WorldPacket *packet);
    void MoveFallReset(WorldPacket *packet);

    // Transport data
    bool OnTransport() { return !m_transportGuid.empty(); }
    bool isTransportLocked() { return m_isTransportLocked; }
    void LockTransportData() { m_isTransportLocked = true; }
    void UnlockTransportData() { m_isTransportLocked = false; }

    WoWGuid GetTransportGuid() { return m_transportGuid; }
    void GetTransportPosition(float &x, float &y, float &z, float &o)
    {
        x = m_transportLocation.x;
        y = m_transportLocation.y;
        z = m_transportLocation.z;
        o = m_transportLocation.o;
    }

    void GetTransportPosition(LocationVector &vec)
    {
        vec.x = m_transportLocation.x;
        vec.y = m_transportLocation.y;
        vec.z = m_transportLocation.z;
        vec.o = m_transportLocation.o;
    }

    void SetTransportData(WoWGuid transGuid, uint32 vehicleId, float xOff, float yOff, float zOff, float oOff, int8 seatId, uint32 time1 = 0, uint32 time2 = 0)
    {
        m_vehicleId = vehicleId;
        m_transportGuid = transGuid;
        m_transportSeatId = seatId;
        m_transportLocation.ChangeCoords(xOff, yOff, zOff, oOff);
        m_transportTime = time1;
        m_transportTime2 = time2;
    }

    void ClearTransportData()
    {
        m_vehicleId = 0;
        m_transportGuid = 0;
        m_transportSeatId = 0;
        m_transportTime = m_transportTime2 = 0;
        m_transportLocation.ChangeCoords(0.f, 0.f, 0.f, 0.f);
    }

    // Speed
    float GetMoveSpeed(MovementSpeedTypes speedType) { return m_currSpeeds[speedType]; }

    void GetSpeedPointers(float *&walk, float *&run, float *&fly)
    {
        walk = &m_currSpeeds[MOVE_SPEED_WALK];
        run = &m_currSpeeds[MOVE_SPEED_RUN];
        fly = &m_currSpeeds[MOVE_SPEED_FLY];
    }

private:
    bool UpdateAcknowledgementData(uint16 moveCode);

public:
    void AppendSplineData(bool bits, ByteBuffer *buffer);
    void WriteObjectUpdate(ByteBuffer *bits, ByteBuffer *bytes);

    // Movement codes
    void HandlePlayerMove(bool read, ByteBuffer &buffer);
    void HandleHeartbeat(bool read, ByteBuffer &buffer);
    void HandleJump(bool read, ByteBuffer &buffer);
    void HandleFallLand(bool read, ByteBuffer &buffer);
    void HandleStartForward(bool read, ByteBuffer &buffer);
    void HandleStartBackward(bool read, ByteBuffer &buffer);
    void HandleStartStrafeLeft(bool read, ByteBuffer &buffer);
    void HandleStartStrafeRight(bool read, ByteBuffer &buffer);
    void HandleStartTurnLeft(bool read, ByteBuffer &buffer);
    void HandleStartTurnRight(bool read, ByteBuffer &buffer);
    void HandleStartPitchDown(bool read, ByteBuffer &buffer);
    void HandleStartPitchUp(bool read, ByteBuffer &buffer);
    void HandleStartAscend(bool read, ByteBuffer &buffer);
    void HandleStartDescend(bool read, ByteBuffer &buffer);
    void HandleStartSwim(bool read, ByteBuffer &buffer);
    void HandleStop(bool read, ByteBuffer &buffer);
    void HandleStopStrafe(bool read, ByteBuffer &buffer);
    void HandleStopTurn(bool read, ByteBuffer &buffer);
    void HandleStopPitch(bool read, ByteBuffer &buffer);
    void HandleStopAscend(bool read, ByteBuffer &buffer);
    void HandleStopSwim(bool read, ByteBuffer &buffer);
    void HandleSetFacing(bool read, ByteBuffer &buffer);
    void HandleSetPitch(bool read, ByteBuffer &buffer);
    void HandleFallReset(bool read, ByteBuffer &buffer);
    void HandleSetRunMode(bool read, ByteBuffer &buffer);
    void HandleSetWalkMode(bool read, ByteBuffer &buffer);
    void HandleSetPitchRate(bool read, ByteBuffer &buffer);
    void HandleSetCanFly(bool read, ByteBuffer &buffer);
    void HandleUnsetCanFly(bool read, ByteBuffer &buffer);
    void HandleSetHover(bool read, ByteBuffer &buffer);
    void HandleUnsetHover(bool read, ByteBuffer &buffer);
    void HandleWaterWalk(bool read, ByteBuffer &buffer);
    void HandleLandWalk(bool read, ByteBuffer &buffer);
    void HandleFeatherFall(bool read, ByteBuffer &buffer);
    void HandleNormalFall(bool read, ByteBuffer &buffer);
    void HandleRoot(bool read, ByteBuffer &buffer);
    void HandleUnroot(bool read, ByteBuffer &buffer);
    void HandleUpdateKnockBack(bool read, ByteBuffer &buffer);
    void HandleUpdateTeleport(bool read, ByteBuffer &buffer);
    void HandleChangeTransport(bool read, ByteBuffer &buffer);
    void HandleNotActiveMover(bool read, ByteBuffer &buffer);
    void HandleSetCollisionHeight(bool read, ByteBuffer &buffer);
    void HandleUpdateCollisionHeight(bool read, ByteBuffer &buffer);
    // Speed codes
    void HandleUpdateWalkSpeed(bool read, ByteBuffer &buffer);
    void HandleUpdateRunSpeed(bool read, ByteBuffer &buffer);
    void HandleUpdateRunBackSpeed(bool read, ByteBuffer &buffer);
    void HandleUpdateSwimSpeed(bool read, ByteBuffer &buffer);
    void HandleUpdateFlightSpeed(bool read, ByteBuffer &buffer);
    void HandleSetWalkSpeed(bool read, ByteBuffer &buffer);
    void HandleSetRunSpeed(bool read, ByteBuffer &buffer);
    void HandleSetRunBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSetSwimSpeed(bool read, ByteBuffer &buffer);
    void HandleSetSwimBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSetFlightSpeed(bool read, ByteBuffer &buffer);
    void HandleSetFlightBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSetTurnRate(bool read, ByteBuffer &buffer);
    // Acknowledgement codes
    void HandleAckRoot(bool read, ByteBuffer &buffer);
    void HandleAckUnroot(bool read, ByteBuffer &buffer);
    void HandleAckFeatherFall(bool read, ByteBuffer &buffer);
    void HandleAckForceWalkSpeedChange(bool read, ByteBuffer &buffer);
    void HandleAckForceRunSpeedChange(bool read, ByteBuffer &buffer);
    void HandleAckForceRunBackSpeedChange(bool read, ByteBuffer &buffer);
    void HandleAckForceSwimSpeedChange(bool read, ByteBuffer &buffer);
    void HandleAckForceFlightSpeedChange(bool read, ByteBuffer &buffer);
    void HandleAckGravityEnable(bool read, ByteBuffer &buffer);
    void HandleAckGravityDisable(bool read, ByteBuffer &buffer);
    void HandleAckHover(bool read, ByteBuffer &buffer);
    void HandleAckWaterWalk(bool read, ByteBuffer &buffer);
    void HandleAckKnockBack(bool read, ByteBuffer &buffer);
    void HandleAckSetCanFly(bool read, ByteBuffer &buffer);
    void HandleAckSetCollisionHeight(bool read, ByteBuffer &buffer);
    void HandleAckSetCanTransitionBetweenSwimAndFly(bool read, ByteBuffer &buffer);
    // Spline codes
    void HandleSplineDone(bool read, ByteBuffer &buffer);
    void HandleSplineSetWalkSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetRunSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetRunBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetSwimSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetSwimBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetFlightSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetFlightBackSpeed(bool read, ByteBuffer &buffer);
    void HandleSplineSetPitchRate(bool read, ByteBuffer &buffer);
    void HandleSplineSetTurnRate(bool read, ByteBuffer &buffer);
    void HandleSplineSetWalkMode(bool read, ByteBuffer &buffer);
    void HandleSplineSetRunMode(bool read, ByteBuffer &buffer);
    void HandleSplineGravityEnable(bool read, ByteBuffer &buffer);
    void HandleSplineGravityDisable(bool read, ByteBuffer &buffer);
    void HandleSplineSetHover(bool read, ByteBuffer &buffer);
    void HandleSplineSetUnhover(bool read, ByteBuffer &buffer);
    void HandleSplineStartSwim(bool read, ByteBuffer &buffer);
    void HandleSplineStopSwim(bool read, ByteBuffer &buffer);
    void HandleSplineSetFlying(bool read, ByteBuffer &buffer);
    void HandleSplineUnsetFlying(bool read, ByteBuffer &buffer);
    void HandleSplineSetWaterWalk(bool read, ByteBuffer &buffer);
    void HandleSplineSetLandWalk(bool read, ByteBuffer &buffer);
    void HandleSplineSetFeatherFall(bool read, ByteBuffer &buffer);
    void HandleSplineSetNormalFall(bool read, ByteBuffer &buffer);
    void HandleSplineRoot(bool read, ByteBuffer &buffer);
    void HandleSplineUnroot(bool read, ByteBuffer &buffer);
    // Vehicle codes
    void HandleDismissControlledVehicle(bool read, ByteBuffer &buffer);
    void HandleChangeSeatsOnControlledVehicle(bool read, ByteBuffer &buffer);
    // Embedded codes
    void HandleEmbeddedMovement(bool read, ByteBuffer &buffer);

private:
    // Internal functions
    void UpdateMovementFlagMask()
    {
        m_movementFlagMask = 0;
        for(uint8 i = 0; i < 6; i++)
        {
            if(GetFlags(i) == 0)
                continue;
            m_movementFlagMask |= 1<<i;
        }
    }

    // Movement information
protected:
    Mutex m_movementLock;
    // Vector linked to object position
    LocationVector *m_serverLocation;

    bool m_isTransportLocked;
    WoWGuid m_moverGuid, m_transportGuid, m_clientGuid, m_clientTransGuid;
    uint8 m_movementFlagMask, m_movementFlags[6], m_serverFlags[6];
    LocationVector m_transportLocation, m_clientLocation, m_clientTransLocation;
    uint32 m_clientTime, m_serverTime, m_jumpTime, m_transportTime, m_transportTime2;
    int8 m_transportSeatId; uint32 m_vehicleId;

    float pitching, splineElevation;
    float m_jumpZSpeed, m_jump_XYSpeed, m_jump_sin, m_jump_cos;
    uint32 m_serverCounter, m_clientCounter;
    bool m_incrementMoveCounter;

    uint8 m_underwaterState;
    float m_collisionHeight;
    bool m_isKnockBacked;

    struct
    {
        WoWGuid ex_guid;
        float ex_float;
        int8  ex_byte;

        void set(WoWGuid xguid, float xfloat, uint8 xbyte)
        {
            ex_guid = xguid;
            ex_float = xfloat;
            ex_byte = xbyte;
        }
        void clear() { set(0, 0, 0); }
    } m_extra;

protected: // Speed and Status information
    float m_heightOffset; // Hover diff etc
    float m_currSpeeds[MOVE_SPEED_MAX], m_movementSpeeds[MOVE_SPEED_MAX], m_speedOffset[MOVE_SPEED_MAX];

    // Pending speeds
    uint32 m_speedTimers[MOVE_SPEED_MAX];
    float m_pendingSpeeds[MOVE_SPEED_MAX];

    bool m_pendingEnable[MOVEMENT_STATUS_MAX];

    bool UpdatePendingSpeed(MovementSpeedTypes speedType, float sentSpeed)
    {
        if(sentSpeed != m_pendingSpeeds[speedType])
            return false;
        m_speedTimers[speedType] = 0; // Timer not needed anymore
        m_pendingSpeeds[speedType] = 0.0f; // Pending speed can be cleared
        m_movementSpeeds[speedType] = sentSpeed;
        return true;
    }

private:
    Unit *m_Unit;

public:
    void UpdateModifier(uint32 auraSlot, uint8 index, Modifier *mod, bool apply);

    class ModifierHolder
    {
    public:
        ModifierHolder(uint32 slot,SpellEntry* info) : auraSlot(slot),spellInfo(info) {for(uint8 i=0;i<3;i++)mod[i]=NULL;};

        uint32 auraSlot;
        SpellEntry *spellInfo;
        Modifier *mod[3];
    };

    typedef std::map<std::pair<uint32, uint32>, Modifier*> modifierMap;
    typedef std::map<uint32, modifierMap > modifierTypeMap;

    modifierMap GetModMapByModType(uint32 modType) { return m_modifiersByModType[modType]; }
    bool HasAurasWithModType(uint32 modType) { return m_modifiersByModType[modType].size(); }

    // Update Mask
    UpdateMask &getModMask() { return m_modifierMask; }
    bool GetModMaskBit(uint32 type) { return m_modifierMask.GetBit(type); }
    void SetModMaskBit(uint32 type) { m_modifierMask.SetBit(type); };
    void UnsetModMaskBit(uint32 type) { m_modifierMask.UnsetBit(type); };
    void ClearModMaskBits() { m_modifierMask.Clear(); };
    bool GetAndUnsetModMaskBit(uint32 type)
    {
        bool res = false;
        if(res = m_modifierMask.GetBit(type))
            m_modifierMask.UnsetBit(type);
        return res;
    }

private:
    // Ordered by aura slot
    UpdateMask m_modifierMask;
    std::map<uint32, ModifierHolder*> m_modifierHolders;
    // Storage is <ModType, <Index, Modifier> >
    modifierTypeMap m_modifiersByModType;
    // Storage is <SpellGroup, <ModType, <Index, Modifier> > >
    std::map<std::pair<uint8, uint8>, std::map<uint8, int32>> m_spellGroupModifiers;

    void UpdateSpellGroupModifiers(bool apply, Modifier *mod);
};

enum MovementStatusElements
{
    MSEHasGuidByte0,
    MSEHasGuidByte1,
    MSEHasGuidByte2,
    MSEHasGuidByte3,
    MSEHasGuidByte4,
    MSEHasGuidByte5,
    MSEHasGuidByte6,
    MSEHasGuidByte7,
    MSEHasMovementFlags,
    MSEHasMovementFlags2,
    MSEHasTimestamp,
    MSEHasOrientation,
    MSEHasTransportData,
    MSEHasTransportGuidByte0,
    MSEHasTransportGuidByte1,
    MSEHasTransportGuidByte2,
    MSEHasTransportGuidByte3,
    MSEHasTransportGuidByte4,
    MSEHasTransportGuidByte5,
    MSEHasTransportGuidByte6,
    MSEHasTransportGuidByte7,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEHasPitch,
    MSEHasFallData,
    MSEHasFallDirection,
    MSEHasSplineElevation,
    MSEHasSpline,

    MSEGuidByte0,
    MSEGuidByte1,
    MSEGuidByte2,
    MSEGuidByte3,
    MSEGuidByte4,
    MSEGuidByte5,
    MSEGuidByte6,
    MSEGuidByte7,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSETransportGuidByte0,
    MSETransportGuidByte1,
    MSETransportGuidByte2,
    MSETransportGuidByte3,
    MSETransportGuidByte4,
    MSETransportGuidByte5,
    MSETransportGuidByte6,
    MSETransportGuidByte7,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSETransportTime2,
    MSETransportVehicleId,
    MSEPitch,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSESplineElevation,

    MSECounter,

    // Special
    MSEZeroBit,         // writes bit value 1 or skips read bit
    MSEOneBit,          // writes bit value 0 or skips read bit
    MSEEnd,             // marks end of parsing
    MSEExtraElement,    // Used to signalize reading into ExtraMovementStatusElement, element sequence inside it is declared as separate array
                        // Allowed internal elements are: GUID markers (not transport), MSEExtraFloat, MSEExtraInt8
    MSEExtraFloat,
    MSEExtraInt8,
};

MovementStatusElements const* GetMovementStatusElementsSequence(Opcodes opcode);
