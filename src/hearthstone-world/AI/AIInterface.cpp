/***
 * Demonstrike Core
 */

#include "StdAfx.h"

extern pAIEvent AIEventHandlers[NUM_AI_EVENTS];

AIInterface::AIInterface()
{
    m_CastNext = NULL;
    m_CastTimer = 0;
    m_creatureState = STOPPED;
    m_canCallForHelp = false;
    m_hasCalledForHelp = false;
    m_fleeTimer = 0;
    m_FleeDuration = 0;
    m_canFlee = false;
    m_hasFled = false;
    m_canRangedAttack = false;
    m_FleeHealth = m_CallForHelpHealth = 0.0f;
    m_AIState = STATE_IDLE;
    unitBehavior = Behavior_Default;

    m_updateListTimer = 1;
    m_updateTargetsTimer = TARGET_UPDATE_INTERVAL;

    m_nextTarget = NULL;
    m_Unit = NULL;
    m_PetOwner = NULL;
    firstLeaveCombat = true;
    m_outOfCombatRange = 10000;
    m_totemSpell = NULL;
    m_totemSpellTimer = m_totemSpellTime = 0;

    tauntedBy = NULL;
    isTaunted = false;
    m_AllowedToEnterCombat = true;
    m_currentHighestThreat = 0;

    disable_combat = false;
    disable_melee = false;
    disable_ranged = false;
    disable_spell = false;
    disable_targeting = false;

    waiting_for_cooldown = false;
    m_isGuard = false;
    m_is_in_instance = false;
    skip_reset_hp = false;
    m_guardCallTimer = 0;

    m_aiTargets.clear();
    m_spells.clear();
}

AIInterface::~AIInterface()
{
    MovementHandler.DeInitialize();
    for(std::map<uint32, AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); itr++)
        delete itr->second;

    m_spells.clear();

    m_Unit = NULL;
    m_PetOwner = NULL;
    soullinkedWith = NULL;
}

void AIInterface::Init(Unit* un, AIType at, MovementType mt)
{
    ASSERT(un != NULL);
    ASSERT(at != AITYPE_PET);

    m_Unit = un;
    m_AIType = at;
    m_AIState = STATE_IDLE;

    if(isTargetDummy(un->GetEntry()))
    {
        m_AIType = AITYPE_DUMMY;
        disable_targeting = true;
        un->m_runSpeed = 0.0f;
        un->m_flySpeed = 0.0f;
        un->m_walkSpeed = 0.0f;
        un->m_swimSpeed = 0.0f;
    }

    if( m_Unit->IsCreature() && castPtr<Creature>(m_Unit)->GetCreatureData() && castPtr<Creature>(m_Unit)->GetCreatureData()->Type == CRITTER )
        disable_targeting = true;

    m_guardTimer = getMSTime();

    MovementHandler.Initialize(this, un, mt);
}

void AIInterface::Init(Unit* un, AIType at, MovementType mt, Unit* owner)
{
    ASSERT(un != NULL);
    ASSERT(at == AITYPE_PET || at == AITYPE_TOTEM);

    m_Unit = un;
    m_AIType = at;
    m_PetOwner = owner;
    m_AIState = STATE_IDLE;

    if(isTargetDummy(un->GetEntry()))
    {
        m_AIType = AITYPE_DUMMY;
        disable_targeting = true;
        un->m_runSpeed = 0.0f;
        un->m_flySpeed = 0.0f;
        un->m_walkSpeed = 0.0f;
        un->m_swimSpeed = 0.0f;
    }

    if( castPtr<Creature>(m_Unit)->GetCreatureData() && castPtr<Creature>(m_Unit)->GetCreatureData()->Type == CRITTER )
        disable_targeting = true;

    MovementHandler.Initialize(this, un, mt);
}

