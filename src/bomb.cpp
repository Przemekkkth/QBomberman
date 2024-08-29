#include "bomb.h"
#include "pixmapmanager.h"
#include "animator.h"
#include "ray.h"
#include "player.h"

Bomb::Bomb(Player *player)
 :  SPRITE_WIDTH(64),
    SPRITE_HEIGHT(64),
    m_state(State::waitingForExplosion),
    m_isMoving(false)
{
    m_detonationClock.restart();
    m_rays.resize(4);
    m_spritesheet = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Bomb);
    setAnimator();
    setSize(SPRITE_WIDTH, SPRITE_HEIGHT);
    m_owner = player;
}

Bomb::~Bomb()
{
    for (unsigned int i = 0; i < m_rays.size(); i++)
        delete m_rays[i];

    delete m_animator;
}

void Bomb::setAnimator()
{
    m_animator = new Animator();
    m_animator->m_mapStates[QStringLiteral("bomb")].push_back(m_spritesheet.copy(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
    m_animator->m_mapStates[QStringLiteral("bomb")].push_back(m_spritesheet.copy(SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
    m_animator->m_mapStates[QStringLiteral("bomb")].push_back(m_spritesheet.copy(2 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
    m_animator->changeActiveState(QStringLiteral("bomb"));
    m_animator->setDelayBetweenFrames(1.0f);
}

void Bomb::setDetonationTime(int milisecond)
{
    m_detonationTime = milisecond;
}

void Bomb::setRayOnScreenTime(int milisecond)
{
    m_rayOnScreenTime = milisecond;
}

bool Bomb::isObjectInRay(PhysicalBody &physicalBody)
{
    if (m_rays[0] != nullptr)
    {
        if (m_state == exploding)
        {
            if (isCollision(*this))
            {
                return true;
            }
        }

        for (unsigned int i = 0; i < m_rays.size(); i++)
        {
            if (m_rays[i]->isCollision(physicalBody))
            {
                return true;
            }
        }
    }


    return false;
}

Bomb::State Bomb::getState()
{
    return m_state;
}

void Bomb::setPosition(int x, int y)
{
    x = m_positionInTilesCoordsX = x / TILE_SIZE;
    y = m_positionInTilesCoordsY = y / TILE_SIZE;

    setPositionX(static_cast<float>(x)*TILE_SIZE + TILE_SIZE/2);
    setPositionY(static_cast<float>(y)*TILE_SIZE + TILE_SIZE/2);
}

void Bomb::setLevelPointer(Level *level)
{
    this->level = level;
}

void Bomb::isCollidedWithOwner(unsigned int i)
{
    switch (i)
    {
    case Ray::Up:
    {
        if (m_rays[i]->raySize() <= m_rays[i]->animator()->m_currentFrame) {
            return;
        }
        if(m_positionInTilesCoordsX  == m_owner->getPositionInTilesCoordsX() &&
            (m_positionInTilesCoordsY - (m_rays[i]->animator()->m_currentFrame + 1)) == m_owner->getPositionInTilesCoordsY()) {
            m_owner->onBombCollision();
            break;
        }
    }break;
    case Ray::Down:
    {
        if (m_rays[i]->raySize() <= m_rays[i]->animator()->m_currentFrame) {
            return;
        }
        if(m_positionInTilesCoordsX  == m_owner->getPositionInTilesCoordsX() &&
            (m_positionInTilesCoordsY + (m_rays[i]->animator()->m_currentFrame + 1)) == m_owner->getPositionInTilesCoordsY()) {
            m_owner->onBombCollision();
            break;
        }
    }break;
    case Ray::Left:
    {
        if(m_positionInTilesCoordsX  == (m_owner->getPositionInTilesCoordsX() - (m_rays[i]->animator()->m_currentFrame + 1)) &&
            m_positionInTilesCoordsY  == m_owner->getPositionInTilesCoordsY()) {
            m_owner->onBombCollision();
            break;
        }
    }break;
    case Ray::Right:
    {
        if(m_positionInTilesCoordsX  == (m_owner->getPositionInTilesCoordsX() + (m_rays[i]->animator()->m_currentFrame + 1)) &&
            m_positionInTilesCoordsY  == m_owner->getPositionInTilesCoordsY()) {
            m_owner->onBombCollision();
            break;
        }
    }break;
    }
}

void Bomb::isCollidedWithEnemies(unsigned int i)
{
    if (m_rays[i]->raySize() <= m_rays[i]->animator()->m_currentFrame) {
        return;
    }
    switch (i)
    {
    case Ray::Up:
    {
        for (int i = 0; i < m_owner->enemies().size(); ++i) {
            Player* enemy = m_owner->enemies()[i];

            if(m_positionInTilesCoordsX  == enemy->getPositionInTilesCoordsX() &&
                (m_positionInTilesCoordsY - (m_rays[i]->animator()->m_currentFrame + 1)) == enemy->getPositionInTilesCoordsY()) {
                enemy->onBombCollision();
                break;
            }
        }
    }break;
    case Ray::Down:
    {
        for (int i = 0; i < m_owner->enemies().size(); ++i) {
            Player* enemy = m_owner->enemies()[i];

            if(m_positionInTilesCoordsX  == enemy->getPositionInTilesCoordsX() &&
                (m_positionInTilesCoordsY + (m_rays[i]->animator()->m_currentFrame + 1)) == enemy->getPositionInTilesCoordsY()) {
                enemy->onBombCollision();
                break;
        }
    }break;
    case Ray::Left:
    {
        for (int i = 0; i < m_owner->enemies().size(); ++i) {
            Player* enemy = m_owner->enemies()[i];

            if (m_positionInTilesCoordsX - (m_rays[i]->animator()->m_currentFrame + 1) == enemy->getPositionInTilesCoordsX() &&
                m_positionInTilesCoordsY  == enemy->getPositionInTilesCoordsY()) {
                enemy->onBombCollision();
                break;
            }
        }

    }break;
    case Ray::Right:
    {
        for (int i = 0; i < m_owner->enemies().size(); ++i) {
            Player* enemy = m_owner->enemies()[i];

            if (m_positionInTilesCoordsX + (m_rays[i]->animator()->m_currentFrame + 1) == enemy->getPositionInTilesCoordsX() &&
                m_positionInTilesCoordsY  == enemy->getPositionInTilesCoordsY()) {
                enemy->onBombCollision();
                break;
            }
        }
    }break;
    }
    }
}

void Bomb::update(float dt)
{
    m_animator->update(dt);

    m_positionInTilesCoordsX = static_cast<int>(getPositionX() / TILE_SIZE);
    m_positionInTilesCoordsY = static_cast<int>(getPositionY() / TILE_SIZE);

    if (m_detonationClock.elapsed() >= m_detonationTime && m_state < State::exploding)
    {
        fixPosition();
        level->destroyTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY, false);
        m_state = State::exploding;
        explode();
        for(unsigned int i = 0; i < m_tilesToDeleteAfterExplosion.size(); i++)
            level->destroyTile(m_tilesToDeleteAfterExplosion[i].first, m_tilesToDeleteAfterExplosion[i].second);
    }
    else if (m_detonationClock.elapsed() >= m_detonationTime + m_rayOnScreenTime && m_state < State::exploded)
    {
        m_detonationClock.restart();
        m_state = State::exploded;

    }

    if (m_state == State::exploding) {
        for (unsigned int i = 0; i < m_rays.size(); i++) {
            m_rays[i]->update(dt);
            isCollidedWithOwner(i);
            isCollidedWithEnemies(i);
        }
    }
}

Ray *Bomb::getRayPhysicalBody(unsigned int side)
{
    return m_rays.at(side);
}

int Bomb::getPositionInTileCoordinatesX()
{
    return m_positionInTilesCoordsX;
}

int *Bomb::getPositionPointerInTileCoordinatesX()
{
    return &m_positionInTilesCoordsX;
}

int Bomb::getPositionInTileCoordinatesY()
{
    return m_positionInTilesCoordsY;
}

int *Bomb::getPositionPointerInTileCoordinatesY()
{
    return &m_positionInTilesCoordsY;
}

void Bomb::setMoveDirection(QVector2D direction)
{
    PhysicalBody::setVelocity(SPEED * direction.x(), SPEED * direction.y());
    m_direction = direction;
    m_isMoving = true;
}

void Bomb::stopMoving()
{
    m_isMoving = false;
}

bool Bomb::isMoving()
{
    return m_isMoving;
}

int Bomb::getNextPositionInTileCoordsX()
{
    return m_positionInTilesCoordsX + m_direction.x();
}

int Bomb::getNextPositionInTileCoordsY()
{
    return m_positionInTilesCoordsY + m_direction.y();
}

void Bomb::fixPosition()
{
    setPositionX(m_positionInTilesCoordsX * TILE_SIZE + TILE_SIZE / 2);
    setPositionY(m_positionInTilesCoordsY * TILE_SIZE + TILE_SIZE / 2);
}

void Bomb::explode()
{
    for (unsigned int i = 0; i < m_rays.size(); i++)
    {
        m_rays[i] = new Ray(static_cast<Ray::Side>(i));
        m_rays.at(i)->setPosition(getPositionX(), getPositionY());
        unsigned short size = getRaySizeAfterCollisions(static_cast<Ray::Side>(i));
        m_rays.at(i)->setRaySize(size);
        m_rays.at(i)->setRaySpriteSize(size);
    }
}

unsigned short Bomb::getRaySizeAfterCollisions(Ray::Side side)
{
    unsigned short size = 0;

    switch (side)
    {
    case Ray::Up:
    {
        for (int i = 1; i <= MAX_RAY_SIZE; ++i)
        {
            if (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY - i) <= TT::NONE_WITH_SHADOW)
                size=i;
            else break;
        }
        bool withinBounds = (m_positionInTilesCoordsY - size - 1 >= 0);
        bool isWeakWall = (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY - size - 1) == TT::TileType::WEAK_WALL);
        bool isDoubleWeakWall = (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY - size - 1) == TT::TileType::DOUBLE_WEAK_WALL);

         if (size != MAX_RAY_SIZE && withinBounds && (isWeakWall || isDoubleWeakWall)) {
             size++;
             m_tilesToDeleteAfterExplosion.push_back(std::pair<int, int>(m_positionInTilesCoordsX, m_positionInTilesCoordsY - size));
        }
    }
        break;
    case Ray::Down:
    {
        for (int i = 1; i <= MAX_RAY_SIZE; ++i)
        {
            if (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY + i) <= TT::NONE_WITH_SHADOW)
                size = i;
            else break;
        }

        bool withinBounds = m_positionInTilesCoordsY + size + 1 < level->getHeight();
        bool isWeakWall = (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY + size + 1) == TT::TileType::WEAK_WALL);
        bool isDoubleWeakWall = (level->getTile(m_positionInTilesCoordsX, m_positionInTilesCoordsY + size + 1) == TT::TileType::DOUBLE_WEAK_WALL);


        if (size != MAX_RAY_SIZE && withinBounds && (isWeakWall || isDoubleWeakWall)) {
            size++;
            m_tilesToDeleteAfterExplosion.push_back(std::pair<int, int>(m_positionInTilesCoordsX, m_positionInTilesCoordsY + size));
        }
    }
        break;
    case Ray::Left:
    {
        for (int i = 1; i <= MAX_RAY_SIZE; ++i)
        {
            if ( level->getTile(m_positionInTilesCoordsX - i, m_positionInTilesCoordsY) <= TT::NONE_WITH_SHADOW)
            {
                size = i;
            }
            else break;
        }

        bool isWeakWall = (level->getTile(m_positionInTilesCoordsX - size - 1, m_positionInTilesCoordsY) == TT::TileType::WEAK_WALL);
        bool isDoubleWeakWall = (level->getTile(m_positionInTilesCoordsX - size - 1, m_positionInTilesCoordsY) == TT::TileType::DOUBLE_WEAK_WALL);

        if (size != MAX_RAY_SIZE && ( isWeakWall || isDoubleWeakWall))
        {
            size++;
            m_tilesToDeleteAfterExplosion.push_back(std::pair<int, int>(m_positionInTilesCoordsX - size, m_positionInTilesCoordsY));
        }
    }
        break;
    case Ray::Right:
    {
        for (int i = 1; i <= MAX_RAY_SIZE; ++i)
        {
            if (level->getTile(m_positionInTilesCoordsX + i, m_positionInTilesCoordsY) <= TT::NONE_WITH_SHADOW)
                size = i;
            else break;
        }

        bool isWeakWall = (level->getTile(m_positionInTilesCoordsX + size + 1, m_positionInTilesCoordsY) == TT::TileType::WEAK_WALL);
        bool isDoubleWeakWall = (level->getTile(m_positionInTilesCoordsX + size + 1, m_positionInTilesCoordsY) == TT::TileType::DOUBLE_WEAK_WALL);

        if (size != MAX_RAY_SIZE && (isWeakWall || isDoubleWeakWall))
            m_tilesToDeleteAfterExplosion.push_back(std::pair<int, int>(m_positionInTilesCoordsX + ++size, m_positionInTilesCoordsY));
    }
        break;
    default:
        break;
    }


    return size;
}

void Bomb::draw(GameScene *gs)
{
    if (m_state == State::exploding) {
        for (int i = 0; i < m_rays.size(); ++i) {
            m_rays.at(i)->draw(gs);
        }
    }
    else {
        m_animator->draw(gs, QPointF(getPositionX() - getSizeX() / 2.0f, getPositionY() - getSizeY() / 2.0f));
    }
}
