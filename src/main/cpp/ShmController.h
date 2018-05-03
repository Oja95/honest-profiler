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

    void setSignalHandler();

    char *shm;

private:

    int connect_to_server_and_get_memfd_fd();

    int receive_fd(int conn);
};

#endif //HONEST_PROFILER_SHMCONTROLLER_H
