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

#ifndef PCOCOMMON_H
#define PCOCOMMON_H

#include <map>

///
/// \brief The PcoManager class
///
/// This class allows to inject some latency within the synchronization objects
///
class PcoManager
{
public:

    enum class EventType {
        ThreadCreation,         ///< For the thread constructor function
        ThreadJoin,             ///< For the thread join() function
        MutexLock,              ///< For the mutex lock() function
        MutexUnlock,            ///< For the mutex unlock() function
        WaitConditionWait,      ///< For the condition variable wait() function
        WaitConditionNotify,    ///< For the condition variable notify() function
        WaitConditionNotifyAll, ///< For the condition variable notifyAll() function
        SemaphoreAcquire,       ///< For the semaphore acquire() function
        SemaphoreRelease,       ///< For the semaphore release() function
        Standard                ///< For any object, the default value
    };

    ///
    /// \brief gets the PcoManager singleton instance
    /// \return A pointer to the unique instance.
    ///
    /// The object is created the first time this method is called.
    ///
    static PcoManager *getInstance();

    ///
    /// \brief sets the maximum sleeping time for a specific event type
    /// \param useconds The number of microseconds to sleep
    /// \param eventType The event type
    ///
    /// By default it defines the sleeping time of the event EventType::Standard.
    ///
    /// The method randomSleep() uses the value defined for the specific event type
    /// it gets as an argument, but if this value is not set, then it uses the
    /// value defined for EventType::Standard
    ///
    void setMaxSleepDuration(unsigned int useconds, EventType eventType = EventType::Standard);

    ///
    /// \brief Let the calling thread sleeps for a random period
    /// \param eventType The event type used to get the correct maximum time
    ///
    /// This method is called by the various synchronization objects, letting
    /// them pass the kind of event, depending on their class.
    ///
    void randomSleep(EventType eventType = EventType::Standard);

protected:

    ///
    /// \brief PcoManager constructor
    ///
    /// The constructor is protected, as it is meant to be a singleton.
    ///
    PcoManager();

    /// Map of sleeping times per type of event
    std::map<EventType, unsigned int> m_usecondsMap;

};


#endif // PCOCOMMON_H