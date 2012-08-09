/**
 * @file    OASAudioHandler.h
 * @author  Shreenidhi Chowkwale
 */

#ifndef _OAS_AUDIO_HANDLER_H_
#define _OAS_AUDIO_HANDLER_H_

#include <map>
#include <iostream>
#include <cmath>
#include <queue>
#include <AL/alut.h>
#include "OASAudioSource.h"
#include "OASAudioListener.h"
#include "OASAudioBuffer.h"
#include "OASLogger.h"

namespace oas
{

/**
 * This class manages multiple audio sources and buffers, and provides wrappers for modifying these
 * units.
 */

// Buffer Map types
typedef std::map<std::string, AudioBuffer*>     BufferMap;
typedef BufferMap::iterator                     BufferMapIterator;
typedef BufferMap::const_iterator               BufferMapConstIterator;
typedef std::pair<std::string, AudioBuffer*>    BufferPair;

// Source Map types
typedef std::map<ALuint, AudioSource*>          SourceMap;
typedef SourceMap::iterator                     SourceMapIterator;
typedef SourceMap::const_iterator               SourceMapConstIterator;
typedef std::pair<ALuint, AudioSource*>         SourcePair;

class AudioHandler
{
public:

    /**
     * These constants are used to designate which parameter to modify.
     */
    enum GlobalRenderingParameter
    {
        SPEED_OF_SOUND              = 1,
        DOPPLER_FACTOR              = 2,
        DEFAULT_ROLLOFF             = 3,
        DEFAULT_REFERENCE_DISTANCE  = 4,
    };

    enum SoundRenderingParameter
    {
        ROLLOFF_FACTOR              = 1,
        REFERENCE_DISTANCE          = 2,
        CONE_INNER_ANGLE            = 3,
        CONE_OUTER_ANGLE            = 4,
        CONE_OUTER_GAIN             = 5,
    };

    static AudioHandler& getInstance();

    bool initialize(std::string const& deviceString);
    void release();

    /**
     * @brief Gets the buffer that is associated with the file pointed to by filename. 
     *        Creates a new buffer if necessary.
     */
    ALuint getBuffer(const std::string& filename);

    /**
     * @brief Create a new source based on the input buffer
     * @retval Unique handle for the created source, or -1 on error
     */
    int createSource(const ALuint buffer);

    /**
     * @brief Create a new source with the audio file that is pointed to by filename
     * @retval Unique handle for the created source, or -1 on error
     */
    int createSource(const std::string& filename);

    /**
     * @brief Create a new source based on the specified waveform.
     * @param waveShape Sine        -> waveShape = 1
     *                  Square      -> waveShape = 2
     *                  Sawtooth    -> waveShape = 3
     *                  Whitenoise  -> waveShape = 4
     *                  Impulse     -> waveShape = 5
     * @param frequency Frequency of the waveform, in hertz
     * @param phase Phase of the waveform, in degrees from -180 to +180
     * @param duration Duration of waveform in seconds
     * @retval Unique handle for the created source, or -1 on error
     */
    int createSource(ALint waveShape, ALfloat frequency, ALfloat phase, ALfloat duration);

    /**
     * @brief Retrieve a const pointer to the most recently modified audio unit
     */
    const AudioUnit* getRecentlyModifiedAudioUnit();

    /**
     * @brief Retrieve a const pointer to the Listener
     */
    const AudioListener* getListener();

    /**
     * @brief Retrieve copies of all updated sources inside the given queue
     * @param sources
     */
    void populateQueueWithUpdatedSources(std::queue <const AudioUnit*> &sources);

    /**
     * @brief Updates sources without retrieving copies of them
     */
    void updateSources();

    /**
     * @note:
     * The following functions operate on existing sources. If the given source handle is invalid,
     * there is no change made to the OpenAL state, and the function does nothing.
     *
     * If the operation is successful, the particular source is marked as the most recently modified source.
     */

    /**
     * @brief Deletes the source with the given handle. Any associated buffers are not deleted.
     */
    void deleteSource(const ALuint source);

    /**
     * @brief Deletes the given source.
     */
    void deleteSource(AudioSource *source);

    /**
     * @brief Begin playing the source with the given handle.
     */
    void playSource(const ALuint source);

    /**
     * @brief Stop playing the source with the given handle.
     */
    void stopSource(const ALuint source);

    /**
     * @brief Pause the playback of the source with the given handle.
     */
    void pauseSource(const ALuint source);

    /**
     * Set the default rolloff factor for sound sources created after this point.
     * This property will not be applied to existing sources - it will only affect sources
     * created in the future.
     *
     * The rolloff factor is used to describe the rate at which the sound becomes more and more
     * inaudible (attenuates) as it goes farther and farther away from the listener. The default
     * is 1.0. Values must be greater than or equal to 0.
     */
    void setDefaultRolloffFactor(const ALfloat rolloff);