void AIInterface::HandleEvent(uint32 eevent, Unit* pUnit, uint32 misc1)
{
    ASSERT(m_Unit != NULL);
    if(m_Unit == NULL)
        return;

    if(eevent >= NUM_AI_EVENTS)
        return;

    // Passive NPCs (like target dummies) shouldn't do anything.
    if(m_Unit->IsCreature())
    {
        if(m_AIType == AITYPE_DUMMY)
        {
            if(eevent == EVENT_ENTERCOMBAT || eevent == EVENT_DAMAGETAKEN)
                m_Unit->CombatStatus.OnDamageDealt(pUnit, 1); // Fill our attackers combat system with info(Lol)
            return;
        }

        if(castPtr<Creature>(m_Unit)->GetCreatureData() && castPtr<Creature>(m_Unit)->GetCreatureData()->Type == CRITTER)
            return;
    }

    if(AIEventHandlers[eevent] != NULL)
        (*this.*AIEventHandlers[eevent])(pUnit, misc1);
}

void AIInterface::Update(uint32 p_time)
{
    if(m_AIType == AITYPE_TOTEM && m_Unit->IsTotem())
    {
        _UpdateTotem(p_time);
        return;
    }

    _UpdateTargets(p_time);

    _UpdateCombat(p_time);

    MovementHandler.Update(p_time);

    if(m_fleeTimer)
    {
        if(m_fleeTimer > p_time)
        {
            m_fleeTimer -= p_time;
            if(!m_nextTarget) //something happened to our target, lets find another one
                SetNextTarget(FindTarget());
            if(m_nextTarget)
                _CalcDestinationAndMove(m_nextTarget, 5.0f);
        }
        else
        {
            m_fleeTimer = 0;
            if(!m_nextTarget)
                SetNextTarget(FindTarget());
        }
    }

    if(!m_fleeTimer && m_AIState == STATE_EVADE)
        MovementHandler.HandleEvade();
}

void AIInterface::_UpdateTotem(uint32 p_time)
{
    if(m_totemSpell == NULL)
        return;
    if(m_totemSpellTime == 0)
        return;

    if(m_totemSpellTimer <= p_time)
    {
        SpellCastTargets targets;
        if(false)//m_totemSpell->IsAreaOfEffectSpell())
        {
            m_nextTarget = NULL;
            targets.m_targetMask = TARGET_FLAG_SELF|TARGET_FLAG_SOURCE_LOCATION|TARGET_FLAG_DEST_LOCATION;
            targets.m_unitTarget = m_Unit->GetGUID();
            targets.m_srcX = targets.m_destX = m_Unit->GetPositionX();
            targets.m_srcY = targets.m_destY = m_Unit->GetPositionY();
            targets.m_srcZ = targets.m_destZ = m_Unit->GetPositionZ();
        }
        else
        {
            if(m_nextTarget == NULL) // Find a totem target
                SetNextTarget(FindTarget());
            else if(!m_nextTarget->IsInWorld() || !IsInrange(m_Unit, m_nextTarget, m_Unit->GetCombatReach())
                || !sFactionSystem.CanEitherUnitAttack(m_Unit, m_nextTarget, false))
                SetNextTarget(NULL);
            if(m_nextTarget==NULL)
                return;
        }

        Spell *pSpell = new Spell(m_Unit, m_totemSpell, true, NULL);
        if(m_nextTarget)
        {
            targets.m_targetMask = TARGET_FLAG_OBJECT|TARGET_FLAG_UNIT;
            targets.m_unitTarget = m_nextTarget->GetGUID();
        }
        pSpell->prepare(&targets);
        m_totemSpellTimer = m_totemSpellTime;
    } else m_totemSpellTimer -= p_time;
}

bool AIInterface::SetNextTarget(Unit* nextTarget)
{
    if(nextTarget) m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, nextTarget->GetGUID());
    else m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
    return (m_nextTarget = nextTarget) != NULL;
}

