/**
 * @file    OASAudioSource.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_AUDIOSOURCE_H_
#define _OAS_AUDIOSOURCE_H_

#include <string>
#include <AL/alut.h>
#include "OASAudioUnit.h"
#include "OASTime.h"

namespace oas
{
/**
 * Contains some basic properties and functions useful for modifying sound in OpenAL
 */
class AudioSource : public AudioUnit
{

public:

    /**
     * The state is defined as follows:
     * UNKNOWN: state is unknown
     * PLAYING: source is playing
     * PAUSED:  source is paused at a specific point, and playback will resume from here
     * STOPPED: source is stopped and playback will resume from the beginning
     * DELETED: source is in the process of being deleted
     */
    enum SourceState
    {
        ST_UNKNOWN = 0,
        ST_INITIAL = 1,
        ST_PLAYING = 2,
        ST_PAUSED =  3,
        ST_STOPPED = 4,
        ST_DELETED = 5
    };

    /**
     * @brief Get the handle for this source
     */
    virtual unsigned int getHandle() const;

    /**
     * @brief Get the name of the underlying buffer that is attached to this source
     */
    unsigned int getBuffer() const;

    /**
     * @brief Update the state of the sound source, and perform automated operations (e.g. fade)
     * @param forceUpdate If true, it will force the state to be checked and updated via OpenAL,
     *                    else it will only update the state if the sound source was playing.
     * @return True if something changed, false if nothing changed
     */
    bool update(bool forceUpdate = false);

    /**
     * @brief Play the source all the way through
     */
    bool play();

    /**
     * @brief Stop playing the source. Playback will resume from the beginning
     */
    bool stop();

    /**
     * Pause the playback of the source, saving the current playback position. To resume playback,
     * play() must be used. Pausing a source that is already paused has no effect. Stopping a
     * paused source resets the playback position to the beginning, as expected.
     */
    bool pause();

    /**
     * Set the playback position of the source, in seconds. If the source is already playing,
     * playback will skip to the desired location. Otherwise, the playback position will be applied
     * next time the source is played. If the specified position is outside the bounds of the
     * sound source, this will have no effect.
     */
    bool setPlaybackPosition(ALfloat seconds);

    /**
     * @brief Set the gain
     */
    virtual bool setGain(ALfloat gain);

    /**
     * @brief Set the audio source to fade in/out to a gain value, over the given duration (in seconds)
     */
    bool setFade(ALfloat fadeToGainValue, ALfloat durationInSeconds);

    /**
     * @brief Set the position
     */
    virtual bool setPosition(ALfloat x, ALfloat y, ALfloat z);

    /**
     * @brief Set the velocity
     */
    virtual bool setVelocity(ALfloat x, ALfloat y, ALfloat z);

    /**
     * @brief Set the direction of the source
     */
    bool setDirection(ALfloat x, ALfloat y, ALfloat z);

    /**
     * @brief Set the source to play in a continuous loop, until it is stopped
     */
    bool setLoop(ALint isLoop);

    /**
     * @brief Change the pitch of the source.
     * @param pitchFactor Doubling the factor will increase by one octave, and halving will decrease by one octave.
     *                    Default = 1.
     */
    bool setPitch(ALfloat pitchFactor);

    /**
     * @brief Set the rolloff factor for this sound source.
     * The rolloff factor is used to describe the rate at which the sound becomes more and more
     * inaudible (attenuates) as it goes farther and farther away from the listener. The default
     * is 1.0, and values must be greater than or equal to 0. A rolloff factor of 0 implies that
     * the sound does not attenuate with respect to distance - it will seem just as loud no matter
     * how far away it is from the listener.
     *
     * See the documentation of setReferenceDistance() for a theoretical example of using rolloff.
     */
    bool setRolloffFactor(ALfloat rolloff);

    /**
     * @brief Set the reference distance for this sound source.
     * The reference distance is used to describe when the gain of the sound should be clamped,
     * or when attenuation due to distance should no longer have any effect. The default reference
     * distance is 1.0. If the distance between the sound and the listener is less than or equal to
     * the reference distance, the sound will not be attenuated (i.e. the rolloff factor will not be
     * applied). Reference distance values must be greater than or equal to 0.
     *
     * Here is a simple example of the appropriate usage of reference distance and rolloff:
     *
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
    bool setReferenceDistance(ALfloat referenceDistance);

    /**
     * Set the inner angle of the directional sound cone, in degrees. This will only apply to directional
     * sound sources. The default is 45.0 degrees.
     *
     * If the listener falls within the region defined by the inner angle, then the sound is not attenuated,
     * and the listener hears it at the regular gain value.
     */
    bool setConeInnerAngle(ALfloat innerAngleInDegrees);

