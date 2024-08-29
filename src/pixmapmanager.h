#ifndef PIXMAPMANAGER_H
#define PIXMAPMANAGER_H


#include "resourceholder.h"
#include <QPixmap>

class PixmapManager
{
public:
    enum TextureID
    {
        Ball, Button, PressedButton, Title, MenuBG, CreditsLogo, Raycast, Tiles, Player, Bomb, Player2, Frame, PiLogo, Avatar
    };
    static PixmapManager* Instance();
    QPixmap& getPixmap(TextureID id);
private:
    PixmapManager();
    PixmapManager(PixmapManager& other) = delete;
    void operator=(const PixmapManager &) = delete;

    ResourceHolder<QPixmap, TextureID> m_textures;
    static PixmapManager* ptr;
};

#endif // PIXMAPMANAGER_H
