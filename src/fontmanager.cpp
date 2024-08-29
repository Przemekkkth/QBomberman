#include "fontmanager.h"
#include <QFontDatabase>

FontManager::FontManager()
{
   int btnId = QFontDatabase::addApplicationFont(":/assets/ahronbd.ttf");
   QString family = QFontDatabase::applicationFontFamilies(btnId).at(0);
   QFont btnFont(family);
   m_fontMap[FontID::BUTTON] = btnFont;

   int mainID = QFontDatabase::addApplicationFont(":/assets/Cat.ttf");
   QString mainFamily = QFontDatabase::applicationFontFamilies(mainID).at(0);
   QFont mainFont(mainFamily);
   m_fontMap[FontID::MAIN] = mainFont;
}

FontManager* FontManager::ptr = nullptr;

FontManager *FontManager::Instance()
{
    if(!ptr)
    {
        ptr = new FontManager();
    }
    return ptr;
}

QFont FontManager::getFont(FontID id) const
{
    return m_fontMap[id];
}
