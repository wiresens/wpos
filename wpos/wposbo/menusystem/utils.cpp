#include "utils.h"

QPixmap cropedIcon(const QString& absolute_file_path, double size){
    QPixmap pixmap(absolute_file_path);
    QMatrix matrix;
    auto width = size / pixmap.width();
    auto height = size / pixmap.height();
    matrix.scale(width, height);
    return QPixmap(pixmap.transformed( matrix ));
}
