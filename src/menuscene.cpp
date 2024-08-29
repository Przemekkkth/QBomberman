#include "menuscene.h"
#include "utils.h"
#include "pixmapmanager.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>

MenuScene::MenuScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_bgItem = new QGraphicsPixmapItem();
    QPixmap bgPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::MenuBG).scaled(SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    m_bgItem->setPixmap(bgPixmap);
    addItem(m_bgItem);


    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    m_startButton = new Button(QStringLiteral("Start"));
    addItem(m_startButton);
    m_startButton->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0, SCREEN::PHYSICAL_SIZE.height() / 2.0 - 100);

    m_optionsButton = new Button(QStringLiteral("Options"));
    addItem(m_optionsButton);
    m_optionsButton->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0, SCREEN::PHYSICAL_SIZE.height() / 2.0);

    m_creditsButton = new Button(QStringLiteral("Credits"));
    addItem(m_creditsButton);
    m_creditsButton->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0, SCREEN::PHYSICAL_SIZE.height() / 2.0 + 100);

    m_exitButton = new Button(QStringLiteral("Exit"));
    addItem(m_exitButton);
    m_exitButton->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0, SCREEN::PHYSICAL_SIZE.height() / 2.0 + 200);

    m_titleItem = new QGraphicsPixmapItem();
    m_titleItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Title));
    m_titleItem->setScale(0.3);
    m_titleItem->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0 - (m_titleItem->boundingRect().width() * m_titleItem->scale()) / 2.0, 20);
    addItem(m_titleItem);


    createConnections();
}

MenuScene::~MenuScene()
{
}

void MenuScene::createConnections()
{
    connect(m_startButton, &Button::clicked, [this](){
        emit this->gameSceneActivated();
    });

    connect(m_optionsButton, &Button::clicked, [this](){
        emit optionsSceneActivated();
    });

    connect(m_creditsButton, &Button::clicked, [this](){
        emit creditsSceneActivated();
    });

    connect(m_exitButton, &Button::clicked, [this](){
        QApplication::instance()->quit();
    });
}

void MenuScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void MenuScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}
