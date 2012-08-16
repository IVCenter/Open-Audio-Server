/**
 * @file    Listener.h
 * @author  Shreenidhi Chowkwale
 *
 */

#ifndef _OAS_LISTENER_H_
#define _OAS_LISTENER_H_

#include <vector>

#include "ClientInterface.h"
#include "config.h"

#ifdef OSG_FOUND
#include <osg/Node>
#endif

namespace oasclient
{

/**
 * @brief Used to communicate properties of the listener to the server.
 */
#ifdef OSG_FOUND
class Listener : public osg::Node
#else
class Listener
#endif
{
public:

    /**
     * Retrieve a reference to the singleton Listener object, which
     * can then be used to modify listener attributes.
     */
    static Listener& getInstance();

    /**
     * Modify the global (listener's) gain level. The default is 1, and a value of 0 will mute all
     * sounds completely.
     */
    bool setGain(float gain);

    /**
     * Modify the listener's position. Default is <0, 0, 0>
     */
    bool setPosition(float x, float y, float z);

    /**
     * Modify the listener's velocity. Default is <0, 0, 0>. Note that this is ONLY used for
     * doppler effect calculations, and does not cause the position to be updated.
     * If the velocity is NOT set, then doppler effect simulation will not occur.
     */
    bool setVelocity(float x, float y, float z);

    /**
     * Modify the listener's orientation, in terms of a "look-at" vector and "up" vector.
     * Defaults are <0, 0, -1> and <0, 1, 0>, respectively. This corresponds to positive Y
     * going up, and positive Z coming out of the screen.
     *
     * It is simple to convert to another coordinate system. For example, if your application
     * uses positive Y going into the screen and positive Z going up, use <0, 1, 0> for your
     * look-at vector and <0, 0, 1> for your up vector.
     */
    bool setOrientation(float atX, float atY, float atZ,
                                float upX, float upY, float upZ);


    /**
     * These constants are designate various global parameters
     */
    enum GlobalRenderingParameter
    {
        /**
         * The speed of sound is used for doppler effect calculations. It must
         * be in the same units as the velocities of the listener and sound
         * sources. The default is 343.3, appropriate for the speed of sound
         * through air in meters per second. For an application whose distance
         * units are all in millimeters, the speed of sound should be set to
         * 343300 (mm/s).
         */
        SPEED_OF_SOUND = 1,

        /**
         * The doppler factor can be used to emphasize or de-emphasize the pitch
         * shifts due to the doppler effect. The default value is 1.0, and a
         * value of 0 will effectively disable the doppler effect.
         */
        DOPPLER_FACTOR = 2,

        /**
         * The default rolloff factor, when set, will be the rolloff factor
         * applied to all sound sources generated thereafter. When it is set,
         * it will not be applied to existing sound sources. Rolloff factor
         * can be modified on individual sources as well.
         *
         * See oasclient::Sound::ROLLOFF_FACTOR for more information.
         */
        DEFAULT_ROLLOFF = 3,

        /**
         * The default reference distance, when set, will be the reference
         * distance applied to all sound sources generated thereafter. When it
         * is set, it will not be applied to existing sound sources. Reference
         * distance can be modified on individual sources as well.
         *
         * See oasclient::Sound::REFERENCE_DISTANCE for more information.
         */
        DEFAULT_REFERENCE_DISTANCE = 4,
    };

    /**
     * Set sound rendering parameters
     */
    bool setGlobalRenderingParameters(GlobalRenderingParameter whichParameter, float value);

    /**
     * Convenience method to set oasclient::Listener::SPEED_OF_SOUND
     *
     */
    bool setGlobalSpeedOfSound(float value);

    /**
     * Convenience method to set oasclient::Listener::DOPPLER_FACTOR
     */
    bool setGlobalDopplerFactor(float value);

    /**
     * Convenience method to set oasclient::Listener::DEFAULT_ROLLOFF
     */
    bool setGlobalRolloffFactor(float value);

    /**
     * Convenience method to set oasclient::Listener::DEFAULT_REFERENCE_DISTANCE
     */
    bool setGlobalReferenceDistance(float value);

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
    Listener();

    float _posX, _posY, _posZ;
    float _velX, _velY, _velZ;
    float _atX, _atY, _atZ;
    float _upX, _upY, _upZ;
    float _gain;

};
}

#endif // _OAS_LISTENER_H_
