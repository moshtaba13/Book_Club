#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include "Cart.h"
#include "Book.h"

class CartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CartWidget(Cart *cart, QWidget *parent = nullptr);

public slots:
    void updateUI();
    void refreshFromServer();

signals:
    void backToHomeRequested();
    void checkoutSuccessful();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    QWidget* createCartItemRow(const Book &book);

    Cart *m_cart;

    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *itemsLayout;

    QLabel *lblTotalItemsVal;
    QLabel *lblTotalPriceVal;
    QLabel *lblDiscountVal;
    QLabel *lblFinalPriceVal;
    QPushButton *btnCheckout;
};

#endif // CARTWIDGET_H