/***
 * Demonstrike Core
 */

#include "StdAfx.h"

static const uint32 TeamCountToId[6] = {
    0,                      // 0
    0,                      // 1
    0,                      // 2
    ARENA_TEAM_TYPE_2V2,    // 3
    ARENA_TEAM_TYPE_3V3,    // 4
    ARENA_TEAM_TYPE_5V5,    // 5
};

static const uint32 IdToTeamCount[6] = {
    3,
    4,
    5,
    0,
    0,
    0,
};

ArenaTeam::ArenaTeam(uint32 Type, uint32 Id)
{
    m_id = Id;
    m_type = Type;
    AllocateSlots(Type);
    m_leader = 0;
    m_emblemStyle = 0;
    m_emblemColour = 0;
    m_borderColour = 0;
    m_borderStyle = 0;
    m_backgroundColour = 0;
    m_stat_rating = 0;
    m_stat_gamesplayedweek = 0;
    m_stat_gamesplayedseason = 0;
    m_stat_gameswonseason = 0;
    m_stat_gameswonweek = 0;
    m_stat_ranking = 0;
}

ArenaTeam::ArenaTeam(Field * f)
{
    WoWGuid guid;
    uint32 z = 0, i, lowGuid;
    const char * data;
    int ret;

    m_id = f[z++].GetUInt32();
    m_type = f[z++].GetUInt32();
    m_leader = f[z++].GetUInt32();
    m_name = f[z++].GetString();
    m_emblemStyle = f[z++].GetUInt32();
    m_emblemColour = f[z++].GetUInt32();
    m_borderStyle = f[z++].GetUInt32();
    m_borderColour = f[z++].GetUInt32();
    m_backgroundColour = f[z++].GetUInt32();
    m_stat_rating = f[z++].GetUInt32();
    AllocateSlots(m_type);

    m_stat_gamesplayedweek = 0;
    m_stat_gamesplayedseason = 0;
    m_stat_gameswonseason = 0;
    m_stat_gameswonweek = 0;
    sscanf(f[z++].GetString(), "%u %u %u %u", &m_stat_gamesplayedweek, &m_stat_gameswonweek, &m_stat_gamesplayedseason, &m_stat_gameswonseason);

    m_stat_ranking = f[z++].GetUInt32();
    for(i = 0; i < m_slots; i++)
    {
        data = f[z++].GetString();
        ret = sscanf(data, "%u %u %u %u %u %u", &lowGuid, &m_members[i].Played_ThisWeek, &m_members[i].Won_ThisWeek,
            &m_members[i].Played_ThisSeason, &m_members[i].Won_ThisSeason, &m_members[i].PersonalRating);
        if(ret >= 5)
        {
            m_members[i].Info = objmgr.GetPlayerInfo(guid = MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_TYPE_PLAYER));
            if(m_members[i].Info)
            {
                m_members[i].Info->arenaTeam[m_type] = this;
                ++m_memberCount;
            }

            if (ret == 5)
            {
                // In case PersonalRating is not in the string just set the rating to the team rating
                m_members[i].PersonalRating = m_stat_rating;
            }
        } else m_members[i].Info = NULL;
    }
}

void ArenaTeam::SendPacket(WorldPacket * data)
{
    PlayerInfo * info;
    for(uint32 i = 0; i < m_memberCount; i++)
    {
        info = m_members[i].Info;
        if(info && info->m_loggedInPlayer)
            info->m_loggedInPlayer->GetSession()->SendPacket(data);
    }
}

void ArenaTeam::Destroy()
{
    char buffer[1024];
    std::vector<PlayerInfo*> tokill;
    tokill.reserve(m_memberCount);
    snprintf(buffer,1024, "The arena team, '%s', disbanded.", m_name.c_str());
    WorldPacket data;
    sChatHandler.FillSystemMessageData(&data, buffer);
    SendPacket(&data);

    for(uint8 i = 0; i < m_memberCount; i++)
    { if(m_members[i].Info) tokill.push_back(m_members[i].Info); }

    for(std::vector<PlayerInfo*>::iterator itr = tokill.begin(); itr != tokill.end(); itr++)
        RemoveMember(*itr);

    objmgr.RemoveArenaTeam(this);
    delete this;
}

