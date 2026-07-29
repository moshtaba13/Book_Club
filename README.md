# BookClub

BookClub is a client–server e-book platform built in C++/Qt6, where users can
browse, buy, and read books, publishers can publish and manage their catalog,
and an admin oversees the whole system. It was built as an Advanced
Programming course project, applying OOP, Qt's Signal/Slot mechanism,
multithreaded socket programming, and SQLite persistence.

## Features

**All users**
- Register / log in / log out, with password recovery via a security question
- Encrypted-at-rest personal data, one-way hashed passwords

**Regular users**
- Pick 1–3 favorite genres on first login (editable anytime from Profile)
- Personalized recommendations based on favorite genres
- Browse by genre, newest, best-selling, top-rated, and free books
- Search books by title, author, or publisher username
- Book detail page with reviews and 1–5 star ratings
- Shopping cart with live discount calculation and checkout
- Personal library: purchased books, saved-for-later list, and custom shelves
- Built-in PDF reader with zoom, page navigation, and resume-from-last-page
- Real-time notifications (new releases in favorite genres, discounts on saved books, etc.)

**Publishers**
- Add / edit / deactivate / reactivate books (cover image + PDF upload)
- Apply discounts on their own books
- Sales dashboard: revenue, top/bottom sellers, average ratings

**Admin**
- View and search all users and publishers
- Block / unblock / delete accounts
- View, edit, and delete any book; moderate reviews
- Live server dashboard: online users, connection status, request log, server health

## Architecture

The project follows a Client–Server architecture over raw TCP sockets:

```
Client  <---- JSON over TCP socket ---->  Server
(Qt Widgets UI)                            (Qt, no UI event loop blocking)
```

- **Client** — Qt Widgets desktop app. All business rules are enforced
  server-side; the client only renders state and sends requests.
- **Server** — handles many clients concurrently (one thread per connection),
  validates every request, talks to a SQLite database, and pushes real-time
  notifications back to connected clients without polling. It has its own
  Qt Widgets dashboard instead of running headless, showing live stats on
  connected users, requests, and server health.
- **Common** — shared code (`Book`, `Member`/`User`/`Publisher`/`Admin`,
  `Cart`, `Review`, `Shelf`, `Notification`, and the `protocol.h` request/
  response contract) linked into both Client and Server so the wire format
  can't drift between the two.

### Project layout

```
Book_Club/
├── Client/
│   ├── general/      # main window, login/register flow
│   ├── user/         # home, search, cart, library, reader, profile, notifications
│   ├── publisher/    # publisher dashboard, add/edit book
│   ├── admin/        # admin login + panel
│   ├── network/      # NetworkClient, model (de)serialization, cover loader
│   └── resources/    # images, resources.qrc
├── Server/
│   ├── network/      # Server (QTcpServer), ClientHandler, per-domain request handlers
│   ├── managers/     # business logic (Auth, User, Book, Purchase, Notification, File)
│   ├── repositories/ # SQL access layer, one per table/domain
│   ├── ui/            # ServerWindow — live monitoring dashboard
│   └── Database.*    # SQLite schema + connection
└── Common/
    ├── models/        # Book, Cart, Review, Shelf, Notification, Purchase
    ├── users/         # Member, User, Publisher, Admin
    └── protocol.h     # RequestType enum + JSON contract shared by Client/Server
```

## Tech stack

- **C++17**
- **Qt 6.5+** — Widgets, Network, Sql, Pdf, PdfWidgets
- **SQLite** (via `QSqlDatabase`) for persistence
- **CMake** build system, with CPack configured to produce a Windows
  installer bundling both the Client and Server executables

## Building

Requires Qt 6.5+ (with the Pdf/PdfWidgets modules) and CMake 3.19+.

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64   # adjust to your Qt install
cmake --build .
```

This builds two executables: `Server/BookClubServer` and `Client/Client`.

To produce a standalone installer (no Qt install required on the target
machine):

```bash
cmake --build . --target package
```

## Running

1. **Start the server first.** Run `BookClubServer` from within its own
   folder — it stores its SQLite database and uploaded book/cover files
   under a `data/` directory relative to the working directory it's
   launched from. By default it listens on port `1234` (override with the
   `BOOKCLUB_SERVER_PORT` environment variable).
2. **Start one or more `Client` instances**, which connect to
   `127.0.0.1:1234` by default.
3. A default admin account is created automatically on first run:
   `admin` / `admin123`.

## Design notes

- Every request from the client is validated and authorized on the server;
  the client never has direct database or file-system access.
- Passwords are stored one-way hashed (SHA-256 + per-user salt); other
  personal fields are stored two-way encrypted, per the project spec.
- The server pushes notifications and catalog changes to connected clients
  in real time rather than requiring a refresh or re-login.
- Domain logic is split into `managers/` (business rules) and
  `repositories/` (SQL access), with `network/handlers/` translating
  incoming requests into calls against them — keeping networking, storage,
  and logic decoupled.
