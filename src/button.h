#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QGraphicsObject>


class Button : public QGraphicsObject
{
    Q_OBJECT
public:
    Button(QString text = "");

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QPainterPath shape() const override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void clicked();
private:
    bool m_pressed;
    QPixmap m_buttonPixmap, m_pressedButtonPixmap;
    QString m_text;
};



#endif // BUTTON_H
