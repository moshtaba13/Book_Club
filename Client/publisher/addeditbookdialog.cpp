#include "addeditbookdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

AddEditBookDialog::AddEditBookDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false), editingBookId(0)
{
    this->setWindowTitle("Add / Edit Book");
    this->setMinimumWidth(400);
    this->setStyleSheet("QDialog { background-color: #FFF8F2; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(10);

    QLabel *lblTitle = new QLabel("Title", this);
    lblTitle->setStyleSheet("color: #2C3E50; font-weight: bold;");
    layout->addWidget(lblTitle);
    leTitle = new QLineEdit(this);
    leTitle->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(leTitle);

    QLabel *lblAuthor = new QLabel("Author", this);
    lblAuthor->setStyleSheet("color: #2C3E50; font-weight: bold;");
    layout->addWidget(lblAuthor);
    leAuthor = new QLineEdit(this);
    leAuthor->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(leAuthor);

    QLabel *lblGenre = new QLabel("Genre", this);
    lblGenre->setStyleSheet("color: #2C3E50; font-weight: bold;");
    layout->addWidget(lblGenre);
    cbGenre = new QComboBox(this);
    cbGenre->addItems({"Fiction", "Non-Fiction", "Academic", "Drama", "Mystery",
                       "Crime", "Romance", "History", "Science Fiction", "Scary"});
    cbGenre->setStyleSheet("background-color: rgba(255,255,255,220); border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(cbGenre);

    QLabel *lblDesc = new QLabel("Description", this);
    lblDesc->setStyleSheet("color: #2C3E50; font-weight: bold;");
    layout->addWidget(lblDesc);
    teDescription = new QTextEdit(this);
    teDescription->setMaximumHeight(70);
    teDescription->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(teDescription);

    QHBoxLayout *priceRow = new QHBoxLayout();
    QVBoxLayout *priceCol = new QVBoxLayout();
    QLabel *lblPrice = new QLabel("Price ($)", this);
    lblPrice->setStyleSheet("color: #2C3E50; font-weight: bold;");
    priceCol->addWidget(lblPrice);
    lePrice = new QLineEdit(this);
    lePrice->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    priceCol->addWidget(lePrice);
    priceRow->addLayout(priceCol);

    QVBoxLayout *discountCol = new QVBoxLayout();
    QLabel *lblDiscount = new QLabel("Discount (%)", this);
    lblDiscount->setStyleSheet("color: #2C3E50; font-weight: bold;");
    discountCol->addWidget(lblDiscount);
    leDiscount = new QLineEdit(this);
    leDiscount->setPlaceholderText("0");
    leDiscount->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    discountCol->addWidget(leDiscount);
    priceRow->addLayout(discountCol);

    layout->addLayout(priceRow);

    QHBoxLayout *coverRow = new QHBoxLayout();
    btnChooseCover = new QPushButton("Choose Cover Image", this);
    btnChooseCover->setCursor(Qt::PointingHandCursor);
    btnChooseCover->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 8px; padding: 6px; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    coverRow->addWidget(btnChooseCover);
    lblCoverStatus = new QLabel("No file chosen", this);
    lblCoverStatus->setStyleSheet("color: #706357; font-size: 11px;");
    coverRow->addWidget(lblCoverStatus, 1);
    layout->addLayout(coverRow);

    QHBoxLayout *pdfRow = new QHBoxLayout();
    btnChoosePdf = new QPushButton("Choose PDF File", this);
    btnChoosePdf->setCursor(Qt::PointingHandCursor);
    btnChoosePdf->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 8px; padding: 6px; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    pdfRow->addWidget(btnChoosePdf);
    lblPdfStatus = new QLabel("No file chosen", this);
    lblPdfStatus->setStyleSheet("color: #706357; font-size: 11px;");
    pdfRow->addWidget(lblPdfStatus, 1);
    layout->addLayout(pdfRow);

    layout->addSpacing(10);

    QHBoxLayout *buttonsRow = new QHBoxLayout();
    btnCancel = new QPushButton("Cancel", this);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet("color: #803040; background: transparent; border: none; font-weight: bold;");
    buttonsRow->addWidget(btnCancel);
    buttonsRow->addStretch();

    btnSave = new QPushButton("Save Book", this);
    btnSave->setCursor(Qt::PointingHandCursor);
    btnSave->setMinimumHeight(40);
    btnSave->setStyleSheet(
        "QPushButton { background-color: #2C3E50; color: white; border-radius: 8px; font-weight: bold; padding: 8px 20px; }"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    buttonsRow->addWidget(btnSave);
    layout->addLayout(buttonsRow);

    connect(btnChooseCover, &QPushButton::clicked, this, &AddEditBookDialog::onChooseCoverClicked);
    connect(btnChoosePdf, &QPushButton::clicked, this, &AddEditBookDialog::onChoosePdfClicked);
    connect(btnSave, &QPushButton::clicked, this, &AddEditBookDialog::onSaveClicked);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString AddEditBookDialog::genreToString(genre g) const
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

void AddEditBookDialog::loadForEdit(const Book &book)
{
    isEditMode = true;
    editingBookId = book.getId();

    leTitle->setText(book.getTitle());
    leAuthor->setText(book.getAuthor());
    cbGenre->setCurrentText(genreToString(book.getGenre()));
    teDescription->setPlainText(book.getDiscription());
    lePrice->setText(QString::number(book.getPrice()));
    leDiscount->setText(QString::number(book.getDiscount()));

    coverPath = book.getImagePath();
    pdfPath = book.getPdfPath();
    lblCoverStatus->setText(coverPath.isEmpty() ? "No file chosen" : "Selected: " + coverPath.split('/').last());
    lblPdfStatus->setText(pdfPath.isEmpty() ? "No file chosen" : "Selected: " + pdfPath.split('/').last());

    this->setWindowTitle("Edit Book");
}

void AddEditBookDialog::onChooseCoverClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose Cover Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!path.isEmpty()) {
        coverPath = path;
        lblCoverStatus->setText("Selected: " + path.split('/').last());
    }
}

void AddEditBookDialog::onChoosePdfClicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose PDF File", "", "PDF Files (*.pdf)");
    if (!path.isEmpty()) {
        pdfPath = path;
        lblPdfStatus->setText("Selected: " + path.split('/').last());
    }
}

void AddEditBookDialog::onSaveClicked()
{
    if (leTitle->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the book title.");
        return;
    }
    if (leAuthor->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the author name.");
        return;
    }

    bool priceOk;
    double price = lePrice->text().toDouble(&priceOk);
    if (!priceOk || price < 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid price.");
        return;
    }

    bool discountOk = true;
    double discount = 0.0;
    if (!leDiscount->text().trimmed().isEmpty()) {
        discount = leDiscount->text().toDouble(&discountOk);
    }
    if (!discountOk || discount < 0 || discount > 100) {
        QMessageBox::warning(this, "Error", "Discount must be between 0 and 100.");
        return;
    }

    accept();
}

Book AddEditBookDialog::getResultBook() const
{
    QString genreText = cbGenre->currentText();
    genre g = genre::Fiction;
    if (genreText == "Fiction") g = genre::Fiction;
    else if (genreText == "Non-Fiction") g = genre::Non_fiction;
    else if (genreText == "Academic") g = genre::Academic;
    else if (genreText == "Drama") g = genre::Drama;
    else if (genreText == "Mystery") g = genre::Mystery;
    else if (genreText == "Crime") g = genre::Crime;
    else if (genreText == "Romance") g = genre::Romance;
    else if (genreText == "History") g = genre::History;
    else if (genreText == "Science Fiction") g = genre::Science_fiction;
    else if (genreText == "Scary") g = genre::Scary;

    Book result(leTitle->text().trimmed(), leAuthor->text().trimmed(), g, lePrice->text().toDouble());
    if (!isEditMode) {
        result.setPublishDate(QDateTime::currentDateTime());
    }
    result.setDescription(teDescription->toPlainText().trimmed());
    result.setDiscount(leDiscount->text().trimmed().isEmpty() ? 0.0 : leDiscount->text().toDouble());
    result.setImagePath(coverPath);
    result.setPdfPath(pdfPath);
    result.setisActive(true);

    if (isEditMode) {
        result.setId(editingBookId);
    }

    return result;
}