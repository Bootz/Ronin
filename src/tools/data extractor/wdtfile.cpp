/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2014-2017 Sandshroud <https://github.com/Sandshroud>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mpqfile.h"
#include "system.h"
#include "wdtfile.h"
#include "adtfile.h"
#include <cstdio>

char * wdtGetPlainName(char * FileName)
{
    char * szTemp;

    if((szTemp = strrchr(FileName, '\\')) != NULL)
        FileName = szTemp + 1;
    return FileName;
}

WDTFile::WDTFile(HANDLE mpqarchive, char* file_name, char* file_name1) : WDT(mpqarchive, file_name, false)
{
    filename.append(file_name1,strlen(file_name1));
}

bool WDTFile::init(char* /*map_id*/, unsigned int mapID)
{
    if (WDT.isEof())
    {
        //printf("Can't find WDT file.\n");
        return false;
    }

    char fourcc[5];
    uint32 size;

    while (!WDT.isEof())
    {
        WDT.read(fourcc,4);
        WDT.read(&size, 4);

        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = WDT.getPos() + size;

        if (!strcmp(fourcc,"MAIN"))
        {
        }
        if (!strcmp(fourcc,"MWMO"))
        {
            // global map objects
            if (size)
            {
                char *buf = new char[size];
                WDT.read(buf, size);
                char *p=buf;
                int q = 0;
                gWmoInstansName = new string[size];
                while (p < buf + size)
                {
                    char* s=wdtGetPlainName(p);
                    FixNameCase(s,strlen(s));
                    p=p+strlen(p)+1;
                    gWmoInstansName[q++] = s;
                }
                delete[] buf;
            }
        }
        else if (!strcmp(fourcc, "MODF"))
        {
            // global wmo instance data
            if (size)
            {
                gnWMO = (int)size / 64;

                for (int i = 0; i < gnWMO; ++i)
                {
                    int id;
                    WDT.read(&id, 4);
                    WMOInstance inst(WDT,gWmoInstansName[id].c_str(), mapID, 65, 65);
                }

                delete[] gWmoInstansName;
            }
        }
        WDT.seek((int)nextpos);
    }

    WDT.close();
    return true;
}

WDTFile::~WDTFile(void)
{
    WDT.close();
}

ADTFile* WDTFile::GetADTMap(HANDLE mpqarchive, int x, int y)
{
    if(!(x>=0 && y >= 0 && x<64 && y<64))
        return NULL;
    char name[512];
    sprintf(name,"World\\Maps\\%s\\%s_%d_%d.adt", filename.c_str(), filename.c_str(), y, x);
    if(!SFileHasFile(WorldMpq, name))
        return NULL;
    return new ADTFile(mpqarchive, name);
}

ADTFile* WDTFile::GetObjMap(HANDLE mpqarchive, int x, int y)
{
    if(!(x>=0 && y >= 0 && x<64 && y<64))
        return NULL;
    char name[512];
    sprintf(name,"World\\Maps\\%s\\%s_%d_%d_obj0.adt", filename.c_str(), filename.c_str(), y, x);
    if(!SFileHasFile(WorldMpq, name))
        return NULL;
    return new ADTFile(mpqarchive, name);
}
