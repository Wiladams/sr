
#include <cstdio>
#include <cstring>

#include "Network.hpp"
//#include "sites.hpp"
#include "httpsites.hpp"

struct BufferChunk {
    size_t fSize;
    char *fData;
    bool fIOwnData;

    BufferChunk(char *buff, int size)
    {
        fData = (uint8_t *)buff;
        fSize = size;
        fIOwnData = false;
    }

    BufferChunk(int size)
    {
        fData = {new uint8_t[size]{}};
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

class IPSocket {
    SOCKET fSocket;
    IPAddress * fAddress;
    bool fIsValid;

public:
    
    IPSocket(const char *hostname, const char *portname)
        : fIsValid(false),
        fAddress(nullptr)
    {
        IPHost * host = IPHost::create(hostname, portname);
        if (host == nullptr)
        {
            printf("could not find host: %s\n", hostname);
            return ;
        }

        // Create a socket based on the host
        fSocket = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);

        if (fSocket == INVALID_SOCKET) {
            printf("INVALID_SOCKET: %Id\n", s);
            return ;
        }
        
        fAddress = host->getAddress();
        if (fAddress == nullptr) {
            return ;
        }

        fIsValid = true;
    }

    ~IPSocket() {
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
        closesocket(fSocket);
        return true;
    }

    bool bind()
    {
    }

    int sendChunk(BufferChunk &chunk, int flags=0)
    {
        int retCode = ::send(fSocket, (char *)chunk.fData, chunk.fSize, flags);
        return retCode;
    }

    int receiveChunk(BufferChunk &chunk, int flags = 0)
    {
        int retCode = ::recv(fSocket, (char *)chunk.fData, chunk.fSize, flags);
        return retCode;
    }
};

void preload()
{
    uint16_t version = MAKEWORD(2,2);
    WSADATA lpWSAData;

    int res = WSAStartup(version, &lpWSAData);
    printf("WSAStartup: %d\n", res);
}

bool pingHttp(const char * hostname)
{
    char request[512];
    sprintf(request, "GET / HTTP/1.1\r\n"
                     "User-Agent: My Fake Agent\r\n"
                     "Host: %s\r\n"
                     "\r\n", hostname);

    char * portname = "80";

    IPSocket s(hostname, portname);

    s.connect();

    BufferChunk chunk(request, strlen(request));
    int retCode = s.sendChunk(chunk, 0);

    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return false;
    }

    // Get a response back from the server
    static const int recvSize = 1024 * 64; // 64k 
    BufferChunk rchunk(recvSize+1);
    retCode = s.receiveChunk(rchunk, 0);
    //printf("recv: %d\n", retCode);
    if (retCode == SOCKET_ERROR) {
        printf("SOCKET_ERROR\n");
        return false;
    }

    // Null terminate so we can print
    // as zstring
    rchunk.fData[retCode] = 0;

    printf("\n== RESPONSE BEGIN (%s)==\n", hostname);
    printf("%s", rchunk.fData);
    printf("\n== RESPONSE END (%s)==\n", hostname);



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