    /**
     * Set the default reference distance for sound sources created after this point.
     * This property will not be applied to existing sources - it will only affect sources
     * created in the future.
     *
     * The reference distance describes an area around a sound source in which the gain is
     * not attenuated based on distance. The default is 1.0, and values must be greater than or
     * equal to 0.
     */
    void setDefaultReferenceDistance(const ALfloat referenceDistance);

    /**
     * @brief Set the playback position of the source, in seconds.
     */
    void setSourcePlaybackPosition(const ALuint source, const ALfloat seconds);

    /**
     * @brief Set the source's position.
     */
    void setSourcePosition(const ALuint source, const ALfloat x, const ALfloat y, const ALfloat z);

    /**
     * @brief Set the source's gain. 
     * @param gain A value 0.0 will effectively mute the source.
     */
    void setSourceGain(const ALuint source, const ALfloat gain);

    /**
     * @brief Set the source to play repeatedly
     * @param isLoop A value of 1 will enable looping, and a value of 0 will disable looping.
     */
    void setSourceLoop(const ALuint source, const ALint isLoop);

    /**
     * @brief Set the source to be moving at the given speed in the same direction that it is facing.
     */
    void setSourceSpeed(const ALuint source, const ALfloat speed);

    /**
     * @brief Set the source to be moving with the given velocity
     */
    void setSourceVelocity(const ALuint source, const ALfloat x, const ALfloat y, const ALfloat z);

    /**
     * @brief Set the direction the source is pointing at, using Cartesian coordinates.
     */
    void setSourceDirection(const ALuint source, const ALfloat x, const ALfloat y, const ALfloat z);

    /**
     * @brief Set the direction the source is pointing at with just an angle.
     * @param angleInRadians The angle must be given in radians
     */
    void setSourceDirection(const ALuint source, const ALfloat angleInRadians);

    /**
     * @brief Change the pitch of the source.
     * @param pitchFactor Doubling the factor will increase by one octave, and halving will decrease by one octave.
     *                    Default = 1.
     */
    void setSourcePitch(const ALuint source, const ALfloat pitchFactor);

    /**
     * @brief Set the source to fade in or out over a period of time.
     * @param fadeToGainValue This will be gain value that the source will fade to. For example, if a source was
     * 						  being faded out, then fadeToGainValue should be 0. For a fade in, set this parameter
     * 						  to be greater than what the current gain is.
	 * @param durationInSeconds This is the amount of time over which the fade should take place.
	 * 							If the duration is small (< 1 second), then the fade occur quickly.
	 * 							If the duration is large, then the fade will be more gradual.
     */
    void setSourceFade(const ALuint source, const ALfloat fadeToGainValue, const ALfloat durationInSeconds);

    /**
     * @brief Get the current state of the source.
     * @retval 0 = Error/Unknown
     * @retval 1 = Initialized
     * @retval 2 = Playing
     * @retval 3 = Paused
     * @retval 4 = Stopped
     * @retval 5 = Deleted
     */
    int getSourceState(const ALuint source);

    /**
     * @brief Change the overall gain via the listener object
     */
    void setListenerGain(const ALfloat gain);

    /**
     * @brief Set the position of the listener
     */
    void setListenerPosition(const ALfloat x, const ALfloat y, const ALfloat z);

    /**
     * @brief Set the velocity of the listener
     */
    void setListenerVelocity(const ALfloat x, const ALfloat y, const ALfloat z);

    /**
     * @brief Set the listener orientation
     */
    void setListenerOrientation(const ALfloat atX, const ALfloat atY, const ALfloat atZ,
                                       const ALfloat upX, const ALfloat upY, const ALfloat upZ);

    /**
     * @brief Set global rendering parameter
     */
    void setGlobalRenderingParameter(const ALuint whichParameter, ALfloat value);

    /**
     * @brief Set sound rendering parameter
     */
    void setSoundRenderingParameter(const ALuint source, const ALuint whichParameter, ALfloat value);

private:
    AudioHandler();

    AudioSource* _getSource(const ALuint source);
    void _clearRecentlyModifiedAudioUnit();
    void _setRecentlyModifiedAudioUnit(const AudioUnit*);
    void _processLazyDeletionQueue();

    BufferMap _bufferMap;
    SourceMap _sourceMap;

    AudioSource* _recentSource;

    const AudioUnit *_recentlyModifiedAudioUnit;

    std::queue<AudioSource*> _lazyDeletionQueue;

    std::string _deviceString;
    ALCdevice* _device;
    ALCcontext* _context;

    ALfloat _defaultRolloff;
    ALfloat _defaultReferenceDistance;
};

}
#endif

