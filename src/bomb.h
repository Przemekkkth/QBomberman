#ifndef BOMB_H
#define BOMB_H
#include "physicalbody.h"
#include <QTime>
#include <QElapsedTimer>
#include <QVector2D>
#include "ray.h"

class Animator;
class Level;
class Player;
//class Ray;

class Bomb : public PhysicalBody
{
public:
    static const int TILE_SIZE = 64;
    const int MAX_RAY_SIZE = 4;
    const int SPEED = 200;
    enum State
    {
        waitingForExplosion,
        exploding,
        exploded
    };
    const int SPRITE_WIDTH, SPRITE_HEIGHT;
public:
    Bomb(Player* player);
    virtual ~Bomb();
    ///Sets Animator
    void setAnimator();
    ///Sets time to detonation
    /// <param name="time"> time in seconds</param>
    void setDetonationTime(int milisecond);
    ///Sets explosion time (time when explosion ray is on screen)
    /// <param name="time"> time in seconds</param>
    void setRayOnScreenTime(int milisecond);
    ///Check is anything colliding with rays
    /// <param name="physicalBody"> collider of an object</param>
    /// <returns>true when is collision</returns>
    /// <returns>false when isn't collision</returns>
    bool isObjectInRay(PhysicalBody & physicalBody);
    /// <returns>true when bomb exploded and rays are on</returns>
    /// <returns>false if not</returns>
    Bomb::State getState();
    ///Spawns bomb on position
    /// <param name="position"> position of bomb</param>
    void setPosition(int x, int y);

    void setLevelPointer(Level * level);

    void update(float dt);

    Ray* getRayPhysicalBody(unsigned int side);

    int getPositionInTileCoordinatesX();
    int* getPositionPointerInTileCoordinatesX();
    int getPositionInTileCoordinatesY();
    int* getPositionPointerInTileCoordinatesY();

    void setMoveDirection(QVector2D direction);

    void stopMoving();

    bool isMoving();

    int getNextPositionInTileCoordsX();
    int getNextPositionInTileCoordsY();

    void fixPosition();
    void draw(GameScene* gs);
private:
    Animator* m_animator;
    Level* level;
    int m_positionInTilesCoordsX;
    int m_positionInTilesCoordsY;
    QPixmap m_spritesheet;
    struct rayLength
    {
        int up;
        int down;
        int left;
        int right;
    };
    QVector<Ray*> m_rays;
    int m_detonationTime;
    int m_rayOnScreenTime;
    QElapsedTimer m_detonationClock;
    QVector<QPair<int, int>>m_tilesToDeleteAfterExplosion;
    State m_state;
    QVector2D m_direction;
    bool m_isMoving;
    void explode();

    unsigned short getRaySizeAfterCollisions(Ray::Side side);
    Player* m_owner;


    //void draw(sf::RenderTarget&, sf::RenderStates) const;

    //sf::Sound m_explosionSound;
    //sf::SoundBuffer m_soundBuffer;
    void isCollidedWithOwner(unsigned int i);
    void isCollidedWithEnemies(unsigned int i);
};

#endif // BOMB_H
