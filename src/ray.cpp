#include "ray.h"
#include "gamescene.h"
#include "pixmapmanager.h"
#include "animator.h"
#include "gamescene.h"

Ray::Ray(Side side)
    :   m_rayLength(0)
{
    m_side = side;
    m_texture = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Raycast);
    setAnimator();
}

Ray::~Ray()
{
    delete m_animator;
}

void Ray::setAnimator()
{
    m_animator = new Animator();
    m_animator->m_mapStates[QStringLiteral("explosion")].push_back(m_texture.copy(0,0, 1 * TILE_SIZE, TILE_SIZE));
    m_animator->m_mapStates[QStringLiteral("explosion")].push_back(m_texture.copy(0,0, 2 * TILE_SIZE, TILE_SIZE));
    m_animator->m_mapStates[QStringLiteral("explosion")].push_back(m_texture.copy(0,0, 3 * TILE_SIZE, TILE_SIZE));
    m_animator->m_mapStates[QStringLiteral("explosion")].push_back(m_texture.copy(0,0, 4 * TILE_SIZE, TILE_SIZE));
    m_animator->setDelayBetweenFrames(0.25f);
    m_animator->changeActiveState(QStringLiteral("explosion"));
}

int Ray::getSide()
{
    return m_side;
}

void Ray::setRaySpriteSize(unsigned short size)
{
    m_size = size;
    switch (m_side)
    {
    case Ray::Up:
        m_angle = 270;
        PhysicalBody::setSize(TILE_SIZE, TILE_SIZE*size + TILE_SIZE);
        setPositionY(static_cast<int>(getPositionY() / TILE_SIZE) * TILE_SIZE);
        PhysicalBody::setPositionY(getPositionY()- ((TILE_SIZE*size) + TILE_SIZE)/2 + TILE_SIZE);
        break;
    case Ray::Down:
        m_angle = 90;
        setPositionY(static_cast<int>(getPositionY() / TILE_SIZE) * TILE_SIZE);
        PhysicalBody::setSize(TILE_SIZE, TILE_SIZE*size);
        PhysicalBody::setPositionY(getPositionY() +TILE_SIZE + (TILE_SIZE*size) / 2 );
        break;
    case Ray::Left:
        m_angle = 180;
        setPositionX(static_cast<int>(getPositionX() / TILE_SIZE) * TILE_SIZE);
        PhysicalBody::setSize(TILE_SIZE*size, TILE_SIZE);
        PhysicalBody::setPositionX(getPositionX() - (TILE_SIZE*size) / 2);
        break;
    case Ray::Right:
        m_angle = 0;
        setPositionX(static_cast<int>(getPositionX() / TILE_SIZE) * TILE_SIZE);
        PhysicalBody::setSize(TILE_SIZE*size, TILE_SIZE);
        PhysicalBody::setPositionX(getPositionX() + TILE_SIZE + (TILE_SIZE*size) / 2);
        break;
    default:
        break;
    }
}

void Ray::setPosition(float x, float y)
{
    m_spritePos.setX(x);
    m_spritePos.setY(y);
    PhysicalBody::setPositionX(x);
    PhysicalBody::setPositionY(y);
}

void Ray::update(float dt)
{
    m_animator->update(dt);
    setRaySpriteSize(m_animator->m_currentFrame);

}

void Ray::draw(GameScene *gs)
{
    const int halfTileSize = TILE_SIZE / 2;

    if (m_side == Ray::Right) {
        if (m_rayLength > m_animator->m_currentFrame) {
            QPointF pos = QPointF(m_spritePos.x() + halfTileSize, m_spritePos.y() - halfTileSize);
            m_animator->draw(gs, pos, m_angle, m_spriteOriginPoint);
        }
    }
    else if (m_side == Ray::Left) {
        if (m_rayLength > m_animator->m_currentFrame) {
            QPointF pos = QPointF(m_spritePos.x() - halfTileSize, m_spritePos.y() + halfTileSize);
            m_animator->draw(gs, pos, m_angle, m_spriteOriginPoint);
        }


    }
    else if (m_side == Ray::Up) {
        if (m_rayLength > m_animator->m_currentFrame) {
            QPointF pos = QPointF(m_spritePos.x() - halfTileSize, m_spritePos.y() - halfTileSize);
            m_animator->draw(gs, pos, m_angle, m_spriteOriginPoint);
        }
    }
    else if (m_side == Ray::Down) {
        if (m_rayLength > m_animator->m_currentFrame) {
            QPointF pos = QPointF(m_spritePos.x() + halfTileSize, m_spritePos.y() + halfTileSize);
            m_animator->draw(gs, pos, m_angle, m_spriteOriginPoint);
        }
    }
}

void Ray::setRaySize(int length)
{
    m_rayLength = length;
}

Animator *Ray::animator() const
{
    return m_animator;
}

int Ray::raySize() const
{
    return m_rayLength;
}
