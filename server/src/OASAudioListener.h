/**
 * @file    OASAudioListener.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_AUDIOLISTENER_H_
#define _OAS_AUDIOLISTENER_H_

#include <AL/alut.h>
#include "OASAudioUnit.h"


namespace oas
{
/**
 * Contains some basic properties and functions useful for modifying sound in OpenAL
 */
class AudioListener : public AudioUnit
{

public:

    /**
     * The AudioListener is a singleton.
     */
    static AudioListener* getInstance();

    /**
     * Get the handle of the listener
     */
    unsigned int getHandle() const;

    /**
     * @brief Update the state of the sound listener
     * @return True if something changed, false if nothing changed
     */
    bool update();

    /**
     * @brief Set the gain
     */
    virtual bool setGain(ALfloat gain);

    /**
     * @brief Set the position
     */
    virtual bool setPosition(ALfloat x, ALfloat y, ALfloat z);

    /**
     * @brief Set the velocity
     */
    virtual bool setVelocity(ALfloat x, ALfloat y, ALfloat z);

    /**
     * @brief Set the orientation vectors (look-at and up)
     */
    bool setOrientation(ALfloat atX, ALfloat atY, ALfloat atZ,
                        ALfloat upX, ALfloat upY, ALfloat upZ);

    /**
     * Set the speed of sound. This is used for doppler effect calculations, and
     * must be in the same units as the velocities of the listener and all sound
     * sources. The default is 343.3 meters per second, appropriate for the
     * speed of sound through air.
     */
    bool setSpeedOfSound(ALfloat speedOfSound);

    /**
     * Set the doppler factor, which can be used to emphasize or de-emphasize the
     * pitch shifts due to the doppler effect. The default value is 1.0. A value
     * of 0 will effectively disable any pitch shift due to the doppler effect.
     */
    bool setDopplerFactor(ALfloat doppler);

    /**
     * @brief Override AudioUnit method
     */
    bool isSoundSource() const;

    /**
     * @brief Get the look at and up vectors
     */
    float getOrientationLookAtX() const;
    float getOrientationLookAtY() const;
    float getOrientationLookAtZ() const;
    float getOrientationUpX() const;
    float getOrientationUpY() const;
    float getOrientationUpZ() const;

    /**
     * @brief Get the speed of sound
     */
    float getSpeedOfSound() const;

    /**
     * @brief Get the doppler factor
     */
    float getDopplerFactor() const;

    /**
     * @brief Get the label for the data entry for the given index
     */
    const char* getLabelForIndex(int index) const;

    /**
     * @brief Get the string for the value of the data entry for the given index
     */
    std::string getStringForIndex(int index) const;

    /**
     * @brief Get the number of data entries
     */
    static int getIndexCount();

    ~AudioListener();

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
    AudioListener();
    void _init();
    void _clearError();
    bool _wasOperationSuccessful();

    /**
     * The following array describes the orientation of the listener, via 2 vectors.
     * The first three entries are the "at" vector, and second three entries are the "up" vector
     */
    ALfloat _orientation[6];

    ALfloat _speedOfSound;
    ALfloat _dopplerFactor;
};
}


#endif /* _OAS_AUDIOSOURCE_H_*/
