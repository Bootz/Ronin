/*
 * Sandshroud Project Ronin
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009-2017 Sandshroud <https://github.com/Sandshroud>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

enum PartyUpdateFlags
{
    GROUP_UPDATE_FLAG_NONE                      = 0x00000000,   // 
    GROUP_UPDATE_FLAG_ONLINE                    = 0x00000001,   // uint16
    GROUP_UPDATE_FLAG_HEALTH                    = 0x00000002,   // uint32
    GROUP_UPDATE_FLAG_MAXHEALTH                 = 0x00000004,   // uint32
    GROUP_UPDATE_FLAG_POWER_TYPE                = 0x00000008,   // uint8
    GROUP_UPDATE_FLAG_POWER                     = 0x00000010,   // uint16
    GROUP_UPDATE_FLAG_MAXPOWER                  = 0x00000020,   // uint16
    GROUP_UPDATE_FLAG_LEVEL                     = 0x00000040,   // uint16
    GROUP_UPDATE_FLAG_ZONEID                    = 0x00000080,   // uint16
    GROUP_UPDATE_FLAG_STATS                     = 0x00000100,   // ??
    GROUP_UPDATE_FLAG_POSITION                  = 0x00000200,   // uint16, uint16, uint16
    GROUP_UPDATE_FLAG_PLAYER_AURAS              = 0x00000400,   // uint8, uint64, uint32 for each uint64
    GROUP_UPDATE_FLAG_PET_GUID                  = 0x00000800,   // uint64
    GROUP_UPDATE_FLAG_PET_NAME                  = 0x00001000,   // string
    GROUP_UPDATE_FLAG_PET_DISPLAYID             = 0x00002000,   // uint16
    GROUP_UPDATE_FLAG_PET_HEALTH                = 0x00004000,   // uint16
    GROUP_UPDATE_FLAG_PET_MAXHEALTH             = 0x00008000,   // uint16
    GROUP_UPDATE_FLAG_PET_POWER_TYPE            = 0x00010000,   // uint8
    GROUP_UPDATE_FLAG_PET_POWER                 = 0x00020000,   // uint16
    GROUP_UPDATE_FLAG_PET_MAXPOWER              = 0x00040000,   // uint16
    GROUP_UPDATE_FLAG_PET_AURAS                 = 0x00080000,   // uint64, uint16 for each uint64
    GROUP_UPDATE_FLAG_VEHICLE_ENTRY             = 0x00100000,   //
    GROUP_UPDATE_FLAG_PHASE                     = 0x00200000,   //

    GROUP_UPDATE_LOCATION = GROUP_UPDATE_FLAG_ZONEID
    | GROUP_UPDATE_FLAG_POSITION,

    GROUP_UPDATE_PET = GROUP_UPDATE_FLAG_PET_GUID |
        GROUP_UPDATE_FLAG_PET_NAME |
        GROUP_UPDATE_FLAG_PET_DISPLAYID |
        GROUP_UPDATE_FLAG_PET_HEALTH |
        GROUP_UPDATE_FLAG_PET_MAXHEALTH |
        GROUP_UPDATE_FLAG_PET_POWER_TYPE |
        GROUP_UPDATE_FLAG_PET_POWER |
        GROUP_UPDATE_FLAG_PET_MAXPOWER |
        GROUP_UPDATE_FLAG_PET_AURAS,

    GROUP_UPDATE_FULL = GROUP_UPDATE_PET
        | GROUP_UPDATE_FLAG_ONLINE
        | GROUP_UPDATE_FLAG_HEALTH
        | GROUP_UPDATE_FLAG_MAXHEALTH
        | GROUP_UPDATE_FLAG_POWER_TYPE
        | GROUP_UPDATE_FLAG_POWER
        | GROUP_UPDATE_FLAG_MAXPOWER
        | GROUP_UPDATE_FLAG_LEVEL
        | GROUP_UPDATE_FLAG_ZONEID
        | GROUP_UPDATE_FLAG_POSITION
        | GROUP_UPDATE_FLAG_PLAYER_AURAS
        | GROUP_UPDATE_FLAG_PHASE
};

Group* Group::Create()
{
    return new Group(true);
}

Group* Group::Init(uint32 groupId, uint32 dungeonId, uint16 groupType)
{
    return new Group(groupId, dungeonId, groupType);
}

Group::Group(bool Assign)
{
    m_GroupType = GROUP_TYPE_PARTY;  // Always init as party

    // Create initial subgroup
    memset(m_SubGroups,0, sizeof(SubGroup*)*8);
    m_SubGroups[0] = new SubGroup(this, 0);

    m_Leader = NULL;
    m_Looter = NULL;
    m_LootMethod = PARTY_LOOT_GROUP;
    m_LootThreshold = 2;
    m_SubGroupCount = 1;
    m_MemberCount = 0;
    m_difficulty = MODE_5PLAYER_NORMAL;
    m_raiddifficulty = MODE_10PLAYER_NORMAL;

    if( Assign )
    {
        m_Id = objmgr.GenerateGroupId();
        ObjectMgr::getSingleton().AddGroup(this);
    }

    m_dirty=false;
    m_updateblock=false;
    m_groupFlags = 0;
    memset(m_targetIcons, 0, sizeof(uint64) * 8);
    m_isqueued=false;
    m_assistantLeader=m_mainAssist=m_mainTank=NULL;
    m_lfdData = NULL;
}

Group::Group(uint32 groupId, uint32 dungeonId, uint16 groupType) : m_Id(groupId), m_GroupType((groupType&GROUP_TYPE_MASK))
{
    // Create initial subgroup
    memset(m_SubGroups,0, sizeof(SubGroup*)*8);
    m_SubGroups[0] = new SubGroup(this, 0);

    m_Leader = NULL;
    m_Looter = NULL;
    m_LootMethod = PARTY_LOOT_GROUP;
    m_LootThreshold = 2;
    m_SubGroupCount = 1;
    m_MemberCount = 0;
    m_difficulty = (groupType&GROUP_TYPE_HEROIC) ? MODE_5PLAYER_HEROIC : MODE_5PLAYER_NORMAL;
    m_raiddifficulty = MODE_10PLAYER_NORMAL;

    m_dirty=false;
    m_updateblock=false;
    m_groupFlags = 0;
    memset(m_targetIcons, 0, sizeof(uint64) * 8);
    m_isqueued=false;
    m_assistantLeader=m_mainAssist=m_mainTank=NULL;
    m_lfdData = NULL;
    if(groupType & GROUP_TYPE_LFD)
    {
        m_lfdData = new LFDDungeonData();
        m_lfdData->dungeonId = dungeonId;
        m_lfdData->encounterMask = 0xFF;
    }

    objmgr.AddGroup(this);
    SaveToDB();
}

Group::~Group()
{
    ObjectMgr::getSingleton().RemoveGroup(this);
}

SubGroup::~SubGroup()
{

}

void SubGroup::RemovePlayer(PlayerInfo * info)
{
    m_GroupMembers.erase(info);
    info->subGroup=-1;
}

bool SubGroup::AddPlayer(PlayerInfo * info)
{
    if(IsFull())
        return false;

    info->subGroup=(int8)GetID();
    m_GroupMembers.insert(info);
    return true;
}

bool SubGroup::HasMember(WoWGuid guid)
{
    for( GroupMembersSet::iterator itr = m_GroupMembers.begin(); itr != m_GroupMembers.end(); itr++ )
        if( (*itr) != NULL && (*itr)->charGuid == guid )
            return true;
    return false;
}

SubGroup * Group::FindFreeSubGroup()
{
    for(uint32 i = 0; i < m_SubGroupCount; i++)
        if(!m_SubGroups[i]->IsFull())
            return m_SubGroups[i];

    return NULL;
}

bool Group::AddMember(PlayerInfo * info, int32 subgroupid/* =-1 */, bool silent/* = false */)
{
    m_groupLock.Acquire();
    Player* pPlayer = info->m_loggedInPlayer;
    m_isqueued = false;

    if(!IsFull())
    {
        SubGroup* subgroup = (subgroupid>0) ? m_SubGroups[subgroupid] : FindFreeSubGroup();
        if(subgroup == NULL)
        {
            m_groupLock.Release();
            return false;
        }

        if(subgroup->AddPlayer(info))
        {
            m_dirty=true;
            ++m_MemberCount;
            if(silent == false) Update();   // Send group update
            if(info->m_Group && info->m_Group != this)
                info->m_Group->RemovePlayer(info);

            if(m_Leader==NULL && info->m_loggedInPlayer)
                m_Leader=info;

            info->m_Group=this;
            info->subGroup = (int8)subgroup->GetID();

            m_groupLock.Release();
            if(pPlayer && pPlayer->GetMapInstance()->IsInstance())
                pPlayer->GetMapInstance()->OnGroupEnter(pPlayer, this);
            if(silent == false) UpdateOutOfRangePlayer(info, GROUP_UPDATE_FULL, true, NULL);
            return true;
        }
        else
        {
            m_groupLock.Release();
            info->m_Group=NULL;
            info->subGroup=-1;
            return false;
        }
    }
    else
    {
        info->m_Group = NULL;
        info->subGroup = -1;
        m_groupLock.Release();
        return false;
    }
}

