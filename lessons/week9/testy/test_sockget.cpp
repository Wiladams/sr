#include <cstring>
#include "w32_socket.hpp"

bool host_serv(const char *hostname, char *servicename, int family, int socktype, bool isnumericstring=false)
{
    //hostname = hostname or "localhost"
    //family = family or AF_UNSPEC;
    //socktype = socktype or SOCK_STREAM;

    int err;
    addrinfo hints;
    struct addrinfo *ppResult;

    // need to zero out some members before using
    memset(&hints, 0, sizeof(hints));

	hints.ai_family = family;
	hints.ai_socktype = socktype;

	hints.ai_flags = AI_CANONNAME;    // return canonical name
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	if (isnumericstring) {
        //hints.ai_flags = AI_NUMERICHOST;
    }

    err = getaddrinfo(hostname, servicename, &hints, &ppResult);

	if (err != 0) {
        return false;
    }

	return true;
}

/*
    family - AF_INET
    socktype = SOCK_STREAM
*/
void CreateSocketAddress(const char hostname, int port, int family, int socktype)
{
/*
    // in case the hostname includes the port number
	local hostportoffset = hostname:find(':')
	if hostportoffset then
		port = tonumber(hostname:sub(hostportoffset+1))
		hostname = hostname:sub(1,hostportoffset-1)
		print("CreateSocketAddress() - Modified: ", hostname, port)
	end
*/

	local addressinfo, err = host_serv(hostname, nil, family, socktype)

	if not addressinfo then
		return nil, err
	end

	-- clone one of the addresses
	local oneaddress = newSocketAddress(addressinfo.ai_addr, addressinfo.ai_addrlen)
	oneaddress:SetPort(port)

	-- free the addrinfos structure
	err = ws2_32.freeaddrinfo(addressinfo)

	return oneaddress;
}

void setup()
{
    // Create a socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // connect to bing.com
    CreateSocketAddress("bing.com", 80, AF_INET, SOCK_STREAM);
    bool success = connect(s, const struct sockaddr * name, int namelen) == 0;

    // get some data

    // close socket
}

void main()
{
    setup();
}