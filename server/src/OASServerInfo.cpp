#include "OASServerInfo.h"

using namespace oas;

ServerInfo::ServerInfo():
	_cacheDirectory(""),
	_port(0),
	_audioDeviceString(""),
	_useGUI(true)
{

}

ServerInfo::ServerInfo( std::string const& cacheDirectory, long int port):
                        _cacheDirectory(cacheDirectory),
                        _port(port),
                        _audioDeviceString(""),
                        _useGUI(true)
{
    
}

std::string const& ServerInfo::getCacheDirectory() const
{
    return this->_cacheDirectory;
}

long int ServerInfo::getPort() const
{
    return this->_port;
}

std::string const& ServerInfo::getAudioDeviceString() const
{
    return this->_audioDeviceString;
}

void ServerInfo::setAudioDeviceString(std::string const& audioDevice)
{
    this->_audioDeviceString = audioDevice;
}

bool ServerInfo::useGUI() const
{
    return this->_useGUI;
}

void ServerInfo::setGUI(bool useGUI)
{
    this->_useGUI = useGUI;
}

