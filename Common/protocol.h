#pragma once
#include <QString>

#include "models/Book.h"

enum class RequestType {

    // ─────────────────────────────────────────
    // احراز هویت
    // ─────────────────────────────────────────
    Login,              // ورود به سیستم
    Register,           // ثبت‌نام کاربر عادی
    RegisterPublisher,  // ثبت‌نام ناشر
    ResetPassword,      // بازیابی رمز عبور با سوال امنیتی
    Logout,             // خروج از سیستم

    // ─────────────────────────────────────────
    // کتاب‌ها
    // ─────────────────────────────────────────
    GetAllBooks,            // دریافت همه کتاب‌های فعال
    GetBooksByGenre,        // دریافت کتاب‌ها بر اساس ژانر
    GetBooksByPublisher,    // دریافت کتاب‌های یه ناشر خاص
    SearchBooks,            // جستجو بر اساس عنوان/نویسنده/ناشر
    GetBookDetails,         // دریافت جزئیات یه کتاب خاص
    AddBook,                // افزودن کتاب جدید (ناشر)
    EditBook,               // ویرایش کتاب (ناشر)
    InActiveBook,           // حذف موقت کتاب (ناشر)
    RestoreBook,            // بازگردانی کتاب حذف‌شده (ناشر)
    ApplyDiscount,          // اعمال تخفیف روی کتاب (ناشر)

    // ─────────────────────────────────────────
    // نظرات و امتیاز
    // ─────────────────────────────────────────
    AddReview,      // ثبت نظر و امتیاز برای کتاب
    EditReview,     // ویرایش نظر ثبت‌شده
    DeleteReview,   // حذف نظر
    GetReviews,     // دریافت نظرات یه کتاب

    // ─────────────────────────────────────────
    // سبد خرید
    // ─────────────────────────────────────────
    AddToCart,      // افزودن کتاب به سبد خرید
    RemoveFromCart, // حذف کتاب از سبد خرید
    GetCart,        // دریافت محتوای سبد خرید با قیمت‌ها
    Checkout,       // نهایی کردن خرید

    // ─────────────────────────────────────────
    // کتابخانه شخصی
    // ─────────────────────────────────────────
    GetPurchasedBooks,  // دریافت کتاب‌های خریداری‌شده
    UpdateLastReadPage, // آپدیت آخرین صفحه مطالعه
    GetLastReadPage,    // دریافت آخرین صفحه مطالعه

    // ─────────────────────────────────────────
    // کتاب‌های ذخیره‌شده (علاقه‌مندی‌ها)
    // ─────────────────────────────────────────
    SaveBook,       // ذخیره کتاب برای مطالعه در آینده
    RemoveSavedBook,// حذف از لیست ذخیره‌شده‌ها
    GetSavedBooks,  // دریافت لیست کتاب‌های ذخیره‌شده

    // ─────────────────────────────────────────
    // قفسه‌های شخصی
    // ─────────────────────────────────────────
    GetShelves,             // دریافت قفسه‌های کاربر
    AddShelf,               // ایجاد قفسه جدید
    RemoveShelf,            // حذف قفسه
    RenameShelf,            // تغییر نام قفسه
    AddBookToShelf,         // افزودن کتاب به قفسه
    RemoveBookFromShelf,    // حذف کتاب از قفسه
    MoveBookBetweenShelves, // انتقال کتاب بین قفسه‌ها

    // ─────────────────────────────────────────
    // پروفایل کاربر
    // ─────────────────────────────────────────
    GetProfile,             // دریافت اطلاعات حساب کاربری
    ChangeUsername,          // تغییر نام کاربری     
    ChangePassword,         // تغییر رمز عبور
    UpdateFavoriteGenres,   // آپدیت ژانرهای مورد علاقه

    // ─────────────────────────────────────────
    // اعلان‌ها
    // ─────────────────────────────────────────
    GetNotifications,           // دریافت اعلان‌های کاربر
    MarkNotificationRead,       // علامت‌گذاری یه اعلان به‌عنوان خوانده‌شده
    MarkAllNotificationsRead,   // علامت‌گذاری همه اعلان‌ها

