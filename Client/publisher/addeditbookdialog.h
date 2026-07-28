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

    // Local filesystem paths chosen via the file pickers. In edit mode,
    // these stay equal to the book's existing (server-side) paths unless
    // the publisher picks a new file - callers should treat a non-absolute
    // path as "unchanged, don't re-upload".
    QString getCoverFilePath() const { return coverPath; }
    QString getPdfFilePath() const { return pdfPath; }

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