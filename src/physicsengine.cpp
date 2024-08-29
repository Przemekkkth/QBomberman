#include "physicsengine.h"
#include "player.h"


PhysicsEngine::PhysicsEngine()
{

}


PhysicsEngine::~PhysicsEngine()
{
    for (unsigned int y = 0; y < m_physicalLevel.size(); ++y)
        for (unsigned int x = 0; x < m_physicalLevel[y].size(); ++x)
            delete m_physicalLevel[y][x];
}


void PhysicsEngine::init(Level * level, QVector<Player *> *players)
{
    m_level = level;

    m_players.resize(players->size());
    for (unsigned int i = 0; i < m_players.size(); ++i)
    {
        m_players[i] = (*players)[i];
    }

    m_physicalLevel.resize(m_level->getHeight());

    for (unsigned int i = 0; i < m_physicalLevel.size(); ++i)
    {
        m_physicalLevel[i].resize(m_level->getWidth());
    }

    for (unsigned int y = 0; y < m_physicalLevel.size(); ++y)
        for (unsigned int x = 0; x < m_physicalLevel[y].size(); ++x)
        {
            m_physicalLevel[y][x] = new PhysicalBody();

            m_physicalLevel[y][x]->setPositionX(static_cast<float>(x)*TILE_SIZE + TILE_SIZE/2);
            m_physicalLevel[y][x]->setPositionY(static_cast<float>(y)*TILE_SIZE + TILE_SIZE/2);
            m_physicalLevel[y][x]->setSize(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE));
        }
}


void PhysicsEngine::update(float delta)
{
    for (unsigned int i = 0; i < m_players.size(); ++i) {
        setBodyPositionInfo(i);

        resetMovement(m_players[i]);

        float movementX = calculateMovement(m_players[i]->getVelocityX(), delta);
        float movementY = calculateMovement(m_players[i]->getVelocityY(), delta);

        if (movementX == 0 && movementY == 0) {
            continue;
        }

        PhysicalBody bodyAfterYMovement = simulateMovementY(m_players[i], movementY);
        PhysicalBody bodyAfterXMovement = simulateMovementX(m_players[i], movementX);

        bool moveInYAxis = true, moveInXAxis = true;

        switch (m_players[i]->m_bodyInfo.state) {
        case PhysicalBody::OnSingleTile:
            handleSingleTileMovement(i, bodyAfterXMovement, bodyAfterYMovement, moveInXAxis, moveInYAxis);
            break;

        case PhysicalBody::OnTwoTilesHorizontal:
            handleTwoTilesHorizontalMovement(i, bodyAfterXMovement, bodyAfterYMovement, moveInXAxis, moveInYAxis);
            break;

        case PhysicalBody::OnTwoTilesVertical:
            handleTwoTilesVerticalMovement(i, bodyAfterXMovement, bodyAfterYMovement, moveInXAxis, moveInYAxis);
            break;

        case PhysicalBody::OnFourTiles:
            handleFourTilesMovement(i, bodyAfterXMovement, bodyAfterYMovement, moveInXAxis, moveInYAxis);
            break;
        }

        updatePlayerMovement(m_players[i], movementX, movementY, moveInXAxis, moveInYAxis);
    }

}

void PhysicsEngine::setBodyPositionInfo(int key)
{
    m_players[key]->m_bodyInfo.centerX = static_cast<int>(m_players[key]->getPositionX()) / TILE_SIZE;
    m_players[key]->m_bodyInfo.centerY = static_cast<int>(m_players[key]->getPositionY()) / TILE_SIZE;

    m_players[key]->m_bodyInfo.upBound = static_cast<int>(m_players[key]->getPositionY() - m_players[key]->getSizeY() / 2) / TILE_SIZE;
    m_players[key]->m_bodyInfo.downBound = static_cast<int>(m_players[key]->getPositionY() + m_players[key]->getSizeY() / 2) / TILE_SIZE;
    m_players[key]->m_bodyInfo.leftBound = static_cast<int>(m_players[key]->getPositionX() - m_players[key]->getSizeX() / 2) / TILE_SIZE;
    m_players[key]->m_bodyInfo.rightBound = static_cast<int>(m_players[key]->getPositionX() + m_players[key]->getSizeX() / 2) / TILE_SIZE;

    if (m_players[key]->m_bodyInfo.upBound == m_players[key]->m_bodyInfo.downBound &&m_players[key]->m_bodyInfo.rightBound == m_players[key]->m_bodyInfo.leftBound)
        m_players[key]->m_bodyInfo.state = PhysicalBody::OnSingleTile;
    else if (m_players[key]->m_bodyInfo.upBound != m_players[key]->m_bodyInfo.downBound && m_players[key]->m_bodyInfo.rightBound == m_players[key]->m_bodyInfo.leftBound)
        m_players[key]->m_bodyInfo.state = PhysicalBody::OnTwoTilesVertical;
    else if (m_players[key]->m_bodyInfo.upBound == m_players[key]->m_bodyInfo.downBound && m_players[key]->m_bodyInfo.rightBound != m_players[key]->m_bodyInfo.leftBound)
        m_players[key]->m_bodyInfo.state = PhysicalBody::OnTwoTilesHorizontal;
    else
        m_players[key]->m_bodyInfo.state = PhysicalBody::OnFourTiles;

}