void Group::SetLeader(Player* pPlayer, bool silent)
{
    if( pPlayer != NULL )
    {
        m_Leader = pPlayer->getPlayerInfo();
        m_dirty = true;
        if( !silent )
        {
            WorldPacket data( SMSG_GROUP_SET_LEADER, pPlayer->GetNameString()->size() + 1 );
            data << pPlayer->GetName();
            SendPacketToAll( &data );
        }
    }
    Update();
}

void Group::Update()
{
    if( m_updateblock )
        return;

    Player* pNewLeader = NULL;
    PlayerInfo *plrinf;

    WorldPacket data(50 + ( m_MemberCount * 20 ) );
    GroupMembersSet::iterator itr1, itr2;

    uint32 i = 0, j = 0;
    uint8 flags;
    SubGroup *sg1 = NULL, *sg2 = NULL;
    m_groupLock.Acquire();

    m_updateblock = true;

    if( m_groupFlags & GROUP_FLAG_REMOVE_OFFLINE_PLAYERS )
    {
        for( i = 0; i < m_SubGroupCount; i++ )
        {
            sg1 = m_SubGroups[i];

            if( sg1 != NULL)
            {
                for( itr1 = sg1->GetGroupMembersBegin(); itr1 != sg1->GetGroupMembersEnd(); )
                {
                    // should never happen but just in case
                    plrinf = *itr1;
                    ++itr1;

                    if( plrinf == NULL )
                        continue;

                    if( plrinf->m_loggedInPlayer == NULL )
                    {
                        // bai bai
                        RemovePlayer(plrinf);
                    }
                }
            }
        }
    }

    m_updateblock = false;

    if( m_Leader == NULL || ( m_Leader != NULL && m_Leader->m_loggedInPlayer == NULL ) )
    {
        pNewLeader = FindFirstPlayer();
        if( pNewLeader != NULL )
            m_Leader = pNewLeader->getPlayerInfo();
    }

    if( m_Looter != NULL && m_Looter->m_loggedInPlayer == NULL )
    {
        if( pNewLeader == NULL )
            pNewLeader = FindFirstPlayer();
        if( pNewLeader != NULL )
            m_Looter = pNewLeader->getPlayerInfo();
    }

    for( i = 0; i < m_SubGroupCount; i++ )
    {
        sg1 = m_SubGroups[i];

        if( sg1 != NULL)
        {
            for( itr1 = sg1->GetGroupMembersBegin(); itr1 != sg1->GetGroupMembersEnd(); itr1++ )
            {
                // should never happen but just in case
                if( (*itr1) == NULL )
                    continue;

                /* skip offline players */
                if( (*itr1)->m_loggedInPlayer == NULL )
                    continue;

                data.Initialize(SMSG_GROUP_LIST);
                data << uint8(m_GroupType); //0=party,1=raid
                data << uint8(sg1->GetID());

                flags = 0; // Player flags
                if( (*itr1) == m_assistantLeader ) flags |= 1;
                if( (*itr1) == m_mainTank ) flags |= 2;
                if( (*itr1) == m_mainAssist ) flags |= 4;
                data << flags;

                if(m_lfdData)
                {
                    Loki::AssocVector<WoWGuid, uint8>::iterator itr;
                    if((itr = m_lfdData->memberRoles.find((*itr1)->charGuid)) != m_lfdData->memberRoles.end())
                        data << itr->second;
                    else data << uint8(0);
                    data << uint8(m_lfdData->encounterMask == 0xFF ? 2 : 0) << uint32(m_lfdData->dungeonId) << uint8(0);
                } else data << uint8(0); // No roles

                // Group guid
                data << WoWGuid(MAKE_NEW_GUID(GetID(), 0, HIGHGUID_TYPE_GROUP));
                data << uint32(0); // Update counter
                data << uint32(m_MemberCount-1);    // we don't include self
                for( j = 0; j < m_SubGroupCount; j++ )
                {
                    sg2 = m_SubGroups[j];

                    if( sg2 != NULL)
                    {
                        for( itr2 = sg2->GetGroupMembersBegin(); itr2 != sg2->GetGroupMembersEnd(); itr2++ )
                        {
                            if( (*itr1) == (*itr2) )
                                continue;

                            // should never happen but just in case
                            if( (*itr2) == NULL )
                                continue;

                            data << (*itr2)->charName << (*itr2)->charGuid;
                            data << uint8( (*itr2)->m_loggedInPlayer ? 1 : 0);
                            data << uint8( sg2->GetID() );

                            flags = 0;
                            if( (*itr2) == m_assistantLeader )
                                flags |= 1;
                            if( (*itr2) == m_mainTank )
                                flags |= 2;
                            if( (*itr2) == m_mainAssist )
                                flags |= 4;

                            data << flags;
                            Loki::AssocVector<WoWGuid, uint8>::iterator itr;
                            if(m_lfdData && ((itr = m_lfdData->memberRoles.find((*itr1)->charGuid)) != m_lfdData->memberRoles.end()))
                                data << itr->second;
                            else data << uint8(0);
                        }
                    }
                }

                data << uint64( m_Leader ? m_Leader->charGuid : 0 );
                if(m_MemberCount > 1)
                {
                    data << uint8( m_LootMethod );
                    data << uint64( m_Looter ? m_Looter->charGuid : 0);
                    data << uint8( m_LootThreshold );
                    data << uint8( m_difficulty );      // 5 Normal/Heroic.
                    data << uint8( m_raiddifficulty );  // 10/25 man.
                    data << uint8(0); // Flex diff
                }
                (*itr1)->m_loggedInPlayer->PushPacket( &data );
            }
        }
    }

    if( m_dirty )
    {
        m_dirty = false;
        SaveToDB();
    }

    m_groupLock.Release();
}

