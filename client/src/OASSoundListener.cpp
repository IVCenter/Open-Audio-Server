/**
 * @file    OASSoundListener.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "OASSound.h"

using namespace oasclient;

// static
OASSoundListener& OASSoundListener::getInstance()
{
    static OASSoundListener instance;

    return instance;
}

bool OASSoundListener::setListenerGain(float gain)
{
    bool result = OASClientInterface::writeToServer("GAIN %f", gain);
    if (result)
    {
        _gain = gain;
    }
    return result;
}


bool OASSoundListener::setListenerPosition(float x, float y, float z)
{
    bool result = OASClientInterface::writeToServer("SLPO %f %f %f", x, y, z);
    if (result)
    {
        _posX = x;
        _posY = y;
        _posZ = z;
    }
    return result;
}


bool OASSoundListener::setListenerVelocity(float x, float y, float z)
{
    bool result = OASClientInterface::writeToServer("SLVE %f %f %f", x, y, z);
    if (result)
    {
        _velX = x;
        _velY = y;
        _velZ = z;
    }

    return result;
}

bool OASSoundListener::setListenerOrientation(float atX, float atY, float atZ,
        float upX, float upY, float upZ)
{
    bool result = OASClientInterface::writeToServer("SLOR %f %f %f %f %f %f",
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

std::vector<float> OASSoundListener::getPosition() const
{
    std::vector<float> retvec(3);

    retvec[0] = _posX;
    retvec[1] = _posY;
    retvec[2] = _posZ;

    return retvec;
}

std::vector<float> OASSoundListener::getOrientation() const
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

std::vector<float> OASSoundListener::getVelocity() const
{
    std::vector<float> retvec(3);

    retvec[0] = _velX;
    retvec[1] = _velY;
    retvec[2] = _velZ;

    return retvec;
}

float OASSoundListener::getGain() const
{
    return _gain;
}

// private constructor
OASSoundListener::OASSoundListener()
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

