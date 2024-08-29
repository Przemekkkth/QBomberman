#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QTimer>
#include <QImage>
#include <QGraphicsSimpleTextItem>
#include "ray.h"
#include "level.h"
#include "physicsengine.h"
//#include "bombmanager.h"

class BombManager;
class Player;

struct KeyStatus
{
    bool m_pressed = false;
    bool m_held = false;
    bool m_released = false;
};

struct MouseStatus
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    bool m_released = false;
    bool m_pressed = false;
};

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();
    void start();
    void stop();
private slots:
    void loop();
signals:
    void menuActivated();
private:
    void init();
    void onUserCreate();
    void renderScene();
    void handlePlayerInput();
    void resetStatus();

    void draw();
    void drawLevel();
    void drawStats();
    void drawResult();
    void update(float deltaTime);
    KeyStatus* m_keys[256];
    MouseStatus* m_mouse;
    const int FPS = 60;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f, m_loopTime = 0.0f;
    const float m_loopSpeed = int(1000.0f/FPS);


     Ray* m_left;
     Ray* m_up;
     Ray* m_right;
     Ray* m_down;
     Player* m_player1;
     Player* m_player2;
     Level* m_level;
     QVector<Player*> m_players;
     PhysicsEngine* m_physicsEngine;

     BombManager* m_bombManager;
     QSet<int> m_pressedKeys;
     QRectF m_returnMenuRect, m_playAgainRect, m_exitRect;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // GAMESCENE_H