void Group::Disband()
{
    m_groupLock.Acquire();
    m_updateblock=true;

    if(m_isqueued)
    {
        m_isqueued=false;
        WorldPacket data;
        sChatHandler.FillSystemMessageData(&data, "A change was made to your group. Removing the arena queue.");
        SendPacketToAll(&data);
    }

    for(uint8 i = 0; i < m_SubGroupCount; i++)
        if(SubGroup *sg = m_SubGroups[i])
            sg->Disband();

    m_groupLock.Release();
    CharacterDatabase.Execute("DELETE FROM groups WHERE group_id = %u", m_Id);
    delete this;    // destroy ourselves, the destructor removes from eventmgr and objectmgr.
}

void SubGroup::Disband()
{
    WorldPacket data(SMSG_GROUP_DESTROYED, 1), data2(SMSG_PARTY_COMMAND_RESULT, 12);
    data2 << uint32(2) << uint8(0) << uint32(0);    // you leave the group

    GroupMembersSet::iterator itr = m_GroupMembers.begin();
    GroupMembersSet::iterator it2;
    for(; itr != m_GroupMembers.end();)
    {
        if( (*itr) != NULL )
        {
            if( (*itr)->m_loggedInPlayer )
            {
                Player* plr = (*itr)->m_loggedInPlayer;
                if( plr->GetSession() != NULL )
                {
                    plr->PushPacket(&data);
                    plr->PushPacket(&data2);
                    //are we in an instance?
                    if( plr->GetGroup() && plr->GetInstanceID())
                    {
                        sWorldMgr.PlayerLeftGroup(plr->GetGroup(),plr);
                    }
                    //clear the raid screen
                    GetParent()->SendNullUpdate(plr);
                }
            }

            (*itr)->m_Group = NULL;
            (*itr)->subGroup = -1;
        }

        m_Parent->m_MemberCount--;
        it2 = itr++;

        m_GroupMembers.erase(it2);
    }

    m_Parent->m_SubGroups[m_Id] = NULL;
    delete this;
}

