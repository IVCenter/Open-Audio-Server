/**
 * @file    OASSound.h
 * @author  Shreenidhi Chowkwale
 *
 * This file contains two classes: OASSound, and OASSoundListener.
 * OASSound objects correspond to individual sound sources, whereas
 * OASSoundListener is a singleton that can be used to modify the
 * attributes of the listener.
 */

#ifndef _OAS_SOUND_H_
#define _OAS_SOUND_H_

#include <vector>
#include <cmath>
#include <sstream>
#include "OASClientInterface.h"

namespace oasclient
{

/**
 * @class OASSound
 * Each instance of this object corresponds to one fully independent sound source that can be
 * positioned, played, etc.
 */
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
     * The state is defined as follows:
     * UNKNOWN: state is unknown, or the source does not exist
     * INITIAL: source has a buffer allocated to it, but it has never been used
     * PLAYING: source is playing or has finished playing all the way through
     * PAUSED:  source is paused at a specific point, and playback will resume from here
     * STOPPED: source is stopped and playback will resume from the beginning
     * DELETED: source is in the process of being deleted, or was just deleted
     */
    enum SoundState
    {
        ST_UNKNOWN = 0,
        ST_INITIAL = 1,
        ST_PLAYING = 2,
        ST_PAUSED =  3,
        ST_STOPPED = 4,
        ST_DELETED = 5
    };

    /**
     * Create a new sound source based on a file with the given path and filename.
     * The full path that will be used is "path/filename".
     * @param path This is the path to the folder containing the file.
     *              (e.g. "/home/user/data")
     * @param filename This is the actual name of the file. (e.g. "funnysound.wav")
     */
    OASSound(const std::string &path, const std::string &filename);

    /**
     * Create a new sound source based on the given filepath. The filename will be extracted from
     * the filepath automatically. If no filepath is specified, the sound object does not
     * correspond to any sound, and does not attempt any network communication.
     * @param filepath This is the full path to the file. (e.g. "/home/user/data/funnysound.wav")
     */
    OASSound(const std::string &filepath = "");

    /**
     * Create a new sound source based on the specified wavetype, frequency and phaseshift.
     */
    OASSound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds);

    /**
     * Releases associated sound with server, if any.
     */
    ~OASSound();

    /**
     * Initialize this source with the sound file specified by the given path and filename. If the
     * sound source is already initialized, it will be reset first.
     *
     * If both the path and the filename parameters are provided, then they are concatenated with
     * a forward slash in between, to produce the full path (e.g. "path/filename").
     *
     * If filename is not specified, it is assumed that the path parameter is already the full
     * path to the file. In this case, the filename component will be extracted.
     *
     * If neither the path nor the filename are specified, this method will reset the object and
     * return false.
     *
     * @param path This is the path to the folder containing the file (e.g. "/home/user/data")
     * @param filename This is the actual name of the file. (e.g. "funnysound.wav")
     */
    bool initialize(const std::string &path = "", const std::string &filename = "");

    /**
     * Initialize this source with the specified waveform. If the source is already initialized,
     * it will be reset and then reassociated with the desired waveform.
     */
    bool initialize(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds);

    /**
     * Determine whether or not this sound object is valid.
     */
    bool isValid() const;

    /**
     * Get the internal handle corresponding to this sound source.
     */
    long getHandle() const;

    /**
     * Play the sound source. If the source is already playing, play will restart from
     * the beginning.
     */
    bool play();

    /**
     * Stop playing the sound source. If the source is already stopped, this will have
     * no effect. Playback will resume from the beginning the next time play() is used.
     */
    bool stop();

    /**
     * Pause the sound source. The current playback position is saved. Playback will resume
     * from this location ONLY when play() is called on this sound source. Pausing a source
     * that is already paused will have no effect.
     */
    bool pause();

    /**
     * Set the sound source's playback position, in seconds. If the source is already playing,
     * playback will skip to the desired location. Otherwise, the playback position will be
     * applied next time the source is played. If the specified value is outside the bounds
     * of the sound source, this will have no effect.
     */
    bool setPlaybackPosition(float seconds);

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

    /**
     * Update the current state of the sound source by asking the server.
     */
    bool updateState();

    /**
     * Get the state of the sound source, as stored locally in this object.
     * NOTE: This does not communicate with the server to check the state.
     *       Use updateState() first to retrieve the most current state.
     */
    SoundState getState() const;

    /**
     * @brief Get the position of this sound source as a std::vector
     */
    std::vector<float> getPosition() const;

    /**
     * @brief Get the direction of this sound source as a std::vector
     */
    std::vector<float> getDirection() const;

    /**
     * @brief Get the velocity of this sound source as a std::vector
     */
    std::vector<float> getVelocity() const;

    /**
     * @brief Get the pitch of this sound source
     */
    float getPitch() const;

    /**
     * @brief Get the gain (volume) of this sound source
     */
    float getGain() const;

    /**
     * @brief Check if this sound source is set to loop or not
     */
    bool isLooping() const;

