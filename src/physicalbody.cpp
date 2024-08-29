#include "physicalbody.h"

PhysicalBody::PhysicalBody()
{
    m_sizeX = m_sizeY = m_posX = m_posY = m_vX = m_vY = 0.0f;
}

PhysicalBody::~PhysicalBody()
{
}

void PhysicalBody::setVelocity(float x, float y)
{
    m_vX = x;
    m_vY = y;
}

void PhysicalBody::setSize(float x, float y)
{
    m_sizeX = x;
    m_sizeY = y;
}

bool PhysicalBody::isCollision(float x, float y, float sizeX, float sizeY)
{
    return !(m_posY + m_sizeY / 2 < y - sizeY / 2) && !(m_posY - m_sizeY / 2 > y + sizeY / 2) &&
           !(m_posX + m_sizeX / 2 < x - sizeX / 2) && !( m_posX - m_sizeX / 2 > x + sizeX / 2);
}

bool PhysicalBody::isCollision(PhysicalBody & pyRef)
{
    return isCollision(pyRef.getPositionX(), pyRef.getPositionY(), pyRef.getSizeX(), pyRef.getSizeY());
}

float PhysicalBody::getPositionX() const
{
    return m_posX;
}

float PhysicalBody::getPositionY() const
{
    return m_posY;
}

float PhysicalBody::getSizeX() const
{
    return m_sizeX;
}

float PhysicalBody::getSizeY() const
{
    return m_sizeY;
}

float PhysicalBody::getVelocityX() const
{
    return m_vX;
}

float PhysicalBody::getVelocityY() const
{
    return m_vY;
}

void PhysicalBody::setPositionX(float x)
{
    m_posX = x;
}

void PhysicalBody::setPositionY(float y)
{
    m_posY = y;
}

void PhysicalBody::setMovementX(float movement)
{
    m_movementX = movement;
}

void PhysicalBody::setMovementY(float movement)
{
    m_movementY = movement;
}
