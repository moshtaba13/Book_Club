#ifndef BOOKDETAILWIDGET_H
#define BOOKDETAILWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QTextEdit>
#include "Book.h"
#include "Cart.h"
#include "User.h"

class BookDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookDetailWidget(Cart *cart, User *user, QWidget *parent = nullptr);
    void loadBook(const Book &book);

    // Downloads the PDF for bookId (must already be purchased) and opens it
    // with the system's default PDF viewer. Shows a warning dialog on
    // failure. Static so it can be called directly from anywhere (e.g. a
    // "Read" button in the personal library) without needing a loaded
    // BookDetailWidget instance.
    static void openBookFile(int bookId, QWidget *parentForDialogs);

signals:
    void backToHomeRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAddToCartClicked();
    void onReadClicked();
    void onBackClicked();
    void onSubmitReviewClicked();

private:
    QString genreToString(genre g) const;
    void refreshReviews();
    void resetReviewForm();

    User *currentUserPtr;
    bool editingReview = false;
    QPushButton *saveButton;

    Cart *mainCart;
    Book currentBook;

    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *authorLabel;
    QLabel *genreLabel;
    QLabel *descriptionLabel;
    QLabel *priceLabel;
    QLabel *originalPriceLabel;
    QLabel *ratingLabel;
    QPushButton *addToCartButton;
    QPushButton *readButton;
    QPushButton *backButton;

    QVBoxLayout *reviewsListLayout;
    QComboBox *starSelector;
    QTextEdit *reviewTextEdit;
    QPushButton *submitReviewButton;
};
#endif // BOOKDETAILWIDGET_H