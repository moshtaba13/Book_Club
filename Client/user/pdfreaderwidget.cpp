#include "pdfreaderwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QEventLoop>
#include <QPdfPageNavigator>
#include "networkclient.h"

PdfReaderWidget::PdfReaderWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("pdfReaderPage");
    this->setStyleSheet("QWidget#pdfReaderPage { background-color: #FCFCFC; }");

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // Header: back button, book title, page navigation and zoom controls -
    // styled to match BookDetailWidget's header.
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(75);
    headerWidget->setMaximumHeight(75);
    headerWidget->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-bottom: 1px solid rgba(0,0,0,15);");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 0, 25, 0);
    headerLayout->setSpacing(12);

    backButton = new QPushButton("⬅ Back", headerWidget);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4dbde;"
        "   color: #803040;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding-left: 15px;"
        "   padding-right: 15px;"
        "   min-height: 35px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
        );
    headerLayout->addWidget(backButton);

    titleLabel = new QLabel(headerWidget);
    titleLabel->setFont(QFont("Segoe UI", 13, QFont::Bold));
    titleLabel->setStyleSheet("color: #2E4D63; background: transparent;");
    headerLayout->addWidget(titleLabel, 1);

    zoomOutButton = new QPushButton("−", headerWidget);
    zoomInButton = new QPushButton("+", headerWidget);
    for (QPushButton *btn : {zoomOutButton, zoomInButton}) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedWidth(34);
        btn->setStyleSheet(
            "QPushButton { background-color: #FFD1DC; color: #803040; border: none; border-radius: 8px; font-weight: bold; min-height: 32px; }"
            "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
            );
    }
    headerLayout->addWidget(zoomOutButton);
    headerLayout->addWidget(zoomInButton);

    prevPageButton = new QPushButton("◀", headerWidget);
    nextPageButton = new QPushButton("▶", headerWidget);
    for (QPushButton *btn : {prevPageButton, nextPageButton}) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedWidth(34);
        btn->setStyleSheet(
            "QPushButton { background-color: #2C3E50; color: white; border: none; border-radius: 8px; font-weight: bold; min-height: 32px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
    }
    headerLayout->addWidget(prevPageButton);

    pageSpinBox = new QSpinBox(headerWidget);
    pageSpinBox->setMinimum(1);
    pageSpinBox->setMaximum(1);
    pageSpinBox->setFixedWidth(60);
    pageSpinBox->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 6px; padding: 4px; color: #2E4D63;");
    headerLayout->addWidget(pageSpinBox);

    pageCountLabel = new QLabel(headerWidget);
    pageCountLabel->setStyleSheet("color: #2E4D63; background: transparent;");
    headerLayout->addWidget(pageCountLabel);

    goToPageButton = new QPushButton("Go", headerWidget);
    goToPageButton->setCursor(Qt::PointingHandCursor);
    goToPageButton->setStyleSheet(
        "QPushButton { background-color: #FFD1DC; color: #803040; border: none; border-radius: 8px; font-weight: bold; padding: 4px 10px; min-height: 32px; }"
        "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
        );
    headerLayout->addWidget(goToPageButton);
    headerLayout->addWidget(nextPageButton);

    outerLayout->addWidget(headerWidget);

    // Body: the actual PDF page view.
    document = new QPdfDocument(this);

    pdfView = new QPdfView(this);
    pdfView->setDocument(document);
    pdfView->setPageMode(QPdfView::PageMode::SinglePage);
    pdfView->setZoomMode(QPdfView::ZoomMode::Custom);
    pdfView->setZoomFactor(1.0);
    pdfView->setStyleSheet("background-color: #E9ECEF;");
    outerLayout->addWidget(pdfView, 1);

    connect(backButton, &QPushButton::clicked, this, &PdfReaderWidget::onBackClicked);
    connect(prevPageButton, &QPushButton::clicked, this, &PdfReaderWidget::onPrevPageClicked);
    connect(nextPageButton, &QPushButton::clicked, this, &PdfReaderWidget::onNextPageClicked);
    connect(goToPageButton, &QPushButton::clicked, this, &PdfReaderWidget::onGoToPageClicked);
    connect(zoomInButton, &QPushButton::clicked, this, &PdfReaderWidget::onZoomInClicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &PdfReaderWidget::onZoomOutClicked);
    connect(pdfView->pageNavigator(), &QPdfPageNavigator::currentPageChanged,
            this, &PdfReaderWidget::onCurrentPageChanged);
}

