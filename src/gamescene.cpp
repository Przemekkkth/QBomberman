#include "gamescene.h"
#include "utils.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QApplication>
#include "player.h"
#include "fontmanager.h"
#include "pixmapmanager.h"
#include "button.h"

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    init();

}

GameScene::~GameScene()
{
    delete m_level;
    delete m_player1;
    delete m_player2;
    delete m_physicsEngine;
}

void GameScene::start()
{
    onUserCreate();

    static bool isEventFilterInstalled = false;
    if (!isEventFilterInstalled) {
        this->installEventFilter(this);
        isEventFilterInstalled = true;
    }
    m_timer.stop();
    m_timer.start(int(1000.0f/FPS));
    m_elapsedTimer.start();
}

void GameScene::stop()
{
    m_timer.stop();
    m_elapsedTimer.restart();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    while(m_loopTime > m_loopSpeed)
    {
        const float deltaTime = m_loopSpeed / 1000.0f; //Qt use int(ms) so divide by 1000 to use second 1.0f
        m_loopTime -= m_loopSpeed;
        update(deltaTime);

        handlePlayerInput();
        draw();
        resetStatus();
    }
}

void GameScene::init()
{
    m_level = new Level();
    m_player1 = new Player(Player::PlayerType::Player1);
    m_player2 = new Player(Player::PlayerType::Player2);
    m_physicsEngine = new PhysicsEngine();
    m_player1->addEnemy(m_player2);
    m_player2->addEnemy(m_player1);
    m_players.push_back(m_player1);
    m_players.push_back(m_player2);

    setBackgroundBrush(COLOR_STYLE::BACKGROUND);
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i] = new KeyStatus();
    }
    m_mouse = new MouseStatus();
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);

}

void GameScene::onUserCreate()
{
    m_level->loadFromFile(QStringLiteral(":/assets/sample_level.txt"));

    m_player1->setRespawnPosition(1, 1);
    m_player1->setAfterRespawnSafeTime(2500);
    m_player1->spawn();
    m_player1->update(0);
    m_player1->setWin(false);
    m_player1->setRespawns(2);
    m_player1->setAlive();
    m_player1->setLevelPointer(m_level);

    m_player2->setRespawnPosition(10, 8);
    m_player2->setAfterRespawnSafeTime(2500);
    m_player2->spawn();
    m_player2->update(0);
    m_player2->setWin(false);
    m_player2->setRespawns(2);
    m_player2->setAlive();
    m_player2->setLevelPointer(m_level);

    m_physicsEngine->init(m_level, &m_players);
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::handlePlayerInput()
{
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Space]]->m_released)
    {
        m_player1->onActionKeyPressed();
    }

    if (m_keys[KEYBOARD::KeysMapper[Qt::Key_Control]]->m_released) {
        m_player2->onActionKeyPressed();
    }

    if (m_keys[KEYBOARD::KeysMapper[Qt::Key_Backspace]]->m_released) {
        emit menuActivated();
    }

    if (m_mouse->m_released && (!m_player1->getRespawnsCount() || !m_player2->getRespawnsCount())) {
        if (m_mouse->m_x >= m_returnMenuRect.x() && m_mouse->m_x <= m_returnMenuRect.x() + m_returnMenuRect.width() &&
            m_mouse->m_y >= m_returnMenuRect.y() && m_mouse->m_y <= m_returnMenuRect.y() + m_returnMenuRect.height()) {
            emit menuActivated();
        }
        else if (m_mouse->m_x >= m_playAgainRect.x() && m_mouse->m_x <= m_playAgainRect.x() + m_playAgainRect.width() &&
                m_mouse->m_y >= m_playAgainRect.y() && m_mouse->m_y <= m_playAgainRect.y() + m_playAgainRect.height()) {
            onUserCreate();
        }
        else if (m_mouse->m_x >= m_exitRect.x() && m_mouse->m_x <= m_exitRect.x() + m_exitRect.width() &&
                 m_mouse->m_y >= m_exitRect.y() && m_mouse->m_y <= m_exitRect.y() +  m_exitRect.height()) {
            QApplication::instance()->quit();
        }
    }
}

void GameScene::resetStatus()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_released = false;
    }
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_pressed = false;
    }
    m_mouse->m_released = false;
}