    // ─────────────────────────────────────────
    // ادمین - مدیریت کاربران
    // ─────────────────────────────────────────
    GetAllMembers,      // دریافت لیست تمام کاربران
    GetAllUsers,        // دریافت لیست همه کاربران عادی
    GetAllPublishers,   // دریافت لیست همه ناشران
    SearchUsers,        // جستجو در کاربران
    GetBlockedUsers,    // دریافت کاربران مسدودشده
    GetUnblockedUser,   // دریافت کاربران فعال
    BlockUser,          // مسدودسازی کاربر
    UnblockUser,        // رفع مسدودیت کاربر
    DeleteUser,         // حذف حساب کاربری
    // ─────────────────────────────────────────
    // ادمین - مدیریت کتاب‌ها
    // ─────────────────────────────────────────
    GetAllBooksAdmin,   // دریافت همه کتاب‌ها (فعال و غیرفعال)
    DeleteBookAdmin,    // حذف کتاب توسط ادمین
    EditBookAdmin,      // ویرایش کتاب توسط ادمین
    DeleteReviewAdmin,

    // ─────────────────────────────────────────
    // ناشر - آمار و داشبورد
    // ─────────────────────────────────────────
    GetPublisherStats,      // آمار کلی ناشر (درآمد، تعداد کتاب)
    GetTopSellingBooks,     // پرفروش‌ترین کتاب‌ها
    GetLeastSellingBooks,   // کم‌فروش‌ترین کتاب‌ها

    // ─────────────────────────────────────────
    // صفحه اصلی کاربر
    // ─────────────────────────────────────────
    GetRecommendedBooks,    // کتاب‌های پیشنهادی بر اساس ژانرهای علاقه
    GetNewestBooks,         // جدیدترین کتاب‌ها
    GetMostRatedBooks,      // محبوب‌ترین کتاب‌ها (بیشترین امتیاز)
    GetFreeBooks,           // کتاب‌های رایگان
    
    GetBookFile,  // دریافت فایل PDF کتاب
    GetBookCover, // دریافت تصویر کاور
};

/**
 * @brief وضعیت پاسخ سرور به کلاینت
 */
enum class ResponseStatus {
    Success,        // عملیات موفق
    Failure,        // خطا در عملیات
    Unauthorized,   // دسترسی غیرمجاز (لاگین نشده یا نقش کافی نداری)
    NotFound,       // مورد درخواستی پیدا نشد
    AlreadyExists,  // مورد از قبل وجود داره (مثل username تکراری)
    InvalidData     // داده‌های ورودی نامعتبر
};

