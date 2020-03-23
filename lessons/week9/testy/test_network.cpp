
#include <cstdio>

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
    printf("===================\n");
    
}

void setup()
{
    char * hostname = "github.com";
    IPHostEntry host;

    if (!DNSResolver::getHostByName(hostname, host))
    {
        printf("could not find host: %s\n", hostname);
        return ;
    }

    printf("Name: %s\n", host.getName());
}

void main()
{
    preload();
    setup();
}