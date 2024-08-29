#ifndef CREDITSSCENE_H
#define CREDITSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


class Button;
class CreditsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CreditsScene(QObject *parent = nullptr);

signals:
    void menuActivated();
private:
    void createConnections();
    Button* m_backButton;

    // QGraphicsScene interface
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // CREDITSSCENE_H
