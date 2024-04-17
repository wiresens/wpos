#include "utils.h"
#include <QTransform>

QPixmap cropedIcon(const QString& absolute_file_path, double size){
    QPixmap pixmap(absolute_file_path);
    QTransform transform;
    auto width = size / pixmap.width();
    auto height = size / pixmap.height();
    transform.scale(width, height);
    return QPixmap(pixmap.transformed( transform ));
}
