/**
 * @file    Sound.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_SOUND_H_
#define _OAS_SOUND_H_

#include <vector>
#include <cmath>
#include <sstream>
#include "ClientInterface.h"
#include "config.h"

#ifdef OSG_FOUND
#include <osg/Node>
#endif

namespace oasclient
{

/**
 * @brief Each instance of this object corresponds to one fully independent sound source that can be
 * positioned, played, etc.
 */
#ifdef OSG_FOUND
class Sound : public osg::Node
#else
class Sound
#endif
{
public:

    /**
     * These are the waveform types supported by the server for sound sources generated based
     * on simple waves. The sine wave is the most commonly requested type.
     */
    enum WaveformType
    {
        SINE = 1,       /**< Waveform based on a sine wave (creates a predictable beeping noise) */
        SQUARE = 2,     /**< Waveform based on a square wave */
        SAWTOOTH = 3,   /**< Waveform based on a sawtooth wave */
        WHITENOISE = 4, /**< Create whitenoise */
        IMPULSE = 5     /**< Create an impulse wave */
    };

    /**
     * Describes the state of the sound source.
     */
    enum SoundState
    {
        ST_UNKNOWN = 0, /**< State is unknown, or the source does not exist */
        ST_INITIAL = 1, /**< Source has a buffer allocated to it, but it has never been used (i.e. never been played) */
        ST_PLAYING = 2, /**< Source is playing or has finished playing all the way through */
        ST_PAUSED =  3, /**< Source is paused at a specific point, and playback will resume from here */
        ST_STOPPED = 4, /**< Source is stopped and playback will resume from the beginning */
        ST_DELETED = 5  /**< Source is in the process of being deleted, or was just deleted */
    };

    /**
     * These constants define the sound rendering parameters that can be modified.
     *
     * @par Example of Using the Sound Rendering Parameters
     * Suppose you have two sounds - a helicopter, and someone talking (speech). The helicopter should be
     * very audible in a wide surrounding area, whereas the speech should only be audible in a small area.
     * To get the desired effect, and assuming units that are approximately in meters, we would want the
     * reference distance of the helicopter to be ~25. On the other hand, the speech sound would have a
     * reference distance of ~0.5. The rolloff factor for the helicopter would be in the range of
     * ~0.1, while the speech sound could have a rolloff factor between 1 and 2. The helicopter would then
     * be audible even if the listener was far away, and the listener would have to be in the immediate
     * vicinity of the speech source in order to hear it. Naturally, a wide range of other values can be
     * used depending on the application, but the desired effect would be the same.
     */

    enum RenderingParameter
    {
        ROLLOFF_FACTOR      = 1, /**< The rolloff factor is used to describe the rate at which the sound becomes more and more
                                      inaudible (attenuates) as it goes farther and farther away from the listener. The default
                                      is 1.0, and values must be greater than or equal to 0. A rolloff factor of 0 implies that
                                      the sound does not attenuate with respect to distance - it will seem just as loud no matter
                                      how far away it is from the listener. */
        REFERENCE_DISTANCE  = 2, /**< The reference distance is used to describe when the gain of the sound should be clamped,
                                      or when attenuation due to distance should no longer have any effect. The default reference
                                      distance is 1.0. If the distance between the sound and the listener is less than or equal to
                                      the reference distance, the sound will not be attenuated (i.e. the rolloff factor will be
                                      applied). Reference distance values must be greater than or equal to 0. */
    };


    /**
     * Create a new sound source based on a file with the given path and filename.
     * The full path that will be used is "path/filename".
     * @param path This is the path to the folder containing the file.
     *              (e.g. "/home/user/data")
     * @param filename This is the actual name of the file. (e.g. "funnysound.wav")
     */
    Sound(const std::string &path, const std::string &filename);

    /**
     * Create a new sound source based on the given filepath. The filename will be extracted from
     * the filepath automatically. If no filepath is specified, the sound object does not
     * correspond to any sound, and does not attempt any network communication.
     * @param filepath This is the full path to the file. (e.g. "/home/user/data/funnysound.wav")
     */
    Sound(const std::string &filepath = "");

    /**
     * Create a new sound source based on the specified wavetype, frequency and phaseshift.
     */
    Sound(WaveformType waveType, float frequency, float phaseShift, float durationInSeconds);

    /**
     * Releases associated sound with server, if any.
     */
    ~Sound();

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
     * @deprecated
     * Set the direction of the sound source by specifying an angle in the X-Z plane. Note that
     * there is no default value for this angle, because it is not possible to represent
     * a lack of directionality using angles. This function is considered deprecated, and kept
     * only to support old client applications using the Windows Audio Server.
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
     * Set a rendering parameter of this sound source.
     */
    bool setRenderingParameter(RenderingParameter whichParameter, float value);

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

} // end namespace oasclient

#endif // _OAS_SOUND_H_
