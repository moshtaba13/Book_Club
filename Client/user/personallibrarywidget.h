#ifndef PERSONALLIBRARYWIDGET_H
#define PERSONALLIBRARYWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QComboBox>
#include "User.h"
#include "Shelf.h"
#include "Purchase.h"

class PersonalLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalLibraryWidget(QWidget *parent = nullptr);
    void loadUser(User &user);

signals:
    void backToHomeRequested();
    void readRequested(int bookId, const QString &title);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onCreateShelfClicked();
    void onBackClicked();

private:
    void reloadLibraryData();
    void refreshMyBooks();
    void refreshSavedBooks();
    void refreshShelves();
    QComboBox* buildShelfComboBox(int excludeIndex = -1);

    int currentUserId = 0;
    QVector<Purchase> purchasedBooks;
    QVector<Book> savedBooks;
    QVector<Shelf> shelves;

    QPushButton *backButton;

    QVBoxLayout *myBooksLayout;
    QVBoxLayout *savedBooksLayout;
    QVBoxLayout *shelvesLayout;

    QLineEdit *leNewShelfName;
    QPushButton *btnCreateShelf;
};
#endif // PERSONALLIBRARYWIDGET_H