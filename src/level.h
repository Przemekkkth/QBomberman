#ifndef LEVEL_H
#define LEVEL_H
#include <QString>
#include "utils.h"
#include <QMap>
#include <QVector>
#include <QPair>
#include <QPixmap>


class Level
{
public:
    Level();
    ~Level();
    const QString LEVEL_ID;
    const float CURRENT_VERSION;

    /// Loads level from file
    bool loadFromFile(QString path);

    /// Get Tile Type at index (x,y)
    TT::TileType getTile(size_t x, size_t y);

    /// Get level width in tiles
    size_t getWidth() const;

    /// Get level height in tiles
    size_t getHeight() const;

    ///Changes particular tile's type
    bool destroyTile(size_t x, size_t y, bool destroyTexture=true);
    void setTileAsDestroyable(size_t x, size_t y);
    void setTileAsBomb(size_t x, size_t y);

    //void setLevelView(LevelView* view);
    QPixmap m_spriteSheets;
    QPixmap getTilePixmap(size_t x, size_t y);

    QVector < QVector <TT::TileType> > data() const;
    int tileHeight() const;
    int tileWidth() const;
private:
    size_t m_width;
    size_t m_height;
    //LevelView* m_view;
    const float m_version = 1.0f;

    //std::vector< std::vector<TT::TileType> > m_data;
    QVector < QVector <TT::TileType> > m_data;

    //std::map<int, std::pair<int, int> > m_destroyableTiles;
    QMap<int, QPair<int, int> > m_destroyableTiles;
    int m_destroyableTilesKey;

    //std::map<int, std::pair<int, int> >::iterator getIteratorByValue(std::pair<int,int>);

    QMap<int, QPair<int, int> >::iterator getIteratorByValue(QPair<int, int> );

    int m_rows;
    int m_columns;
    int m_cellHeight, m_cellWidth;
};

#endif // LEVEL_H
