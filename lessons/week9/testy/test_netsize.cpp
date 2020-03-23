#include <cstdio>
#include <cstring>

#include "Network.hpp"

void preload()
{
    uint16_t version = MAKEWORD(2,2);
    WSADATA lpWSAData;

    int res = WSAStartup(version, &lpWSAData);
    printf("WSAStartup: %d\n", res);
    printf(" Max Sockets: %d\n", lpWSAData.iMaxSockets);
    printf("  Max Udp Dg: %d\n", lpWSAData.iMaxUdpDg);
    printf("      Vendor: %s\n", lpWSAData.lpVendorInfo);
    printf(" Description: %s\n", lpWSAData.szDescription);
    printf("      Status: %s\n", lpWSAData.szSystemStatus);
    
}

void setup()
{
    printf("    sockaddr: %zd\n", sizeof(sockaddr));
    printf(" sockaddr_in: %zd\n", sizeof(sockaddr_in));
    printf("sockaddr_in6: %zd\n", sizeof(sockaddr_in6));

}

void main()
{
    preload();
    setup();
}