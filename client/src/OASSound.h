/**
 * @file    OASSound.h
 * @author  Shreenidhi Chowkwale
 */

#ifndef _OAS_SOUND_H_
#define _OAS_SOUND_H_

#include <cmath>
#include "OASClientInterface.h"

namespace oasclient
{

class OASSound
{
public:

    /**
     * These are the waveform types supported by the server for sound sources generated based
     * on simple waves. The sine wave is the most commonly requested type.
     */
    enum WaveformType
    {
        SINE = 1,
        SQUARE = 2,
        SAWTOOTH = 3,
        WHITENOISE = 4,
        IMPULSE = 5
    };

    /**
     * Create a new sound source based on a file with the given path and filename.
     */
    OASSound(const std::string &sPath, const std::string &sFilename);

    /**
     * Create a new sound source based on the specified wavetype, frequency and phaseshift.
     */
    OASSound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds);
    ~OASSound();

    bool isValid();

    long getHandle();

    /**
     * Play the sound source. If the source is already playing, play will restart from
     * the beginning.
     */
    bool play();

    /**
     * Stop playing the sound source. If the source is already stopped, this will have
     * no effect.
     */
    bool stop();

    /**
     * Set the sound source to loop or stop looping. By default, sound sources do not loop.
     */
    bool setLoop(bool loop);

    /**
     * Set the gain (volume) of the sound source. The default is 1.0. A value of 0.0 will mute,
     * and values greater than 1.0 (for amplification) are not guaranteed to have an effect.
     */
    bool setGain(float gain);

    /**
     * Set the position of the sound source. The default position is <0, 0, 0>.
     */
    bool setPosition(float x, float y, float z);

    /**
     * Set the direction of the sound source by specifying a directional vector. By default, sound
     * sources are not directional, with a direction vector of <0, 0, 0>. Non-directional sources
     * emit sound equally in all directions, similar to point light sources. Directional sources
     * emit sound in a cone.
     */
    bool setDirection(float x, float y, float z);

    /**
     * Set the direction of the sound source by specifying an angle in the X-Z plane. Note that
     * there is no default value for this angle, because it is not possible to represent
     * a lack of directionality using angles.
     */
    bool setDirection(float angle);

    /**
     * Set the velocity of the sound source. The velocity is used ONLY for the doppler effect
     * calculations. The server does not internally update the position based on the specify the
     * velocity.
     */
    bool setVelocity(float x, float y, float z);

    /**
     * Set the pitch of the sound. This works by changing the rate of playback of the sound source.
     * The default pitch is 1.0. A higher-than-default pitch will result in faster playback,and a
     * lower-than-default pitch will result in slower playback.
     * @param pitchFactor The value to set the the pitch to. Multiplying the pitchFactor by 2 will
     *                    increase the pitch by one octave, and dividing the pitchFactor by 2 will
     *                    decrease the pitch by one octave. Values must be greater than 0. Default
     *                    is 1.0.
     */
    bool setPitch(float pitchFactor);

    /**
     * Gradually and linearly change the sound's current gain value to the specified gain value, over
     * the given duration, in seconds. This can be used to slowly fade a sound's volume in or out.
     */
    bool fade(float finalGain, float durationInSeconds);


private:
    void _init();
    long _getHandleFromServer();

    long _handle;
    std::string _filename;
    std::string _path;

    float _posX, _posY, _posZ;
    float _dirX, _dirY, _dirZ;
    float _velX, _velY, _velZ;

    float _pitch;
    float _gain;
    bool _wasPlaying;
    bool _isLooping;

    bool _isValid;
};

}

#endif // _OAS_SOUND_H_

