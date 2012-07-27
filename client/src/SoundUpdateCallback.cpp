/**
 * @file    SoundUpdateCallback.cpp
 * @author  Shreenidhi Chowkwale
 */

#include "SoundUpdateCallback.h"

using namespace oasclient;

SoundUpdateCallback::SoundUpdateCallback(Sound *sound, double updateIntervalInSeconds) :
        osg::NodeCallback(),
        _sound(sound),
        _updateIntervalInSeconds(updateIntervalInSeconds),
        _firstRun(true),
        _prevTime(0)
{
}

SoundUpdateCallback::~SoundUpdateCallback()
{
}

void SoundUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nodeVisitor)
{
    const osg::FrameStamp *frameStamp = nodeVisitor->getFrameStamp();

    if ( !_sound || !_sound->isValid() || (frameStamp == NULL))
    {
        std::cerr << "SoundUpdateCallback::operator() - first if check failed" << std::endl;
        traverse(node, nodeVisitor);
        return;
    }

    const double currTime = frameStamp->getReferenceTime();
    const double diffTime = currTime - _prevTime;

    if (diffTime >= _updateIntervalInSeconds)
    {
        const osg::Vec3 currPosition = osg::computeLocalToWorld(nodeVisitor->getNodePath()).getTrans();
        osg::Vec3 velocity(0, 0, 0);

        // Update the position and compute the velocity only if the position changed
        if (currPosition != _prevPosition)
        {
            _sound->setPosition(currPosition.x(), currPosition.y(), currPosition.z());

            // Automatically compute the velocity, based on movement
            if (_firstRun)
            {
                _firstRun = false;
            }
            else
            {
                velocity = currPosition - _prevPosition;
                velocity /= diffTime;
            }
        }

        _sound->setVelocity(velocity.x(), velocity.y(), velocity.z());
        _prevTime = currTime;
        _prevPosition = currPosition;

        // Compute the direction of the sound source, if it is directional
    }

    traverse(node, nodeVisitor);
}