bool ArenaTeam::AddMember(PlayerInfo * info)
{
    uint32 base_field;
    Player* plr = info->m_loggedInPlayer;
    if(m_memberCount >= m_slots)
        return false;

    memset(&m_members[m_memberCount], 0, sizeof(ArenaTeamMember));
    m_members[m_memberCount].PersonalRating = 1500;
    m_members[m_memberCount++].Info = info;
    SaveToDB();

    if(plr!=NULL)
    {
        base_field = (m_type*6) + PLAYER_FIELD_ARENA_TEAM_INFO_1_1;
        plr->SetUInt32Value(base_field, m_id);
        plr->SetUInt32Value(base_field+1, m_leader.getLow());
        plr->GetSession()->SystemMessage("You are now a member of the arena team, '%s'.", m_name.c_str());
    }

    info->arenaTeam[m_type] = this;
    return true;
}

bool ArenaTeam::RemoveMember(PlayerInfo * info)
{
    for(uint32 i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info == info)
        {
            /* memcpy all the blocks in front of him back (so we only loop O(members) instead of O(slots) */
            for(uint32 j = (i+1); j < m_memberCount; ++j)
                memcpy(&m_members[j-1], &m_members[j], sizeof(ArenaTeamMember));

            --m_memberCount;
            SaveToDB();

            if(info->m_loggedInPlayer)
                info->m_loggedInPlayer->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (m_type*6), 0);

            info->arenaTeam[m_type] = NULL;
            return true;
        }
    }

    return false;
}

void ArenaTeam::Stat(WorldPacket & data)
{
    data.Initialize(SMSG_ARENA_TEAM_STATS);
    data << m_id;
    data << m_stat_rating;
    data << m_stat_gamesplayedweek;
    data << m_stat_gameswonweek;
    data << m_stat_gamesplayedseason;
    data << m_stat_gameswonseason;
    data << m_stat_ranking;
}

void ArenaTeam::Query(WorldPacket & data)
{
    data.Initialize(SMSG_ARENA_TEAM_QUERY_RESPONSE);
    data << m_id;
    data << m_name;
    data << GetPlayersPerTeam();
    data << m_emblemColour;
    data << m_emblemStyle;
    data << m_borderColour;
    data << m_borderStyle;
    data << m_backgroundColour;
}

void ArenaTeam::Roster(WorldPacket & data)
{
    data.Initialize(SMSG_ARENA_TEAM_ROSTER);
    data.reserve(m_memberCount * 81 + 9);
    data << m_id;
    data << uint8(0); //3.0.8, played? Specifies whether we are sending 2 extra floats for each member
    data << m_memberCount;
    data << GetPlayersPerTeam();

    for(uint32 i = 0; i < m_memberCount; i++)
    {
        PlayerInfo* info = m_members[i].Info;
        // TODO : burlex figure out why this became null
        if( info != NULL )
        {
            data << uint64(info->guid);
            data << uint8( (info->m_loggedInPlayer != NULL) ? 1 : 0 );
            data << info->name;
            data << uint32( m_members[i].Info->guid == m_leader ? 0 : 1); // Unk
            data << uint8( info->lastLevel );
            data << uint8( info->_class );
            data << m_members[i].Played_ThisWeek;
            data << m_members[i].Won_ThisWeek;
            data << m_members[i].Played_ThisSeason;
            data << m_members[i].Won_ThisSeason;
            data << m_members[i].PersonalRating;
            // if uint8 flag at the beginning is not 0, 2 unknown floats are expected here
        }
    }
}

