/**
 * @file    OASSoundUpdateCallback.h
 * @author  Shreenidhi Chowkwale
 *
 * This file contains an update callback corresponding to one OASSound object.
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
#include "OASSound.h"

namespace oasclient
{

class OASSoundUpdateCallback : public osg::NodeCallback
{
public:
    OASSoundUpdateCallback(OASSound *sound = NULL, const double _updateIntervalInSeconds = 0.01);

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);

    const OASSound* getSound() const;
    void setSound(OASSound *sound);

protected:
    virtual ~OASSoundUpdateCallback();

    OASSound *_sound;
    bool _firstRun;
    double _prevTime;
    osg::Vec3 _prevPosition;
    const double _updateIntervalInSeconds;
}

}

#endif // OSG_FOUND

#endif // _OAS_OSGSOUNDUPDATECALLBACK_H_