void AIInterface::AttackReaction(Unit* pUnit, uint32 damage_dealt, uint32 spellId)
{
    ASSERT(m_Unit != NULL);
    if( m_AIState == STATE_EVADE || m_fleeTimer != 0 || !pUnit || !pUnit->isAlive() || !m_Unit->isAlive() )
        return;
    if( m_Unit == pUnit || m_Unit->IsVehicle() )
        return;

    if(pUnit->IsVehicle())
    {
        uint32 count = castPtr<Vehicle>(pUnit)->GetPassengerCount();
        if(!count) // No players.
            return;
    }

    uint32 threat = _CalcThreat(damage_dealt, spellId ? dbcSpell.LookupEntry(spellId) : NULL, pUnit);
    if( m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING )
    {
        WipeTargetList();

        HandleEvent(EVENT_ENTERCOMBAT, pUnit, 0);
        if(!threat)
            threat = 1;
    }

    HandleEvent(EVENT_DAMAGETAKEN, pUnit, threat);
}

bool AIInterface::HealReaction(Unit* caster, Unit* victim, uint32 amount, SpellEntry * sp)
{
    ASSERT(m_Unit != NULL);

    if(!caster || !victim)
    {
        //printf("!!!BAD POINTER IN AIInterface::HealReaction!!!\n");
        return false;
    }

    // apply spell modifiers
    if (sp != NULL && sp->SpellGroupType)
    {
        caster->SM_FIValue(SMT_THREAT_REDUCED,(int32*)&amount,sp->SpellGroupType);
        caster->SM_PIValue(SMT_THREAT_REDUCED,(int32*)&amount,sp->SpellGroupType);
    }
    amount += (amount * caster->GetGeneratedThreatModifier() / 100);

    bool casterInList = false, victimInList = false;

    ai_TargetLock.Acquire();
    if(m_aiTargets.find(caster->GetGUID()) != m_aiTargets.end())
        casterInList = true;
    if(m_aiTargets.find(victim->GetGUID()) != m_aiTargets.end())
        victimInList = true;
    ai_TargetLock.Release();

    if(!victimInList && !casterInList) // none of the Casters is in the Creatures Threat list
        return false;

    if(!casterInList && victimInList) // caster is not yet in Combat but victim is
    {
        // get caster into combat if he's hostile
        if(sFactionSystem.CanEitherUnitAttack(m_Unit, caster))
        {
            ai_TargetLock.Acquire();
            m_aiTargets.insert(std::make_pair(caster->GetGUID(), amount));
            ai_TargetLock.Release();
            return true;
        }
        return false;
    }
    else if(casterInList && victimInList) // both are in combat already
    {
        // mod threat for caster
        modThreat(caster->GetGUID(), amount);
        return true;
    }
    else // caster is in Combat already but victim is not
    {
        modThreat(caster->GetGUID(), amount);
        // both are players so they might be in the same group
        if( caster->IsPlayer() && victim->IsPlayer() )
        {
            if( castPtr<Player>( caster )->GetGroup() == castPtr<Player>( victim )->GetGroup() )
            {
                // get victim into combat since they are both
                // in the same party
                if( sFactionSystem.CanEitherUnitAttack( m_Unit, victim ) )
                {
                    ai_TargetLock.Acquire();
                    m_aiTargets.insert(std::make_pair( victim->GetGUID(), 1 ) );
                    ai_TargetLock.Release();
                    return true;
                }
                return false;
            }
        }
    }

    return false;
}

void AIInterface::OnDeath(WorldObject* pKiller)
{
    ASSERT(m_Unit != NULL);

    if(pKiller != NULL && pKiller->IsUnit())
        HandleEvent(EVENT_UNITDIED, castPtr<Unit>(pKiller), 0);
    else
        HandleEvent(EVENT_UNITDIED, m_Unit, 0);
}

void AIInterface::OnRespawn(Unit* unit)
{
    ASSERT(unit != NULL);

    HandleEvent(EVENT_UNITRESPAWN, unit, 0);
}

