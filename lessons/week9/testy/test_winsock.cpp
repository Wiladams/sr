#include "w32_socket.hpp"
#include <cstdio>
#include <cstring>

/*
typedef struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    char* ai_canonname;
    struct sockaddr * ai_addr;
    struct addrinfo* ai_next;
} ADDRINFOA,  *PADDRINFOA;
*/
void printHost(const struct addrinfo *host, int tabs)
{
    printf("Host: %s\n", host->ai_canonname);
    printf("       Flags: %d\n", host->ai_flags);
    printf("      Family: %d\n", host->ai_family);
    printf("   Sock Type: %d\n", host->ai_socktype);
    printf("    Protocol: %d\n", host->ai_protocol);
    printf(" Address Len: %zd\n", host->ai_addrlen);

    char addressString[256];
    DWORD addressStringLength = 256;

    int res = WSAAddressToStringA(host->ai_addr, host->ai_addrlen, nullptr,
        addressString, &addressStringLength);
    printf(" Address [%d]: %s\n", res, addressString);

    if (host->ai_next != nullptr) {
        printHost(host->ai_next, tabs+4);
    }
}

bool test_gethost(char *hostname)
{
    printf("==== test_gethost ====\n");

    char *servicename = "http";
	//int family = AF_UNSPEC;
    //int family = AF_INET6;
    int family = AF_INET;
	int socktype = SOCK_STREAM;

    int  err;
    struct addrinfo hints;
    struct addrinfo *ppResult;

    // need to zero out some members before using
    memset(&hints, 0, sizeof(hints));

	hints.ai_family = family;
	hints.ai_socktype = socktype;

    //if isnumericstring {
    //  hints.ai_flags = AI_NUMERICHOST;
    //}

    printf("++++++++ Looking for: %s ++++++++\n", hostname);
    err = getaddrinfo(hostname, servicename, &hints, &ppResult);

    if (err != 0) {
        printf("getaddrinfo FAILED [%d]\n", err);
        return false;
    }

    // enumerate some of the stuff we found out
    printHost(ppResult, 0);

    freeaddrinfo(ppResult);

    return true;

}


void test_localhost()
{
    printf("==== test_localhost ====\n");
    const int namelen = 256;
    char name[namelen];
    name[0] = 0;

    int res = gethostname(name, namelen);
    printf("[%d] %s\n", res, name);
}

void startup()
{
    uint16_t version = MAKEWORD(2,2);
    WSADATA lpWSAData;

    int res = WSAStartup(version, &lpWSAData);
    printf("Startup: %d\n", res);

    test_gethost("microsoft.com");
    test_gethost("facebook.com");
    test_gethost("google.com");
    test_gethost("amazon.com");
    test_gethost("netflix.com");
    test_gethost("msn.com");
    test_gethost("adafruit.com");
    test_gethost("news.ycombinator.com");

    test_localhost();
}

void main()
{
    startup();
}