void PhysicsEngine::setBodyPositionNextToAnotherBodyInAxisY(PhysicalBody * body, PhysicalBody * tile)
{
    if (body->getPositionY() > tile->getPositionY())
        body->setPositionY(tile->getPositionY() + tile->getSizeY() / 2 + body->getSizeY() / 2 + 0.1f);
    else
        body->setPositionY(tile->getPositionY() - tile->getSizeY() / 2 - body->getSizeY() / 2 - 0.1f);
}

void PhysicsEngine::setBodyPositionNextToAnotherBodyInAxisX(PhysicalBody * body, PhysicalBody * tile)
{
    if (body->getPositionX() > tile->getPositionX())
        body->setPositionX(tile->getPositionX() + tile->getSizeX() / 2 + body->getSizeX() / 2 + 0.1f);
    else
        body->setPositionX(tile->getPositionX() - tile->getSizeX() / 2 - body->getSizeX() / 2 - 0.1f);
}

void PhysicsEngine::resetMovement(Player *player)
{
    player->setMovementX(0);
    player->setMovementY(0);
}

float PhysicsEngine::calculateMovement(float velocity, float delta)
{
    return velocity * delta;
}

PhysicalBody PhysicsEngine::simulateMovementY(Player *player, float movementY)
{
    PhysicalBody body = *player;
    body.setPositionY(player->getPositionY() + movementY);
    return body;
}

PhysicalBody PhysicsEngine::simulateMovementX(Player *player, float movementX)
{
    PhysicalBody body = *player;
    body.setPositionX(player->getPositionX() + movementX);
    return body;
}

void PhysicsEngine::handleSingleTileMovement(int playerIndex, PhysicalBody &bodyAfterX, PhysicalBody &bodyAfterY, bool &moveX, bool &moveY)
{
    Player* player = m_players[playerIndex];

    moveY = checkAndHandleYAxisCollision(player, bodyAfterY, -1, moveY) &&
            checkAndHandleYAxisCollision(player, bodyAfterY, 1, moveY);

    moveX = checkAndHandleXAxisCollision(player, bodyAfterX, -1, moveX) &&
            checkAndHandleXAxisCollision(player, bodyAfterX, 1, moveX);
}

void PhysicsEngine::handleTwoTilesHorizontalMovement(int playerIndex, PhysicalBody &bodyAfterX, PhysicalBody &bodyAfterY, bool &moveX, bool &moveY)
{
    Player* player = m_players[playerIndex];
    int additionalTileX = getAdditionalTileX(player);

    moveY = checkAndHandleYAxisCollision(player, bodyAfterY, -1, moveY) &&
            checkAndHandleYAxisCollision(player, bodyAfterY, 1, moveY) &&
            checkAndHandleYAxisCollision(player, bodyAfterY, -1, moveY, additionalTileX) &&
            checkAndHandleYAxisCollision(player, bodyAfterY, 1, moveY, additionalTileX);
}

void PhysicsEngine::handleTwoTilesVerticalMovement(int playerIndex, PhysicalBody &bodyAfterX, PhysicalBody &bodyAfterY, bool &moveX, bool &moveY)
{
    Player* player = m_players[playerIndex];
    int additionalTileY = getAdditionalTileY(player);

    moveX = checkAndHandleXAxisCollision(player, bodyAfterX, -1, moveX) &&
            checkAndHandleXAxisCollision(player, bodyAfterX, 1, moveX) &&
            checkAndHandleXAxisCollision(player, bodyAfterX, -1, moveX, additionalTileY) &&
            checkAndHandleXAxisCollision(player, bodyAfterX, 1, moveX, additionalTileY);
}