bool AIInterface::FindFriends(float dist)
{
    ASSERT(m_Unit != NULL);
    if( m_Unit->IsPet() ) //pet's do not have friends; Players are exploiting this :-/
        return false;
    if(!ai_TargetLock.AttemptAcquire())
        return false;
    ai_TargetLock.Release();

    Unit* pUnit = NULL;
    bool result = false;
    for(std::unordered_set<WorldObject* >::iterator itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); itr++)
    {
        if((*itr) == NULL || !(*itr)->IsInWorld() || (*itr)->GetTypeId() != TYPEID_UNIT)
            continue;

        pUnit = castPtr<Unit>((*itr));
        if(!pUnit->isAlive())
            continue;
        if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            continue;
        if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
            continue;
        if( !sFactionSystem.CanEitherUnitAttack(GetMostHated(), pUnit) )
            continue;
        if( !m_Unit->PhasedCanInteract(pUnit) )
            continue;
        if( sFactionSystem.isCombatSupport( m_Unit, pUnit ) && ( pUnit->GetAIInterface()->getAIState() == STATE_IDLE || pUnit->GetAIInterface()->getAIState() == STATE_SCRIPTIDLE ) )//Not sure
        {
            if( m_Unit->GetDistanceSq(pUnit) < dist)
            {
                result = true;
                Unit* pUnit = NULL;
                ai_TargetLock.Acquire();
                TargetMap::iterator it, it2;
                for(TargetMap::iterator it = m_aiTargets.begin(), it2; it != m_aiTargets.end();)
                    if(Unit *unit = m_Unit->GetMapMgr()->GetUnit((it2 = it++)->first))
                        castPtr<Unit>(*itr)->GetAIInterface()->AttackReaction( unit, 1, 0 );
                ai_TargetLock.Release();
                break;
            }
        }
    }

    // check if we're a civillan, in which case summon guards on a despawn timer
    CreatureData * ctrData = castPtr<Creature>(m_Unit)->GetCreatureData();
    if( ctrData && ctrData->Type == HUMANOID && ctrData->Civilian )
        CallGuards();
    return result;
}

float AIInterface::_CalcDistanceFromHome()
{
    ASSERT(m_Unit != NULL);
    if (m_AIType == AITYPE_PET)
        return m_Unit->GetDistanceSq(m_PetOwner);
    else if(m_Unit->GetTypeId() == TYPEID_UNIT)
    {
        LocationVector m_Ret = GetReturnPos();
        if(m_Ret.x != 0.0f && m_Ret.y != 0.0f)
            return m_Unit->GetDistanceSq(m_Ret.x, m_Ret.y, m_Ret.z);
    }

    return 0.0f;
}

bool AIInterface::setInFront(Unit* target) // not the best way to do it, though
{
    ASSERT(m_Unit != NULL);

    float dx = target->GetPositionX() - m_Unit->GetPositionX();
    float dy = target->GetPositionY() - m_Unit->GetPositionY();
    float ang = atan2(dy, dx); ang = (ang >= 0) ? ang : 2 * M_PI + ang;
    m_Unit->SetPosition(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), ang);
    return m_Unit->isTargetInFront(target);
}

void AIInterface::ResetProcCounts(bool all)
{
    uint32 time = getMSTime();
    if(m_spells.size())
    {
        for(std::map<uint32, AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); itr++)
        {
            if(!itr->second->ProcLimit)
                continue;

            if(!all && itr->second->ProcResetDelay)
                if(itr->second->ProcResetTimer > time)
                    continue;
            itr->second->procCounter = 0;
        }
    }
}