Player* Group::FindFirstPlayer()
{
    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();

    for( uint32 i = 0; i < m_SubGroupCount; i++ )
    {
        if( m_SubGroups[i] != NULL )
        {
            for( itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++ )
            {
                if( (*itr) != NULL )
                {
                    if( (*itr)->m_loggedInPlayer != NULL )
                    {
                        m_groupLock.Release();
                        return (*itr)->m_loggedInPlayer;
                    }
                }
            }
        }
    }

    m_groupLock.Release();
    return NULL;
}

void Group::RemovePlayer(PlayerInfo * info)
{
    WorldPacket data(50);
    Player* pPlayer = info->m_loggedInPlayer;

    m_isqueued=false;

    m_groupLock.Acquire();
    SubGroup *sg=NULL;
    if(info->subGroup >= 0 && info->subGroup <= 8)
        sg = m_SubGroups[info->subGroup];

    if(sg == NULL || sg->m_GroupMembers.find(info) == sg->m_GroupMembers.end())
    {
        for(uint32 i = 0; i < m_SubGroupCount; i++)
        {
            if(m_SubGroups[i] != NULL)
            {
                if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
                {
                    sg = m_SubGroups[i];
                    break;
                }
            }
        }
    }

    info->m_Group=NULL;
    info->subGroup=-1;

    if(sg == NULL)
    {
        m_groupLock.Release();
        return;
    }

    m_dirty=true;
    sg->RemovePlayer(info);
    --m_MemberCount;

    m_groupLock.Release();

    if( pPlayer != NULL )
    {
        if( pPlayer->GetSession() != NULL )
        {
            SendNullUpdate( pPlayer );

            data.SetOpcode( SMSG_GROUP_DESTROYED );
            pPlayer->PushPacket( &data );

            data.Initialize( SMSG_PARTY_COMMAND_RESULT );
            data << uint32(2) << uint8(0) << uint32(0);  // you leave the group
            pPlayer->PushPacket( &data );
        }

        //Remove some party auras.
        pPlayer->m_AuraInterface.RemoveAllAreaAuras(pPlayer->GetGUID());
    }

    if(m_MemberCount < 2)
    {
        // disband the group, except battleground groups.
        if(!(m_groupFlags & GROUP_FLAG_DONT_DISBAND_WITH_NO_MEMBERS))
        {
            Disband();
            return;
        }
    }

    /* eek! ;P */
    Player* newPlayer = NULL;
    if(m_Looter == info)
    {
        if( newPlayer = FindFirstPlayer() )
            m_Looter = newPlayer->getPlayerInfo();
        else m_Looter = NULL;
    }

    if(m_Leader == info)
        m_Leader = NULL;//don't bother setting a new leader, it will be set during update()

    Update();
}

void Group::ExpandToRaid()
{
    if(m_isqueued)
    {
        m_isqueued=false;
        WorldPacket data;
        sChatHandler.FillSystemMessageData(&data, "A change was made to your group. Removing the arena queue.");
        SendPacketToAll(&data);
    }
    // Very simple ;)

    m_groupLock.Acquire();
    m_SubGroupCount = 8;

    for(uint8 i = 1; i < m_SubGroupCount; i++)
        m_SubGroups[i] = new SubGroup(this, i);

    m_GroupType = GROUP_TYPE_RAID;
    m_dirty=true;
    Update();
    m_groupLock.Release();
}

void Group::SetLooter(Player* pPlayer, uint8 method, uint16 threshold)
{
    m_LootMethod = method;
    m_Looter = pPlayer ? pPlayer->getPlayerInfo() : NULL;
    m_LootThreshold  = threshold;
    m_dirty = true;

    Update();
}

