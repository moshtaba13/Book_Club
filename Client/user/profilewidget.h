#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVector>
#include <QVBoxLayout>
#include "User.h"
#include "UserManager.h"

class ProfileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileWidget(UserManager *manager, QWidget *parent = nullptr);
    void loadUser(const User &user);

signals:
    void backToHomeRequested();
    void userUpdated(const User &updatedUser);
    void logoutRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSaveInfoClicked();
    void onChangePasswordClicked();
    void onSaveGenresClicked();
    void onBackClicked();

private:
    QString genreToString(genre g) const;
    void refreshGenreCheckboxes();
    void refreshPurchaseHistory();

    UserManager *userManager;
    User currentUser;

    QPushButton *backButton;

    // اطلاعات حساب
    QLineEdit *leUsername;

    QPushButton *btnSaveInfo;

    // تغییر رمز عبور
    QLineEdit *leNewPassword;
    QLineEdit *leConfirmPassword;
    QPushButton *btnChangePassword;
    QLineEdit *leCurrentPassword;

    // ژانرهای مورد علاقه
    QVector<QCheckBox*> genreCheckboxes;
    QVector<genre> genreValues;
    QPushButton *btnSaveGenres;

    // تاریخچه خرید
    QLabel *lblPurchaseCount;
    QVBoxLayout *purchaseHistoryLayout;

    QPushButton *btnLogout;
};
#endif // PROFILEWIDGET_H