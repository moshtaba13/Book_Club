#ifndef GENRESELECTIONWIDGET_H
#define GENRESELECTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QVector>
#include "Book.h"

class GenreSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GenreSelectionWidget(QWidget *parent = nullptr);

signals:
    void genresSelected(const QVector<genre> &selectedGenres);

private slots:
    void onContinueClicked();
    void onCheckboxToggled();

private:
    QVector<QCheckBox*> genreCheckboxes;
    QVector<genre> genreValues;

    QLabel *lblTitle;
    QLabel *lblSubtitle;
    QLabel *lblWarning;
    QPushButton *btnContinue;

    QString genreToString(genre g) const;
};

#endif // GENRESELECTIONWIDGET_H