void PhysicsEngine::handleFourTilesMovement(int playerIndex, PhysicalBody &bodyAfterX, PhysicalBody &bodyAfterY, bool &moveX, bool &moveY)
{
    Player* player = m_players[playerIndex];

    moveX = !handleFourTilesCollision(player, bodyAfterX, bodyAfterY, moveX, moveY, player->m_bodyInfo.leftBound, player->m_bodyInfo.upBound) &&
            !handleFourTilesCollision(player, bodyAfterX, bodyAfterY, moveX, moveY, player->m_bodyInfo.rightBound, player->m_bodyInfo.upBound) &&
            !handleFourTilesCollision(player, bodyAfterX, bodyAfterY, moveX, moveY, player->m_bodyInfo.leftBound, player->m_bodyInfo.downBound) &&
            !handleFourTilesCollision(player, bodyAfterX, bodyAfterY, moveX, moveY, player->m_bodyInfo.rightBound, player->m_bodyInfo.downBound);
}

void PhysicsEngine::updatePlayerMovement(Player *player, float movementX, float movementY, bool moveX, bool moveY)
{
    if (moveX) {
        player->setMovementX(movementX);
    }
    if (moveY) {
        player->setMovementY(movementY);
    }
}

bool PhysicsEngine::checkAndHandleYAxisCollision(Player *player, PhysicalBody &bodyAfterY, int direction, bool moveY, int xCoord)
{
    if (xCoord == -1) {
        xCoord = player->m_bodyInfo.centerX;
    }

    int yCoord = player->m_bodyInfo.centerY + direction;

    if (m_level->getTile(xCoord, yCoord) > TT::NONE_WITH_SHADOW && bodyAfterY.isCollision(*m_physicalLevel[yCoord][xCoord])) {
        handleCollision(player, xCoord, yCoord, 0, direction);
        setBodyPositionNextToAnotherBodyInAxisY(player, m_physicalLevel[yCoord][xCoord]);
        return false;
    }

    return moveY;
}

bool PhysicsEngine::checkAndHandleXAxisCollision(Player *player, PhysicalBody &bodyAfterX, int direction, bool moveX, int yCoord)
{
    if (yCoord == -1) {
        yCoord = player->m_bodyInfo.centerY;
    }

    int xCoord = player->m_bodyInfo.centerX + direction;

    if (m_level->getTile(xCoord, yCoord) > TT::NONE_WITH_SHADOW && bodyAfterX.isCollision(*m_physicalLevel[yCoord][xCoord])) {
        handleCollision(player, xCoord, yCoord, direction, 0);
        setBodyPositionNextToAnotherBodyInAxisX(player, m_physicalLevel[yCoord][xCoord]);
        return false;
    }

    return moveX;
}

bool PhysicsEngine::handleFourTilesCollision(Player *player, PhysicalBody &bodyAfterX, PhysicalBody &bodyAfterY, bool &moveX, bool &moveY, int xBound, int yBound)
{
    if (m_level->getTile(xBound, yBound) != TT::BOMB && m_level->getTile(xBound, yBound) > TT::NONE_WITH_SHADOW) {
        if (bodyAfterX.isCollision(*m_physicalLevel[yBound][xBound])) {
            setBodyPositionNextToAnotherBodyInAxisX(player, m_physicalLevel[yBound][xBound]);
            moveX = false;
        }
        if (bodyAfterY.isCollision(*m_physicalLevel[yBound][xBound])) {
            setBodyPositionNextToAnotherBodyInAxisY(player, m_physicalLevel[yBound][xBound]);
            moveY = false;
        }
        return true;
    }
    return false;
}

void PhysicsEngine::handleCollision(Player *player, int xCoord, int yCoord, int xDirection, int yDirection)
{
    if (m_level->getTile(xCoord, yCoord) == TT::BOMB) {
        player->setSideBombCollidingWith(xDirection, yDirection);
        player->setIsCollidingWithBomb(true);
    }
}

int PhysicsEngine::getAdditionalTileX(const Player *player)
{
    if (player->m_bodyInfo.leftBound == player->m_bodyInfo.centerX) {
        return player->m_bodyInfo.rightBound;
    } else {
        return player->m_bodyInfo.leftBound;
    }
}

int PhysicsEngine::getAdditionalTileY(const Player *player)
{
    if (player->m_bodyInfo.upBound == player->m_bodyInfo.centerY) {
        return player->m_bodyInfo.downBound;
    } else {
        return player->m_bodyInfo.upBound;
    }
}