void ArenaTeam::SaveToDB()
{
    std::stringstream ss;
    uint32 i;
    ss << "REPLACE INTO arenateams VALUES("
        << m_id << ","
        << m_type << ","
        << m_leader.raw() << ",'"
        << m_name << "',"
        << m_emblemStyle << ","
        << m_emblemColour << ","
        << m_borderStyle << ","
        << m_borderColour << ","
        << m_backgroundColour << ","
        << m_stat_rating << ",'"
        << m_stat_gamesplayedweek << " " << m_stat_gameswonweek << " "
        << m_stat_gamesplayedseason << " " << m_stat_gameswonseason << "',"
        << m_stat_ranking;

    for(i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info)
        {
            ss << ",'" << m_members[i].Info->guid.raw() << " " << m_members[i].Played_ThisWeek << " "
                << m_members[i].Won_ThisWeek << " " << m_members[i].Played_ThisSeason << " "
                << m_members[i].Won_ThisSeason << " " << m_members[i].PersonalRating << "'";
        }
        else
        {
            ss << ",'0 0 0 0 0 0'";
        }
    }

    for(; i < 10; i++)
    {
        ss << ",'0 0 0 0 0 0'";
    }

    ss << ")";
    CharacterDatabase.Execute(ss.str().c_str());
}

bool ArenaTeam::HasMember(WoWGuid guid)
{
    for(uint32 i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info && m_members[i].Info->guid == guid)
            return true;
    }
    return false;
}

void ArenaTeam::SetLeader(PlayerInfo * info)
{
    WoWGuid old_leader = m_leader;
    m_leader = info->guid;
    char buffer[1024];
    snprintf(buffer, 1024,"%s is now the captain of the arena team, '%s'.", info->name, m_name.c_str());
    WorldPacket data;
    sChatHandler.FillSystemMessageData(&data, buffer);
    SendPacket(&data);

    /* set the fields */
    for(uint32 i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info == info)       /* new leader */
        {
            if(m_members[i].Info->m_loggedInPlayer)
                m_members[i].Info->m_loggedInPlayer->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (m_type*6) + 1, 0);
        }
        else if(m_members[i].Info->guid == old_leader)
        {
            if(m_members[i].Info->m_loggedInPlayer)
                m_members[i].Info->m_loggedInPlayer->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (m_type*6) + 1, 1);
        }
    }

    SaveToDB();
}

ArenaTeamMember * ArenaTeam::GetMember(PlayerInfo * info)
{
    for(uint32 i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info == info)
            return &m_members[i];
    }
    return NULL;
}

ArenaTeamMember* ArenaTeam::GetMemberByGuid(WoWGuid guid)
{
    if(!m_memberCount) // If we don't have members, whats the point?
        return NULL;

    for(uint32 i = 0; i < m_memberCount; i++)
    {
        if(m_members[i].Info && m_members[i].Info->guid == guid)
            return &m_members[i];
    }
    return NULL;
}

void WorldSession::HandleArenaTeamRosterOpcode(WorldPacket & recv_data)
{
    uint8 slot;
    uint32 teamId;
    ArenaTeam * team;
    recv_data >> teamId;
    team = objmgr.GetArenaTeamById(teamId);
    if(team)
    {
        slot = TeamCountToId[team->m_type];
        WorldPacket data(1000);
        team->Roster(data);
        SendPacket(&data);
    }
}

void WorldSession::HandleArenaTeamQueryOpcode(WorldPacket & recv_data)
{
    ArenaTeam * team;
    uint32 team_id;
    recv_data >> team_id;

    team = objmgr.GetArenaTeamById(team_id);
    if(team != NULL)
    {
        WorldPacket data(1000);
        team->Query(data);
        SendPacket(&data);

        team->Stat(data);
        SendPacket(&data);
    }
}

