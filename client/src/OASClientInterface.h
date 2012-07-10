/**
 * @file    OASClientInterface.h
 * @author  Shreenidhi Chowkwale
 */

#ifndef _OAS_CLIENT_INTERFACE_H_
#define _OAS_CLIENT_INTERFACE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace oasclient
{

/**
 * @class OASClientInterface
 * This class provides the network interface for the client to connect to the server.
 */
class OASClientInterface
{
public:

    /**
     * Define an anonymous enum to hold this PACKET_SIZE constant value.
     */
    enum { PACKET_SIZE = 1024};

    /**
     * Initialize the connection to the audio server with the specified host location and port.
     */
    static bool initialize(const std::string &host, const unsigned short port);

    /**
     * Shutdown the connection to the server and clean up any allocated resources.
     */
    static bool shutdown();

    /**
     * Write data to the server, using a format similar to the printf() family of functions.
     * NOTE: Client applications should not need to call this function.
     */
    static bool writeToServer(const char *format, ...);

    /**
     * Read data from the server. Data and number of bytes are returned by reference via the
     * function parameters.
     * NOTE: Client applications should not need to call this function.
     */
    static bool readFromServer(char *&data, size_t &count);

    /**
     * Transfer the file with the given path and filename to the server.
     * NOTE: Client applications should not need to call this function.
     */
    static bool sendFile(const std::string &sPath, const std::string &sFilename);

private:
    static int _socketFD;

};

}
#endif // _OAS_CLIENT_INTERFACE_H_

