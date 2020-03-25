
#include <cstdio>
#include <cstring>

#include "Network.hpp"
//#include "sites.hpp"
#include "httpsites.hpp"

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

bool pingHttp(const char * hostname)
{
    char request[512];
    sprintf(request, "GET / HTTP/1.1\r\n"
                     "Host: %s\r\n"
                     "\r\n", hostname);
    //sprintf(request, "GET / HTTP/1.1\r\n"
    //                 "User-Agent: My Fake Agent\r\n"
    //                 "Host: %s\r\n"
    //                 "\r\n", hostname);

    char * portname = "80";
    IPHost * host = IPHost::create(hostname, portname);

    if (host == nullptr)
    {
        printf("could not find host: %s\n", hostname);
        return false;
    }

    // Create a socket based on the host
    //SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKET s = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);

    if (s == INVALID_SOCKET) {
        printf("INVALID_SOCKET: %Id\n", s);
        return false;
    }

    IPAddress *addr = host->getAddress();
    int retCode = connect(s, addr->fAddress, addr->fAddressLength);

    if (retCode != 0) {
        printf("Failed to connect: %d\n", retCode);
        return false;
    }

    retCode = send(s, request, strlen(request), 0);

    //printf("send: %d\n", retCode);
    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return false;
    }

    // Get a response back from the server
    static const int recvSize = 1024 * 64; // 64k 
    char response[recvSize+1];
    retCode = recv(s, response, recvSize, 0);
    //printf("recv: %d\n", retCode);
    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return false;
    }

    // Null terminate so we can print
    // as zstring
    response[retCode] = 0;

    printf("\n== RESPONSE BEGIN (%s)==\n", hostname);
    printf("%s", response);
    printf("\n== RESPONSE END ==\n");

    closesocket(s);

    return true;
}

void setup()
{
    int nSites = sizeof(sites)/sizeof(char *);

    printf("SCAN BEGIN[%d]\n", nSites);

    for (int idx = 0; idx < nSites; idx++)
    {
        bool success = pingHttp(sites[idx]);
        if (!success) {
            printf("FAILED: %s\n", sites[idx]);
            continue;
        }
    }
    printf("\n== SCAN END ==\n");
}

// When we're about to exit
// do any cleanup
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
