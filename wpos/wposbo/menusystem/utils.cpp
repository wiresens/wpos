// file      :  utils.cpp
// birth     :  27/9/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "utils.h"
#include <QTransform>

QPixmap cropedIcon(const QString& absolute_file_path, double size)
{
    QPixmap pixmap(absolute_file_path);
    QTransform transform;
    auto width = size / pixmap.width();
    auto height = size / pixmap.height();
    transform.scale(width, height);
    return QPixmap(pixmap.transformed(transform));
}
