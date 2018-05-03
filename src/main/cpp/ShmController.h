#ifndef HONEST_PROFILER_SHMCONTROLLER_H
#define HONEST_PROFILER_SHMCONTROLLER_H

#include <sys/mman.h>

#include <sys/socket.h>
#include <linux/un.h>

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <thread>

#include "memfd.hpp"

class ShmController {

public:
    ShmController();

    void start();

    char *shm;

private:

    int connect_to_server_and_get_memfd_fd();

    int receive_fd(int conn);

//    void sigAlrmHandle();
//
//    void sigAlrmHandle(void (*action)(int, siginfo_t *, void *));
//    static void sigAlrmHandle(int sig, siginfo_t *siginfo, void *dno);
};

#endif //HONEST_PROFILER_SHMCONTROLLER_H