    /**
     * Set the outer angle of the directional sound cone, in degrees. This will only apply to directional
     * sound sources. The default is 180.0 degrees.
     *
     * If the listener falls within the region defined by the outer angle, but outside the region of the inner
     * angle, then the sound will be attenuated based on the computed angle between sound source and listener.
     */
    bool setConeOuterAngle(ALfloat outerAngleInDegrees);

    /**
     * Set the gain of the sound source for the region outside the directional sound cone. This will only apply to
     * directional sound sources. The default is 0. The coneOuterGain is multiplied by the source's
     * internal gain to get the effective gain.
     */
    bool setConeOuterGain(ALfloat coneOuterGain);

    /**
     * @brief Deletes the audio resources allocated for this sound source
     */
    bool deleteSource();

    /**
     * @brief Get the current state of the source
     */
    SourceState getState() const;

    /**
     * @brief Get the pitch
     */
    float getPitch() const;

    /**
     * @brief Get the rolloff factor
     */
    float getRolloffFactor() const;

    /**
     * @brief Get the reference distance
     */
    float getReferenceDistance() const;

    /**
     * @brief Get the cone inner angle
     */
    float getConeInnerAngle() const;

    /**
     * @brief Get the cone outer angle
     */
    float getConeOuterAngle() const;

    /**
     * @brief Get the cone outer gain
     */
    float getConeOuterGain() const;

    /**
     * @brief Get the x, y, z direction
     */
    float getDirectionX() const;
    float getDirectionY() const;
    float getDirectionZ() const;

    /**
     * @brief Determine if the source is looping or not
     */
    bool isLooping() const;

    /**
     * @brief Determine if the source is directional or not
     */
    bool isDirectional() const;

    /**
     * @brief Override AudioUnit method
     */
    bool isSoundSource() const;

    /**
     * @brief Resets the handle counter, and any other state applicable to all sources
     */
    static void resetSources();

    /**
     * @brief Get the label for the data entry for the given index
     */
    virtual const char* getLabelForIndex(int index) const;

    /**
     * @brief Get the string for the value of the data entry for the given index
     */
    virtual std::string getStringForIndex(int index) const;


    /**
     * @brief Get the number of data entries monitored by AudioSource objects
     */
    static int getIndexCount();

    /**
     * @brief Creates a new audio source using the specified buffer
     * @param buffer Handle to a buffer that contains sound data
     */
    AudioSource(ALuint buffer);

    AudioSource();

    ~AudioSource();

protected:

    /**
     * Inherited members from superclass AudioUnit are
     *
     *
     * ALfloat _gain;
     * ALfloat _positionX, _positionY, _positionZ;
     * ALfloat _velocityX, _velocityY, _velocityZ;
     */

private:
    void _init();
    ALuint _generateNextHandle();
    void _clearError();
    bool _wasOperationSuccessful();
    bool _checkIncrementalFade();
    bool _needsFade();

    /*
     * 'id' is used to interact with the OpenAL library, and the values are arbitrary.
     * The 'id' is strictly internal to the source, and no other object needs to know it.
     */
    ALuint _id;

    /*
     * 'handle' is used to interact with the client, and the values are guaranteed to
     * start from 1 and increment by 1 for each source that is generated.
     */
    ALuint _handle;

    ALuint _buffer;
    SourceState _state;

    ALfloat _directionX, _directionY, _directionZ;

    ALfloat _pitch;

    ALfloat _rolloff, _referenceDistance;
    ALfloat _coneInnerAngle;
    ALfloat _coneOuterAngle;
    ALfloat _coneOuterGain;

    ALint _isLooping;
    bool _isDirectional;

    double _fadeFinalGain;
    double _fadeInitialGain;
    double _fadeGainDiff;
    double _fadeDuration;
    Time _fadeStartTime;
    Time _fadeEndTime; 	// _fadeEndTime = _fadeStartTime + _fadeDuration

    static ALuint _nextHandle;

};
}


#endif /* _OAS_AUDIOSOURCE_H_*/