private:
    void _init();
    void _reset();
    void _getHandleFromServer();
    void _splitFilename(const std::string &joinedFilepath);

    int _handle;
    std::string _filename;
    std::string _path;

    SoundState _state;

    float _posX, _posY, _posZ;
    float _dirX, _dirY, _dirZ;
    float _velX, _velY, _velZ;

    float _pitch;
    float _gain;
    bool _isLooping;

    bool _isValid;
};

/**
 * @class OASSoundListener
 * Used to communicate properties of the listener to the server.
 */
class OASSoundListener
{
public:

    /**
     * Retrieve a reference to the singleton OASSoundListener object, which
     * can then be used to modify listener attributes.
     */
    static OASSoundListener& getInstance();

    /**
     * Modify the global (listener's) gain level. The default is 1, and a value of 0 will mute all
     * sounds completely.
     */
    bool setListenerGain(float gain);

    /**
     * Modify the listener's position. Default is <0, 0, 0>
     */
    bool setListenerPosition(float x, float y, float z);

    /**
     * Modify the listener's velocity. Default is <0, 0, 0>. Note that this is ONLY used for
     * doppler effect calculations, and does not cause the position to be updated.
     * If the velocity is NOT set, then doppler effect simulation will not occur.
     */
    bool setListenerVelocity(float x, float y, float z);

    /**
     * Modify the listener's orientation, in terms of a "look-at" vector and "up" vector.
     * Defaults are <0, 0, -1> and <0, 1, 0>, respectively.
     */
    bool setListenerOrientation(float atX, float atY, float atZ,
                                float upX, float upY, float upZ);


    /**
     * These constants are used to designate which parameter to modify.
     */
    enum SoundRenderingParameter
    {
        SPEED_OF_SOUND = 1,
        DOPPLER_FACTOR = 2,
    };

    /**
     * Set sound rendering parameters
     */
    bool setSoundRenderingParameters(SoundRenderingParameter whichParameter, float value);

    /**
     * @brief Get the current position of the listener as a std::vector
     */
    std::vector<float> getPosition() const;

    /**
     * Get the orientation as one vector, containing first the three "look-at" vector
     * coordinates, and then the three "up" vector coordinates.
     */
    std::vector<float> getOrientation() const;

    /**
     * @brief Get the velocity as a std::vector
     */
    std::vector<float> getVelocity() const;

    /**
     * @brief Get the gain set for the listener
     */
    float getGain() const;

private:
    /**
     * @brief Private constructor, for singleton
     */
    OASSoundListener();

    float _posX, _posY, _posZ;
    float _velX, _velY, _velZ;
    float _atX, _atY, _atZ;
    float _upX, _upY, _upZ;
    float _gain;

};
}

#endif // _OAS_SOUND_H_

