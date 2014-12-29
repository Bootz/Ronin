
#include "StdAfx.h"

initialiseSingleton( ItemManager );

ItemManager::ItemManager()
{
    
}

ItemManager::~ItemManager()
{

}

void ItemManager::LoadItemData()
{
    QueryResult *result = CharacterDatabase.Query("SELECT * FROM item_data");
    if(result == NULL)
        return;

    std::map<WoWGuid, std::set<ItemData*>> containerStorage;
    do
    {
        Field *fields = result->Fetch();
        ItemData *data = new ItemData();
        data->giftData = NULL;
        data->containerData = NULL;
        for(uint8 i = 0; i < 10; i++)
        {
            data->enchantData[i] = NULL;
            if(i >= 5) continue;
            data->chargeData[i] = NULL;
        }

        data->itemGuid = fields[0].GetUInt64();
        data->itemContainer = fields[1].GetUInt64();
        data->itemCreator = fields[2].GetUInt64();
        data->containerSlot = fields[3].GetUInt32();
        data->itemStackCount = fields[4].GetUInt32();
        data->itemFlags = fields[5].GetUInt32();
        data->itemRandomSeed = fields[6].GetUInt32();
        data->itemRandomProperty = fields[7].GetUInt32();
        data->itemDurability = fields[8].GetUInt32();
        data->itemPlayedTime = fields[9].GetUInt32();
        if(uint32 giftItemId = fields[10].GetUInt32())
        {
            data->giftData = new ItemData::ItemGiftData;
            data->giftData->giftItemId = giftItemId;
            data->giftData->giftCreatorId = fields[11].GetUInt64();
        }
        if(uint16 containerSlots = fields[12].GetUInt16())
        {
            data->containerData = new ItemData::ContainerData;
            data->containerData->numSlots = containerSlots;
        }

        m_itemData.insert(std::make_pair(data->itemGuid, data));
        containerStorage[data->itemContainer].insert(data);
    }while(result->NextRow());
    delete result;

    if(result = CharacterDatabase.Query("SELECT * FROM item_enchantments"))
    {
        do
        {
            Field *fields = result->Fetch();
            WoWGuid guid = fields[0].GetUInt64();
            if(m_itemData.find(guid) == m_itemData.end())
                continue;
            uint8 slotId = fields[1].GetUInt8();
            ItemData *data = m_itemData.at(guid);
            if(data->enchantData[slotId] != NULL)
                continue;
            data->enchantData[slotId] = new ItemData::EnchantData();
            data->enchantData[slotId]->enchantId = fields[2].GetUInt32();
            data->enchantData[slotId]->enchantCharges = fields[3].GetUInt32();
            data->enchantData[slotId]->expirationTime = fields[4].GetUInt64();
        } while(result->NextRow());
        delete result;
    }

    if(result = CharacterDatabase.Query("SELECT * FROM item_spellcharges"))
    {
        do
        {
            Field *fields = result->Fetch();
            WoWGuid guid = fields[0].GetUInt64();
            if(m_itemData.find(guid) == m_itemData.end())
                continue;
            ItemData *data = m_itemData.at(guid);
            for(uint8 i = 0; i < 5; i++)
            {
                if(uint32 charges = fields[i+1].GetInt32())
                {
                    data->chargeData[i] = new ItemData::SpellCharges();
                    data->chargeData[i]->spellCharges = charges;
                }
            }
        } while(result->NextRow());
        delete result;
    }

    for(std::map<WoWGuid, std::set<ItemData*>>::iterator itr = containerStorage.begin(); itr != containerStorage.end(); itr++)
    {
        WoWGuid containerGuid = itr->first;
        switch(containerGuid.getHigh())
        {
        case HIGHGUID_TYPE_CONTAINER:
            {
                ItemData *data = GetItemData(containerGuid);
                if(data->containerData == NULL)
                    continue; // Something wrong here
                for(std::set<ItemData*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
                    data->containerData->m_items.insert(std::make_pair(uint16((*itr2)->containerSlot&0xFFFF), (*itr2)->itemGuid));
            }break;
        case HIGHGUID_TYPE_GUILD:
            {
                GuildBankItemStorage *guildStorage = new GuildBankItemStorage();
                for(std::set<ItemData*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
                {
                    uint16 tabId = ((*itr2)->containerSlot>>16), tabSlot = ((*itr2)->containerSlot&0xFFFF);
                    guildStorage->bankTabs[tabId].insert(std::make_pair(tabSlot, (*itr2)->itemGuid));
                }
                //AddGuildStorage(containerGuid, guildStorage);
            }break;
        case HIGHGUID_TYPE_PLAYER:
            {
                PlayerInventory *inventory = new PlayerInventory();
                for(std::set<ItemData*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
                    inventory->m_playerInventory.insert(std::make_pair((*itr2)->containerSlot, (*itr2)->itemGuid));
                //AddPlayerInventory(containerGuid, inventory);
            }break;
            // Loooooot
        case HIGHGUID_TYPE_CORPSE:
        case HIGHGUID_TYPE_VEHICLE:
        case HIGHGUID_TYPE_CREATURE:
        case HIGHGUID_TYPE_GAMEOBJECT:
            {

            }break;
        }
    }
}

uint32 CalcWeaponDurability(uint32 subClass, uint32 quality, uint32 itemLevel);
uint32 CalcArmorDurability(uint32 inventoryType, uint32 quality, uint32 itemLevel);

void ItemManager::InitializeItemPrototypes()
{
    // It has to be empty for us to fill in db2 data
    if(!m_itemPrototypeContainer.empty())
        return;

    sLog.Notice("ItemPrototypeSystem", "Loading %u items!", db2Item.GetNumRows());
    ItemDataEntry *itemData = NULL;
    ItemSparseEntry *sparse = NULL;
    for(uint32 i = 0; i < db2Item.GetNumRows(); i++)
    {
        itemData = db2Item.LookupRow(i);
        sparse = db2ItemSparse.LookupEntry(itemData->ID);
        if(sparse == NULL)
            continue;

        ItemPrototype* proto = new ItemPrototype();
        // Non dbc data
        proto->minDamage = 0;
        proto->maxDamage = 0;
        proto->Armor = 0;
        proto->MaxDurability = 0;
        // These are set later.
        proto->ItemId = itemData->ID;
        proto->Class = itemData->Class;
        proto->SubClass = itemData->SubClass;
        proto->subClassSound = itemData->SoundOverrideSubclass;
        proto->Name1 = sparse->Name;
        proto->DisplayInfoID = itemData->DisplayId;
        proto->Quality = sparse->Quality;
        proto->Flags = sparse->Flags;
        proto->FlagsExtra = sparse->Flags2;
        proto->BuyPrice = sparse->BuyPrice;
        proto->SellPrice = sparse->SellPrice;
        proto->InventoryType = itemData->InventoryType;
        proto->AllowableClass = sparse->AllowableClass;
        proto->AllowableRace = sparse->AllowableRace;
        proto->ItemLevel = sparse->ItemLevel;
        proto->RequiredLevel = sparse->RequiredLevel;
        proto->RequiredSkill = sparse->RequiredSkill;
        proto->RequiredSkillRank = sparse->RequiredSkillRank;
        proto->RequiredSpell = sparse->RequiredSpell;
        proto->RequiredPlayerRank1 = sparse->RequiredHonorRank;
        proto->RequiredPlayerRank2 = sparse->RequiredCityRank;
        proto->RequiredFaction = sparse->RequiredReputationFaction;
        proto->RequiredFactionStanding = sparse->RequiredReputationRank;
        proto->MaxCount = sparse->Stackable;
        proto->Unique = sparse->MaxCount;
        proto->ContainerSlots = sparse->ContainerSlots;
        for(uint8 i = 0; i < 10; i++)
        {
            proto->Stats[i].Type = sparse->ItemStatType[i];
            proto->Stats[i].Value = sparse->ItemStatValue[i];
            if(i > 5)
                continue;
            proto->Spells[i].Id = sparse->SpellId[i];
            proto->Spells[i].Trigger = sparse->SpellTrigger[i];
            proto->Spells[i].Charges = sparse->SpellCharges[i];
            proto->Spells[i].Cooldown = sparse->SpellCooldown[i];
            proto->Spells[i].Category = sparse->SpellCategory[i];
            proto->Spells[i].CategoryCooldown = sparse->SpellCategoryCooldown[i];
            if(i > 3)
                continue;
            proto->ItemSocket[i] = sparse->SocketColor[i];
            proto->ItemContent[i] = sparse->SocketContent[i];
        }

        proto->DamageType = sparse->DamageType;
        proto->Delay = sparse->Delay;
        proto->Range = sparse->RangedModRange;
        proto->Bonding = sparse->Bonding;
        proto->Description = sparse->Description;
        proto->PageId = sparse->PageText;
        proto->PageLanguage = sparse->LanguageID;
        proto->PageMaterial = sparse->PageMaterial;
        proto->QuestId = sparse->StartQuest;
        proto->LockId = sparse->LockID;
        proto->LockMaterial = sparse->Material;
        proto->SheathID = sparse->Sheath;
        proto->RandomPropId = sparse->RandomProperty;
        proto->RandomSuffixId = sparse->RandomSuffix;
        proto->ItemSet = sparse->ItemSet;
        proto->ZoneNameID = sparse->Area;
        proto->MapID = sparse->Map;
        proto->BagFamily = sparse->BagFamily;
        proto->SocketBonus = sparse->SocketBonus;
        proto->GemProperties = sparse->GemProperties;
        proto->ArmorDamageModifier = sparse->ArmorDamageModifier;
        proto->Duration = sparse->Duration;
        proto->ItemLimitCategory = sparse->ItemLimitCategory;
        proto->HolidayId = sparse->HolidayId;
        proto->StatScalingFactor = sparse->StatScalingFactor;
        proto->lowercase_name = std::string(proto->Name1);
        for(uint32 j = 0; j < proto->lowercase_name.length(); ++j)
            proto->lowercase_name[j] = tolower(proto->lowercase_name[j]);
        m_itemPrototypeContainer.insert(std::make_pair(itemData->ID, proto));
    }
    LoadItemOverrides();
}

void ItemManager::LoadItemOverrides()
{
    ItemPrototype* proto = NULL;
    sLog.Notice("ItemPrototypeSystem", "Loading item overrides...");
    QueryResult* result = WorldDatabase.Query("SELECT * FROM item_overrides");
    if(result != NULL)
    {
        if(result->GetFieldCount() == 109)
        {
            uint8 overrideFlags = 0x00;
            uint32 entry = 0, field_Count = 0;
            do
            {
                Field *fields = result->Fetch();
                entry = fields[field_Count++].GetUInt32();
                if(m_itemPrototypeContainer.find(entry) == m_itemPrototypeContainer.end())
                {
                    ItemPrototype* proto = new ItemPrototype();
                    proto->ItemId = entry;
                    proto->Class = 0;
                    proto->SubClass = 0;
                    proto->subClassSound = 0;
                    proto->Name1 = "";
                    proto->DisplayInfoID = 0;
                    proto->Quality = 0;
                    proto->Flags = 0;
                    proto->FlagsExtra = 0;
                    proto->BuyPrice = 0;
                    proto->SellPrice = 0;
                    proto->InventoryType = 0;
                    proto->AllowableClass = 0;
                    proto->AllowableRace = 0;
                    proto->ItemLevel = 0;
                    proto->RequiredLevel = 0;
                    proto->RequiredSkill = 0;
                    proto->RequiredSkillRank = 0;
                    proto->RequiredSpell = 0;
                    proto->RequiredPlayerRank1 = 0;
                    proto->RequiredPlayerRank2 = 0;
                    proto->RequiredFaction = 0;
                    proto->RequiredFactionStanding = 0;
                    proto->MaxCount = 0;
                    proto->Unique = 0;
                    proto->ContainerSlots = 0;
                    proto->minDamage = 0;
                    proto->maxDamage = 0;
                    proto->Armor = 0;
                    for(uint8 i = 0; i < 10; i++)
                    {
                        proto->Stats[i].Type = 0;
                        proto->Stats[i].Value = 0;
                        if(i > 5)
                            continue;
                        proto->Spells[i].Id = 0;
                        proto->Spells[i].Trigger = 0;
                        proto->Spells[i].Charges = 0;
                        proto->Spells[i].Cooldown = 0;
                        proto->Spells[i].Category = 0;
                        proto->Spells[i].CategoryCooldown = 0;
                        if(i > 3)
                            continue;
                        proto->ItemSocket[i] = 0;
                        proto->ItemContent[i] = 0;
                    }

                    proto->DamageType = 0;
                    proto->Delay = 0;
                    proto->Range = 0;
                    proto->Bonding = 0;
                    proto->Description = "";
                    proto->PageId = 0;
                    proto->PageLanguage = 0;
                    proto->PageMaterial = 0;
                    proto->QuestId = 0;
                    proto->LockId = 0;
                    proto->LockMaterial = 0;
                    proto->SheathID = 0;
                    proto->RandomPropId = 0;
                    proto->RandomSuffixId = 0;
                    proto->ItemSet = 0;
                    proto->MaxDurability = 0;
                    proto->ZoneNameID = 0;
                    proto->MapID = 0;
                    proto->BagFamily = 0;
                    proto->SocketBonus = 0;
                    proto->GemProperties = 0;
                    proto->ArmorDamageModifier = 0;
                    proto->Duration = 0;
                    proto->ItemLimitCategory = 0;
                    proto->HolidayId = 0;
                    proto->StatScalingFactor = 0;
                    m_itemPrototypeContainer.insert(std::make_pair(entry, proto));
                    overrideFlags = 0x01 | 0x02;
                } else proto = m_itemPrototypeContainer.at(entry);

                if(proto->Class != fields[field_Count].GetUInt32())
                {
                    proto->Class = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(proto->SubClass != fields[field_Count].GetUInt32())
                {
                    proto->SubClass = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(proto->subClassSound != fields[field_Count].GetInt32())
                {
                    proto->subClassSound = fields[field_Count].GetInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(strcmp(proto->Name1, fields[field_Count].GetString()))
                {
                    proto->Name1 = strdup(fields[field_Count].GetString());
                    overrideFlags |= 0x02;

                    proto->lowercase_name = std::string(proto->Name1);
                    for(uint32 j = 0; j < proto->lowercase_name.length(); ++j)
                        proto->lowercase_name[j] = tolower(proto->lowercase_name[j]);
                }field_Count++;
                if(proto->DisplayInfoID != fields[field_Count].GetUInt32())
                {
                    proto->DisplayInfoID = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(proto->Quality != fields[field_Count].GetUInt32())
                {
                    proto->Quality = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->Flags != fields[field_Count].GetUInt32())
                {
                    proto->Flags = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->FlagsExtra != fields[field_Count].GetUInt32())
                {
                    proto->FlagsExtra = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->BuyPrice != fields[field_Count].GetUInt32())
                {
                    proto->BuyPrice = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->SellPrice != fields[field_Count].GetUInt32())
                {
                    proto->SellPrice = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->InventoryType != fields[field_Count].GetUInt32())
                {
                    proto->InventoryType = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(proto->AllowableClass != fields[field_Count].GetUInt32())
                {
                    proto->AllowableClass = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->AllowableRace != fields[field_Count].GetUInt32())
                {
                    proto->AllowableRace = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ItemLevel != fields[field_Count].GetUInt32())
                {
                    proto->ItemLevel = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredLevel != fields[field_Count].GetUInt32())
                {
                    proto->RequiredLevel = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredSkill != fields[field_Count].GetUInt32())
                {
                    proto->RequiredSkill = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredSkillRank != fields[field_Count].GetUInt32())
                {
                    proto->RequiredSkillRank = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredSpell != fields[field_Count].GetUInt32())
                {
                    proto->RequiredSpell = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredPlayerRank1 != fields[field_Count].GetUInt32())
                {
                    proto->RequiredPlayerRank1 = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredPlayerRank2 != fields[field_Count].GetUInt32())
                {
                    proto->RequiredPlayerRank2 = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredFaction != fields[field_Count].GetUInt32())
                {
                    proto->RequiredFaction = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RequiredFactionStanding != fields[field_Count].GetUInt32())
                {
                    proto->RequiredFactionStanding = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->MaxCount != fields[field_Count].GetUInt32())
                {
                    proto->MaxCount = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->Unique != fields[field_Count].GetUInt32())
                {
                    proto->Unique = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ContainerSlots != fields[field_Count].GetUInt32())
                {
                    proto->ContainerSlots = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                for(uint8 i = 0; i < 10; i++)
                {
                    if(proto->Stats[i].Type != fields[field_Count].GetUInt32())
                    {
                        proto->Stats[i].Type = fields[field_Count].GetUInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Stats[i].Value != fields[field_Count].GetInt32())
                    {
                        proto->Stats[i].Value = fields[field_Count].GetInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                }
                if(proto->DamageType != fields[field_Count].GetUInt32())
                {
                    proto->DamageType = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->Delay != fields[field_Count].GetUInt32())
                {
                    proto->Delay = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->Range != fields[field_Count].GetFloat())
                {
                    proto->Range = fields[field_Count].GetFloat();
                    overrideFlags |= 0x02;
                }field_Count++;
                for(uint8 i = 0; i < 5; i++)
                {
                    if(proto->Spells[i].Id != fields[field_Count].GetUInt32())
                    {
                        proto->Spells[i].Id = fields[field_Count].GetUInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Spells[i].Trigger != fields[field_Count].GetUInt32())
                    {
                        proto->Spells[i].Trigger = fields[field_Count].GetUInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Spells[i].Charges != fields[field_Count].GetInt32())
                    {
                        proto->Spells[i].Charges = fields[field_Count].GetInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Spells[i].Cooldown != fields[field_Count].GetInt32())
                    {
                        proto->Spells[i].Cooldown = fields[field_Count].GetInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Spells[i].Category != fields[field_Count].GetUInt32())
                    {
                        proto->Spells[i].Category = fields[field_Count].GetUInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                    if(proto->Spells[i].CategoryCooldown != fields[field_Count].GetInt32())
                    {
                        proto->Spells[i].CategoryCooldown = fields[field_Count].GetInt32();
                        overrideFlags |= 0x02;
                    }field_Count++;
                }
                if(proto->Bonding != fields[field_Count].GetUInt32())
                {
                    proto->Bonding = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(strcmp(proto->Description, fields[field_Count].GetString()))
                {
                    proto->Description = strdup(fields[field_Count].GetString());
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->PageId != fields[field_Count].GetUInt32())
                {
                    proto->PageId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->PageLanguage != fields[field_Count].GetUInt32())
                {
                    proto->PageLanguage = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->PageMaterial != fields[field_Count].GetUInt32())
                {
                    proto->PageMaterial = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->QuestId != fields[field_Count].GetUInt32())
                {
                    proto->QuestId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->LockId != fields[field_Count].GetUInt32())
                {
                    proto->LockId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->LockMaterial != fields[field_Count].GetUInt32())
                {
                    proto->LockMaterial = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->SheathID != fields[field_Count].GetUInt32())
                {
                    proto->SheathID = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x01;
                }field_Count++;
                if(proto->RandomPropId != fields[field_Count].GetUInt32())
                {
                    proto->RandomPropId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->RandomSuffixId != fields[field_Count].GetUInt32())
                {
                    proto->RandomSuffixId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ItemSet != fields[field_Count].GetUInt32())
                {
                    proto->ItemSet = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ZoneNameID != fields[field_Count].GetUInt32())
                {
                    proto->ZoneNameID = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->MapID != fields[field_Count].GetUInt32())
                {
                    proto->MapID = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->BagFamily != fields[field_Count].GetUInt32())
                {
                    proto->BagFamily = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->SocketBonus != fields[field_Count].GetUInt32())
                {
                    proto->SocketBonus = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->GemProperties != fields[field_Count].GetUInt32())
                {
                    proto->GemProperties = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ArmorDamageModifier != fields[field_Count].GetFloat())
                {
                    proto->ArmorDamageModifier = fields[field_Count].GetFloat();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->Duration != fields[field_Count].GetUInt32())
                {
                    proto->Duration = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->ItemLimitCategory != fields[field_Count].GetUInt32())
                {
                    proto->ItemLimitCategory = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->HolidayId != fields[field_Count].GetUInt32())
                {
                    proto->HolidayId = fields[field_Count].GetUInt32();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(proto->StatScalingFactor != fields[field_Count].GetFloat())
                {
                    proto->StatScalingFactor = fields[field_Count].GetFloat();
                    overrideFlags |= 0x02;
                }field_Count++;
                if(overrideFlags > 0) m_overwritten.insert(std::make_pair(entry, overrideFlags));
                proto = NULL;
                overrideFlags = 0;
                entry = field_Count = 0;
            } while( result->NextRow() );
            sLog.Notice("ItemPrototypeSystem", "%u item overrides loaded!", m_overwritten.size());
        } delete result;    
    }

    sLog.Notice("ItemPrototypeSystem", "Setting static dbc data...");
    uint8 i = 0;
    ItemArmorQuality *ArmorQ = NULL; ItemArmorShield *ArmorS = NULL;
    ItemArmorTotal *ArmorT = NULL; ArmorLocationEntry *ArmorE = NULL;
    ItemDamageEntry *Damage = NULL;
    for(iterator itr = itemPrototypeBegin(); itr != itemPrototypeEnd(); ++itr)
    {
        proto = (*itr)->second;
        uint32 Quality = proto->Quality;
        if(Quality >= ITEM_QUALITY_DBC_MAX)
            continue;

        ArmorQ = NULL, ArmorS = NULL, ArmorT = NULL, ArmorE = NULL, Damage = NULL;
        if(proto->Class == ITEM_CLASS_WEAPON)
        {
            proto->MaxDurability = CalcWeaponDurability(proto->SubClass, proto->Quality, proto->ItemLevel);
            switch(proto->InventoryType)
            {
            case INVTYPE_WEAPON:
            case INVTYPE_WEAPONMAINHAND:
            case INVTYPE_WEAPONOFFHAND:
                {
                    if (proto->FlagsExtra & 0x200)
                        Damage = dbcDamageOneHandCaster.LookupEntry(proto->ItemLevel);
                    else Damage = dbcDamageOneHand.LookupEntry(proto->ItemLevel);
                }break;
            case INVTYPE_AMMO:
                {
                    Damage = dbcDamageAmmo.LookupEntry(proto->ItemLevel);
                }break;
            case INVTYPE_2HWEAPON:
                {
                    if (proto->FlagsExtra & 0x200)
                        Damage = dbcDamageTwoHandCaster.LookupEntry(proto->ItemLevel);
                    else Damage = dbcDamageTwoHand.LookupEntry(proto->ItemLevel);
                }break;
            case INVTYPE_RANGED:
            case INVTYPE_THROWN:
            case INVTYPE_RANGEDRIGHT:
                {
                    switch(proto->SubClass)
                    {
                    case ITEM_SUBCLASS_WEAPON_BOW:
                    case ITEM_SUBCLASS_WEAPON_GUN:
                    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                        Damage = dbcDamageRanged.LookupEntry(proto->ItemLevel);
                        break;
                    case ITEM_SUBCLASS_WEAPON_THROWN:
                        Damage = dbcDamageThrown.LookupEntry(proto->ItemLevel);
                        break;
                    case ITEM_SUBCLASS_WEAPON_WAND:
                        Damage = dbcDamageDamageWand.LookupEntry(proto->ItemLevel);
                        break;
                    default:
                        break;
                    }
                }break;
            default: break;
            }
        }
        else if(proto->Class == ITEM_CLASS_ARMOR)
        {
            uint32 inventoryType = proto->InventoryType;
            if (proto->InventoryType == INVTYPE_ROBE)
                inventoryType = INVTYPE_CHEST;
            if (proto->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
                ArmorS = dbcArmorShield.LookupEntry(proto->ItemLevel);
            else if(ArmorE = dbcArmorLocation.LookupEntry(inventoryType))
            {
                if(proto->SubClass && proto->SubClass <= 5)
                {
                    ArmorQ = dbcArmorQuality.LookupEntry(proto->ItemLevel);
                    ArmorT = dbcArmorTotal.LookupEntry(proto->ItemLevel);
                }
            }
            proto->MaxDurability = CalcArmorDurability(inventoryType, proto->Quality, proto->ItemLevel);
        }

        if(Damage)
        {
            float dps = Damage->mod_DPS[Quality];
            float avgDamage = dps * proto->Delay * 0.001f;
            proto->minDamage = (proto->StatScalingFactor * -0.5f + 1.0f) * avgDamage;
            proto->maxDamage = floor(float(avgDamage * (proto->StatScalingFactor * 0.5f + 1.0f) + 0.5f));
        }

        if(ArmorS)
            proto->Armor = ArmorS->mod_Resist[Quality];
        else if(ArmorQ && ArmorT && ArmorE)
            proto->Armor = uint32(ArmorQ->mod_Resist[Quality] * ArmorT->mod_Resist[proto->SubClass-1] * ArmorE->Value[proto->SubClass-1] + 0.5f);
    }
}

static float const qualityDurabilityMultipliers[ITEM_QUALITY_DBC_MAX] = { 1.0f, 1.0f, 1.0f, 1.17f, 1.37f, 1.68f, 0.0f };

uint32 CalcWeaponDurability(uint32 subClass, uint32 quality, uint32 itemLevel)
{
    static float const weaponMultipliers[ITEM_SUBCLASS_WEAPON_FISHING_POLE+1] =
    { 0.89f, 1.03f, 0.77f, 0.77f, 0.89f, 1.03f, 1.03f, 0.89f, 1.03f, 0.00f, 1.03f, 0.00f, 0.00f, 0.64f, 0.00f, 0.64f, 0.64f, 0.00f, 0.77f, 0.64f, 0.64f, };
    return 5 * uint32(17.f * qualityDurabilityMultipliers[quality] * weaponMultipliers[subClass] * (itemLevel <= 17 ? (0.966f - (17.f - float(itemLevel)) / 54.0f) : 1.0f) + 0.5f);
}

uint32 CalcArmorDurability(uint32 inventoryType, uint32 quality, uint32 itemLevel)
{
    static float const armorMultipliers[NUM_INVENTORY_TYPES] =
    { 0.00f, 0.59f, 0.00f, 0.59f, 0.00f, 1.00f, 0.35f, 0.75f, 0.49f, 0.35f, 0.35f, 0.00f, 0.00f, 0.00f, 
    1.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, };
    return 5 * uint32(23.f * qualityDurabilityMultipliers[quality] * armorMultipliers[inventoryType] * (itemLevel <= 28 ? (0.966f - (28.f - float(itemLevel)) / 54.0f) : 1.0f) + 0.5f);
}

ItemPrototype* ItemManager::LookupEntry(uint32 entry)
{
    iterator itr = itemPrototypeFind(entry);
    if(itr == itemPrototypeEnd())
        return NULL;
    return (*itr)->second;
}