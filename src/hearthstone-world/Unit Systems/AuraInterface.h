/***
 * Demonstrike Core
 */

#pragma once

#define MAX_POSITIVE_AURAS 48 // ?
#define MAX_AURAS 80 // 48 buff slots, 32 debuff slots.
#define MAX_PASSIVE_AURAS 165
#define TOTAL_AURAS MAX_AURAS+MAX_PASSIVE_AURAS

class Unit;
class Modifier;

struct AuraCheckResponse
{
    uint32 Error;
    uint32 Misc;
};

enum AURA_CHECK_RESULT
{
    AURA_CHECK_RESULT_NONE                = 1,
    AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT = 2,
    AURA_CHECK_RESULT_LOWER_BUFF_PRESENT  = 3,
};

class SERVER_DECL AuraInterface
{
public:
    AuraInterface();
    ~AuraInterface();

    void Init(Unit* unit);
    void DeInit();
    void RelocateEvents();
    void OnChangeLevel(uint32 newlevel);
    void SaveAuras(std::stringstream&);
    uint8 GetFreeSlot(bool ispositive);
    void OnAuraRemove(Aura* aura, uint8 aura_slot);

    bool IsDazed();
    bool IsPoisoned();
    void UpdateDuelAuras();
    void BuildAllAuraUpdates();
    void UpdateAuraStateAuras(uint32 oldflag);
    bool BuildAuraUpdateAllPacket(WorldPacket* data);
    void SpellStealAuras(Unit* caster, int32 MaxSteals);
    void UpdateShapeShiftAuras(uint32 oldSS, uint32 newSS);
    void AttemptDispel(Unit* caster, int32 Mechanic, bool hostile);
    void MassDispel(Unit* caster, uint32 index, SpellEntry* Dispelling, uint32 MaxDispel, uint8 start, uint8 end);
    bool OverrideSimilarAuras(Unit *caster, Aura *aura);

    /*****************************
    ************ Info ************
    ******************************/
    uint32 GetSpellIdFromAuraSlot(uint32 slot);
    AuraCheckResponse AuraCheck(SpellEntry *info, uint64 casterGuid);
    uint32 GetAuraSpellIDWithNameHash(uint32 name_hash);

    bool HasAura(uint32 spellid);
    bool HasAuraVisual(uint32 visualid);//not spell id!!!
    bool HasActiveAura(uint32 spelllid);
    bool HasNegativeAura(uint32 spell_id); //just to reduce search range in some cases
    bool HasAuraWithMechanic(uint32 mechanic);
    bool HasActiveAura(uint32 spelllid,uint64);
    bool HasPosAuraWithMechanic(uint32 mechanic);
    bool HasNegAuraWithMechanic(uint32 mechanic);
    bool HasNegativeAuraWithNameHash(uint32 name_hash); //just to reduce search range in some cases
    bool HasCombatStatusAffectingAuras(uint64 checkGuid);
    bool HasAurasOfNameHashWithCaster(uint32 namehash, uint64 casterguid);

    /*****************************
    ************ Add *************
    ******************************/
    void AddAura(Aura* aur);
//  uint32 AddAuraVisual(uint32 SpellId, uint32 count, bool positive);

    /*****************************
    ********** Removal ***********
    ******************************/

    //! Remove aura from unit
    void RemoveAura(Aura* aur);
    void RemoveAuraBySlot(uint8 Slot);
    bool RemoveAuras(uint32 * SpellIds);
    void RemoveAuraNoReturn(uint32 spellId);
    bool RemovePositiveAura(uint32 spellId);
    bool RemoveNegativeAura(uint32 spellId);
    bool RemoveAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
    bool RemoveAuraPosByNameHash(uint32 namehash);//required to remove weaker instances of a spell
    bool RemoveAuraNegByNameHash(uint32 namehash);//required to remove weaker instances of a spell
    void RemoveAuraBySlotOrRemoveStack(uint8 Slot);
    bool RemoveAura(uint32 spellId, uint64 guid = 0);
    void EventRemoveAura(uint32 SpellId) { RemoveAura(SpellId); }


