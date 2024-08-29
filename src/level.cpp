#include "level.h"
#include "utils.h"
#include "pixmapmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Level::Level()
    : LEVEL_ID("BOMBERMAN_LEVEL"), CURRENT_VERSION(1.0), m_destroyableTilesKey(0)
{
    m_spriteSheets = PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Tiles);
    m_rows = 7;
    m_columns = 7;
    m_cellHeight = 64;
    m_cellWidth = 64;
}


Level::~Level()
{
}


bool Level::loadFromFile(QString path)
{
    QFile levelFile(path);
    QTextStream stream(&levelFile);
    if (levelFile.open(QIODevice::ReadOnly) ) {
        QString identifier = stream.readLine();
        qDebug() << "id " << identifier;
        float version = stream.readLine().toFloat();
        qDebug() << "version " << version;
        if (identifier != LEVEL_ID && version != CURRENT_VERSION) {
            qDebug() << "Wrong file format ";
            return false;
        }

        QStringList sizes = stream.readLine().split(QStringLiteral(" "), Qt::SkipEmptyParts);
        if (sizes.size() == 2) {
            m_width = sizes.at(0).toUInt();
            m_height = sizes.at(1).toUInt();
        }
        else {
            qDebug() << "No level size ";
            return false;
        }

        m_data.clear();
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            qDebug() << line;
            QStringList list = line.split(QStringLiteral(" "), Qt::SkipEmptyParts);
            QVector<TT::TileType> tiles;
            tiles.reserve(m_width);
            for (int i = 0; i < list.size(); ++i) {
                tiles.append(static_cast<TT::TileType>(list[i].toInt()));
            }
            m_data.append(tiles);
        }

        return true;
    }
    else {
        levelFile.close();
        return false;
    }

    return false;
}


TT::TileType Level::getTile(size_t x, size_t y)
{
    return m_data[y][x];
}


size_t Level::getWidth() const
{
    return m_width;
}


size_t Level::getHeight() const
{
    return m_height;
}

bool Level::destroyTile(size_t x, size_t y, bool destroyTexture)
{
    if(m_data[y][x] <= TT::TileType::NONE_WITH_SHADOW)
        return false;

    if (m_data[y - 1][x] >= TT::TileType::WEAK_WALL && m_data[y - 1][x] != TT::TileType::BOMB)
    {
        m_data[y][x] = TT::TileType::NONE_WITH_SHADOW;
        //m_view->ChangeTileTexture(x, y, TT::TileType::NONE_WITH_SHADOW);
    }
    else
    {
        m_data[y][x] = TT::TileType::NONE;
        //m_view->ChangeTileTexture(x, y, TT::TileType::NONE);
    }

    if (m_data[y + 1][x] == TT::TileType::NONE_WITH_SHADOW)
    {
        m_data[y+1][x] = TT::TileType::NONE;
        //m_view->ChangeTileTexture(x, y+1, TT::TileType::NONE);
    }
    if (m_data[y - 1][x] == TT::TileType::DOUBLE_WEAK_WALL)
    {
        m_data[y - 1][x] = TT::TileType::WEAK_WALL;
        //m_view->ChangeTileTexture(x, y - 1, TT::TileType::WEAK_WALL);
    }
    if (m_data[y - 1][x] == TT::TileType::DOUBLE_INDESTRUCTIBLE_WALL)
    {
        m_data[y - 1][x] = TT::TileType::INDESTRUCTIBLE_WALL;
        //m_view->ChangeTileTexture(x, y - 1, TT::TileType::INDESTRUCTIBLE_WALL);
    }
    if (m_data[y - 1][x] == TT::TileType::HALF_INDESTRUCTIBLE_WALL)
    {
        m_data[y - 1][x] = TT::TileType::INDESTRUCTIBLE_WALL;
        //m_view->ChangeTileTexture(x, y - 1, TT::TileType::INDESTRUCTIBLE_WALL);
    }
    return true;

}

void Level::setTileAsBomb(size_t x, size_t y)
{
    m_data[y][x] = TT::TileType::BOMB;
}

QPixmap Level::getTilePixmap(size_t x, size_t y)
{
    int index = getTile(x, y);
    int _x = static_cast<int>(index % (m_rows) * m_cellWidth);
    int _y = static_cast<int>(index / (m_rows) * m_cellHeight);

    //sprite.setTexture(m_texture);
    //sprite.setTextureRect(sf::IntRect(x, y, static_cast<int>(m_cellWidth), static_cast<int>(m_cellHeight)));

    return m_spriteSheets.copy(_x, _y, m_cellWidth, m_cellHeight);
}

QVector<QVector<TT::TileType> > Level::data() const
{
    return m_data;
}

int Level::tileHeight() const
{
    return m_cellHeight;
}

int Level::tileWidth() const
{
    return m_cellWidth;
}

void Level::setTileAsDestroyable(size_t x, size_t y)
{
    m_data[y][x] = TT::TileType::WEAK_WALL;
}

// void Level::setLevelView(LevelView * view)
// {
//     m_view = view;
// }
