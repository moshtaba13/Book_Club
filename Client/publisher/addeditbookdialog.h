#ifndef ADDEDITBOOKDIALOG_H
#define ADDEDITBOOKDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "Book.h"

class AddEditBookDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddEditBookDialog(QWidget *parent = nullptr);
    void loadForEdit(const Book &book);
    Book getResultBook() const;

private slots:
    void onChooseCoverClicked();
    void onChoosePdfClicked();
    void onSaveClicked();

private:
    QString genreToString(genre g) const;

    bool isEditMode;
    int editingBookId;
    QString coverPath;
    QString pdfPath;

    QLineEdit *leTitle;
    QLineEdit *leAuthor;
    QComboBox *cbGenre;
    QTextEdit *teDescription;
    QLineEdit *lePrice;
    QLineEdit *leDiscount;
    QLabel *lblCoverStatus;
    QPushButton *btnChooseCover;
    QLabel *lblPdfStatus;
    QPushButton *btnChoosePdf;
    QPushButton *btnSave;
    QPushButton *btnCancel;
};
#endif // ADDEDITBOOKDIALOG_H