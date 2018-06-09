#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <QString>
#include <QVector>

struct GameClass
{
    QString gameDir;
    QString addonDir;

    QString name;
    QString usedAddonsFile;
    QString logoFile;

    QVector<QString> pathExe;
};

struct AddonRemember
{
    QString current;
    QString last;
};

#endif // MYSTRUCT_H