void AIInterface::CallGuards()
{
    ASSERT(m_Unit != NULL);
    if(!m_Unit->IsCreature())
        return;

    Creature* m_Creature = castPtr<Creature>(m_Unit);
    if( m_Creature->isDead() || !m_Creature->isAlive() || m_Creature->GetInRangePlayersCount() == 0 || m_Creature->GetMapMgr() == NULL || m_Creature->m_isGuard )
        return;

    if( getMSTime() > m_guardTimer && !IS_INSTANCE(m_Unit->GetMapId()) )
    {
        m_guardTimer = getMSTime() + 15000;
        uint16 AreaId = m_Creature->GetAreaId();
        AreaTableEntry * at = dbcAreaTable.LookupEntry(AreaId);
        if(at == NULL)
            return;

        ZoneGuardEntry * zoneSpawn = ZoneGuardStorage.LookupEntry(at->ZoneId);
        if(zoneSpawn == NULL)
            return;

        uint32 team = sFactionSystem.isAlliance(m_Unit) ? 0 : 1; // Set team
        uint32 guardId = team ? (zoneSpawn->HordeEntry ? zoneSpawn->HordeEntry : 3296) : (zoneSpawn->AllianceEntry ? zoneSpawn->AllianceEntry : 68);
        CreatureData * ctrData = sCreatureDataMgr.GetCreatureData( guardId );
        if(ctrData == NULL || m_Unit->GetEntry() == guardId)
            return; // Do not let guards spawn themselves.

        float x = m_Unit->GetPositionX() + (float((rand() % 150) + 100) / 1000.0f );
        float y = m_Unit->GetPositionY() + (float((rand() % 150) + 100) / 1000.0f );
        float z = m_Unit->GetCHeightForPosition();

        // "Guards!"
        m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, team ? LANG_ORCISH : LANG_COMMON, "Guards!");

        uint8 spawned = 0;
        std::unordered_set<Player*>::iterator hostileItr = m_Unit->GetInRangePlayerSetBegin();
        for(; hostileItr != m_Unit->GetInRangePlayerSetEnd(); ++hostileItr)
        {
            if(spawned >= 3)
                break;

            if(!sFactionSystem.CanEitherUnitAttack(*hostileItr, m_Unit))
                continue;

            Creature* guard = m_Unit->GetMapMgr()->CreateCreature(guardId);
            if(guard == NULL)
                continue;

            guard->Load(m_Unit->GetMapMgr()->iInstanceMode, x, y, z);
            guard->SetInstanceID(m_Unit->GetInstanceID());
            guard->SetZoneId(m_Unit->GetZoneId());
            guard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); /* shitty DBs */
            guard->m_noRespawn = guard->m_isGuard = true;
            if(!guard->CanAddToWorld())
            {
                guard->SafeDelete();
                return;
            }

            uint32 t = spawned ? 0 : RandomUInt(8)*1000;
            if( t == 0 )
                guard->PushToWorld(m_Unit->GetMapMgr());
            else sEventMgr.AddEvent(guard,&Creature::AddToWorld, m_Unit->GetMapMgr(), EVENT_UNK, t, 1, 0);

            //despawn after 5 minutes.
            sEventMgr.AddEvent(guard, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, 60*5*1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
            //Start patrolling if nothing else to do.
            sEventMgr.AddEvent(guard, &Creature::SetGuardWaypoints, EVENT_UNK, 10000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
            spawned++;
        }
    }
}

bool isGuard(uint32 id)
{
    switch(id)
    {
        /* stormwind guards */
    case 68:
    case 1423:
    case 1756:
    case 15858:
    case 15859:
    case 16864:
    case 20556:
    case 18948:
    case 18949:
    case 1642:
        /* ogrimmar guards */
    case 3296:
    case 15852:
    case 15853:
    case 15854:
    case 18950:
        /* undercity guards */
    case 5624:
    case 18971:
    case 16432:
        /* exodar */
    case 16733:
    case 18815:
        /* thunder bluff */
    case 3084:
        /* silvermoon */
    case 16221:
    case 17029:
    case 16222:
        /* ironforge */
    case 727:
    case 5595:
    case 12996:
        /* darnassus? */
        {
            return true;
        }break;
    }
    return false;
}

