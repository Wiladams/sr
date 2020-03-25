
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

// When we're about to exit
// do any cleanup
void onExit()
{
    WSACleanup();
}

#define PORT 9090
#define MAXLINE 1024

void setup()
{
    char *hostname = "DESKTOP-UUUQ00U";
    char *portname = "8080";

    // Create socket object
    IPSocket s(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Get server address
    //struct sockaddr_in servaddr;
    //memset(&servaddr, 0, sizeof(servaddr));
    //servaddr.sin_family = AF_INET;
    //servaddr.sin_port = htons(PORT);
    //servaddr.sin_addr.S_addr = INADDR_ANY;  // the local machine
    IPHost *host = IPHost::create(hostname, "9090", AF_INET, SOCK_DGRAM);
    IPAddress *servaddr = host->getAddress();

    char *msg = "Hello UDP!!";
    int msgLen = strlen(msg);

    int sentCode = s.sendTo(servaddr->fAddress, servaddr->fAddressLength, msg, msgLen);
    printf("Message Sent: %d ==> %d\n", sentCode, WSAGetLastError());

    char inBuff[MAXLINE];
    struct sockaddr_in inserv;
    //memset(&servaddr, 0, sizeof(servaddr));

    int servLen;
    int recvLen = s.receiveFrom((sockaddr *)&inserv, &servLen, inBuff, MAXLINE);
    printf("Message Received: [%d]\n", recvLen);
    if (recvLen < 0)
        return ;

    // Null terminate message
    inBuff[recvLen] = 0;
    printf("%s\n", inBuff);
}

void main()
{
    preload();
    setup();
    onExit();
}
