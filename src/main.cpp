#include <QApplication>
#include "view.h"
#include "level.h"


int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    View v;
    //Level l;
    //l.loadFromFile(QStringLiteral(":/assets/sample_level.txt"));
    v.setWindowTitle(QStringLiteral("Bomberman Qt/C++"));


    v.show();
    a.exec();
}
