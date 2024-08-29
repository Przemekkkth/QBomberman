#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include "gamescene.h"
#include "menuscene.h"
#include "optionsscene.h"
#include "creditsscene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View();

signals:

private:
    CreditsScene m_creditsScene;
    GameScene m_gameScene;
    MenuScene m_menuScene;
    OptionsScene m_optionsScene;

    void createConnections();
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
