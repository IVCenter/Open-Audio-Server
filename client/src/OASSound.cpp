/**
 * @file    OASSound.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "OASSound.h"

using namespace oasclient;

OASSound::OASSound(const std::string &path, const std::string &filename)
{
    initialize(path, filename);
}

OASSound::OASSound(const std::string &filepath)
{
    initialize(filepath);
}

OASSound::OASSound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds)
{
    initialize(waveType, frequency, phaseShift, durationInSeconds);
}

OASSound::~OASSound()
{
    _reset();
}

bool OASSound::initialize(const std::string &path, const std::string &filename)
{
    _reset();

    if (0 == filename.length())
    {
        if (0 == path.length())
            return false;

        _splitFilename(path);
    }
    else
    {
        _path = path;
        _filename = filename;
    }

    _getHandleFromServer();

    if (-1 == _handle)
    {
        if (OASClientInterface::sendFile(_path, _filename))
        {
            _getHandleFromServer();
        }
    }

    // If the handle is greater than or equal to 0, this sound is valid
    if (0 <= _handle)
        _isValid = true;

    return isValid();
}

bool OASSound::initialize(enum WaveformType waveType, float frequency, float phaseShift, float durationInSeconds)
{
    _reset();

    if (OASClientInterface::writeToServer("WAVE %d, %f, %f, %f", waveType,
            frequency,
            phaseShift,
            durationInSeconds))
    {
        OASClientInterface::readIntegerFromServer(_handle);
    }

    // If the handle is greater than or equal to 0, this sound is valid
    if (0 <= _handle)
        _isValid = true;

    return isValid();
}

bool OASSound::isValid() const
{
    return _isValid;
}

long OASSound::getHandle() const
{
    return _handle;
}


bool OASSound::play()
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("PLAY %ld", _handle);
    if (result)
        _state = ST_PLAYING;

    return result;
}

bool OASSound::stop()
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("STOP %ld", _handle);
    if (result)
        _state = ST_STOPPED;

    return result;
}

bool OASSound::pause()
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("PAUS %ld", _handle);
    if (result)
        _state = ST_PAUSED;

    return result;
}

bool OASSound::setPlaybackPosition(float seconds)
{
    if (!isValid())
        return false;

    return OASClientInterface::writeToServer("SSEC %ld %f", _handle, seconds);
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

bool OASSound::updateState()
{
    if (!isValid())
        return false;

    bool result = OASClientInterface::writeToServer("STAT %ld", _handle);
    
    if (result)
    {
        int state;

        result = OASClientInterface::readIntegerFromServer(state);
        if (result)
        {
            if (state <= ST_UNKNOWN || state > ST_DELETED)
            {
                _state = ST_UNKNOWN;
            }
            else
            {
                _state = (OASSound::SoundState) state;
            }
        }
    }

    return result;
}

OASSound::SoundState OASSound::getState() const
{
    return _state;
}

std::vector<float> OASSound::getPosition() const
{
    std::vector<float> retvec(3);

    retvec[0] = _posX;
    retvec[1] = _posY;
    retvec[2] = _posZ;

    return retvec;
}

std::vector<float> OASSound::getDirection() const
{
    std::vector<float> retvec(3);

    retvec[0] = _dirX;
    retvec[1] = _dirY;
    retvec[2] = _dirZ;

    return retvec;
}

std::vector<float> OASSound::getVelocity() const
{
    std::vector<float> retvec(3);

    retvec[0] = _velX;
    retvec[1] = _velY;
    retvec[2] = _velZ;

    return retvec;
}

float OASSound::getPitch() const
{
    return _pitch;
}

float OASSound::getGain() const
{
    return _gain;
}

bool OASSound::isLooping() const
{
    return _isLooping;
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
    _isLooping = false;
    _state = ST_UNKNOWN;
}

void OASSound::_reset()
{
    if (isValid())
        OASClientInterface::writeToServer("RHDL %ld", _handle);

    _path.clear();
    _filename.clear();

    _init();
}

void OASSound::_getHandleFromServer()
{
    if (OASClientInterface::writeToServer("GHDL %s", _filename.c_str()))
    {
        OASClientInterface::readIntegerFromServer(_handle);
    }
}

void OASSound::_splitFilename(const std::string &joinedFilePath)
{
    size_t pathPos = joinedFilePath.find_last_of('/');

    // If the forward slash wasn't found, use empty path field
    if (pathPos == joinedFilePath.npos)
    {
        _path = "";
        _filename = joinedFilePath;
    }
    else
    {
        _path = joinedFilePath.substr(0, pathPos);
        _filename = joinedFilePath.substr(pathPos + 1);
    }
}

