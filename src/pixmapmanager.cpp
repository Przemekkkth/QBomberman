#include "pixmapmanager.h"

PixmapManager* PixmapManager::ptr = nullptr;

PixmapManager *PixmapManager::Instance()
{
    if(!ptr)
    {
        ptr = new PixmapManager();
    }
    return ptr;
}

QPixmap& PixmapManager::getPixmap(TextureID id)
{
    return m_textures.get(id);
}

PixmapManager::PixmapManager()
{
    //m_textures.load(TextureID::Ball, ":/res/sprite/ball.png");
    //std::unique_ptr<QPixmap> ButtonSelected(new QPixmap(getPixmap(TextureID::Buttons).copy(0,50,200,50)));
    //m_textures.insertResource(TextureID::ButtonSelected, std::move(ButtonSelected));

    m_textures.load(TextureID::Button, QStringLiteral(":/assets/unpressButton.png"));
    m_textures.load(TextureID::PressedButton, QStringLiteral(":/assets/pressButton.png"));
    m_textures.load(TextureID::Title, QStringLiteral(":/assets/menuLogo.png"));
    m_textures.load(TextureID::MenuBG, QStringLiteral(":/assets/menuBackground.png"));
    m_textures.load(TextureID::CreditsLogo, QStringLiteral(":/assets/credits.png"));
    m_textures.load(TextureID::Raycast, QStringLiteral(":/assets/sample_raytextures.png"));
    m_textures.load(TextureID::Tiles, QStringLiteral(":/assets/sample_terraintextures.png"));
    m_textures.load(TextureID::Player, QStringLiteral(":/assets/playersheets.png"));
    m_textures.load(TextureID::Bomb, QStringLiteral(":/assets/sample_bombtextures.png"));
    m_textures.load(TextureID::Player2, QStringLiteral(":/assets/playersheets1.png"));
    m_textures.load(TextureID::Frame, QStringLiteral(":/assets/frame.png"));
    m_textures.load(TextureID::PiLogo, QStringLiteral(":/assets/menuPiGames.png"));
    m_textures.load(TextureID::Avatar, QStringLiteral(":/assets/avatar.png"));
}