void PdfReaderWidget::openBook(int bookId, const QString &title)
{
    // If a different book was already open, make sure its position is
    // saved before we swap the document out from under it.
    if (currentBookId != -1 && currentBookId != bookId)
        saveCurrentPage();

    QJsonObject data;
    data["book_id"] = bookId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetBookFile, data);
    if (response.value("status").toString() != "Success") {
        QMessageBox::warning(this, "Cannot Open Book",
                             response.value("message").toString("You must purchase this book first."));
        return;
    }

    QByteArray pdfBytes = QByteArray::fromBase64(
        response.value("data").toObject().value("pdf_data").toString().toLatin1());

    QString dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/bookclub";
    QDir().mkpath(dir);

    QString filePath = QString("%1/book_%2.pdf").arg(dir).arg(bookId);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly) || file.write(pdfBytes) < 0) {
        QMessageBox::warning(this, "Error", "Could not save the book file for reading.");
        return;
    }
    file.close();

    loadingBook = true;
    currentBookId = bookId;
    titleLabel->setText(title);

    document->load(filePath);
    // QPdfDocument::load() resolves synchronously for local files, but wait
    // on the status signal too in case a given Qt build makes it async.
    if (document->status() == QPdfDocument::Status::Loading) {
        QEventLoop loop;
        connect(document, &QPdfDocument::statusChanged, &loop, &QEventLoop::quit);
        loop.exec();
    }

    if (document->status() != QPdfDocument::Status::Ready) {
        QMessageBox::warning(this, "Error", "Could not open the book for reading.");
        loadingBook = false;
        return;
    }

    pageSpinBox->setMaximum(document->pageCount());
    pageCountLabel->setText(QString("/ %1").arg(document->pageCount()));

    // Resume from the last page read, if the server has one on record.
    QJsonObject pageResponse = NetworkClient::instance().sendRequest(RequestType::GetLastReadPage, data);
    int lastPage = 0;
    if (pageResponse.value("status").toString() == "Success")
        lastPage = pageResponse.value("data").toObject().value("page").toInt();

    lastPage = qBound(0, lastPage, document->pageCount() - 1);
    pdfView->pageNavigator()->jump(lastPage, QPointF());
    updatePageControls();

    loadingBook = false;
}

void PdfReaderWidget::updatePageControls()
{
    if (document->status() != QPdfDocument::Status::Ready)
        return;

    int page = pdfView->pageNavigator()->currentPage();
    pageSpinBox->blockSignals(true);
    pageSpinBox->setValue(page + 1);
    pageSpinBox->blockSignals(false);

    prevPageButton->setEnabled(page > 0);
    nextPageButton->setEnabled(page < document->pageCount() - 1);
}

void PdfReaderWidget::saveCurrentPage()
{
    if (currentBookId == -1 || document->status() != QPdfDocument::Status::Ready)
        return;

    QJsonObject data;
    data["book_id"] = currentBookId;
    data["page"] = pdfView->pageNavigator()->currentPage();
    NetworkClient::instance().sendRequest(RequestType::UpdateLastReadPage, data);
}

void PdfReaderWidget::onBackClicked()
{
    saveCurrentPage();
    emit backRequested();
}

void PdfReaderWidget::onPrevPageClicked()
{
    int page = pdfView->pageNavigator()->currentPage();
    if (page > 0)
        pdfView->pageNavigator()->jump(page - 1, QPointF());
}

void PdfReaderWidget::onNextPageClicked()
{
    int page = pdfView->pageNavigator()->currentPage();
    if (page < document->pageCount() - 1)
        pdfView->pageNavigator()->jump(page + 1, QPointF());
}

void PdfReaderWidget::onGoToPageClicked()
{
    int page = qBound(1, pageSpinBox->value(), document->pageCount()) - 1;
    pdfView->pageNavigator()->jump(page, QPointF());
}

void PdfReaderWidget::onZoomInClicked()
{
    pdfView->setZoomFactor(qMin(pdfView->zoomFactor() * 1.25, 5.0));
}

void PdfReaderWidget::onZoomOutClicked()
{
    pdfView->setZoomFactor(qMax(pdfView->zoomFactor() / 1.25, 0.2));
}

void PdfReaderWidget::onCurrentPageChanged(int page)
{
    Q_UNUSED(page);
    updatePageControls();

    if (loadingBook)
        return;

    saveCurrentPage();
}

void PdfReaderWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
