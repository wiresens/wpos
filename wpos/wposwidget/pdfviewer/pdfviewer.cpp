#include "pdfviewer.h"
#include "ui_pdfviewer.h"

#include "pageselector.h"
#include "zoomselector.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QtPdf/QPdfBookmarkModel>
#include <QtPdf/QPdfDocument>
#include <QtPdf/QPdfPageNavigation>

const qreal zoomMultiplier = 1.414213562373 ; //qSqrt(2.0)

Q_LOGGING_CATEGORY(lcExample, "qt.examples.pdfviewer")

PdfViewer::PdfViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_zoomSelector(new ZoomSelector(this))
    , m_pageSelector(new PageSelector(this))
    , m_pdf(new QPdfDocument(this))
{
    ui->setupUi(this);
    m_zoomSelector->setMaximumWidth(150);
    ui->mainToolBar->insertWidget(ui->actionZoomIn, m_zoomSelector);

    m_pageSelector->setMaximumWidth(150);

    ui->mainToolBar->addWidget(m_pageSelector);
    ui->mainToolBar->addAction(ui->actionContinuous);

    m_pageSelector->setPageNavigation(ui->pdfView->pageNavigation());

    connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView, &QPdfView::setZoomMode);
    connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView, &QPdfView::setZoomFactor);
    m_zoomSelector->reset();

    QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
    bookmarkModel->setDocument(m_pdf);

    ui->bookmarkView->setModel(bookmarkModel);
    connect(ui->bookmarkView, SIGNAL(activated(QModelIndex)), this, SLOT(bookmarkSelected(QModelIndex)));

    ui->tabWidget->setTabEnabled(1, false); // disable 'Pages' tab for now

    ui->pdfView->setDocument(m_pdf);
    connect(ui->pdfView, &QPdfView::zoomFactorChanged, m_zoomSelector, &ZoomSelector::setZoomFactor);
}

PdfViewer::~PdfViewer()
{
    delete ui;
}

void PdfViewer::open(const QUrl &docLocation)
{
    if (docLocation.isLocalFile()) {
        m_pdf->load(docLocation.toLocalFile());
        const auto documentTitle = m_pdf->metaData(QPdfDocument::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    } else {
        qCDebug(lcExample) << docLocation << "is not a valid local file";
        QMessageBox::critical(this, tr("Failed to open"), tr("%1 is not a valid local file").arg(docLocation.toString()));
    }
    qCDebug(lcExample) << docLocation;
}

void PdfViewer::close(){
    m_pdf->close();
}

void PdfViewer::bookmarkSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    const int page = index.data(QPdfBookmarkModel::PageNumberRole).toInt();
    ui->pdfView->pageNavigation()->setCurrentPage(page);
}

void PdfViewer::on_actionOpen_triggered()
{
    QUrl url = QFileDialog::getOpenFileUrl(this, tr("Choose a PDF"), QUrl(), "Portable Documents (*.pdf)");
    if (url.isValid()) open(url);
}

void PdfViewer::on_actionQuit_triggered()
{
    QApplication::quit();
}

void PdfViewer::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Viewer"), tr("PDF Viewer for wPOS"));
}

void PdfViewer::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void PdfViewer::on_actionZoomIn_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() * zoomMultiplier);
}

void PdfViewer::on_actionZoomOut_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() / zoomMultiplier);
}

void PdfViewer::on_actionPreviousPage_triggered()
{
    ui->pdfView->pageNavigation()->goToPreviousPage();
}

void PdfViewer::on_actionNextPage_triggered()
{
    ui->pdfView->pageNavigation()->goToNextPage();   
}

void PdfViewer::on_actionContinuous_triggered()
{
    ui->pdfView->setPageMode(ui->actionContinuous->isChecked() ? QPdfView::MultiPage : QPdfView::SinglePage);
}