void Group::AddMemberTargets(SpellTargetClass *spell, uint32 effIndex, bool subGroupOnly, Player *plr, uint32 TargetType, float range, uint32 maxTargets, uint32 classMask)
{
    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();
    uint32 min = subGroupOnly ? plr->GetSubGroup() : 0, max = subGroupOnly ? plr->GetSubGroup() : m_SubGroupCount;
    for(uint8 i = min; i < max; i++)
    {
        for(itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++)
        {
            Player *target = (*itr)->m_loggedInPlayer;
            if(target == NULL || target == plr)
                continue;
            if(target->GetMapInstance() != plr->GetMapInstance())
                continue;
            if(classMask && (target->getClassMask() & classMask) == 0)
                continue;
            if(!plr->isInRange(target, range))
                continue;
            spell->AddTarget(effIndex, TargetType, target);
        }
    }

    m_groupLock.Release();
}

void Group::SendPacketToAllButOne(WorldPacket *packet, Player* pSkipTarget)
{
    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();
    for(uint8 i = 0; i < m_SubGroupCount; i++)
    {
        for(itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++)
        {
            if((*itr)->m_loggedInPlayer != NULL && (*itr)->m_loggedInPlayer != pSkipTarget)
                (*itr)->m_loggedInPlayer->PushPacket(packet);
        }
    }

    m_groupLock.Release();
}

void Group::OutPacketToAllButOne(uint16 op, uint16 len, const void* data, Player* pSkipTarget)
{
    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();
    for(uint8 i = 0; i < m_SubGroupCount; i++)
    {
        for(itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++)
        {
            if((*itr)->m_loggedInPlayer != NULL && (*itr)->m_loggedInPlayer != pSkipTarget)
                (*itr)->m_loggedInPlayer->PushData( op, len, data );
        }
    }

    m_groupLock.Release();
}

bool Group::HasMember(Player* pPlayer)
{
    if( !pPlayer )
        return false;

    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();

    for( uint32 i = 0; i < m_SubGroupCount; i++ )
    {
        if( m_SubGroups[i] != NULL )
        {
            if( m_SubGroups[i]->m_GroupMembers.find( pPlayer->getPlayerInfo() ) != m_SubGroups[i]->m_GroupMembers.end() )
            {
                m_groupLock.Release();
                return true;
            }
        }
    }

    m_groupLock.Release();
    return false;
}

bool Group::HasMember(PlayerInfo * info)
{
//  GroupMembersSet::iterator itr;
    uint32 i = 0;

    m_groupLock.Acquire();

    for(; i < m_SubGroupCount; i++)
    {
        if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
        {
            m_groupLock.Release();
            return true;
        }
    }

    m_groupLock.Release();
    return false;
}

uint32 Group::GetOnlineMemberCount()
{
    GroupMembersSet::iterator itr;
    uint32 i = 0;
    uint32 count = 0;

    m_groupLock.Acquire();
    for(; i < m_SubGroupCount; i++)
    {
        if(m_SubGroups[i])
        {
            for(itr = m_SubGroups[i]->m_GroupMembers.begin(); itr != m_SubGroups[i]->m_GroupMembers.end(); itr++)
            {
                if((*itr)->m_loggedInPlayer)
                    count++;
            }
        }
    }
    m_groupLock.Release();

    return count;
}

void Group::MovePlayer(PlayerInfo *info, uint8 subgroup)
{
    if( subgroup >= m_SubGroupCount || m_SubGroupCount >8 )
        return;

    m_groupLock.Acquire();

    if(m_SubGroups[subgroup]->IsFull())
    {
        m_groupLock.Release();
        return;
    }

    SubGroup *sg=NULL;
    if(info->subGroup > 0)
        sg = m_SubGroups[info->subGroup];

    //first subgroup don't exists or player is not a member of it, search other subgroups
    if(sg == NULL || sg->m_GroupMembers.find(info) == sg->m_GroupMembers.end())
    {
        for(uint32 i = 0; i < m_SubGroupCount; i++)
        {
            if(m_SubGroups[i] != NULL)
            {
                if(m_SubGroups[i]->m_GroupMembers.find(info) != m_SubGroups[i]->m_GroupMembers.end())
                {
                    //we are in this subgroup
                    sg = m_SubGroups[i];
                    break;
                }
            }
        }
    }

    if(sg == NULL)
    {
        m_groupLock.Release();
        return;
    }

    sg->RemovePlayer(info);

    // Grab the new group, and insert
    sg = m_SubGroups[subgroup];
    if(!sg->AddPlayer(info))
    {
        RemovePlayer(info);
        info->m_Group=NULL;
    }
    else
    {
        info->subGroup=(int8)sg->GetID();
        info->m_Group=this;
    }

    Update();
    m_groupLock.Release();
}

void Group::SendNullUpdate( Player* pPlayer )
{
    // this packet is 28 bytes long.        // AS OF 3.3.0.a
    uint8 buffer[28];
    memset(buffer, 0, 28);
    pPlayer->PushData( SMSG_GROUP_LIST, 28, buffer );
}

void Group::SendLFGLockInfo(Player *pPlayer)
{
    // Long and tiring code for writing dungeon locks
    m_groupLock.Acquire();
    uint8 count = 0;
    WorldPacket data(SMSG_LFG_PARTY_INFO, 200);
    data << count;
    for(uint32 i = 0; i < m_SubGroupCount; i++)
    {
        if(m_SubGroups[i] == NULL)
            continue;

        for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd();)
        {
            Player *plr = (*itr)->m_loggedInPlayer;
            ++itr;
            if(plr == NULL)
                continue;

            ++count;
            data << plr->GetGUID();
            sGroupFinder.BuildPlayerLockInfo(plr, &data);
        }
    }
    data.put<uint8>(0, count);
    m_groupLock.Release();
    pPlayer->PushPacket(&data);
}

