add_executable(BookClubServer
    # بقیه فایل‌ها رو خودت بعداً اینجا اضافه کن
)

target_link_libraries(BookClubServer PRIVATE
    common
    Qt6::Core
    Qt6::Network
    Qt6::Sql
)