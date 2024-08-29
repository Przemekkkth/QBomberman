#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "button.h"

class OptionsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    OptionsScene(QObject* parent = nullptr);
private:
    void createConnections();
    Button* m_backButton;

signals:
    void menuActivated();
    // QGraphicsScene interface
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // OPTIONSSCENE_H