bool Group::QualifiesForGuildXP(Creature *cVictim)
{
    // Only boss kills allow guild XP gain
    if(!cVictim->isBoss())
        return false;

    return false;
}

// player is object class becouse its called from unit class
void Group::SendPartyKillLog( WorldObject* player, WorldObject* Unit )
{
    if( !player || !Unit || !HasMember( castPtr<Player>( player ) ) )
        return;

    WorldPacket data( SMSG_PARTYKILLLOG, 16 );
    data << player->GetGUID();
    data << Unit->GetGUID();
    SendPacketToAll( &data );
}

void Group::SaveToDB()
{
    if(m_groupFlags & GROUP_FLAG_DONT_DISBAND_WITH_NO_MEMBERS)  /* don't save bg groups */
        return;

    std::stringstream ss;
    //uint32 i = 0;
    uint32 fillers = 8 - m_SubGroupCount;

    ss << "REPLACE INTO groups VALUES("
        << m_Id << ","
        << uint32(m_GroupType) << ","
        << uint32(m_SubGroupCount) << ","
        << uint32(m_LootMethod) << ","
        << uint32(m_LootThreshold) << ","
        << uint32(m_difficulty) << ","
        << uint32(m_raiddifficulty) << ",";

    if(m_assistantLeader)
        ss << m_assistantLeader->charGuid.getLow() << ",";
    else
        ss << "0,";

    if(m_mainTank)
        ss << m_mainTank->charGuid.getLow() << ",";
    else
        ss << "0,";

    if(m_mainAssist)
        ss << m_mainAssist->charGuid.getLow() << ",";
    else
        ss << "0,";

    for(uint32 i = 0; i < m_SubGroupCount; i++)
    {
        uint32 j = 0;
        if (m_SubGroups[i])
        {
            for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); j<5 && itr != m_SubGroups[i]->GetGroupMembersEnd(); j++, itr++)
            {
                ss << (*itr)->charGuid.getLow() << ",";
            }
        }

        for(; j < 5; j++)
            ss << "0,";
    }

    for(uint32 i = 0; i < fillers; i++)
        ss << "0,0,0,0,0,";

    ss << (uint32)UNIXTIME << ")";
    /*sLog.printf("==%s==\n", ss.str().c_str());*/
    CharacterDatabase.Execute(ss.str().c_str());
}

void Group::LoadFromDB(Field *fields)
{
#define LOAD_ASSISTANT(__i, __d) g = fields[__i].GetUInt32(); if(g != 0) { __d = objmgr.GetPlayerInfo(g); }

    uint32 g;
    m_updateblock=true;
    m_Id = fields[0].GetUInt32();

    ObjectMgr::getSingleton().AddGroup( this );

    m_GroupType = fields[1].GetUInt8();
    m_SubGroupCount = fields[2].GetUInt8();
    m_LootMethod = fields[3].GetUInt8();
    m_LootThreshold = fields[4].GetUInt8();
    m_difficulty = fields[5].GetUInt8();
    m_raiddifficulty = fields[6].GetUInt8();

    LOAD_ASSISTANT(7, m_assistantLeader);
    LOAD_ASSISTANT(8, m_mainTank);
    LOAD_ASSISTANT(9, m_mainAssist);

    // create groups
    for(int i = 1; i < m_SubGroupCount; i++)
        m_SubGroups[i] = new SubGroup(this, i);

    // assign players into groups
    for(int i = 0; i < m_SubGroupCount; i++)
    {
        for(int j = 0; j < 5; ++j)
        {
            uint32 guid = fields[9 + (i*5) + j].GetUInt32();
            if( guid == 0 )
                continue;

            PlayerInfo * inf = objmgr.GetPlayerInfo(guid);
            if(inf == NULL)
                continue;

            AddMember(inf);
            m_dirty=false;
        }
    }
    m_updateblock=false;
}

