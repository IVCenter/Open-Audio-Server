/**
 * @file    SoundUpdateCallback.h
 * @author  Shreenidhi Chowkwale
 *
 * This file contains an update callback corresponding to one Sound object.
 * When the callback is used during OpenSceneGraph's update traversal, the
 * position and velocity of the sound will be updated as necessary.
 */

#ifndef _OASSOUNDUPDATECALLBACK_H_
#define _OASSOUNDUPDATECALLBACK_H_

#include "config.h"

#ifdef OSG_FOUND

#include <osg/ref_ptr>
#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>
#include "OASClient.h"

namespace oasclient
{

class SoundUpdateCallback : public osg::NodeCallback
{
public:
    SoundUpdateCallback(Sound *sound = NULL, const double _updateIntervalInSeconds = 0.01);

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

    const Sound* getSound() const;
    void setSound(Sound *sound);

protected:
    virtual ~SoundUpdateCallback();

    Sound *_sound;
    bool _firstRun;
    double _prevTime;
    osg::Vec3 _prevPosition;
    const double _updateIntervalInSeconds;
};

}

#endif // OSG_FOUND

#endif // _OASSOUNDUPDATECALLBACK_H_
