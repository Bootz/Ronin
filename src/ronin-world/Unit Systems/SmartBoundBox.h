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

#pragma once

class SERVER_DECL SmartBounding
{
protected:
    struct VectPoint { float x, y, z; };

public:
    SmartBounding() {}
    ~SmartBounding() {}

    void UpdatePosition(uint32 startTime, float sX, float sY, float sZ, uint32 timeDistance, float eX, float eY, float eZ) {}
    void Finalize(float x, float y, float z) {}

    void GetPosition(uint32 msTime, LocationVector *output) {}

private:
    uint32 lastUpdateMS, timeToTarget;
    // Planned implementation is a different bound in all directions
    //float _boundX, _boundY, _boundZ; // However we only have a bound radius stored
    float _boundRadius; // Used in calculating if bound is in range

    float lastPointX, lastPointY, lastPointZ;
    float endX, endY, endZ;

    std::vector<std::shared_ptr<VectPoint>> m_history;
};