void Group::UpdateOutOfRangePlayer(PlayerInfo *info, uint32 Flags, bool Distribute, WorldPacket * Packet)
{
    WorldPacket * data = Packet;
    if(!data)
        data = new WorldPacket(SMSG_PARTY_MEMBER_STATS, 500);
    else data->Initialize(SMSG_PARTY_MEMBER_STATS);

    Player *pPlayer = info->m_loggedInPlayer;
    if(pPlayer == NULL || pPlayer->GetSession() == NULL)
    {
        *data << info->charGuid.asPacked();
        *data << uint32(GROUP_UPDATE_FLAG_ONLINE);
        *data << uint16(0);
    }
    else if(Flags == GROUP_UPDATE_LOCATION)
    {
        *data << info->charGuid.asPacked();
        *data << uint32(GROUP_UPDATE_LOCATION);
        *data << uint16(pPlayer->GetZoneId()) << uint16(pPlayer->GetPositionX()) << uint16(pPlayer->GetPositionY());
    }
    else
    {
        if(pPlayer->getPowerType() != POWER_TYPE_MANA)
            Flags |= GROUP_UPDATE_FLAG_POWER_TYPE;

        if( Flags & GROUP_UPDATE_FLAG_POWER_TYPE )
            Flags |= (GROUP_UPDATE_FLAG_POWER | GROUP_UPDATE_FLAG_MAXPOWER);

        if( Flags & GROUP_UPDATE_FLAG_PET_POWER_TYPE )
            Flags |= (GROUP_UPDATE_FLAG_PET_POWER | GROUP_UPDATE_FLAG_PET_MAXPOWER);

        *data << pPlayer->GetGUID().asPacked();
        *data << Flags;

        if(Flags & GROUP_UPDATE_FLAG_ONLINE)
        {
            uint16 member_flags = 0x01;
            if( pPlayer->IsPvPFlagged() )
                member_flags |= 0x02;
            if( pPlayer->hasStateFlag(UF_CORPSE) )
                member_flags |= 0x04;
            else if( pPlayer->isDead() )
                member_flags |= 0x08;
            *data << uint16(member_flags);
        }

        if(Flags & GROUP_UPDATE_FLAG_HEALTH)
            *data << pPlayer->GetUInt32Value(UNIT_FIELD_HEALTH);

        if(Flags & GROUP_UPDATE_FLAG_MAXHEALTH)
            *data << pPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH);

        if(Flags & GROUP_UPDATE_FLAG_POWER_TYPE)
            *data << uint8(pPlayer->getPowerType());

        if(Flags & GROUP_UPDATE_FLAG_POWER)
            *data << uint16(pPlayer->GetPower(pPlayer->getPowerType()));

        if(Flags & GROUP_UPDATE_FLAG_MAXPOWER)
            *data << uint16(pPlayer->GetMaxPower(pPlayer->getPowerType()));

        if(Flags & GROUP_UPDATE_FLAG_LEVEL)
            *data << uint16(pPlayer->getLevel());

        if(Flags & GROUP_UPDATE_FLAG_ZONEID)
            *data << uint16(pPlayer->GetZoneId());

        if(Flags & GROUP_UPDATE_FLAG_STATS)
            *data << uint8(0) << uint8(0); // Masks

        if(Flags & GROUP_UPDATE_FLAG_POSITION)
            *data << uint16(pPlayer->GetPositionX()) << uint16(pPlayer->GetPositionY());

        if(Flags & GROUP_UPDATE_FLAG_PLAYER_AURAS)
            pPlayer->m_AuraInterface.BuildOutOfRangeAuraUpdate(data);

        if (Flags & GROUP_UPDATE_FLAG_PET_GUID)
		    *data << uint64(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_NAME)
		    *data << uint8(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_DISPLAYID)
		    *data << uint16(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_HEALTH)
		    *data << uint32(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_MAXHEALTH)
		    *data << uint32(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
		    *data << uint8(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_POWER)
		    *data << uint16(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_MAXPOWER)
		    *data << uint16(0);

        if (Flags & GROUP_UPDATE_FLAG_PET_AURAS)
		    *data << uint8(0) << uint64(0) << uint32(0);

        if (Flags & GROUP_UPDATE_FLAG_VEHICLE_ENTRY)
            *data << int32(0);

        if (Flags & GROUP_UPDATE_FLAG_PHASE)
        {
            *data << uint32(8);
            *data << uint32(0);
        }
    }

    if(Distribute)
    {
        Player* plr;
        m_groupLock.Acquire();
        for(uint32 i = 0; i < m_SubGroupCount; i++)
        {
            if(m_SubGroups[i]==NULL)
                continue;

            for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd();)
            {
                plr = (*itr)->m_loggedInPlayer;
                ++itr;
                if((*itr) == info || plr == NULL)
                    continue;
                if(plr->IsVisible(info->charGuid))
                    continue;
                plr->PushPacket(data);
            }
        }
        m_groupLock.Release();
    }

    if(!Packet)
        delete data;
}

void Group::UpdateAllOutOfRangePlayersFor(PlayerInfo *info)
{
    if(m_SubGroupCount>8)
        return;

    WorldPacket data; /* tell the other players about us */
    UpdateOutOfRangePlayer(info, GROUP_UPDATE_FULL, true, &data);

    /* tell us any other players we don't know about */
    Player* plr;

    m_groupLock.Acquire();
    for(uint32 i = 0; i < m_SubGroupCount; i++)
    {
        if(m_SubGroups[i]==NULL)
            continue;

        for(GroupMembersSet::iterator itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++)
        {
            if((*itr) == info || info->m_loggedInPlayer == NULL)
                continue;

            UpdateOutOfRangePlayer((*itr), GROUP_UPDATE_FULL, false, &data);
            info->m_loggedInPlayer->PushPacket(&data);
        }
    }

    m_groupLock.Release();
}

