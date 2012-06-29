/**
 * @file    OASSound.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "OASSound.h"

using namespace oasclient;

OASSound::OASSound(const std::string &sPath, const std::string &sFilename)
{
    _init();
    _path = sPath;
    _filename = sFilename;

    _handle = _getHandleFromServer();

    if (-1 == _handle)
    {
        if (OASClientInterface::sendFile(_path, _filename))
        {
            _handle = _getHandleFromServer();
        }
    }

    // If the handle is greater than or equal to 0, this sound is valid
    if (0 <= _handle)
        _isValid = true;
}

OASSound::OASSound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds)
{
    _init();

    if (OASClientInterface::writeToServer("WAVE %d, %f, %f, %f", waveType,
            frequency,
            phaseShift,
            durationInSeconds))
    {
        char *handleString;
        size_t length;

        if (OASClientInterface::readFromServer(handleString, length))
        {
            _handle = atol(handleString);
        }
    }
}

OASSound::~OASSound()
{
    if (isValid())
    {
        OASClientInterface::writeToServer("RHDL %ld", _handle);
    }
    _path.clear();
    _filename.clear();
}

void OASSound::_init()
{
    _isValid = false;
    _handle = -1;
    _posX = _posY = _posZ = 0;
    _dirX = _dirY = _dirZ = 0;
    _velX = _velY = _velZ = 0;
    _pitch = 1;
    _gain = 1;
    _wasPlaying;
    _isLooping;
}

long OASSound::_getHandleFromServer()
{
    if (!OASClientInterface::writeToServer("GHDL %s", _filename.c_str()))
    {
        return -1;
    }

    char *handleString;
    size_t length;

    if (!OASClientInterface::readFromServer(handleString, length))
    {
        return -1;
    }

    return atol(handleString);
}

bool OASSound::isValid()
{
    return _isValid;
}

long OASSound::getHandle()
{
    return _handle;
}


bool OASSound::play()
{
    if (!isValid())
        return false;

    _wasPlaying = OASClientInterface::writeToServer("PLAY %ld", _handle);
    return _wasPlaying;
}

bool OASSound::stop()
{
    if (!isValid())
        return false;

    return OASClientInterface::writeToServer("STOP %ld", _handle);
}

bool OASSound::setLoop(bool loop)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSLP %ld %ld", _handle, loop ? 1 : 0);

    if (result)
        _isLooping = loop;

    return result;
}

bool OASSound::setGain(float gain)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSVO %ld %f", _handle, gain);

    if (result)
        _gain = gain;

    return result;
}

bool OASSound::setPosition(float x, float y, float z)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSPO %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _posX = x;
        _posY = y;
        _posZ = z;
    }
    return result;
}

bool OASSound::setDirection(float angle)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSDI %ld %f", _handle, angle);

    if (result)
    {
        _dirX = sin(angle);
        _dirY = 0;
        _dirZ = cos(angle);
    }
    return result;
}

bool OASSound::setDirection(float x, float y, float z)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSDI %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _dirX = x;
        _dirY = y;
        _dirZ = z;
    }
    return result;
}

bool OASSound::setVelocity(float x, float y, float z)
{   
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SSVE %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _velX = x;
        _velY = y;
        _velZ = z;
    }
    return result;
}

bool OASSound::setPitch(float pitchFactor)
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("SPIT %ld %f", _handle, pitchFactor);

    if (result)
    {
        _pitch = pitchFactor;
    }

    return result;
}

bool OASSound::fade(float finalGain, float durationInSeconds)
{
    if (!isValid())
        return false;

    return OASClientInterface::writeToServer("FADE %ld %f %f",
                                            _handle, finalGain, durationInSeconds);

}

