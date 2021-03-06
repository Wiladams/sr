#pragma once

/*
References:
https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedInternet3a.html
*/
#include <cstring>
#include <cstdio>
#include <vector>

#include "w32_socket.hpp"

/*
    This should live somewhere else, higher in the stack
*/
struct BufferChunk {
    size_t fSize;
    char *fData;
    bool fIOwnData;

    BufferChunk(void *buff, const int size)
    {
        fData = (char *)buff;
        fSize = size;
        fIOwnData = false;
    }

    BufferChunk(int size)
    {
        fData = {new char[size]{}};
        fSize = size;
        fIOwnData = true;
    }

    ~BufferChunk()
    {
        if (fIOwnData) {
            delete fData;
        }
        fSize = 0;
    }

    char * getDataPointer() {return fData;}
    size_t size() {return fSize;}
};


struct IPAddress {
private:
/*
    IPAddress()
    {
        memset(&fAddress, 0, sizeof(fAddress));
        fAddressLength = 0;
    }
*/
public:
    struct sockaddr *fAddress;
    int fAddressLength;

    // Construct from traditional sockaddr and length
    IPAddress(const struct sockaddr *addr, const int addrLen)
    {
        fAddress = (struct sockaddr *)malloc(addrLen);
        memcpy(fAddress, addr, addrLen);
        fAddressLength = addrLen;
    }

    ~IPAddress()
    {
        free(fAddress);
    }

/*
    int toString(int addressStringLength, char *addressString)
    {
        int returnLength=addressStringLength;
        int res = WSAAddressToStringA(fAddress, fAddressLength, nullptr,addressString, &returnLength);
        
        return returnLength;
    }
*/
};

class IPHost {
    char fHostName[256];
    std::vector<IPAddress *> fAddresses;
    std::vector<char *> fAliases;

public:
    static IPHost * create(const char *hostname, const char *portname, int family = AF_INET, int socktype = SOCK_STREAM,  bool isnumericstring=false)
    {
        int err;
        addrinfo hints;
        struct addrinfo *ppResult;

        // need to zero out some members before using
        memset(&hints, 0, sizeof(hints));

	    hints.ai_family = family;
        hints.ai_socktype = socktype;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_CANONNAME;    // hostname is a canonical name

        if (isnumericstring) {
            hints.ai_flags = AI_NUMERICHOST;
        }

        err = getaddrinfo(hostname, portname, &hints, &ppResult);

        if (err != 0) {
            return nullptr;
        }

        // Create the host
        IPHost *host = new IPHost();
        // Add initial name and address
        host->setName(ppResult->ai_canonname);
        host->addAddress(ppResult->ai_addr, ppResult->ai_addrlen);

        // go through the rest of the entries
        // and add their information as well
        struct addrinfo * sentinel = ppResult->ai_next;
        while (sentinel != nullptr) {
            if (sentinel->ai_canonname != nullptr)
            {
                host->addAlias(sentinel->ai_canonname);
            }

            host->addAddress(sentinel->ai_addr, sentinel->ai_addrlen);
            sentinel = sentinel->ai_next;
        }

        // free the addrinfos structure
	    freeaddrinfo(ppResult);

	    return host;
    }

    void setName(const char *name)
    {
        if (name == nullptr) {
            memset(fHostName, 0, sizeof(fHostName));
        }

        strncpy(fHostName, name, sizeof(fHostName));
    }
    const char * getName() const {return fHostName;}

    // addAddress
    bool addAddress(const struct sockaddr *addr, const int addrlen)
    {
        fAddresses.push_back(new IPAddress(addr, addrlen));
        
        return true;
    }

    int numberOfAddresses() {return fAddresses.size();}

    IPAddress * getAddress(const int idx = 0)
    {
        return fAddresses[idx];
    }

    // addAlias
    bool addAlias(const char *alias)
    {
        return false;
    }
};

class IPSocket {
    SOCKET fSocket;
    IPAddress * fAddress;
    bool fIsValid;

public:
    IPSocket(int family, int socktype, int protocol = 0)
        : fIsValid(false),
        fAddress(nullptr)
    {
        fSocket = WSASocketA(family, socktype, protocol, nullptr, 0, 0);

        if (fSocket == INVALID_SOCKET) {
            printf("INVALID_SOCKET: %Id\n", fSocket);
            return ;
        }

        fIsValid = true;
    }

    IPSocket(const char *hostname, const char *portname, int family, int socktype, int protocol = 0)
        : fIsValid(false),
        fAddress(nullptr)
    {
        IPHost * host = IPHost::create(hostname, portname, family, socktype);
        if (host == nullptr)
        {
            printf("could not find host: %s\n", hostname);
            return ;
        }

        // Create a socket based on the host
        fSocket = WSASocketA(family, socktype, protocol, nullptr, 0, 0);

        if (fSocket == INVALID_SOCKET) {
            printf("INVALID_SOCKET: %Id\n", fSocket);
            return ;
        }
        
        fAddress = host->getAddress();
        if (fAddress == nullptr) {
            return ;
        }

        fIsValid = true;
    }

    IPSocket(const char *hostname, const char *portname)
        :IPSocket(hostname, portname, AF_INET, SOCK_STREAM)
    {
    }

    virtual ~IPSocket() {
        close();
    }

    bool isValid() {return fIsValid;}

    bool connect()
    {
        int retCode = ::connect(fSocket, fAddress->fAddress, fAddress->fAddressLength);

        if (retCode != 0) {
            printf("Failed to connect: %d\n", retCode);
            return false;
        }

        return true;
    }

    bool close() {
        ::closesocket(fSocket);
        return true;
    }

    int bind()
    {
        return ::bind(fSocket, fAddress->fAddress, fAddress->fAddressLength);
    }

    int bindTo(const sockaddr *addr, const int addrLen)
    {
        return ::bind(fSocket, addr, addrLen);
    }

    // Typically when you're creating a server, you will 
    // need to tell the socket to start listening for
    // connections.  This is as opposed to be active, like
    // a client connection.
    int makePassive(const int backlog=5)
    {
        return ::listen(fSocket, backlog);
    }

    // Send to a specific address
    // The address was specified when we 
    // created the socket
    int sendTo(const struct sockaddr *addrTo, int addrToLen, const char *buff, const int bufflen)
    {
        return ::sendto(fSocket, buff, bufflen, 0, addrTo, addrToLen);
    }

    int receiveFrom(struct sockaddr *addrFrom, int *addrFromLen, char *buff, int bufflen)
    {
        return ::recvfrom(fSocket, buff, bufflen, 0, addrFrom, addrFromLen);
    }

    // Send a chunk of memory
    // return the number of octets sent
    int sendChunk(BufferChunk &chunk, int flags=0)
    {
        int retCode = ::send(fSocket, (char *)chunk.fData, chunk.fSize, flags);
        return retCode;
    }

    // receive a chunk of memmory
    // return number of octets received
    int receiveChunk(BufferChunk &chunk, int flags = 0)
    {
        int retCode = ::recv(fSocket, (char *)chunk.fData, chunk.fSize, flags);
        return retCode;
    }
};


