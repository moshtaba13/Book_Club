#ifndef PDFREADERWIDGET_H
#define PDFREADERWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPdfDocument>
#include <QPdfView>

class PdfReaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PdfReaderWidget(QWidget *parent = nullptr);

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
