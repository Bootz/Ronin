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

//
// MapCell.cpp
//

#include "StdAfx.h"

MapCell::MapCell()
{
    _forcedActive = false;
}

MapCell::~MapCell()
{
    UnloadCellData(true);
}

void MapCell::Init(uint32 x, uint32 y, uint32 mapid, MapInstance* instance)
{
    _mapData = instance->GetBaseMap();
    _instance = instance;
    _active = false;
    _loaded = false;
    _x = x;
    _y = y;
    _unloadpending=false;
}

void MapCell::AddObject(WorldObject* obj)
{
    Guard guard(cellLock);
    if(obj->IsPlayer())
        m_playerSet[obj->GetGUID()] = obj;
    else
    {
        m_nonPlayerSet[obj->GetGUID()] = obj;
        if(obj->IsCreature())
            m_creatureSet[obj->GetGUID()] = obj;
        else if(obj->IsGameObject())
            m_gameObjectSet[obj->GetGUID()] = obj;
    }
}

void MapCell::RemoveObject(WorldObject* obj)
{
    Guard guard(cellLock);
    m_pendingRemovals.insert(obj->GetGUID());
    _instance->CellRemovalPending(_x, _y);
}

void MapCell::ProcessRemovals()
{
    Guard guard(cellLock);
    while(!m_pendingRemovals.empty())
    {
        WoWGuid guid = *m_pendingRemovals.begin();
        m_pendingRemovals.erase(m_pendingRemovals.begin());
        m_playerSet.erase(guid);
        m_nonPlayerSet.erase(guid);
        m_creatureSet.erase(guid);
        m_gameObjectSet.erase(guid);
    }
}

WorldObject *MapCell::FindObject(WoWGuid guid)
{
    Guard guard(cellLock);
    MapCell::CellObjectMap::iterator itr;
    if((itr = m_playerSet.find(guid)) != m_playerSet.end() || (itr = m_nonPlayerSet.find(guid)) != m_nonPlayerSet.end())
        return itr->second;
    return NULL;
}

void MapCell::ProcessObjectSets(WorldObject *obj, ObjectProcessCallback *callback, uint32 objectMask)
{
    WorldObject *curObj;
    if(objectMask == 0)
    {
        for(MapCell::CellObjectMap::iterator itr = m_nonPlayerSet.begin(); itr != m_nonPlayerSet.end(); itr++)
        {
            if((curObj = itr->second) == NULL || obj == curObj)
                continue;
            std::lock(obj->GetLock(), curObj->GetLock());
            (*callback)(obj, curObj);
            curObj->LockRelease();
            obj->LockRelease();
        }

        for(MapCell::CellObjectMap::iterator itr = m_playerSet.begin(); itr != m_playerSet.end(); itr++)
        {
            if((curObj = itr->second) == NULL || obj == curObj)
                continue;
            std::lock(obj->GetLock(), curObj->GetLock());
            (*callback)(obj, curObj);
            curObj->LockRelease();
            obj->LockRelease();
        }
    }
    else
    {
        if(objectMask & TYPEMASK_TYPE_UNIT)
        {
            for(MapCell::CellObjectMap::iterator itr = m_creatureSet.begin(); itr != m_creatureSet.end(); itr++)
            {
                if((curObj = itr->second) == NULL || obj == curObj)
                    continue;
                std::lock(obj->GetLock(), curObj->GetLock());
                (*callback)(obj, curObj);
                curObj->LockRelease();
                obj->LockRelease();
            }
        }

        if(objectMask & TYPEMASK_TYPE_PLAYER)
        {
            for(MapCell::CellObjectMap::iterator itr = m_playerSet.begin(); itr != m_playerSet.end(); itr++)
            {
                if((curObj = itr->second) == NULL || obj == curObj)
                    continue;
                std::lock(obj->GetLock(), curObj->GetLock());
                (*callback)(obj, curObj);
                curObj->LockRelease();
                obj->LockRelease();
            }
        }

        if(objectMask & TYPEMASK_TYPE_GAMEOBJECT)
        {
            for(MapCell::CellObjectMap::iterator itr = m_gameObjectSet.begin(); itr != m_gameObjectSet.end(); itr++)
            {
                if((curObj = itr->second) == NULL || obj == curObj)
                    continue;
                std::lock(obj->GetLock(), curObj->GetLock());
                (*callback)(obj, curObj);
                curObj->LockRelease();
                obj->LockRelease();
            }
        }
    }
}

void MapCell::SetActivity(bool state)
{
    Guard guard(cellLock);
    uint32 x = _x/8, y = _y/8;
    if(state && _unloadpending)
        CancelPendingUnload();
    else if(!state && !_unloadpending)
        QueueUnloadPending();
    _active = state;
}

