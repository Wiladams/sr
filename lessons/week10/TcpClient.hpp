#pragma once

#include "Network.hpp"

class TcpClient {
private:
    IPSocket fSocket;
    IPAddress *fAddress;
    bool    fIsValid;
    int fLastError;

public:
    TcpClient(const char *hostname, const char * portname)
        : fIsValid(false)
    {
        // Create a socket
        fSocket = IPSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (!fSocket.isValid()) {
            return ;
        }

        // Get address to host
        IPHost * host = IPHost::create(hostname, portname, AF_INET, SOCK_STREAM);
        if (host == nullptr)
        {
            printf("could not find host: %s\n", hostname);
            return ;
        }

        fAddress = host->getAddress();
        if (fAddress == nullptr) {
            return ;
        }

        fIsValid = true;
    }

    bool connect()
    {
        int retCode = ::connect(fSocket.fSocket, fAddress->fAddress, fAddress->fAddressLength);

        if (retCode != 0) {
            printf("TcpClient.connect, Failed to connect: %d\n", retCode);
            return false;
        }

        return true;
    }

    int send(const char *buff, const int buffLen, int flags = 0)
    {
        return fSocket.send(buff, buffLen, flags);
    }

    int receive(char *buff, int buffLen, int flags=0)
    {
        return fSocket.receive(buff, buffLen, flags);
    }
};

