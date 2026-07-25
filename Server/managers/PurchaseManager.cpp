#include "PurchaseManager.h"
#include "NotificationManager.h"

PurchaseManager& PurchaseManager::instance() {
    static PurchaseManager instance;
    return instance;
}

bool PurchaseManager::addToCart(int userId, int bookId) {
    if (PurchaseRepository::instance().hasPurchased(userId, bookId))
        return false;

    return CartRepository::instance().addBook(userId, bookId);
}

bool PurchaseManager::checkout(int userId)
{
    Cart cart = CartRepository::instance().loadCart(userId);

    if (cart.isEmpty())
        return false;

    for (const Book &book : cart.getItems()) {
        Purchase purchase(
            book,
            book.getFinalPrice(),
            QDateTime::currentDateTime()
        );

        if (!PurchaseRepository::instance().save(userId, purchase))
            return false;

        SavedBookRepository::instance().remove(userId, book.getId());
        
        auto publisher = MemberRepository::instance().findByUsername(book.getPublisherUsername());
        if (publisher) {
            Publisher *pub = dynamic_cast<Publisher*>(publisher.get());
            if (pub) {
                double newRevenue = pub->getTotalRevenue() + book.getFinalPrice();
                MemberRepository::instance().updateTotalRevenue(pub->getId(), newRevenue);
            }
        }

        NotificationManager::instance().notifNewSale(publisher->getId(), book);
    }

    CartRepository::instance().clear(userId);
    cart.CheckOut();

    return true;
}