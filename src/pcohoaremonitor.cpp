#include "pcohoaremonitor.h"

PcoHoareMonitor::Condition::Condition() : waitingSem(0), nbWaiting(0) {}

PcoHoareMonitor::PcoHoareMonitor() : monitorMutex(1), monitorSignale(0), monitorNbSignale(0) {}

void PcoHoareMonitor::monitorIn() {
    monitorMutex.acquire();
}

void PcoHoareMonitor::monitorOut() {
    if (monitorNbSignale > 0)
        monitorSignale.release();
    else
        monitorMutex.release();
}

void PcoHoareMonitor::wait(Condition &cond) {
    cond.nbWaiting += 1;
    if (monitorNbSignale > 0)
        monitorSignale.release();
    else
        monitorMutex.release();
    cond.waitingSem.acquire();
    cond.nbWaiting -= 1;
}

void PcoHoareMonitor::signal(Condition &cond) {
    if (cond.nbWaiting>0) {
        monitorNbSignale += 1;
        cond.waitingSem.release();
        monitorSignale.acquire();
        monitorNbSignale -= 1;
    }
}
