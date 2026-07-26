#ifndef HOME_H
#define HOME_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Book.h"

class home : public QWidget
{
    Q_OBJECT
public:
    explicit home(QWidget *parent = nullptr);
    ~home();

    void loadGenreBooks(const QVector<Book> &books);
    void loadFeaturedBooks(const QVector<Book> &books);
    void loadRecommendedBooks(const QVector<Book> &books);
    void loadNewReleases(const QVector<Book> &books);
    void loadBestSellers(const QVector<Book> &books);
    void loadFreeBooks(const QVector<Book> &books);
    void showSearchResults(const QVector<Book> &results, const QString &query);
    void clearSearchResults();


protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void cartRequested();
    void bookClicked(const Book &book);
    void profileRequested();
    void libraryRequested();
    void searchRequested(const QString &query);
    void notificationsRequested();
private:
    QWidget *createBookWidget(const Book &book, QWidget *parent);

    QWidget *headerWidget;
    QLabel *lblLogo;
    QLineEdit *leSearch;
    QPushButton *btnNotification;
    QPushButton *btnCart;
    QPushButton *btnProfile;

    QScrollArea *scrollArea;
    QWidget *scrollWidget;

    QFrame *genresFrame;
    QFrame *featuredFrame;
    QFrame *recommendedFrame;
    QFrame *newReleasesFrame;
    QFrame *bestSellersFrame;
    QFrame *freeBooksFrame;

    QHBoxLayout *genresItemsLayout;
    QHBoxLayout *featuredItemsLayout;
    QHBoxLayout *recommendedItemsLayout;
    QHBoxLayout *newReleasesItemsLayout;
    QHBoxLayout *bestSellersItemsLayout;
    QHBoxLayout *freeBooksItemsLayout;

    QPushButton *btnLibrary;

    QFrame *searchResultsFrame;
    QHBoxLayout *searchResultsItemsLayout;
    QLabel *lblSearchResultsTitle;
};
#endif // HOME_H