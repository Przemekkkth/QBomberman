#include "optionsscene.h"
#include "pixmapmanager.h"
#include "utils.h"
#include "fontmanager.h"
#include <QKeyEvent>

OptionsScene::OptionsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    QGraphicsPixmapItem* bgItem = new QGraphicsPixmapItem();
    QPixmap bgPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::MenuBG).scaled(SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    bgItem->setPixmap(bgPixmap);
    addItem(bgItem);

    QFont bigFont = FontManager::Instance()->getFont(FontManager::FontID::MAIN);
    bigFont.setPixelSize(36);
    QFont normalFont = FontManager::Instance()->getFont(FontManager::FontID::MAIN);
    normalFont.setPixelSize(24);

    QGraphicsItemGroup *group1Item = new QGraphicsItemGroup();
    group1Item->setPos(20, 20);
    QGraphicsSimpleTextItem *t0Item = new QGraphicsSimpleTextItem(group1Item);
    t0Item->setPos(0, 0);
    t0Item->setFont(bigFont);
    t0Item->setPen(QColor(Qt::blue));
    t0Item->setBrush(QColor(Qt::blue));
    t0Item->setText(QStringLiteral("Player 1"));

    QGraphicsSimpleTextItem *t1Item = new QGraphicsSimpleTextItem(group1Item);
    t1Item->setPos(0, 50);
    t1Item->setFont(normalFont);
    t1Item->setPen(QColor(Qt::blue));
    t1Item->setBrush(QColor(Qt::blue));
    t1Item->setText(QStringLiteral("left arrow - move left"));

    QGraphicsSimpleTextItem *t2Item = new QGraphicsSimpleTextItem(group1Item);
    t2Item->setPos(0, 100);
    t2Item->setFont(normalFont);
    t2Item->setPen(QColor(Qt::blue));
    t2Item->setBrush(QColor(Qt::blue));
    t2Item->setText(QStringLiteral("right arrow - move right"));

    QGraphicsSimpleTextItem *t3Item = new QGraphicsSimpleTextItem(group1Item);
    t3Item->setPos(0, 150);
    t3Item->setFont(normalFont);
    t3Item->setPen(QColor(Qt::blue));
    t3Item->setBrush(QColor(Qt::blue));
    t3Item->setText(QStringLiteral("up arrow - move up"));

    QGraphicsSimpleTextItem *t4Item = new QGraphicsSimpleTextItem(group1Item);
    t4Item->setPos(0, 200);
    t4Item->setFont(normalFont);
    t4Item->setPen(QColor(Qt::blue));
    t4Item->setBrush(QColor(Qt::blue));
    t4Item->setText(QStringLiteral("down arrow - move down"));

    QGraphicsSimpleTextItem *t5Item = new QGraphicsSimpleTextItem(group1Item);
    t5Item->setPos(0, 250);
    t5Item->setFont(normalFont);
    t5Item->setPen(QColor(Qt::blue));
    t5Item->setBrush(QColor(Qt::blue));
    t5Item->setText(QStringLiteral("space - set bomb"));

    addItem(group1Item);

    QGraphicsItemGroup *group2Item = new QGraphicsItemGroup();
    group2Item->setPos(SCREEN::PHYSICAL_SIZE.width() / 2, 20);
    QGraphicsSimpleTextItem *t6Item = new QGraphicsSimpleTextItem(group2Item);
    t6Item->setPos(0, 0);
    t6Item->setFont(bigFont);
    t6Item->setPen(QColor(Qt::blue));
    t6Item->setBrush(QColor(Qt::blue));
    t6Item->setText(QStringLiteral("Player 2"));

    QGraphicsSimpleTextItem *t7Item = new QGraphicsSimpleTextItem(group2Item);
    t7Item->setPos(0, 50);
    t7Item->setFont(normalFont);
    t7Item->setPen(QColor(Qt::blue));
    t7Item->setBrush(QColor(Qt::blue));
    t7Item->setText(QStringLiteral("A - move left"));

    QGraphicsSimpleTextItem *t8Item = new QGraphicsSimpleTextItem(group2Item);
    t8Item->setPos(0, 100);
    t8Item->setFont(normalFont);
    t8Item->setPen(QColor(Qt::blue));
    t8Item->setBrush(QColor(Qt::blue));
    t8Item->setText(QStringLiteral("D - move right"));

    QGraphicsSimpleTextItem *t9Item = new QGraphicsSimpleTextItem(group2Item);
    t9Item->setPos(0, 150);
    t9Item->setFont(normalFont);
    t9Item->setPen(QColor(Qt::blue));
    t9Item->setBrush(QColor(Qt::blue));
    t9Item->setText(QStringLiteral("W - move up"));

    QGraphicsSimpleTextItem *t10Item = new QGraphicsSimpleTextItem(group2Item);
    t10Item->setPos(0, 200);
    t10Item->setFont(normalFont);
    t10Item->setPen(QColor(Qt::blue));
    t10Item->setBrush(QColor(Qt::blue));
    t10Item->setText(QStringLiteral("S - move down"));

    QGraphicsSimpleTextItem *t11Item = new QGraphicsSimpleTextItem(group2Item);
    t11Item->setPos(0, 250);
    t11Item->setFont(normalFont);
    t11Item->setPen(QColor(Qt::blue));
    t11Item->setBrush(QColor(Qt::blue));
    t11Item->setText(QStringLiteral("Left Control - set bomb"));

    addItem(group2Item);

    QGraphicsSimpleTextItem *t12Item = new QGraphicsSimpleTextItem();
    t12Item->setFont(normalFont);
    t12Item->setPen(QColor(Qt::blue));
    t12Item->setBrush(QColor(Qt::blue));
    t12Item->setText(QStringLiteral("Backspace - back to menu"));
    t12Item->setPos(0.5 * (SCREEN::PHYSICAL_SIZE.width() - t12Item->boundingRect().width()), 350);
    addItem(t12Item);

    QGraphicsSimpleTextItem *t13Item = new QGraphicsSimpleTextItem();
    t13Item->setFont(normalFont);
    t13Item->setPen(QColor(Qt::blue));
    t13Item->setBrush(QColor(Qt::blue));
    t13Item->setText(QStringLiteral("Escape - exit game"));
    t13Item->setPos(0.5 * (SCREEN::PHYSICAL_SIZE.width() - t13Item->boundingRect().width()), 400);
    addItem(t13Item);

    m_backButton = new Button(QStringLiteral("RETURN TO MENU"));
    m_backButton->setPos(165, SCREEN::PHYSICAL_SIZE.height() - 50);
    addItem(m_backButton);
    createConnections();
}

void OptionsScene::createConnections()
{
    connect(m_backButton, &Button::clicked, [this](){
        emit menuActivated();
    });
}

void OptionsScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace) {
        emit menuActivated();
    }
    QGraphicsScene::keyReleaseEvent(event);
}