bool isTargetDummy(uint32 id)
{
    switch(id)
    {
    case 1921:
    case 2673:
    case 2674:
    case 4952:
    case 5202:
    case 5652:
    case 5723:
    case 11875:
    case 12385:
    case 12426:
    case 16211:
    case 16897:
    case 17059:
    case 17060:
    case 17578:
    case 18215:
    case 18504:
    case 19139:
    case 21157:
    case 24792:
    case 25225:
    case 25297:
    case 30527:
    case 31143:
    case 31144:
    case 31146:
    case 32541:
    case 32542:
    case 32543:
    case 32545:
    case 32546:
    case 32547:
    case 32666:
    case 32667:
    case 33272:
    case 33243:
    case 33229:
        {
            return true;
        }break;
    }
    return false;
}

void AIInterface::WipeCurrentTarget()
{
    if(m_nextTarget == NULL)
        return;

    TargetMap::iterator itr;
    if( (itr = m_aiTargets.find(m_nextTarget->GetGUID())) != m_aiTargets.end() )
        m_aiTargets.erase( itr );

    ClearFollowInformation(m_nextTarget);
    SetNextTarget(NULL);
}

/* Crow: THIS FUNCTION IS HEAVILY DEPENDENT ON THE CREATURE PROTO COLUMN!
void AIInterface::CheckHeight()
{
    ASSERT(m_Unit != NULL);

    if(m_Unit->GetMapMgr())
    {
        if(m_Unit->IsCreature())
        {
            if(!(castPtr<Creature>(m_Unit)->CanMove & LIMIT_AIR))
            {
                m_moveFly = false;
                return;
            }
        }

        uint32 m = m_Unit->GetMapId();
        float x = m_Unit->GetPositionX();
        float y = m_Unit->GetPositionY();
        float z = m_Unit->GetPositionZ();
        if(m_destinationX && m_destinationY)
        {
            x = m_destinationX;
            y = m_destinationY;
            z = (z > m_destinationZ ? z : m_destinationZ); // Crow: Call it hacky, but it works.
        }

        float landheight_z = m_Unit->GetCHeightForPosition(true, x, y, z);
        if(landheight_z)
        {
            if(landheight_z < (z-3.0f))
                m_moveFly = true;
            else
                m_moveFly = false;
        }
        m_Unit->UpdateVisibility();
    }
}*/

uint32 AIInterface::GetWeaponEmoteType(bool ranged)
{
    uint32 emotetype = EMOTE_STATE_READYUNARMED;
    if(ranged) emotetype = EMOTE_STATE_READYBOW;
    if(!m_Unit->IsCreature())
        return emotetype;

    if(CreatureData *ctrData = castPtr<Creature>(m_Unit)->GetCreatureData())
    {
        if(ranged == false)
        {
            uint32 weaponids[2] = { ctrData->Item1, ctrData->Item2 };
            for(uint8 i = 0; i < 2; i++)
            {
                if(weaponids[i])
                {
                    if(ItemDataEntry* ItemE = db2Item.LookupEntry(weaponids[i]))
                    {
                        switch(ItemE->InventoryType)
                        {
                        case INVTYPE_WEAPON:
                        case INVTYPE_WEAPONMAINHAND:
                        case INVTYPE_WEAPONOFFHAND:
                            {
                                emotetype = EMOTE_STATE_READY1H;
                            }break;
                        case INVTYPE_2HWEAPON:
                            {
                                emotetype = EMOTE_STATE_READY2H;
                            }break;
                        }
                    }
                }
            }
        }
        else if(ctrData->Item3)
        {
            if(ItemDataEntry* ItemE = db2Item.LookupEntry(ctrData->Item3))
                if(ItemE->SubClass == ITEM_SUBCLASS_WEAPON_GUN || ItemE->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW)
                    emotetype = EMOTE_STATE_READYRIFLE;
        }
    }
    return emotetype;
}