void GameScene::draw()
{
    clear();
    drawLevel();
    for (int i = 0; i < m_players.size(); ++i) {
        m_players.at(i)->draw(this);
    }
    drawStats();
    drawResult();
}

void GameScene::drawLevel()
{
    int rowCount    = m_level->data().size();
    int columnCount = m_level->data().at(0).size();

    int tileWidth   = m_level->tileWidth();
    int tileHeight  = m_level->tileHeight();

    for (int y = 0; y < rowCount; ++y) {
        for (int x = 0; x < columnCount; ++x) {
            QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
            pItem->setPos(x * tileWidth, y * tileHeight);
            pItem->setPixmap(m_level->getTilePixmap(x, y));

            addItem(pItem);
        }
    }
}

void GameScene::drawStats()
{
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::MAIN);
    font.setPixelSize(36);

    QGraphicsSimpleTextItem* tItem1 = new QGraphicsSimpleTextItem();
    tItem1->setFont(font);
    tItem1->setPen(QPen(Qt::white));
    tItem1->setBrush(Qt::white);
    tItem1->setText("Player 1 Lives: " + QString::number(m_player1->getRespawnsCount()));
    tItem1->setPos(20, 10);
    addItem(tItem1);

    QGraphicsSimpleTextItem* tItem2 = new QGraphicsSimpleTextItem();
    tItem2->setFont(font);
    tItem2->setPen(QPen(Qt::white));
    tItem2->setBrush(Qt::white);
    tItem2->setText("Player 2 Lives: " + QString::number(m_player2->getRespawnsCount()));
    tItem2->setPos(SCREEN::PHYSICAL_SIZE.width() - 300, SCREEN::PHYSICAL_SIZE.height() - 50);
    addItem(tItem2);
}

void GameScene::drawResult()
{
    if(m_player1->getRespawnsCount() == 0 || m_player2->getRespawnsCount() == 0) {
        QGraphicsRectItem* bgItem = new QGraphicsRectItem();
        bgItem->setRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
        QColor bgColor(255, 255, 255, 128);
        bgItem->setPen(bgColor);
        bgItem->setBrush(bgColor);
        addItem(bgItem);

        QGraphicsPixmapItem* frameItem = new QGraphicsPixmapItem();
        frameItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Frame));
        frameItem->setScale(0.3);
        frameItem->setPos(SCREEN::PHYSICAL_SIZE.width()  / 2.0f - ( (frameItem->boundingRect().width() / 2.0f) * frameItem->scale()),
                          SCREEN::PHYSICAL_SIZE.height() / 2.0f - ( (frameItem->boundingRect().width() / 2.0f) * frameItem->scale()) );

        QFont font = FontManager::Instance()->getFont(FontManager::FontID::MAIN);
        font.setPixelSize(136);
        QGraphicsSimpleTextItem *tItem = new QGraphicsSimpleTextItem(frameItem);
        tItem->setFont(font);
        tItem->setPen(QPen(Qt::blue));
        tItem->setBrush(Qt::blue);

        QString whoWinsStr = !m_player1->getRespawnsCount() ? "Player2 wins" : (!m_player2->getRespawnsCount() ? "Player1 wins" : "Unknown");
        tItem->setText(whoWinsStr);
        tItem->setPos(frameItem->boundingRect().width() / 2.0 - tItem->boundingRect().width() / 2.0, 50);

        QGraphicsItemGroup *buttonGroup = new QGraphicsItemGroup(frameItem);

        Button* returnToMenuBtn = new Button(QStringLiteral("Return to Menu"));
        returnToMenuBtn->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0f, 250);
        m_returnMenuRect = QRectF(returnToMenuBtn->pos().x() - returnToMenuBtn->boundingRect().width() / 2.0f,
                                  returnToMenuBtn->pos().y() - returnToMenuBtn->boundingRect().height() / 2.0f,
                                  returnToMenuBtn->boundingRect().width(),
                                  returnToMenuBtn->boundingRect().height());

        Button* playAgainBtn = new Button(QStringLiteral("Play again"));
        playAgainBtn->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0f, 350);
        m_playAgainRect = QRectF(playAgainBtn->pos().x() - playAgainBtn->boundingRect().width() / 2.0f,
                                 playAgainBtn->pos().y() - playAgainBtn->boundingRect().height() / 2.0f,
                                 playAgainBtn->boundingRect().width(),
                                 playAgainBtn->boundingRect().height());

        Button* exitAgainBtn = new Button(QStringLiteral("Exit game"));
        exitAgainBtn->setPos(SCREEN::PHYSICAL_SIZE.width() / 2.0f, 450);
        m_exitRect = QRectF(exitAgainBtn->pos().x() - playAgainBtn->boundingRect().width()  / 2.0f,
                            exitAgainBtn->pos().y() - playAgainBtn->boundingRect().height() / 2.0f,
                            exitAgainBtn->boundingRect().width(),
                            exitAgainBtn->boundingRect().height());

        buttonGroup->addToGroup(returnToMenuBtn);
        buttonGroup->addToGroup(playAgainBtn);
        buttonGroup->addToGroup(exitAgainBtn);
        addItem(frameItem);
    }
}