// ─────────────────────────────────────────
// RequestType → QString
// برای تبدیل enum به رشته جهت ارسال در JSON
// ─────────────────────────────────────────
inline QString requestTypeToString(RequestType type)
{
    switch (type) {
    // احراز هویت
    case RequestType::Login:                    return "Login";
    case RequestType::Register:                 return "Register";
    case RequestType::RegisterPublisher:        return "RegisterPublisher";
    case RequestType::ResetPassword:            return "ResetPassword";
    case RequestType::Logout:                   return "Logout";

    // کتاب‌ها
    case RequestType::GetAllBooks:              return "GetAllBooks";
    case RequestType::GetBooksByGenre:          return "GetBooksByGenre";
    case RequestType::GetBooksByPublisher:      return "GetBooksByPublisher";
    case RequestType::SearchBooks:              return "SearchBooks";
    case RequestType::GetBookDetails:           return "GetBookDetails";
    case RequestType::AddBook:                  return "AddBook";
    case RequestType::EditBook:                 return "EditBook";
    case RequestType::InActiveBook:             return "InActiveBook";
    case RequestType::RestoreBook:              return "RestoreBook";
    case RequestType::ApplyDiscount:            return "ApplyDiscount";

    // نظرات
    case RequestType::AddReview:                return "AddReview";
    case RequestType::EditReview:               return "EditReview";
    case RequestType::DeleteReview:             return "DeleteReview";
    case RequestType::GetReviews:               return "GetReviews";

    // سبد خرید
    case RequestType::AddToCart:                return "AddToCart";
    case RequestType::RemoveFromCart:           return "RemoveFromCart";
    case RequestType::GetCart:                  return "GetCart";
    case RequestType::Checkout:                 return "Checkout";

    // کتابخانه شخصی
    case RequestType::GetPurchasedBooks:        return "GetPurchasedBooks";
    case RequestType::UpdateLastReadPage:       return "UpdateLastReadPage";
    case RequestType::GetLastReadPage:          return "GetLastReadPage";

    // ذخیره‌شده‌ها
    case RequestType::SaveBook:                 return "SaveBook";
    case RequestType::RemoveSavedBook:          return "RemoveSavedBook";
    case RequestType::GetSavedBooks:            return "GetSavedBooks";

    // قفسه‌ها
    case RequestType::GetShelves:               return "GetShelves";
    case RequestType::AddShelf:                 return "AddShelf";
    case RequestType::RemoveShelf:              return "RemoveShelf";
    case RequestType::RenameShelf:              return "RenameShelf";
    case RequestType::AddBookToShelf:           return "AddBookToShelf";
    case RequestType::RemoveBookFromShelf:      return "RemoveBookFromShelf";
    case RequestType::MoveBookBetweenShelves:   return "MoveBookBetweenShelves";

    // پروفایل
    case RequestType::GetProfile:               return "GetProfile";
    case RequestType::ChangeUsername:            return "ChangeUsername";
    case RequestType::ChangePassword:           return "ChangePassword";
    case RequestType::UpdateFavoriteGenres:     return "UpdateFavoriteGenres";

    // اعلان‌ها
    case RequestType::GetNotifications:         return "GetNotifications";
    case RequestType::MarkNotificationRead:     return "MarkNotificationRead";
    case RequestType::MarkAllNotificationsRead: return "MarkAllNotificationsRead";

    // ادمین - کاربران
    case RequestType::GetAllMembers:            return "GetAllMembers";
    case RequestType::GetAllUsers:              return "GetAllUsers";
    case RequestType::GetAllPublishers:         return "GetAllPublishers";
    case RequestType::SearchUsers:              return "SearchUsers";
    case RequestType::GetBlockedUsers:          return "GetBlockedUsers";
    case RequestType::BlockUser:                return "BlockUser";
    case RequestType::UnblockUser:              return "UnblockUser";
    case RequestType::DeleteUser:               return "DeleteUser";

    // ادمین - کتاب‌ها
    case RequestType::GetAllBooksAdmin:         return "GetAllBooksAdmin";
    case RequestType::DeleteBookAdmin:          return "DeleteBookAdmin";
    case RequestType::EditBookAdmin:            return "EditBookAdmin";
    case RequestType::DeleteReviewAdmin:        return "DeleteReviewAdmin";
    // ناشر - آمار
    case RequestType::GetPublisherStats:        return "GetPublisherStats";
    case RequestType::GetTopSellingBooks:       return "GetTopSellingBooks";
    case RequestType::GetLeastSellingBooks:     return "GetLeastSellingBooks";

    // صفحه اصلی
    case RequestType::GetRecommendedBooks:      return "GetRecommendedBooks";
    case RequestType::GetNewestBooks:           return "GetNewestBooks";
    case RequestType::GetMostRatedBooks:        return "GetMostRatedBooks";
    case RequestType::GetFreeBooks:             return "GetFreeBooks";
    
    case RequestType::GetBookCover:             return "GetBookCover";
    case RequestType::GetBookFile:              return "GetBookFile";

    default:                                    return "Unknown";
    }
}