void WorldSession::HandleArenaTeamAddMemberOpcode(WorldPacket & recv_data)
{
    uint32 teamId;
    std::string player_name;
    recv_data >> teamId >> player_name;

    ArenaTeam * pTeam = objmgr.GetArenaTeamById(teamId);
    if( pTeam == NULL )
        return;
    else if(!pTeam->HasMember(GetPlayer()->GetGUID()))
    {
        GetPlayer()->SoftDisconnect();
        return;
    }

    Player* plr = objmgr.GetPlayer(player_name.c_str(), false);
    if(plr == NULL)
    {
        SystemMessage("Player `%s` is non-existent or not online.", player_name.c_str());
        return;
    }

    if(pTeam->m_leader != _player->GetGUID())
    {
        SystemMessage("You are not the captain of this arena team.");
        return;
    }

    if(plr->getLevel() < 70)
    {
        SystemMessage("Player must be level 70 to join an arena team.");
        return;
    }

    if(plr->m_playerInfo->arenaTeam[pTeam->m_type] != NULL)
    {
        SystemMessage("That player is already in an arena team of this type.");
        return;
    }

    if(plr->m_arenateaminviteguid != 0)
    {
        SystemMessage("That player is already invited to an arena team");
        return;
    }

    if(plr->GetTeam() != _player->GetTeam() && !HasGMPermissions())
    {
        SystemMessage("That player is a member of a different faction.");
        return;
    }

    plr->m_arenateaminviteguid = _player->m_playerInfo->arenaTeam[pTeam->m_type]->m_id;
    WorldPacket data(SMSG_ARENA_TEAM_INVITE, 40);
    data << _player->GetName();
    data << _player->m_playerInfo->arenaTeam[pTeam->m_type]->m_name;
    plr->GetSession()->SendPacket(&data);
}

void WorldSession::HandleArenaTeamRemoveMemberOpcode(WorldPacket & recv_data)
{
    ArenaTeam * team;
    uint8 slot;
    uint32 teamId;
    std::string name;
    PlayerInfo * inf;
    recv_data >> teamId >> name;

    team = objmgr.GetArenaTeamById(teamId);
    if(!team)
    {
        GetPlayer()->SoftDisconnect();
        return;
    }

    slot = team->m_type;

    if( (team = _player->m_playerInfo->arenaTeam[slot]) == NULL )
    {
        SystemMessage("You are not in an arena team of this type.");
        return;
    }

    if(team->m_leader != _player->GetGUID())
    {
        SystemMessage("You are not the leader of this team.");
        return;
    }

    if( (inf = objmgr.GetPlayerInfoByName(name.c_str())) == NULL )
    {
        SystemMessage("That player cannot be found.");
        return;
    }

    if(!team->HasMember(inf->guid))
    {
        SystemMessage("That player is not in your arena team.");
        return;
    }

    if(team->RemoveMember(inf))
    {
        char buffer[1024];
        snprintf(buffer,1024,"%s was removed from the arena team '%s'.", inf->name, team->m_name.c_str());
        WorldPacket data;
        sChatHandler.FillSystemMessageData(&data, buffer);
        SendPacket(&data);
        SystemMessage("Removed %s from the arena team '%s'.", inf->name, team->m_name.c_str());
    }
}

void WorldSession::HandleArenaTeamInviteAcceptOpcode(WorldPacket & recv_data)
{
    if(_player->m_arenateaminviteguid == 0)
    {
        SystemMessage("You have not been invited into another arena team.");
        return;
    }

    ArenaTeam *team = objmgr.GetArenaTeamById(_player->m_arenateaminviteguid);
    _player->m_arenateaminviteguid=0;
    if(team == 0)
    {
        SystemMessage("That arena team no longer exists.");
        return;
    }

    if(team->m_memberCount >= team->m_slots)
    {
        SystemMessage("That team is now full.");
        return;
    }

    if(_player->m_playerInfo->arenaTeam[team->m_type] != NULL)      /* shouldn't happen */
    {
        SystemMessage("You have already been in an arena team of that size.");
        return;
    }

    if(team->AddMember(_player->m_playerInfo))
    {
        char buffer[1024];
        snprintf(buffer,1024,"%s joined the arena team, '%s'.", _player->GetName(), team->m_name.c_str());
        WorldPacket data;
        sChatHandler.FillSystemMessageData(&data, buffer);
        SendPacket(&data);
    } else SendNotification("Internal error.");
}

