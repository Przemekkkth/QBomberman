#ifndef PLAYER_H
#define PLAYER_H
#include "physicalbody.h"
#include "ray.h"
#include "level.h"
#include "animator.h"
#include <QVector2D>
#include <QTime>
#include <QElapsedTimer>

class GameScene;
class Bomb;
class Player : public PhysicalBody
{
public:
    enum PlayerType {
        Player1, Player2
    };

    float TILE_SIZE = 64;
    const int SPRITE_WIDTH;
    const int SPRITE_HEIGHT;

    Player(PlayerType type = PlayerType::Player1);
    virtual ~Player();

    /// Sets animator
    /// <param name="animator">animator</param>
    void setAnimator();
    /// Sets respawns count
    /// <param name="respawns"> respawns </param>
    void setRespawns(short respawns);
    /// Adds value to current respawns
    /// <param name="val"> value to add to current respawns </param>
    void increaseRespawns(short val);
    /// Decrases value from current respawns
    void decreaseRespawns(short val);
    /// Return Player's respawns
    /// <returns> Player's respawns </returns>
    short getRespawnsCount() const;
    /// Sets the player direction, possible values: -1, 0, 1
    /// <param name="x">direction x</param>
    /// <param name="y">direction y</param>
    void onMoveKeyPressed(int x, int y);

    void setAfterRespawnSafeTime(int value);

    bool hasBomb() const;

    Bomb* getBomb() const;

    /// Sends the information about action key down (plant the bomb)
    void onActionKeyPressed();
    //void setUpBomb(TextureAtlas* atlasBomb, TextureAtlas* atlasRay);

    void setLevelPointer(Level * level);

    /// Update player position, update states etc...
    /// <param name="dt">delta time in seconds</param>
    void update(float dt);

    bool isTileCollidingInAxisX(size_t x) const;
    bool isTileCollidingInAxisY(size_t y) const;

    void onBombCollision();

    bool isBombExplosion() const;

    Ray* getRay(unsigned int side) const;

    bool getIsAlive() const;
    void setIsAlive(bool var);

    int getPositionInTilesCoordsX() const;

    int getPositionInTilesCoordsY() const;

    bool isCollidingWithBomb() const;

    void setIsCollidingWithBomb(bool value);

    QVector2D getSideBombCollidingWith() const;

    void setSideBombCollidingWith(int x, int y);

    QVector2D getBombCollidingWithCoordinates() const;

    void setWin(bool val);
    bool getWin() const;

    void setRespawnPosition(size_t x, size_t y);
    void spawn();
    void respawn();
    void setAlive();
    void deleteBomb();

    void setDirection(QString direction);

    void move();
    void changeAnimation();

    void draw(GameScene* gs);
    QVector<Player*> enemies() const;
    void addEnemy(Player* player);
private:
    Level * m_level;
    QPixmap m_spritesheets;
    Animator* m_animator;

    Bomb * m_bomb;
    short m_respawns;
    short m_maxNumberOfRespawns;
    bool m_canBeDamaged;
    QElapsedTimer m_respawnClock;
    int m_respawnSafeTime;

    bool m_isAlive;

    bool m_isCollidingWithBomb;

    QVector2D m_respawnPosition;

    QVector2D m_sideBombCollidingWith;

    QVector2D m_bombCollidingWithLevelCoords;

    void endGame();

    bool m_win;

    QPointF m_pos, m_origin;
    QString m_direction;

    QVector<Player*> m_enemies;
};

#endif // PLAYER_H
