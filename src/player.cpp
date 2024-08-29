#include "player.h"
#include "pixmapmanager.h"
#include "gamescene.h"
#include "bomb.h"

Player::Player(PlayerType type)
    :
    SPRITE_WIDTH(50),
    SPRITE_HEIGHT(42),
    m_bomb(nullptr),
    m_canBeDamaged(true)
{
    if (type == PlayerType::Player1) {
        m_spritesheets = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Player);
    }
    else {
        m_spritesheets = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Player2);
    }
    setAnimator();
}


Player::~Player()
{
    delete m_animator;
}


void Player::setAnimator()
{
    m_animator = new Animator();
    //0-5 right w bomb
    for (int i = 0; i < 6; ++i) {
        m_animator->m_mapStates[QStringLiteral("right_with_bomb")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //6-12 up w bomb
    for (int i = 6; i < 12; ++i) {
        m_animator->m_mapStates[QStringLiteral("up_with_bomb")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //12-16 right
    for (int i = 12; i < 17; ++i) {
        m_animator->m_mapStates[QStringLiteral("right")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //17-21 up
    for (int i = 17; i < 21; ++i) {
        m_animator->m_mapStates[QStringLiteral("up")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //21-27 down
    for (int i = 21; i < 27; ++i) {
        m_animator->m_mapStates[QStringLiteral("down")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //27-33 down w bomb
    for (int i = 27; i < 33; ++i) {
        m_animator->m_mapStates[QStringLiteral("down_with_bomb")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //27-28 idle w bomb
    m_animator->m_mapStates[QStringLiteral("idle_with_bomb")].push_back(m_spritesheets.copy(27 * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    m_animator->m_mapStates[QStringLiteral("idle_with_bomb")].push_back(m_spritesheets.copy(28 * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));

    //33-39 left w bomb
    for (int i = 33; i < 39; ++i) {
        m_animator->m_mapStates[QStringLiteral("left_with_bomb")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    //39-44 left
    for (int i = 39; i < 44; ++i) {
        m_animator->m_mapStates[QStringLiteral("left")].push_back(m_spritesheets.copy(i * SPRITE_WIDTH,0, SPRITE_WIDTH, SPRITE_HEIGHT));
    }

    m_animator->changeActiveState(QStringLiteral("idle_with_bomb"));
    m_animator->setDelayBetweenFrames(.1f);
    setSize(SPRITE_WIDTH, SPRITE_HEIGHT);
    m_origin = QPointF(SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT / 2.0f);

}

void Player::setRespawns(short hp)
{
    m_respawns = m_maxNumberOfRespawns = hp;
}

void Player::increaseRespawns(short val)
{
    m_respawns += val;
}

void Player::decreaseRespawns(short val)
{
    m_respawns -= val;
}

short Player::getRespawnsCount() const
{
    return m_respawns;
}


void Player::onMoveKeyPressed(int x, int y)
{
    const float speed = 150;
    setVelocity(speed*x, speed*y);
}

void Player::setAfterRespawnSafeTime(int value)
{
    m_respawnSafeTime = value;
}

bool Player::hasBomb() const
{
    return (m_bomb != nullptr && m_bomb->getState() == Bomb::waitingForExplosion);
    //return false;
}

Bomb * Player::getBomb() const
{
    return m_bomb;
}

void Player::onActionKeyPressed()
{
    if (m_bomb == nullptr) {
        m_bomb = new Bomb(this);

        m_bomb->setDetonationTime(3000);
        m_bomb->setRayOnScreenTime(1000);
        m_bomb->setPosition(static_cast<int>(getPositionX()), static_cast<int>(getPositionY()));
        m_bomb->setLevelPointer(m_level);
        m_level->setTileAsBomb(getPositionX() / TILE_SIZE, getPositionY() / TILE_SIZE);
    }
}

void Player::setLevelPointer(Level * level)
{
    if (level == nullptr)
    {
        std::cerr << "Level pointer is a null!" << std::endl;
        exit(5);
    }
    this->m_level = level;
}


void Player::update(float dt)
{
    m_animator->update(dt);
    if (m_movementX != 0 && m_movementY != 0) {
        m_movementX /= 1.41f;
        m_movementY /= 1.41f;
    }

    setPositionX(getPositionX() + m_movementX);
    setPositionY(getPositionY() + m_movementY);

    if (m_respawns > 0 && m_respawns < m_maxNumberOfRespawns && (m_respawnClock.elapsed() >= m_respawnSafeTime)) {
        m_respawnClock.restart();
        m_canBeDamaged = true;
    }
    else if (m_respawns <= 0 && m_isAlive) {
        m_isAlive = false;
    }
    else {
        m_isAlive = true;
    }

    if (m_bomb) {
        m_bomb->update(dt);

        if (m_bomb->getState() == Bomb::exploding) {
            m_level->destroyTile(m_bomb->getPositionInTileCoordinatesX(), m_bomb->getPositionInTileCoordinatesY(), false);
        }

        if (m_bomb->getState() == Bomb::exploded) {
            delete m_bomb;
            m_bomb = nullptr;
        }
    }
}

bool Player::isTileCollidingInAxisX(size_t x) const
{
    return (x == getPositionInTilesCoordsX() || x == m_bodyInfo.leftBound || x == m_bodyInfo.rightBound);
}

bool Player::isTileCollidingInAxisY(size_t y) const
{
    return (y == getPositionInTilesCoordsY() || y == m_bodyInfo.upBound || y == m_bodyInfo.downBound);
}

void Player::endGame()
{
    m_vX = 0;
    m_vY = 0;
}

void Player::onBombCollision()
{
    if (m_canBeDamaged)
    {
        respawn();
    }
}

bool Player::isBombExplosion() const
{
    return (m_bomb != nullptr && m_bomb->getState() == Bomb::exploding);
}

Ray* Player::getRay(unsigned int side) const
{
    return m_bomb->getRayPhysicalBody(side);
}

bool Player::getIsAlive() const
{
    return m_isAlive;
}

void Player::setIsAlive(bool var)
{
    m_isAlive = var;
}

int Player::getPositionInTilesCoordsX() const
{
    return static_cast<int>(getPositionX() / TILE_SIZE);
}

int Player::getPositionInTilesCoordsY() const
{
    return static_cast<int>(getPositionY() / TILE_SIZE);
}

bool Player::isCollidingWithBomb() const
{
    return m_isCollidingWithBomb;
}

void Player::setIsCollidingWithBomb(bool value)
{
    m_isCollidingWithBomb = value;
}

QVector2D Player::getSideBombCollidingWith() const
{
    return m_sideBombCollidingWith;
}

void Player::setSideBombCollidingWith(int x, int y)
{
    m_sideBombCollidingWith.setX(x);
    m_sideBombCollidingWith.setY(y);
    if (x || y)
    {
        m_bombCollidingWithLevelCoords.setX(static_cast<int>(getPositionX() / TILE_SIZE + x));
        m_bombCollidingWithLevelCoords.setY(static_cast<int>(getPositionY() / TILE_SIZE + y));
    }
    else
    {
        m_bombCollidingWithLevelCoords = QVector2D(0, 0);
    }
}

QVector2D Player::getBombCollidingWithCoordinates() const
{
    return m_bombCollidingWithLevelCoords;
}

void Player::setWin(bool val)
{
    m_win = val;
}

bool Player::getWin() const
{
    return m_win;
}

void Player::setRespawnPosition(size_t x, size_t y)
{
    m_respawnPosition = QVector2D(x, y);
}

void Player::setAlive()
{
    m_isAlive = true;
    m_canBeDamaged = true;
}

void Player::deleteBomb()
{
    delete m_bomb;
    m_bomb = nullptr;
}

void Player::spawn()
{
    setPositionX(static_cast<int>(m_respawnPosition.x() * TILE_SIZE + TILE_SIZE / 2));
    setPositionY(static_cast<int>(m_respawnPosition.y() * TILE_SIZE + TILE_SIZE / 2));
}

void Player::setDirection(QString direction)
{
    m_direction = direction;
    changeAnimation();
}

void Player::move()
{
    if (m_direction == QStringLiteral("up-left")) {
        onMoveKeyPressed(-1, -1);
    }
    else if (m_direction == QStringLiteral("up-right")) {
        onMoveKeyPressed(1, -1);
    }
    else if (m_direction == QStringLiteral("down-left")) {
        onMoveKeyPressed(-1, 1);
    }
    else if (m_direction == QStringLiteral("down-right")) {
        onMoveKeyPressed(1, 1);
    }
    else if (m_direction == QStringLiteral("up")) {
        onMoveKeyPressed(0, -1);
    }
    else if (m_direction == QStringLiteral("down")) {
        onMoveKeyPressed(0, 1);
    }
    else if (m_direction == QStringLiteral("left")) {
        onMoveKeyPressed(-1, 0);
    }
    else if (m_direction == QStringLiteral("right")) {
        onMoveKeyPressed(1, 0);
    }
    else if (m_direction == QStringLiteral("stop")) {
        onMoveKeyPressed(0, 0);
    }
    else {
        qDebug() << "Invalid movement";
    }
}

void Player::changeAnimation()
{
    if (m_direction == QStringLiteral("up")) {
        m_animator->changeState(QStringLiteral("up_with_bomb"));
    }
    else if (m_direction == QStringLiteral("down")) {
        m_animator->changeState(QStringLiteral("down_with_bomb"));
    }
    else if (m_direction == QStringLiteral("left")) {
        m_animator->changeState(QStringLiteral("left_with_bomb"));
    }
    else if (m_direction == QStringLiteral("right")) {
        m_animator->changeState(QStringLiteral("right_with_bomb"));
    }
    else if (m_direction == QStringLiteral("stop")) {
        m_animator->changeState(QStringLiteral("idle_with_bomb"));
    }
}

void Player::draw(GameScene *gs)
{
    if (m_bomb) {
        m_bomb->draw(gs);
    }

    QPointF pos;
    pos.setX(getPositionX() - getSizeX() / 2);
    pos.setY(getPositionY() - getSizeY() / 2);
    m_animator->draw(gs, pos, 0, m_origin);
}

QVector<Player *> Player::enemies() const
{
    return m_enemies;
}

void Player::addEnemy(Player *player)
{
    m_enemies.push_back(player);
}

void Player::respawn()
{
    m_canBeDamaged = false;
    spawn();
    m_respawns--;
    m_respawnClock.restart();
    m_direction = QStringLiteral("stop");
}