void WorldSession::HandleArenaTeamInviteDenyOpcode(WorldPacket & recv_data)
{
    if(_player->m_arenateaminviteguid == 0)
    {
        SystemMessage("You were not invited.");
        return;
    }

    ArenaTeam *team = objmgr.GetArenaTeamById(_player->m_arenateaminviteguid);
    _player->m_arenateaminviteguid=0;
    if(team == NULL)
        return;

    Player* plr = objmgr.GetPlayer(team->m_leader);
    if(plr != NULL)
        plr->GetSession()->SystemMessage("%s denied your arena team invitation for %s.", _player->GetName(), team->m_name.c_str());
}

void WorldSession::HandleArenaTeamLeaveOpcode(WorldPacket & recv_data)
{
    uint32 teamId;
    recv_data >> teamId;

    ArenaTeam *team = objmgr.GetArenaTeamById(teamId);
    if(team == NULL)
    {
        GetPlayer()->SoftDisconnect();
        return;
    }

    if( (team = _player->m_playerInfo->arenaTeam[team->m_type]) == NULL )
    {
        SystemMessage("You are not in an arena team of this type.");
        return;
    }

    if(team->m_leader == _player->GetGUID())
    {
        SystemMessage("You cannot leave the team yet, promote someone else to captain first.");
        return;
    }

    if(team->RemoveMember(_player->m_playerInfo))
    {
        char buffer[1024];
        snprintf(buffer,1024,"%s left the arena team, '%s'.", _player->GetName(), team->m_name.c_str());
        WorldPacket data;
        sChatHandler.FillSystemMessageData(&data, buffer);
        SendPacket(&data);
        SystemMessage("You have left the arena team, '%s'.", team->m_name.c_str());
    }
}

void WorldSession::HandleArenaTeamDisbandOpcode(WorldPacket & recv_data)
{
    uint32 teamId;
    recv_data >> teamId;

    ArenaTeam *team = objmgr.GetArenaTeamById(teamId);
    if(!team)
    {
        GetPlayer()->SoftDisconnect();
        return;
    }

    if( (team = _player->m_playerInfo->arenaTeam[team->m_type]) == NULL )
    {
        SystemMessage("You are not in an arena team of this type.");
        return;
    }

    if(team->m_leader != _player->GetGUID())
    {
        SystemMessage("You aren't the captain of this team.");
        return;
    }

    team->Destroy();
}

void WorldSession::HandleArenaTeamPromoteOpcode(WorldPacket & recv_data)
{
    uint32 teamId;
    uint8 slot;
    std::string name;
    PlayerInfo * inf;
    recv_data >> teamId >> name;

    ArenaTeam *team = objmgr.GetArenaTeamById(teamId);
    if(!team)
    {
        GetPlayer()->SoftDisconnect();
        return;
    }

    slot = team->m_type;

    if( slot >= NUM_ARENA_TEAM_TYPES )
        return;

    if( (team = _player->m_playerInfo->arenaTeam[slot]) == NULL )
    {
        SystemMessage("You are not in an arena team of this type.");
        return;
    }

    if(team->m_leader != _player->GetGUID())
    {
        SystemMessage("You aren't the captain of this team.");
        return;
    }

    if( (inf = objmgr.GetPlayerInfoByName(name.c_str())) == NULL )
    {
        SystemMessage("That player cannot be found.");
        return;
    }

    if(!team->HasMember(inf->guid))
    {
        SystemMessage("That player is not a member of your arena team.");
        return;
    }

    team->SetLeader(inf);
}
