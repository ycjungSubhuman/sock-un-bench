#include <iostream>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>

void check(int v, const std::string &name)
{
    if(-1 == v)
    {
        std::cerr << name << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
        std::exit(1);
    }
}

// A simple client that sends dummy data of size 'argv[1]' bytes to server
int main(int argc, char **argv)
{
    int32_t buffer[256*256*4] = {0, };
    struct sockaddr_un addr = {AF_UNIX, "./test"};
    const auto sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    assert(-1 != sockfd);
    check(connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)), "connect");

    std::chrono::system_clock clock;

    std::chrono::nanoseconds min{100000000}, max{0}, avg{0};

    int cnt= 0;
    while(true)
    {
        char msg[5];
        auto t = clock.now();
        check(read(sockfd, msg, sizeof(msg)), "read");
        if (0 == std::strcmp(msg,"done"))
        {
            break;
        }
        check(write(sockfd, buffer, sizeof(buffer)), "write");
        check(read(sockfd, msg, sizeof(msg)), "read");
        std::chrono::duration<double> elapsed = clock.now() - t;
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
        avg += ns;
        cnt++;
        if(min > ns)
        {
            min = ns;
        }
        if(max < ns)
        {
            max = ns;
        }
        std::cout << "Delay : " << ns.count() << "ns" << std::endl;
    }
    std::cout << "Min Delay : " << min.count() << "ns" << std::endl;
    std::cout << "Max Delay : " << max.count() << "ns" << std::endl;
    std::cout << "Avg Delay : " << static_cast<float>(avg.count())/cnt << "ns" << std::endl;

    close(sockfd);

    return 0;
}
