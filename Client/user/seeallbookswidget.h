#ifndef SEEALLBOOKSWIDGET_H
#define SEEALLBOOKSWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include "Book.h"

// Opened from home's "See all" buttons - shows every book behind whichever
// section the person tapped, using the same fixed book-card frame and the
// same color palette as the Home page.
class SeeAllBooksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeeAllBooksWidget(QWidget *parent = nullptr);
    ~SeeAllBooksWidget();

    void loadBooks(const QString &sectionTitle, const QVector<Book> &books);
    // Shows one card per genre (its own genre() tells you which genre it
    // represents) with the genre's name as the caption instead of a title;
    // tapping a card emits genreClicked instead of bookClicked.
    void loadGenreHighlights(const QString &sectionTitle, const QVector<Book> &highlightPerGenre);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void backRequested();
    void bookClicked(const Book &book);
    // Emitted instead of bookClicked when this page is showing genre
    // highlight cards (see loadGenreHighlights) and one is tapped.
    void genreClicked(genre g);

private:
    QWidget *headerWidget;
    QPushButton *btnBack;
    QLabel *lblTitle;

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QGridLayout *gridLayout;
};
#endif // SEEALLBOOKSWIDGET_H
