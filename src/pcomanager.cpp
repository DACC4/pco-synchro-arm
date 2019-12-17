/*****************************************************************************
 * Copyright (C) 2020 HEIG-VD                                                *
 *                                                                           *
 * This file is part of PcoSynchro.                                          *
 *                                                                           *
 * PcoSynchro is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * PcoSynchro is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU Lesser General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with PcoSynchro.  If not, see <https://www.gnu.org/licenses/>.      *
 *****************************************************************************/

#include <thread>
#include <chrono>
#include <random>

#include "pcomanager.h"

PcoManager *PcoManager::getInstance()
{
    static PcoManager *pcoManager = new PcoManager();
    return pcoManager;
}

PcoManager::PcoManager()
{
    m_usecondsMap[EventType::Standard] = 0.0;
}


void PcoManager::setMaxSleepDuration(unsigned int useconds, EventType eventType)
{
    m_usecondsMap[eventType] = useconds;
}

void PcoManager::randomSleep(EventType eventType)
{
    unsigned int useconds;
    if (m_usecondsMap.count(eventType) > 0) {
        useconds = m_usecondsMap[eventType];
    }
    else {
        useconds = m_usecondsMap[EventType::Standard];
    }
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, static_cast<int>(useconds));
    auto randomValue = dis(gen);
    std::chrono::microseconds value(randomValue);
    std::this_thread::sleep_for(value);
}
