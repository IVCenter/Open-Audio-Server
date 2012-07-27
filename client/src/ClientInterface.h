/**
 * @file    ClientInterface.h
 * @author  Shreenidhi Chowkwale
 */

#ifndef _OAS_CLIENT_INTERFACE_H_
#define _OAS_CLIENT_INTERFACE_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace oasclient
{

/**
 * @class ClientInterface
 * This class provides the network interface for the client to connect to the server.
 */
class ClientInterface
{
public:

    /**
     * Define an anonymous enum to hold this PACKET_SIZE constant value.
     */
    enum { PACKET_SIZE = 1024};

    friend class Sound;
    friend class Listener;

    /**
     * Initialize the connection to the audio server with the specified host location and port.
     */
    static bool initialize(const std::string &host, const unsigned short port);

    /**
     * Returns true if the client interface is initialized and connected to the server.
     */
    static bool isInitialized();

    /**
     * Shutdown the connection to the server and clean up any allocated resources.
     */
    static bool shutdown();

protected:
    /**
     * Write data to the server, using a format similar to the printf() family of functions.
     */
    static bool writeToServer(const char *format, ...);

    /**
     * Read data from the server. Data and number of bytes are returned by reference via the
     * function parameters. If successful, the caller is responsible for freeing the data that
     * has been dynamically allocated!
     */
    static bool readFromServer(char *&data, size_t &count);

    /**
     * Read an integer from the server. If it fails, the value is set to -1.
     */
    static bool readIntegerFromServer(int &value);

    /**
     * Transfer the file with the given path and filename to the server.
     */
    static bool sendFile(const std::string &sPath, const std::string &sFilename);

private:
    static int _socketFD;

};

}
#endif // _OAS_CLIENT_INTERFACE_H_

