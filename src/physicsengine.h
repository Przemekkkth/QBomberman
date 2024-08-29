#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H
#include <QVector>
#include "physicalbody.h"
#include "level.h"
//
class Player;
class PhysicsEngine
{
public:
    const int TILE_SIZE = 64;

    PhysicsEngine();
    ~PhysicsEngine();

    /// Sets current level data and reference to player
    /// <param name="level">level data</param>
    /// <param name="player">player's body</param>
    void init(Level* level, QVector<Player*>* players);

    /// Check collisions, update speeds, ...
    /// <param name="delta">delta time in ms</param>
    void update(float delta);

private:
    ///Logical level representation
    Level* m_level;

    ///Physical level representation
    QVector < QVector<PhysicalBody* > > m_physicalLevel;


    ///Physical player representation
    QVector <Player*> m_players;

    void setBodyPositionInfo(int key);

    void setBodyPositionNextToAnotherBodyInAxisY(PhysicalBody * bodyToSetPostition, PhysicalBody * referenceBody);

    void setBodyPositionNextToAnotherBodyInAxisX(PhysicalBody * bodyToSetPostition, PhysicalBody * referenceBody);

    void resetMovement(Player* player);

    float calculateMovement(float velocity, float delta);

    PhysicalBody simulateMovementY(Player* player, float movementY);

    PhysicalBody simulateMovementX(Player* player, float movementX);

    void handleSingleTileMovement(int playerIndex, PhysicalBody& bodyAfterX, PhysicalBody& bodyAfterY, bool& moveX, bool& moveY);

    void handleTwoTilesHorizontalMovement(int playerIndex, PhysicalBody& bodyAfterX, PhysicalBody& bodyAfterY, bool& moveX, bool& moveY);

    void handleTwoTilesVerticalMovement(int playerIndex, PhysicalBody& bodyAfterX, PhysicalBody& bodyAfterY, bool& moveX, bool& moveY);

    void handleFourTilesMovement(int playerIndex, PhysicalBody& bodyAfterX, PhysicalBody& bodyAfterY, bool& moveX, bool& moveY);

    void updatePlayerMovement(Player* player, float movementX, float movementY, bool moveX, bool moveY);

    bool checkAndHandleYAxisCollision(Player* player, PhysicalBody& bodyAfterY, int direction, bool moveY, int xCoord = -1);

    bool checkAndHandleXAxisCollision(Player* player, PhysicalBody& bodyAfterX, int direction, bool moveX, int yCoord = -1);

    bool handleFourTilesCollision(Player* player, PhysicalBody& bodyAfterX, PhysicalBody& bodyAfterY, bool& moveX, bool& moveY, int xBound, int yBound);

    void handleCollision(Player* player, int xCoord, int yCoord, int xDirection, int yDirection);

    int getAdditionalTileX(const Player* player);

    int getAdditionalTileY(const Player* player);
};

#endif // PHYSICSENGINE_H