void GameScene::update(float deltaTime)
{
    for (int i = 0; i < m_players.size(); ++i) {
        m_players.at(i)->move();
        m_players.at(i)->update(deltaTime);
    }
    m_physicsEngine->update(deltaTime);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
        }
        else
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(!event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
        }

    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = true;
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = false;
    m_mouse->m_released = true;
    QGraphicsScene::mouseReleaseEvent(event);
}

bool GameScene::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::KeyPress) {

        m_pressedKeys += ((QKeyEvent*)event)->key();

    }
    else if(event->type() == QEvent::KeyRelease)
    {
        m_pressedKeys -= ((QKeyEvent*)event)->key();
    }

    Player* player1 = m_players.at(0);

    if( (m_pressedKeys.contains(Qt::Key_Up) && m_pressedKeys.contains(Qt::Key_Left)) )
    {
        player1->setDirection(QStringLiteral("up-left"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_Up) && m_pressedKeys.contains(Qt::Key_Right)) )
    {
        player1->setDirection(QStringLiteral("up-right"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_Down) && m_pressedKeys.contains(Qt::Key_Left)) )
    {
        player1->setDirection(QStringLiteral("down-left"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_Down) && m_pressedKeys.contains(Qt::Key_Right)) )
    {
        player1->setDirection(QStringLiteral("down-right"));
    }
    else if( m_pressedKeys.contains(Qt::Key_Up))
    {
        player1->setDirection(QStringLiteral("up"));
    }
    else if( m_pressedKeys.contains(Qt::Key_Down))
    {
        player1->setDirection(QStringLiteral("down"));
    }
    else if( m_pressedKeys.contains(Qt::Key_Left))
    {
        player1->setDirection(QStringLiteral("left"));
    }
    else if( m_pressedKeys.contains(Qt::Key_Right))
    {
        player1->setDirection(QStringLiteral("right"));
    }
    else {
        player1->setDirection(QStringLiteral("stop"));
    }


    Player* player2 = m_players.at(1);
    if( (m_pressedKeys.contains(Qt::Key_W) && m_pressedKeys.contains(Qt::Key_A)) )
    {
        player2->setDirection(QStringLiteral("up-left"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_W) && m_pressedKeys.contains(Qt::Key_D)) )
    {
        player2->setDirection(QStringLiteral("up-right"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_S) && m_pressedKeys.contains(Qt::Key_A)) )
    {
        player2->setDirection(QStringLiteral("down-left"));
    }
    else if( (m_pressedKeys.contains(Qt::Key_S) && m_pressedKeys.contains(Qt::Key_D)) )
    {
        player2->setDirection(QStringLiteral("down-right"));
    }
    else if( m_pressedKeys.contains(Qt::Key_W))
    {
        player2->setDirection(QStringLiteral("up"));
    }
    else if( m_pressedKeys.contains(Qt::Key_S))
    {
        player2->setDirection(QStringLiteral("down"));
    }
    else if( m_pressedKeys.contains(Qt::Key_A))
    {
        player2->setDirection(QStringLiteral("left"));
    }
    else if( m_pressedKeys.contains(Qt::Key_D))
    {
        player2->setDirection(QStringLiteral("right"));
    }
    else {
        player2->setDirection(QStringLiteral("stop"));
    }


    return false;
}
