#include "creditsscene.h"
#include "pixmapmanager.h"
#include "utils.h"
#include "button.h"
#include "fontmanager.h"
#include <QKeyEvent>

CreditsScene::CreditsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    QGraphicsPixmapItem* bgItem = new QGraphicsPixmapItem();
    QPixmap bgPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::MenuBG).scaled(SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());

    bgItem->setPixmap(bgPixmap);
    addItem(bgItem);


    QGraphicsPixmapItem* creditsItem = new QGraphicsPixmapItem();
    QPixmap creditsPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::CreditsLogo).scaled(SCREEN::PHYSICAL_SIZE.width(), 100);

    creditsItem->setPixmap(creditsPixmap);
    addItem(creditsItem);

    m_backButton = new Button(QStringLiteral("RETURN TO MENU"));
    m_backButton->setPos(165, SCREEN::PHYSICAL_SIZE.height() - 50);
    addItem(m_backButton);

    QFont font(FontManager::Instance()->getFont(FontManager::FontID::MAIN));
    font.setPixelSize(36);

    QGraphicsSimpleTextItem *tItem = new QGraphicsSimpleTextItem();
    tItem->setFont(font);
    tItem->setText(QStringLiteral("Game is created by :"));
    tItem->setPen(QColor(Qt::blue));
    tItem->setBrush(QColor(Qt::blue));
    tItem->setFont(font);
    tItem->setPos(20, 180);
    addItem(tItem);

    QGraphicsPixmapItem *pItem = new QGraphicsPixmapItem();
    pItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::PiLogo));
    pItem->setPos(tItem->x() + tItem->boundingRect().width() + 20, tItem->y() - 30);
    pItem->setScale(0.5);
    addItem(pItem);

    QGraphicsSimpleTextItem *t1Item = new QGraphicsSimpleTextItem();
    font.setPixelSize(25);
    t1Item->setText(QStringLiteral("Source code: https://github.com/PiGames/Bomberman"));
    t1Item->setPen(QColor(Qt::blue));
    t1Item->setBrush(QColor(Qt::blue));
    t1Item->setFont(font);
    t1Item->setPos(20, 260);
    addItem(t1Item);

    QGraphicsSimpleTextItem *t2Item = new QGraphicsSimpleTextItem();
    font.setPixelSize(36);
    t2Item->setText(QStringLiteral("Forked to Qt/C++ by :"));
    t2Item->setPen(QColor(Qt::blue));
    t2Item->setBrush(QColor(Qt::blue));
    t2Item->setFont(font);
    t2Item->setPos(20, 350);
    addItem(t2Item);

    QGraphicsPixmapItem *p1Item = new QGraphicsPixmapItem();
    p1Item->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Avatar));
    p1Item->setPos(t2Item->x() + t2Item->boundingRect().width() + 20, t2Item->y() - 35);
    p1Item->setScale(0.5);
    addItem(p1Item);

    QGraphicsSimpleTextItem *t3Item = new QGraphicsSimpleTextItem();
    font.setPixelSize(25);
    t3Item->setText(QStringLiteral("Source code: https://github.com/Przemekkkth/qBomberman"));
    t3Item->setPen(QColor(Qt::blue));
    t3Item->setBrush(QColor(Qt::blue));
    t3Item->setFont(font);
    t3Item->setPos(20, 430);
    addItem(t3Item);

    createConnections();
}

void CreditsScene::createConnections()
{
    connect(m_backButton, &Button::clicked, [this](){
        emit menuActivated();
    });
}

void CreditsScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace) {
        emit menuActivated();
    }
    QGraphicsScene::keyReleaseEvent(event);
}
