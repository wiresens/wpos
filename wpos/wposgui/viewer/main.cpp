// Copyright (C) 2024 Wiresens Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "pdf_viewer.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QUrl>

namespace viewer{
int main(int argc, char *argv[]){
    QCoreApplication::setApplicationName("Qt PDF Viewer");
    QCoreApplication::setOrganizationName("Wiresens");

    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    PdfViewer viewer;
    viewer.show();
    if (!parser.positionalArguments().isEmpty())
        viewer.open(QUrl::fromLocalFile(parser.positionalArguments().constFirst()));
    return app.exec();
}
}
