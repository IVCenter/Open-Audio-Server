/**
 * @file OASServer.h
 * @author Shreenidhi Chowkwale
 */

#ifndef _OAS_SERVER_H_
#define _OAS_SERVER_H_

#include <iostream>
#include <queue>
#include <sys/types.h>
#include <pthread.h>
#include <AL/alut.h>

#include "OASFileHandler.h"
#include "OASSocketHandler.h"
#include "OASMessage.h"
#include "OASAudioHandler.h"
#include "OASServerInfo.h"
#include "OASLogger.h"
#include "OASTime.h"
#include "config.h"

#ifdef FLTK_FOUND
#include <FL/Fl.H>
#endif

namespace oas
{
class Server
{
public:
    static Server& getInstance();
    void initialize(int argc, char **argv);
    const ServerInfo* getServerInfo() const;

private:
    Server();
    pthread_t _serverThread;

    ServerInfo* _serverInfo;

    // private worker methods
    bool _readConfigFile(int argc, char **argv);

    static void* _serverLoop(void *parameter);
    static void* _serverLoopNoGUI(void *parameter);
    void _processMessage(const Message &message);
    void _fatalError(const char *errorMessage);
    static void _atExit();

    static void _computeTimeout(struct timespec &timeout, unsigned long timeoutSeconds,
                                unsigned long timeoutNanoseconds);

};
}

#endif

