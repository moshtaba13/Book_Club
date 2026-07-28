#ifndef SEEALLBOOKSWIDGET_H
#define SEEALLBOOKSWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include "Book.h"


class SeeAllBooksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeeAllBooksWidget(QWidget *parent = nullptr);
    ~SeeAllBooksWidget();

    void loadBooks(const QString &sectionTitle, const QVector<Book> &books);
    void loadGenreHighlights(const QString &sectionTitle, const QVector<Book> &highlightPerGenre);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void backRequested();
    void bookClicked(const Book &book);
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
