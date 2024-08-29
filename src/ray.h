#ifndef RAY_H
#define RAY_H
#include <QPointF>
#include <QPixmap>
#include "physicalbody.h"
//#include "animator.h"
//#include "gamescene.h"
class GameScene;
class Animator;
class Ray : public PhysicalBody
{
    const int TILE_SIZE = 64;
public:
    enum Side
    {
        Up,
        Down,
        Left,
        Right
    };

    Ray(Side side);
    virtual ~Ray();
    ///Sets Animator
    void setAnimator();

    int getSide();

    void setRaySpriteSize(unsigned short size);

    void setPosition(float x, float y);
    void update(float dt);

    void draw(GameScene* gs);
    void setRaySize(int length);
    Animator* animator() const;
    int raySize() const;
private:
    Side m_side;
    unsigned short m_size;
    Animator* m_animator;
    QPointF m_spritePos;
    QPointF m_spriteOriginPoint;
    QPixmap m_texture, m_sprite;
    int m_rayLength;
    qreal m_angle;
};

#endif // RAY_H
