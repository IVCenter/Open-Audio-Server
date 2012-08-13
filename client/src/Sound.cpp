/**
 * @file    Sound.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "Sound.h"

using namespace oasclient;

Sound::Sound(const std::string &path, const std::string &filename)
{
    initialize(path, filename);
}

Sound::Sound(const std::string &filepath)
{
    initialize(filepath);
}

Sound::Sound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds)
{
    initialize(waveType, frequency, phaseShift, durationInSeconds);
}

Sound::~Sound()
{
    _reset();
}

bool Sound::initialize(const std::string &path, const std::string &filename)
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
        if (ClientInterface::sendFile(_path, _filename))
        {
            _getHandleFromServer();
        }
    }

    // If the handle is greater than or equal to 0, this sound is valid
    if (0 <= _handle)
        _isValid = true;

    return isValid();
}

bool Sound::initialize(enum WaveformType waveType, float frequency, float phaseShift, float durationInSeconds)
{
    _reset();

    if (ClientInterface::writeToServer("WAVE %d, %f, %f, %f", waveType,
            frequency,
            phaseShift,
            durationInSeconds))
    {
        ClientInterface::readIntegerFromServer(_handle);
    }

    // If the handle is greater than or equal to 0, this sound is valid
    if (0 <= _handle)
        _isValid = true;

    return isValid();
}

void Sound::release()
{
    _reset();
}

bool Sound::isValid() const
{
    return _isValid;
}

long Sound::getHandle() const
{
    return _handle;
}


bool Sound::play()
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("PLAY %ld", _handle);
    if (result)
        _state = ST_PLAYING;

    return result;
}

bool Sound::stop()
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("STOP %ld", _handle);
    if (result)
        _state = ST_STOPPED;

    return result;
}

bool Sound::pause()
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("PAUS %ld", _handle);
    if (result)
        _state = ST_PAUSED;

    return result;
}

bool Sound::setPlaybackPosition(float seconds)
{
    if (!isValid())
        return false;

    return ClientInterface::writeToServer("SSEC %ld %f", _handle, seconds);
}

bool Sound::setLoop(bool loop)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSLP %ld %ld", _handle, loop ? 1 : 0);

    if (result)
        _isLooping = loop;

    return result;
}

bool Sound::setGain(float gain)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSVO %ld %f", _handle, gain);

    if (result)
        _gain = gain;

    return result;
}

bool Sound::setPosition(float x, float y, float z)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSPO %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _posX = x;
        _posY = y;
        _posZ = z;
    }
    return result;
}

bool Sound::setDirection(float angle)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSDI %ld %f", _handle, angle);

    if (result)
    {
        _dirX = sin(angle);
        _dirY = 0;
        _dirZ = cos(angle);
    }
    return result;
}

bool Sound::setDirection(float x, float y, float z)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSDI %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _dirX = x;
        _dirY = y;
        _dirZ = z;
    }
    return result;
}

bool Sound::setVelocity(float x, float y, float z)
{   
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SSVE %ld %f %f %f", _handle, x, y, z);

    if (result)
    {
        _velX = x;
        _velY = y;
        _velZ = z;
    }
    return result;
}

bool Sound::setPitch(float pitchFactor)
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("SPIT %ld %f", _handle, pitchFactor);

    if (result)
    {
        _pitch = pitchFactor;
    }

    return result;
}

bool Sound::fade(float finalGain, float durationInSeconds)
{
    if (!isValid())
        return false;

    _fadeFinalGain = finalGain;
    _fadeInitialGain = getGain();
    _fadeGainDiff = _fadeFinalGain - _fadeInitialGain;

    _fadeDuration = durationInSeconds;
    _fadeStartTime.update(Time::OAS_CLOCK_MONOTONIC);
    _fadeEndTime = _fadeStartTime + Time(_fadeDuration);

    return ClientInterface::writeToServer("FADE %ld %f %f",
                                            _handle, finalGain, durationInSeconds);
}

bool Sound::setRenderingParameter(RenderingParameter whichParameter, float value)
{
    if (!isValid())
        return false;

    return ClientInterface::writeToServer("SPAR %ld %ld %f", _handle, whichParameter, value);
}

bool Sound::updateState()
{
    if (!isValid())
        return false;

    bool result = ClientInterface::writeToServer("STAT %ld", _handle);
    
    if (result)
    {
        int state;

        result = ClientInterface::readIntegerFromServer(state);
        if (result)
        {
            if (state <= ST_UNKNOWN || state > ST_DELETED)
            {
                _state = ST_UNKNOWN;
            }
            else
            {
                _state = (Sound::SoundState) state;
            }
        }
    }

    return result;
}

Sound::SoundState Sound::getState() const
{
    return _state;
}

std::vector<float> Sound::getPosition() const
{
    std::vector<float> retvec(3);

    retvec[0] = _posX;
    retvec[1] = _posY;
    retvec[2] = _posZ;

    return retvec;
}

std::vector<float> Sound::getDirection() const
{
    std::vector<float> retvec(3);

    retvec[0] = _dirX;
    retvec[1] = _dirY;
    retvec[2] = _dirZ;

    return retvec;
}

std::vector<float> Sound::getVelocity() const
{
    std::vector<float> retvec(3);

    retvec[0] = _velX;
    retvec[1] = _velY;
    retvec[2] = _velZ;

    return retvec;
}

float Sound::getPitch() const
{
    return _pitch;
}

float Sound::getGain()
{
    // If the sound source is in the middle of a fade, update the internal gain accordingly
    if (isFading())
    {
        if (_gain == _fadeFinalGain)
        {
            _fadeEndTime.reset();
        }
        else
        {
            Time currTime;
            currTime.update(Time::OAS_CLOCK_MONOTONIC);

            if (currTime >= _fadeEndTime)
            {
                _fadeEndTime.reset();
                _gain = _fadeFinalGain;
            }

            float timePassed = (currTime - _fadeStartTime).asDouble();
            float ratioOfTimeProgress = timePassed / _fadeDuration;

            if (ratioOfTimeProgress > 1)
                ratioOfTimeProgress = 1;

            _gain = ((ratioOfTimeProgress * _fadeGainDiff) + _fadeInitialGain);
        }
    }

    return _gain;
}

bool Sound::isLooping() const
{
    return _isLooping;
}

bool Sound::isFading() const
{
    return _fadeEndTime.hasTime();
}

void Sound::_init()
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
    _fadeDuration = 0;
    _fadeEndTime.reset();
    _fadeStartTime.reset();
    _fadeFinalGain = -1;
    _fadeInitialGain = 0;
}

void Sound::_reset()
{
    if (isValid())
        ClientInterface::writeToServer("RHDL %ld", _handle);

    _path.clear();
    _filename.clear();

    _init();
}

void Sound::_getHandleFromServer()
{
    if (ClientInterface::writeToServer("GHDL %s", _filename.c_str()))
    {
        ClientInterface::readIntegerFromServer(_handle);
    }
}

void Sound::_splitFilename(const std::string &joinedFilePath)
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

