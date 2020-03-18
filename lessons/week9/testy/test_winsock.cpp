#include "w32_socket.hpp"
#include <cstdio>
#include <cstring>


bool test_gethost(char *hostname)
{
    printf("==== test_gethost ====\n");

    char *servicename = "http";
	int family = AF_UNSPEC;
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

    printf("Looking for: %s\n", hostname);
    err = getaddrinfo(hostname, servicename, &hints, &ppResult);
    printf("  getaddrinfo, err: %d\n", err);

    if (err != 0) {
        printf("getaddrinfo FAILED [%d]\n", err);
        return false;
    }

    // enumerate some of the stuff we found out
    
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
    test_localhost();
}

void main()
{
    startup();
}