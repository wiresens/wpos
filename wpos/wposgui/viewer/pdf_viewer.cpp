#include "pdf_viewer.h"
#include "ui_pdf_viewer.h"

// #include "page_selector.h"
#include "zoom_selector.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <QPdfPageNavigator>
#include <QPdfPageSelector>

const qreal zoomMultiplier = 1.414213562373 ; //qSqrt(2.0)
Q_LOGGING_CATEGORY(lcExample, "qt.examples.pdfviewer")

PdfViewer::PdfViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui{new Ui::PdfViewer}
    , m_zoomSelector(new ZoomSelector(this))
    , m_pageSelector(new QPdfPageSelector(this))
    , m_document(new QPdfDocument(this))
{
    ui->setupUi(this);

    m_zoomSelector->setMaximumWidth(150);
    ui->mainToolBar->insertWidget(ui->actionZoomIn, m_zoomSelector);

    m_pageSelector->setMaximumWidth(150);
    ui->mainToolBar->addWidget(m_pageSelector);
    ui->mainToolBar->addAction(ui->actionContinuous);
    connect(m_pageSelector, &QPdfPageSelector::currentPageChanged, this, &PdfViewer::pageSelected);

    m_pageSelector->setDocument(m_document);
    auto page_navigator = ui->pdfView->pageNavigator();
    connect(page_navigator, &QPdfPageNavigator::currentPageChanged, m_pageSelector, &QPdfPageSelector::setCurrentPage);
    // connect(page_navigator, &QPdfPageNavigator::backAvailableChanged, ui->actionBack, &QAction::setEnabled);
    // connect(page_navigator, &QPdfPageNavigator::forwardAvailableChanged, ui->actionForward, &QAction::setEnabled);

    connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView, &QPdfView::setZoomMode);
    connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView, &QPdfView::setZoomFactor);
    m_zoomSelector->reset();

    QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
    bookmarkModel->setDocument(m_document);

    ui->bookmarkView->setModel(bookmarkModel);
    connect(ui->bookmarkView, &QTreeView::activated, this, &PdfViewer::bookmarkSelected);

    ui->tabWidget->setTabEnabled(1, false); // disable 'Pages' tab for now

    ui->pdfView->setDocument(m_document);
    connect(ui->pdfView, &QPdfView::zoomFactorChanged, m_zoomSelector, &ZoomSelector::setZoomFactor);
}

PdfViewer::~PdfViewer()
{
    delete ui;
}

void PdfViewer::open(const QUrl &docLocation)
{
    if (docLocation.isLocalFile()) {
        m_document->load(docLocation.toLocalFile());
        const auto documentTitle = m_document->metaData(QPdfDocument::MetaDataField::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    } else {
        qCDebug(lcExample) << docLocation << "is not a valid local file";
        QMessageBox::critical(this, tr("Failed to open"), tr("%1 is not a valid local file").arg(docLocation.toString()));
    }
    qCDebug(lcExample) << docLocation;
}

void PdfViewer::close(){
    m_document->close();
}

void PdfViewer::bookmarkSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    const int page = index.data(int(QPdfBookmarkModel::Role::Page)).toInt();
    const qreal zoomLevel = index.data(int(QPdfBookmarkModel::Role::Level)).toReal();
    ui->pdfView->pageNavigator()->jump(page, {}, zoomLevel);
}

void PdfViewer::pageSelected(int page)
{
    auto nav = ui->pdfView->pageNavigator();
    nav->jump(page, {}, nav->currentZoom());
    const auto documentTitle = m_document->metaData(QPdfDocument::MetaDataField::Title).toString();
    setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    setWindowTitle(tr("%1: page %2 (%3 of %4)")
                       .arg(documentTitle.isEmpty() ? u"PDF Viewer"_qs : documentTitle,
                            m_pageSelector->currentPageLabel(), QString::number(page + 1), QString::number(m_document->pageCount())));
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
    auto nav = ui->pdfView->pageNavigator();
    nav->jump(nav->currentPage() - 1, {}, nav->currentZoom());
}

void PdfViewer::on_actionNextPage_triggered()
{
    auto nav = ui->pdfView->pageNavigator();
    nav->jump(nav->currentPage() + 1, {}, nav->currentZoom());
}

void PdfViewer::on_actionContinuous_triggered()
{
    ui->pdfView->setPageMode(ui->actionContinuous->isChecked() ? QPdfView::PageMode::MultiPage : QPdfView::PageMode::SinglePage);
}
