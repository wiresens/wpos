#ifndef PDF_VIEWER_H
#define PDF_VIEWER_H

#include <QLoggingCategory>
#include <QMainWindow>

Q_DECLARE_LOGGING_CATEGORY(lcExample)

namespace Ui {
class PdfViewer;
}

class QPdfDocument;
class QPdfView;
class QPdfPageSelector;

// class PageSelector;
class ZoomSelector;

class PdfViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit PdfViewer(QWidget *parent = nullptr);
    ~PdfViewer();

public slots:
    void open(const QUrl &docLocation);
    void close();

private slots:
    void bookmarkSelected(const QModelIndex &index);
    void pageSelected(int page);

    // action handlers
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPreviousPage_triggered();
    void on_actionNextPage_triggered();
    void on_actionContinuous_triggered();

private:
    Ui::PdfViewer *ui{};
    ZoomSelector *m_zoomSelector{};
    QPdfPageSelector *m_pageSelector{};

    QPdfDocument *m_document{};
};

#endif // PDF_VIEWER_H
