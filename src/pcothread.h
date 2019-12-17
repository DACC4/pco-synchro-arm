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

#ifndef PCOTHREAD_H
#define PCOTHREAD_H

#include <thread>
#include <memory>

#include "pcomanager.h"

///
/// \brief The PcoThread class
///
/// This class is meant to be used to start a new thread, and to join it.
///
class PcoThread
{
public:

    ///
    /// \brief The thread constructor (and starter).
    /// \param fn The function to be run by the new thread
    /// \param args The arguments to be sent to the function
    /// The constructor creates and starts the thread automatically.
    ///
    /// This constructor is a template, allowing to pass any type of function
    /// and any type of arguments to this function.
    /// It corresponds to the same constructor as the one of std::thread :
    /// http://www.cplusplus.com/reference/thread/thread/
    ///
    template <class Fn, class... Args>
    explicit PcoThread (Fn&& fn, Args&&... args)
    {
        PcoManager::getInstance()->randomSleep(PcoManager::EventType::ThreadCreation);
        m_thread = std::make_unique<std::thread>(fn, args...);
        PcoManager::getInstance()->randomSleep(PcoManager::EventType::ThreadCreation);
    }

    /// No copy
    PcoThread (const PcoThread&) = delete;

    /// No copy
    PcoThread (const PcoThread&&) = delete;

    /// No copy
    PcoThread& operator= ( const PcoThread & ) = delete;


    ///
    /// \brief joins on the thread.
    ///
    /// This method blocks the caller thread until the thread object finishes its
    /// execution.
    ///
    void join()
    {
        PcoManager::getInstance()->randomSleep(PcoManager::EventType::ThreadJoin);
        m_thread->join();
        PcoManager::getInstance()->randomSleep(PcoManager::EventType::ThreadJoin);
    }

protected:

    /// The internal thread descriptor
    std::unique_ptr<std::thread> m_thread;
};

#endif // PCOTHREAD_H
