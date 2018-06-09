//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Program "Operation Addon" created by Dmytro Korniienko
//  stalker2021@ukr.net
//  Kyiv, Ukraine
//  Used "Qt Creator 4.2.2"
//
//  versions:
//  1.00    25.07.2017: - relise.
//
//  1.01    26.07.2017: - (fixed) addon with pick selected -> select addon without pic in same ListWidgetAddons -> old pic stay;
//                      - add load .jpg pictures.
//
//  2.00    17.08.2017: - (fixed) wrong pic scale, pic cut by height
//                      - (normal behavior, updated) cannot delete some file - Rus_Gen_L.pbo, cwkZodiac.pbo;
//                          *reasone: this files have 'ReadOnly' state.
//                          *action: added information popup.
//                      - add menu bar;
//                      - add resize main window possibility;
//                      - add checking of Addons file in Game directory;
//                      - add checkind of real Used addons;
//                      - add checking of Addons duplication;
//                      - add fast opening Game and Addons directories;
//                      - add load screen;
//                      - add bug file;
//                      - add control from keyboard: Up, Down, Ctrl with +, Crtl with -;
//                      - add information about Program and HotKeys.
//
//  2.10    22.08.2017: - (fixed) 'z' and 'Z' not indicate like same on "Check Duplication";
//                      - add name of addon to information popup (like "Addon is already added");
//                      - add button "back" for selected addon with hotkey (BackSpace);
//                      - add check Addon files by name with hotkay (Ctrl with F);
//                      - add check addons by name or description;
//                      - add check addons by  depend, what use selected addon.
//
//  #need fix:  - chage result of checking popup, form MessageBox to ResulWindow.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(QApplication::applicationDirPath() + "/OA.ico"));
    w.show();

    return a.exec();
}
