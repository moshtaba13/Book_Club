#include "genreselectionwidget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>

GenreSelectionWidget::GenreSelectionWidget(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("genreSelectionPage");

    this->setAutoFillBackground(true);

    this->setStyleSheet(
        "QWidget#genreSelectionPage {"
        "   background-color: #FFFFFF;"
        "}"
        "QLabel {"
        "   background-color: transparent;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(60, 60, 60, 60);
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignTop);

    lblTitle = new QLabel("Welcome to Book Club!", this);
    lblTitle->setFont(QFont("Segoe UI", 22, QFont::Bold));
    lblTitle->setStyleSheet("color: #84C0D2; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblTitle);

    lblSubtitle = new QLabel("Choose 1 to 3 genres you love, so we can recommend books for you", this);
    lblSubtitle->setFont(QFont("Segoe UI", 12, QFont::DemiBold));
    lblSubtitle->setStyleSheet("color: #FF69B4; background: transparent;");
    lblSubtitle->setAlignment(Qt::AlignCenter);
    lblSubtitle->setWordWrap(true);
    mainLayout->addWidget(lblSubtitle);

    mainLayout->addSpacing(15);

    QFrame *gridFrame = new QFrame(this);
    gridFrame->setStyleSheet(
        "QFrame {"
        "   background-color: transparent;"
        "}"
        );
    QGridLayout *gridLayout = new QGridLayout(gridFrame);
    gridLayout->setSpacing(15);

    genreValues = {
        genre::Fiction, genre::Non_fiction, genre::Academic, genre::Drama, genre::Mystery,
        genre::Crime, genre::Romance, genre::History, genre::Science_fiction, genre::Scary
    };

    int row = 0, col = 0;
    for (genre g : genreValues) {
        QCheckBox *chk = new QCheckBox(genreToString(g), gridFrame);
        chk->setFont(QFont("Segoe UI", 11));
        chk->setStyleSheet(
            "QCheckBox {"
            "   color: #84C0D2;"
            "   background: transparent;"
            "   padding: 8px;"
            "}"
            "QCheckBox::indicator {"
            "   width: 18px;"
            "   height: 18px;"
            "   border: 2px solid #FF69B4;"
            "   border-radius: 4px;"
            "   background-color: #FFFFFF;"
            "}"
            "QCheckBox::indicator:checked {"
            "   background-color: #FFB6C1;"
            "   border: 2px solid #FFB6C1;"
            "}"
            );
        connect(chk, &QCheckBox::toggled, this, &GenreSelectionWidget::onCheckboxToggled);
        genreCheckboxes.append(chk);
        gridLayout->addWidget(chk, row, col);

        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }

    mainLayout->addWidget(gridFrame);

    lblWarning = new QLabel("", this);
    lblWarning->setStyleSheet("color: #A33A4A; background: transparent; font-size: 11px;");
    lblWarning->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblWarning);

    mainLayout->addStretch();

    btnContinue = new QPushButton("Continue", this);
    btnContinue->setMinimumHeight(45);
    btnContinue->setCursor(Qt::PointingHandCursor);
    btnContinue->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255,255,255,220);"
        "   color: #2C3E50;"
        "   border: 1.5px solid #FFC0CB;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFC0CB;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #FFB6C1;"
        "}"
        );
    connect(btnContinue, &QPushButton::clicked, this, &GenreSelectionWidget::onContinueClicked);
    mainLayout->addWidget(btnContinue);
}

QString GenreSelectionWidget::genreToString(genre g) const
{
    switch (g) {
    case genre::Fiction: return "Fiction";
    case genre::Non_fiction: return "Non-Fiction";
    case genre::Academic: return "Academic";
    case genre::Drama: return "Drama";
    case genre::Mystery: return "Mystery";
    case genre::Crime: return "Crime";
    case genre::Romance: return "Romance";
    case genre::History: return "History";
    case genre::Science_fiction: return "Science Fiction";
    case genre::Scary: return "Scary";
    }
    return "Unknown";
}

void GenreSelectionWidget::onCheckboxToggled()
{
    int checkedCount = 0;
    for (QCheckBox *chk : genreCheckboxes) {
        if (chk->isChecked()) {
            checkedCount++;
        }
    }

    for (QCheckBox *chk : genreCheckboxes) {
        if (!chk->isChecked()) {
            chk->setEnabled(checkedCount < 3);
        }
    }
}

void GenreSelectionWidget::onContinueClicked()
{
    QVector<genre> selected;
    for (int i = 0; i < genreCheckboxes.size(); ++i) {
        if (genreCheckboxes[i]->isChecked()) {
            selected.append(genreValues[i]);
        }
    }

    if (selected.isEmpty() || selected.size() > 3) {
        lblWarning->setText("Please select 1 to 3 genres.");
        return;
    }

    lblWarning->setText("");
    emit genresSelected(selected);
}