void Group::HandleUpdateFieldChange(uint32 Index, Player* pPlayer)
{
    uint32 Flags = 0;
    if( m_dirty )//sth has corrupted this, workaround
        return;

    m_groupLock.Acquire();
    switch(Index)
    {
    case UNIT_FIELD_HEALTH:
        Flags = GROUP_UPDATE_FLAG_HEALTH;
        break;

    case UNIT_FIELD_MAXHEALTH:
        Flags = GROUP_UPDATE_FLAG_MAXHEALTH;
        break;

    case UNIT_FIELD_POWERS:
    case UNIT_FIELD_POWERS+1:
    case UNIT_FIELD_POWERS+2:
    case UNIT_FIELD_POWERS+3:
    case UNIT_FIELD_POWERS+4:
        Flags = GROUP_UPDATE_FLAG_POWER;
        break;

    case UNIT_FIELD_MAXPOWERS:
    case UNIT_FIELD_MAXPOWERS+1:
    case UNIT_FIELD_MAXPOWERS+2:
    case UNIT_FIELD_MAXPOWERS+3:
    case UNIT_FIELD_MAXPOWERS+4:
        Flags = GROUP_UPDATE_FLAG_MAXPOWER;
        break;

    case UNIT_FIELD_LEVEL:
        Flags = GROUP_UPDATE_FLAG_LEVEL;
        break;
    default:
        break;
    }

    if( Flags != 0 )
        UpdateOutOfRangePlayer( pPlayer->getPlayerInfo(), Flags, true, NULL );

    m_groupLock.Release();
}

void Group::HandlePartialChange(uint32 Type, Player* pPlayer)
{
    uint32 Flags = 0;
    m_groupLock.Acquire();

    switch(Type)
    {
    case PARTY_UPDATE_FLAG_LOCATION:
        Flags = GROUP_UPDATE_LOCATION;
        break;
    }

    if(Flags)
        UpdateOutOfRangePlayer(pPlayer->getPlayerInfo(), Flags, true, 0);

    m_groupLock.Release();
}

void Group::FillLFDMembers(Loki::AssocVector<PlayerInfo*, uint8> *members)
{
    if(m_lfdData == NULL)
        return;

    PlayerInfo *leader = NULL;
    for(Loki::AssocVector<PlayerInfo*, uint8>::iterator itr = members->begin(); itr != members->end(); itr++)
    {
        AddMember(itr->first, -1, true);
        m_lfdData->memberRoles.insert(std::make_pair(itr->first->charGuid, itr->second));
        if(itr->second & ROLEMASK_TANK)
        {
            m_mainTank = itr->first;
            if(leader == NULL)
                leader = itr->first;
        }
        else if(itr->second & ROLEMASK_LEADER)
            leader = itr->first;
    }

    m_Leader = leader;
    SaveToDB();
}

void WorldSession::HandlePartyMemberStatsOpcode(WorldPacket & recv_data)
{
    CHECK_INWORLD_RETURN();
    uint64 guid;
    recv_data >> guid;

    Player * plr = _player->GetMapInstance()->GetPlayer((uint32)guid);
    if(!_player->GetGroup())
        return;

    if( plr )
    {
        if(!_player->GetGroup()->HasMember(plr))
            return;         // invalid player
    }
    else
    {
        plr = objmgr.GetPlayer( (uint32)guid );
        if( !plr )
        {
            WorldPacket data(SMSG_PARTY_MEMBER_STATS_FULL, 3+4+2);
            data << uint8(0);
            data << WoWGuid(guid);
            data << uint32(GROUP_UPDATE_FLAG_ONLINE);
            data << uint16(0);
            SendPacket(&data);
            return;
        }
    }

    if(_player->IsVisible(plr))
        return;

    WorldPacket data(200);
    _player->GetGroup()->UpdateOutOfRangePlayer(plr->getPlayerInfo(), GROUP_UPDATE_FULL, false, &data);
    data.SetOpcode(SMSG_PARTY_MEMBER_STATS_FULL);
    SendPacket(&data);
}

void Group::SetMainAssist(PlayerInfo * pMember)
{
    if(m_mainAssist==pMember)
        return;

    m_mainAssist = pMember;
    m_dirty = true;
    Update();
}

void Group::SetMainTank(PlayerInfo * pMember)
{
    if(m_mainTank==pMember)
        return;

    m_mainTank=pMember;
    m_dirty = true;
    Update();
}

void Group::SetAssistantLeader(PlayerInfo * pMember)
{
    if(m_assistantLeader == pMember)
        return;

    m_assistantLeader = pMember;
    m_dirty = true;
    Update();
}

bool Group::IsGroupFinderInstance(uint32 mapId)
{
    LFGDungeonsEntry *entry;
    if(m_lfdData && (entry = dbcLFGDungeons.LookupEntry(m_lfdData->dungeonId)))
        return entry->mapId == mapId;
    return false;
}

bool Group::HasAcceptableDisenchanters(int32 requiredskill)
{
    if(requiredskill < 0)
        return false;

    Player* plr = NULL;
    GroupMembersSet::iterator itr;
    m_groupLock.Acquire();

    for( uint32 i = 0; i < m_SubGroupCount; i++ )
    {
        if( m_SubGroups[i] != NULL )
        {
            for( itr = m_SubGroups[i]->GetGroupMembersBegin(); itr != m_SubGroups[i]->GetGroupMembersEnd(); itr++ )
            {
                if( (*itr) != NULL && (*itr)->m_loggedInPlayer)
                {
                    plr = (*itr)->m_loggedInPlayer;
                    if(plr->HasSkillLine(333) && (plr->getSkillLineVal(333, true) > uint32(requiredskill)))
                        return true;
                }
            }
        }
    }

    m_groupLock.Release();
    return false;
}
