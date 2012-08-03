/**
 * @file    Listener.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "Listener.h"

using namespace oasclient;

// static
Listener& Listener::getInstance()
{
    static Listener instance;

    return instance;
}

bool Listener::setListenerGain(float gain)
{
    bool result = ClientInterface::writeToServer("GAIN %f", gain);
    if (result)
    {
        _gain = gain;
    }
    return result;
}


bool Listener::setListenerPosition(float x, float y, float z)
{
    bool result = ClientInterface::writeToServer("SLPO %f %f %f", x, y, z);
    if (result)
    {
        _posX = x;
        _posY = y;
        _posZ = z;
    }
    return result;
}


bool Listener::setListenerVelocity(float x, float y, float z)
{
    bool result = ClientInterface::writeToServer("SLVE %f %f %f", x, y, z);
    if (result)
    {
        _velX = x;
        _velY = y;
        _velZ = z;
    }

    return result;
}

bool Listener::setListenerOrientation(float atX, float atY, float atZ,
        float upX, float upY, float upZ)
{
    bool result = ClientInterface::writeToServer("SLOR %f %f %f %f %f %f",
                                        atX, atY, atZ, upX, upY, upZ);
    if (result)
    {
        _atX = atX;
        _atY = atY;
        _atZ = atZ;
        _upX = upX;
        _upY = upY;
        _upZ = upZ;
    }
    return result;
}

bool Listener::setGlobalRenderingParameters(GlobalRenderingParameter whichParameter, float value)
{
    return ClientInterface::writeToServer("PARA %d %f", whichParameter, value);
}

std::vector<float> Listener::getPosition() const
{
    std::vector<float> retvec(3);

    retvec[0] = _posX;
    retvec[1] = _posY;
    retvec[2] = _posZ;

    return retvec;
}

std::vector<float> Listener::getOrientation() const
{
    std::vector<float> retvec(6);

    retvec[0] = _atX;
    retvec[1] = _atY;
    retvec[2] = _atZ;
    retvec[3] = _upX;
    retvec[4] = _upY;
    retvec[5] = _upZ;

    return retvec;
}

std::vector<float> Listener::getVelocity() const
{
    std::vector<float> retvec(3);

    retvec[0] = _velX;
    retvec[1] = _velY;
    retvec[2] = _velZ;

    return retvec;
}

float Listener::getGain() const
{
    return _gain;
}

// private constructor
Listener::Listener()
{
    _gain = 1.0;

    _posX = 0;
    _posY = 0;
    _posZ = 0;

    _velX = 0;
    _velY = 0;
    _velZ = 0;

    _upX = 0;
    _upY = -1;
    _upZ = 0;

    _atX = 0;
    _atY = 0;
    _atZ = -1;
}

