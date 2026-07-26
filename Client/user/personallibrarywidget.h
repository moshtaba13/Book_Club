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

class PersonalLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PersonalLibraryWidget(QWidget *parent = nullptr);
    void loadUser(User &user);

signals:
    void backToHomeRequested();
    void userUpdated(const User &updatedUser);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onCreateShelfClicked();
    void onBackClicked();

private:
    void refreshMyBooks();
    void refreshSavedBooks();
    void refreshShelves();
    QComboBox* buildShelfComboBox(int excludeIndex = -1);

    User *currentUser;

    QPushButton *backButton;

    QVBoxLayout *myBooksLayout;
    QVBoxLayout *savedBooksLayout;
    QVBoxLayout *shelvesLayout;

    QLineEdit *leNewShelfName;
    QPushButton *btnCreateShelf;
};
#endif // PERSONALLIBRARYWIDGET_H