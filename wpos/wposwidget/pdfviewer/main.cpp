#include "pdfviewer.h"
#include <QApplication>
#include <QUrl>

namespace viewer{
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PdfViewer w;
    QStringList args = a.arguments();
    w.show();
    if (args.length() > 1)  w.open(QUrl::fromLocalFile(args[1]));
    return a.exec();
}
}
