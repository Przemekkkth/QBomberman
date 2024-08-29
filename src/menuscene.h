#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include "button.h"
#include "button.h"

class MenuScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MenuScene(QObject* parent = nullptr);
    ~MenuScene();

private:
    void createConnections();

    //Button* m_startButton;
    Button* m_startButton;
    Button* m_optionsButton;
    Button* m_creditsButton;
    Button* m_exitButton;


    QGraphicsPixmapItem* m_titleItem;
    QGraphicsPixmapItem* m_bgItem;
signals:
    void gameSceneActivated();
    void optionsSceneActivated();
    void creditsSceneActivated();

    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MENUSCENE_H
