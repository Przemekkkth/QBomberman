#include "view.h"
#include <QKeyEvent>
#include <QApplication>

View::View()
{
    setScene(&m_menuScene);
    resize(m_menuScene.sceneRect().width()+2, m_menuScene.sceneRect().height()+2);

    setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    createConnections();
}

void View::createConnections()
{
    connect(&m_menuScene, &MenuScene::gameSceneActivated, [this](){
        m_gameScene.start();
        setScene(&m_gameScene);
    });

    connect(&m_menuScene, &MenuScene::optionsSceneActivated, [this](){
        setScene(&m_optionsScene);
    });

    connect(&m_menuScene, &MenuScene::creditsSceneActivated, [this](){
        setScene(&m_creditsScene);
    });

    connect(&m_creditsScene, &CreditsScene::menuActivated, [this](){
       setScene(&m_menuScene);
    });

    connect(&m_optionsScene, &OptionsScene::menuActivated, [this](){
        setScene(&m_menuScene);
    });

    connect(&m_gameScene, &GameScene::menuActivated, [this](){
        m_gameScene.stop();
        setScene(&m_menuScene);
    });
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        QApplication::instance()->quit();
    }
    QGraphicsView::keyReleaseEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}
