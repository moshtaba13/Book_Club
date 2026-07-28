#ifndef PDFREADERWIDGET_H
#define PDFREADERWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPdfDocument>
#include <QPdfView>

// In-app PDF reader used to open purchased books without leaving the
// application (previously, "Read" handed the file off to the OS's default
// PDF viewer via QDesktopServices::openUrl).
//
// Reading position is synced with the server through the GetLastReadPage /
// UpdateLastReadPage protocol messages: openBook() jumps to the page the
// user last left off on, and every page change while a book is open is
// saved back to the server so the position is picked up again next time -
// on this device or any other.
class PdfReaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PdfReaderWidget(QWidget *parent = nullptr);

    // Downloads the PDF for bookId (must already be purchased) via
    // GetBookFile, loads it into the embedded viewer, and jumps to the
    // last page read (via GetLastReadPage). Shows a warning dialog on
    // failure. title is shown in the reader's header.
    void openBook(int bookId, const QString &title);

signals:
    void backRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onBackClicked();
    void onPrevPageClicked();
    void onNextPageClicked();
    void onGoToPageClicked();
    void onZoomInClicked();
    void onZoomOutClicked();
    void onCurrentPageChanged(int page);

private:
    void updatePageControls();
    void saveCurrentPage();

    int currentBookId = -1;
    // Suppresses UpdateLastReadPage round-trips while openBook() itself is
    // moving the view to the saved page (that move is not a "read").
    bool loadingBook = false;

    QPdfDocument *document;
    QPdfView *pdfView;

    QLabel *titleLabel;
    QLabel *pageCountLabel;
    QSpinBox *pageSpinBox;

    QPushButton *backButton;
    QPushButton *prevPageButton;
    QPushButton *nextPageButton;
    QPushButton *goToPageButton;
    QPushButton *zoomOutButton;
    QPushButton *zoomInButton;
};
#endif // PDFREADERWIDGET_H
