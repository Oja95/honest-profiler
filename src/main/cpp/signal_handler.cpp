#include "signal_handler.h"
#include <time.h>

namespace {

// Helper class to store and reset errno when in a signal handler.
    class ErrnoRaii {
    public:
        ErrnoRaii() {
            stored_errno_ = errno;
        }

        ~ErrnoRaii() {
            errno = stored_errno_;
        }

    private:
        int stored_errno_;

        DISALLOW_COPY_AND_ASSIGN(ErrnoRaii);
    };
} // namespace

bool SignalHandler::updateSigprofInterval() {
    bool res = updateSigprofInterval(timingIntervals[intervalIndex]);
    intervalIndex = (intervalIndex + 1) % NUMBER_OF_INTERVALS;
    return res;
}

bool SignalHandler::updateSigprofInterval(const int timingInterval) {
    if (timingInterval == currentInterval)
        return true;

    struct sigevent sevent;
    timer_t timert;
    sevent.sigev_notify = SIGEV_SIGNAL;
    sevent.sigev_signo = SIGPROF;
    sevent.sigev_value.sival_ptr = &timert;

    if (timer_create(CLOCK_PROCESS_CPUTIME_ID, &sevent, &timert) == -1) {
        logError("Failed creating timer with timer_create(), failed with error %d\n", errno);
        return false;
    }

    struct itimerspec timerspec;
    // Try with 100 microsecond interval.
    timerspec.it_interval.tv_nsec = 100000;
    timerspec.it_value.tv_nsec = timerspec.it_interval.tv_nsec;

    if (timer_settime(timert, 0, &timerspec, NULL) == -1) {
        logError("Scheduling profiler interval (timer_settime) failed with error %d\n", errno);
        return false;
    }

    currentInterval = timingInterval;
    return true;
}

struct sigaction SignalHandler::SetAction(void (*action)(int, siginfo_t *, void *)) {
    struct sigaction sa;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = NULL;
    sa.sa_sigaction = action;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    sigemptyset(&sa.sa_mask);

    struct sigaction old_handler;
    if (sigaction(SIGPROF, &sa, &old_handler) != 0) {
        logError("Scheduling profiler action failed with error %d\n", errno);
        return old_handler;
    }

    return old_handler;
}