// ─────────────────────────────────────────
// QString → RequestType
// برای تبدیل رشته دریافتی از JSON به enum
// ─────────────────────────────────────────
inline RequestType stringToRequestType(const QString &str)
{
    // احراز هویت
    if (str == "Login")                     return RequestType::Login;
    if (str == "Register")                  return RequestType::Register;
    if (str == "RegisterPublisher")         return RequestType::RegisterPublisher;
    if (str == "ResetPassword")             return RequestType::ResetPassword;
    if (str == "Logout")                    return RequestType::Logout;

    // کتاب‌ها
    if (str == "GetAllBooks")               return RequestType::GetAllBooks;
    if (str == "GetBooksByGenre")           return RequestType::GetBooksByGenre;
    if (str == "GetBooksByPublisher")       return RequestType::GetBooksByPublisher;
    if (str == "SearchBooks")               return RequestType::SearchBooks;
    if (str == "GetBookDetails")            return RequestType::GetBookDetails;
    if (str == "AddBook")                   return RequestType::AddBook;
    if (str == "EditBook")                  return RequestType::EditBook;
    if (str == "InActiveBook")              return RequestType::InActiveBook;
    if (str == "RestoreBook")               return RequestType::RestoreBook;
    if (str == "ApplyDiscount")             return RequestType::ApplyDiscount;

    // نظرات
    if (str == "AddReview")                 return RequestType::AddReview;
    if (str == "EditReview")                return RequestType::EditReview;
    if (str == "DeleteReview")              return RequestType::DeleteReview;
    if (str == "GetReviews")                return RequestType::GetReviews;

    // سبد خرید
    if (str == "AddToCart")                 return RequestType::AddToCart;
    if (str == "RemoveFromCart")            return RequestType::RemoveFromCart;
    if (str == "GetCart")                   return RequestType::GetCart;
    if (str == "Checkout")                  return RequestType::Checkout;

    // کتابخانه شخصی
    if (str == "GetPurchasedBooks")         return RequestType::GetPurchasedBooks;
    if (str == "UpdateLastReadPage")        return RequestType::UpdateLastReadPage;
    if (str == "GetLastReadPage")           return RequestType::GetLastReadPage;

    // ذخیره‌شده‌ها
    if (str == "SaveBook")                  return RequestType::SaveBook;
    if (str == "RemoveSavedBook")           return RequestType::RemoveSavedBook;
    if (str == "GetSavedBooks")             return RequestType::GetSavedBooks;

    // قفسه‌ها
    if (str == "GetShelves")                return RequestType::GetShelves;
    if (str == "AddShelf")                  return RequestType::AddShelf;
    if (str == "RemoveShelf")               return RequestType::RemoveShelf;
    if (str == "RenameShelf")               return RequestType::RenameShelf;
    if (str == "AddBookToShelf")            return RequestType::AddBookToShelf;
    if (str == "RemoveBookFromShelf")       return RequestType::RemoveBookFromShelf;
    if (str == "MoveBookBetweenShelves")    return RequestType::MoveBookBetweenShelves;

    // پروفایل
    if (str == "GetProfile")                return RequestType::GetProfile;
    if (str == "ChangeUsername")             return RequestType::ChangeUsername;
    if (str == "ChangePassword")            return RequestType::ChangePassword;
    if (str == "UpdateFavoriteGenres")      return RequestType::UpdateFavoriteGenres;

    // اعلان‌ها
    if (str == "GetNotifications")          return RequestType::GetNotifications;
    if (str == "MarkNotificationRead")      return RequestType::MarkNotificationRead;
    if (str == "MarkAllNotificationsRead")  return RequestType::MarkAllNotificationsRead;

    // ادمین - کاربران
    if (str == "GetAllMembers")             return RequestType::GetAllMembers;
    if (str == "GetAllUsers")               return RequestType::GetAllUsers;
    if (str == "GetAllPublishers")          return RequestType::GetAllPublishers;
    if (str == "SearchUsers")               return RequestType::SearchUsers;
    if (str == "GetBlockedUsers")           return RequestType::GetBlockedUsers;
    if (str == "BlockUser")                 return RequestType::BlockUser;
    if (str == "UnblockUser")               return RequestType::UnblockUser;
    if (str == "DeleteUser")                return RequestType::DeleteUser;

    // ادمین - کتاب‌ها
    if (str == "GetAllBooksAdmin")          return RequestType::GetAllBooksAdmin;
    if (str == "DeleteBookAdmin")           return RequestType::DeleteBookAdmin;
    if (str == "EditBookAdmin")             return RequestType::EditBookAdmin;
    if (str == "DeleteReviewAdmin")         return RequestType::DeleteReviewAdmin;
    
    // ناشر - آمار
    if (str == "GetPublisherStats")         return RequestType::GetPublisherStats;
    if (str == "GetTopSellingBooks")        return RequestType::GetTopSellingBooks;
    if (str == "GetLeastSellingBooks")      return RequestType::GetLeastSellingBooks;

    // صفحه اصلی
    if (str == "GetRecommendedBooks")       return RequestType::GetRecommendedBooks;
    if (str == "GetNewestBooks")            return RequestType::GetNewestBooks;
    if (str == "GetMostRatedBooks")         return RequestType::GetMostRatedBooks;
    if (str == "GetFreeBooks")              return RequestType::GetFreeBooks;

    if (str == "GetBookCover")              return RequestType::GetBookCover;
    if (str == "GetBookFile")               return RequestType::GetBookFile;

    return RequestType::Login; // مقدار پیش‌فرض
}

