
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
    char * hostname = "www.bing.com";
    char * portname = "80";

    IPHost * host = IPHost::create(hostname, portname);

    if (host == nullptr)
    {
        printf("could not find host: %s\n", hostname);
        return ;
    }

    printf("==== GETTING HOST ====");
    printf("     Name: %s\n", host->getName());
    printf("Addresses: [%d]\n", host->numberOfAddresses());

    // Create a socket based on the host
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("socket: %Id\n", s);

    IPAddress *addr = host->getAddress();
    int retCode = connect(s, addr->fAddress, addr->fAddressLength);

    printf("socket connect: %d\n", retCode);

    if (retCode != 0) {
        printf("Failed to connect: %d\n", retCode);
    }

    char * request = "GET / HTTP/1.1\r\n"
                     "User-Agent: My Fake Agent\r\n"
                     "Host: www.bing.com\r\n"
                     "\r\n";

    retCode = send(s, request, strlen(request), 0);

    printf("send: %d\n", retCode);
    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return ;
    }

    // Get a response back from the server
    static const int recvSize = 1024 * 64; // 64k 
    char response[recvSize+1];
    retCode = recv(s, response, recvSize, 0);
    printf("recv: %d\n", retCode);
    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return ;
    }

    // Null terminate so we can print
    // as zstring
    response[retCode] = 0;

    printf("== RESPONSE ==\n");
    printf("%s", response);

    closesocket(s);
}

void onExit()
{
    WSACleanup();
}

void main()
{
    preload();
    setup();
    onExit();
}