uint32 MapCell::LoadCellData(CellSpawns * sp)
{
    Guard guard(cellLock);
    if(_loaded == true)
        return 0;

    // start calldown for cell map loading
    _mapData->CellLoaded(_x, _y);
    _loaded = true;

    // check if we have a spawn map, otherwise no use continuing
    if(sp == NULL)
        return 0;

    uint32 loadCount = 0, mapId = _instance->GetMapId();
    InstanceData *data = _instance->m_iData;
    if(sp->CreatureSpawns.size())//got creatures
    {
        for(CreatureSpawnArray::iterator i=sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();++i)
        {
            uint8 creatureState = 0;
            CreatureSpawn *spawn = *i;
            if(data && data->GetObjectState(spawn->guid, creatureState) && creatureState > 0)
                continue;

            WoWGuid guid = spawn->guid;
            if(_instance->IsInstance())
            {
                Loki::AssocVector<WoWGuid, WoWGuid>::iterator itr;
                if((itr = m_sqlIdToGuid.find(guid)) != m_sqlIdToGuid.end())
                    guid = itr->second;
                else m_sqlIdToGuid.insert(std::make_pair(spawn->guid, (guid = MAKE_NEW_GUID(sInstanceMgr.AllocateCreatureGuid(), spawn->guid.getEntry(), HIGHGUID_TYPE_UNIT))));
            }

            if(Creature *c = _instance->CreateCreature(guid))
            {
                c->Load(mapId, spawn->x, spawn->y, spawn->z, spawn->o, _instance->iInstanceMode, spawn);
                c->SetInstanceID(_instance->GetInstanceID());
                if(!c->CanAddToWorld())
                {
                    c->Destruct();
                    continue;
                }

                if(_instance->IsCreaturePoolUpdating())
                    _instance->AddObject(c);
                else c->PushToWorld(_instance);
                loadCount++;
            }
        }
    }

    if(sp->GameObjectSpawns.size())//got GOs
    {
        for(GameObjectSpawnArray::iterator i = sp->GameObjectSpawns.begin(); i != sp->GameObjectSpawns.end(); i++)
        {
            uint8 gameObjState = 0x00;
            GameObjectSpawn *spawn = *i;
            if(data == NULL || !data->GetObjectState(spawn->guid, gameObjState))
                gameObjState = spawn->state;

            WoWGuid guid = spawn->guid;
            if(_instance->IsInstance())
            {
                Loki::AssocVector<WoWGuid, WoWGuid>::iterator itr;
                if((itr = m_sqlIdToGuid.find(guid)) != m_sqlIdToGuid.end())
                    guid = itr->second;
                else m_sqlIdToGuid.insert(std::make_pair(spawn->guid, (guid = MAKE_NEW_GUID(sInstanceMgr.AllocateCreatureGuid(), spawn->guid.getEntry(), HIGHGUID_TYPE_GAMEOBJECT))));
            }

            if(GameObject *go = _instance->CreateGameObject(guid))
            {
                go->Load(mapId, spawn->x, spawn->y, spawn->z, 0.f, spawn->rX, spawn->rY, spawn->rZ, spawn->rAngle, spawn);
                go->SetInstanceID(_instance->GetInstanceID());
                go->SetState(gameObjState);

                if(_instance->IsGameObjectPoolUpdating())
                    _instance->AddObject(go);
                else go->PushToWorld(_instance);
                loadCount++;
            }
        }
    }
    return loadCount;
}

void MapCell::UnloadCellData(bool preDestruction)
{
    Guard guard(cellLock);
    if(_loaded == false)
        return;

    _loaded = false;
    while(!m_nonPlayerSet.empty())
    {
        WorldObject *obj = m_nonPlayerSet.begin()->second;
        m_nonPlayerSet.erase(m_nonPlayerSet.begin());
        if( !preDestruction && _unloadpending )
        {
            if(obj->GetHighGUID() == HIGHGUID_TYPE_TRANSPORTER)
                continue;

            if(obj->GetTypeId() == TYPEID_CORPSE && obj->GetUInt32Value(CORPSE_FIELD_OWNER) != 0)
                continue;
        }

        obj->Cleanup();
    }

    m_nonPlayerSet.clear();
    m_gameObjectSet.clear();
    m_creatureSet.clear();
    m_sqlIdToGuid.clear();

    // Start calldown for cell map unloading
    _mapData->CellUnloaded(_x, _y);
}

void MapCell::QueueUnloadPending()
{
    Guard guard(cellLock);
    if(_unloadpending)
        return;

    _unloadpending = true;
    sLog.Debug("MapCell", "Queueing pending unload of cell %u %u", _x, _y);
}

void MapCell::CancelPendingUnload()
{
    Guard guard(cellLock);
    if(!_unloadpending)
        return;

    _unloadpending = false;
    sLog.Debug("MapCell", "Cancelling pending unload of cell %u %u", _x, _y);
}

void MapCell::Unload()
{
    Guard guard(cellLock);
    if(_active)
        return;

    sLog.Debug("MapCell", "Unloading cell %u %u", _x, _y);
    UnloadCellData(false);
    _unloadpending = false;
}