// ─────────────────────────────────────────
// ResponseStatus → QString
// ─────────────────────────────────────────
inline QString responseStatusToString(ResponseStatus status)
{
    switch (status) {
    case ResponseStatus::Success:        return "Success";
    case ResponseStatus::Failure:        return "Failure";
    case ResponseStatus::Unauthorized:   return "Unauthorized";
    case ResponseStatus::NotFound:       return "NotFound";
    case ResponseStatus::AlreadyExists:  return "AlreadyExists";
    case ResponseStatus::InvalidData:    return "InvalidData";
    default:                             return "Failure";
    }
}

// ─────────────────────────────────────────
// QString → ResponseStatus
// ─────────────────────────────────────────
inline ResponseStatus stringToResponseStatus(const QString &str)
{
    if (str == "Success")        return ResponseStatus::Success;
    if (str == "Unauthorized")   return ResponseStatus::Unauthorized;
    if (str == "NotFound")       return ResponseStatus::NotFound;
    if (str == "AlreadyExists")  return ResponseStatus::AlreadyExists;
    if (str == "InvalidData")    return ResponseStatus::InvalidData;
    return ResponseStatus::Failure;
}

// ─────────────────────────────────────────
// فرمت کلی پیام‌ها بین کلاینت و سرور:
//
// درخواست کلاینت → سرور:
// {
//   "type": "Login",       ← نوع درخواست
//   "data": {              ← داده‌های مورد نیاز
//     "username": "ali",
//     "password": "..."
//   }
// }
//
// پاسخ موفق سرور → کلاینت:
// {
//   "status": "Success",   ← وضعیت
//   "data": { ... }        ← داده‌های برگشتی
// }
//
// پاسخ خطا سرور → کلاینت:
// {
//   "status": "Failure",   ← وضعیت
//   "message": "..."       ← پیام خطا
// }
// ─────────────────────────────────────────



inline QString genreToString(genre g)
{
    switch (g) {
    case genre::Fiction:          return "Fiction";
    case genre::Non_fiction:      return "Non_fiction";
    case genre::Academic:         return "Academic";
    case genre::Drama:            return "Drama";
    case genre::Mystery:          return "Mystery";
    case genre::Crime:            return "Crime";
    case genre::Romance:          return "Romance";
    case genre::History:          return "History";
    case genre::Science_fiction:  return "Science_fiction";
    case genre::Scary:            return "Scary";
    default:                      return "Fiction";
    }
}

inline genre stringToGenre(const QString &str)
{
    if (str == "Fiction")         return genre::Fiction;
    if (str == "Non_fiction")     return genre::Non_fiction;
    if (str == "Academic")        return genre::Academic;
    if (str == "Drama")           return genre::Drama;
    if (str == "Mystery")         return genre::Mystery;
    if (str == "Crime")           return genre::Crime;
    if (str == "Romance")         return genre::Romance;
    if (str == "History")         return genre::History;
    if (str == "Science_fiction") return genre::Science_fiction;
    if (str == "Scary")           return genre::Scary;
    return genre::Fiction;
}