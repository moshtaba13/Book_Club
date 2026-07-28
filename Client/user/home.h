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

    void loadGenreHighlights(const QVector<Book> &highlightPerGenre);
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
    void seeAllRequested(const QString &sectionTitle, const QVector<Book> &books);
    // Genres row overflow - a list of highlight books (one per genre),
    // shown differently from a plain book list (see seeAllRequested).
    void seeAllGenresRequested(const QVector<Book> &highlightPerGenre);
    // Emitted when a Genres-row card (or a card in the genre "See all"
    // grid) is clicked - g is the genre that card represents.
    void genreClicked(genre g);
private:
    QWidget *createBookWidget(const Book &book, QWidget *parent);
    QWidget *createGenreWidget(const Book &highlightBook, QWidget *parent);

    // Full lists behind each section - the row itself only ever shows the
    // first few, "See all" opens the rest of whichever list is here.
    // genresBooksAll holds one "most popular" highlight book per genre
    // (its own genre() tells you which genre it represents), not a list
    // of books within a single genre.
    QVector<Book> genresBooksAll;
    QVector<Book> featuredBooksAll;
    QVector<Book> recommendedBooksAll;
    QVector<Book> newReleasesBooksAll;
    QVector<Book> bestSellersBooksAll;
    QVector<Book> freeBooksBooksAll;

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