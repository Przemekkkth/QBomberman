#include "button.h"
#include "pixmapmanager.h"
#include "fontmanager.h"
#include <QPainter>
#include <QFontMetrics>

Button::Button(QString text)
    : m_pressed(false), m_text(text)
{
    m_buttonPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::Button);
    m_buttonPixmap = m_buttonPixmap.scaled(300, 70);
    m_pressedButtonPixmap = PixmapManager::Instance()->getPixmap(PixmapManager::PressedButton);
    m_pressedButtonPixmap = m_pressedButtonPixmap.scaled(300, 70);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

QRectF Button::boundingRect() const
{
    return QRectF(-m_buttonPixmap.width() / 2.0f,
                  -m_buttonPixmap.height() / 2.0f,
                  m_buttonPixmap.width(),
                  m_buttonPixmap.height());
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if (m_pressed) {
        painter->drawPixmap(-m_buttonPixmap.width() / 2.0f, -m_buttonPixmap.height() / 2.0f, m_pressedButtonPixmap);
    }
    else {
        painter->drawPixmap(-m_buttonPixmap.width() / 2.0f, -m_buttonPixmap.height() / 2.0f, m_buttonPixmap);
    }
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::BUTTON);
    font.setPixelSize(30);

    painter->setFont(font);
    painter->setPen(QColor(0,107,139));
    painter->setBrush(QBrush(QColor(0,107,139) ) );

    QFontMetrics fm(painter->font());
    QRect textRect = fm.boundingRect(m_text);

    painter->drawText(-textRect.width() / 2.0f, textRect.height() / 4.0f, m_text);
    painter->restore();
}

QPainterPath Button::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = true;
    update();
    QGraphicsObject::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    m_pressed = false;
    update();
    QGraphicsObject::mouseReleaseEvent(event);
}

