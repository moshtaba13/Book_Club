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

signals:
    void backToHomeRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAddToCartClicked();
    void onBackClicked();
    void onSubmitReviewClicked();

private:
    QString genreToString(genre g) const;
    void refreshReviews();

    User *currentUserPtr;
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
    QPushButton *backButton;

    QVBoxLayout *reviewsListLayout;
    QComboBox *starSelector;
    QTextEdit *reviewTextEdit;
    QPushButton *submitReviewButton;
};
#endif // BOOKDETAILWIDGET_H