    //! Remove all auras
    void RemoveAllAuras();
    void RemoveAllExpiringAuras();
    void RemoveAllNegativeAuras();
    void RemoveAllNonPassiveAuras();
    void RemoveAllAurasExpiringWithPet();
    void RemoveAllAreaAuras(uint64 skipguid);
    bool RemoveAllAurasFromGUID(uint64 guid); //remove if they come from the same caster.
    void RemoveAllAurasOfType(uint32 auratype);//ex:to remove morph spells
    bool RemoveAllPosAurasFromGUID(uint64 guid); //remove if they come from the same caster.
    bool RemoveAllNegAurasFromGUID(uint64 guid); //remove if they come from the same caster.
    void RemoveAllAurasByInterruptFlag(uint32 flag);
    void RemoveAllAurasWithAuraName(uint32 auraName);
    void RemoveAllAurasWithSpEffect(uint32 EffectId);
    bool RemoveAllPosAurasByNameHash(uint32 namehash);//required to remove weaker instances of a spell
    bool RemoveAllNegAurasByNameHash(uint32 namehash);//required to remove weaker instances of a spell
    bool RemoveAllAuras(uint32 spellId, uint64 guid = 0); //remove stacked auras but only if they come from the same caster. Shaman purge If GUID = 0 then removes all auras with this spellid
    void RemoveAllAurasWithDispelType(uint32 DispelType);
    void RemoveAllAurasWithAttributes(uint8 index, uint32 attributeFlag);
    bool RemoveAllAurasByNameHash(uint32 namehash, bool passive);//required to remove weaker instances of a spell
    void RemoveAllAurasByInterruptFlagButSkip(uint32 flag, uint32 skip);
    void RemoveAllAurasOfSchool(uint32 School, bool Positive, bool Immune);
    uint32 GetAuraCountWithFamilyNameAndSkillLine(uint32 spellFamily, uint32 SkillLine);
    bool RemoveAllAurasByMechanic( uint32 MechanicType, int32 MaxDispel = -1, bool HostileOnly = true ); // Removes all (de)buffs on unit of a specific mechanic type.

    Aura* FindAuraBySlot(uint8 auraSlot);
    Aura* FindAura(uint32 spellId, uint64 guid = 0);
    Aura* FindPositiveAuraByNameHash(uint32 namehash);
    Aura* FindNegativeAuraByNameHash(uint32 namehash);
    Aura* FindActiveAura(uint32 spellId, uint64 guid = 0);
    Aura* FindActiveAuraWithNameHash(uint32 namehash, uint64 guid = 0);

    void EventDeathAuraRemoval();
//  bool HasVisibleAura(uint32 spellid);
    bool SetAuraDuration(uint32 spellId,int32 duration);
    bool SetAuraDuration(uint32 spellId,Unit* caster,int32 duration);

private:
    Unit* m_Unit;
    std::map<uint8, Aura*> m_auras;

    /*******************
    **** Modifiers
    ********/
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

    struct ModifierType
    {
        explicit ModifierType(uint32 type) : auraType(type), auraSlot(0), auraIndex(0) {}
        ModifierType(uint32 slot, uint32 type, uint32 index) : auraType(type), auraSlot(slot), auraIndex(index) {}

        bool operator<(const ModifierType& val) const { return auraType < val.auraType; }
        bool operator==(const ModifierType& val) const { return auraType == val.auraType && auraSlot == val.auraSlot && auraIndex == val.auraIndex; }
        uint32 auraType, auraSlot, auraIndex;
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

    void SM_FIValue( uint32 modifier, int32* v, uint32* group );
    void SM_FFValue( uint32 modifier, float* v, uint32* group );
    void SM_PIValue( uint32 modifier, int32* v, uint32* group );
    void SM_PFValue( uint32 modifier, float* v, uint32* group );

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
