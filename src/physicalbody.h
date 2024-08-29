#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <cmath>
#include "utils.h"
#include <iostream>
class PhysicalBody
{
public:
    PhysicalBody();
    ~PhysicalBody();

    enum BodyPositionState { OnSingleTile, OnTwoTilesHorizontal, OnTwoTilesVertical, OnFourTiles };

    struct MovableBodyInfo
    {
        BodyPositionState state;

        unsigned int upBound;
        unsigned int downBound;
        unsigned int leftBound;
        unsigned int rightBound;
        unsigned int centerX;
        unsigned int centerY;
    };

    MovableBodyInfo m_bodyInfo;

    /// Sets the velocity
    void setVelocity(float x, float y);

    /// Sets the object size
    void setSize(float x, float y);

    /// Check collision between objects
    bool isCollision(float x, float y, float sizeX, float sizeY);

    /// Check collision between objects
    bool isCollision(PhysicalBody& pyRef);

    float getPositionX() const;
    float getPositionY() const;

    float getSizeX() const;
    float getSizeY() const;

    float getVelocityX() const;
    float getVelocityY() const;

    /// Sets position of physical body representation
    /// This method does not change visible position of a body!!!
    void setPositionX(float x);

    /// Sets position of physical body representation
    /// This method does not change visible position of a body!!!
    void setPositionY(float y);

    void setMovementX(float movement);

    void setMovementY(float movement);

protected:
    float m_posX;
    float m_posY;

    float m_sizeX;
    float m_sizeY;

    float m_vX;
    float m_vY;

    float m_movementX; //This is final value pre-checked for collisions occurance
    float m_movementY;
};

#endif // PHYSICALBODY_H
