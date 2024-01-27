#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

class Book {
public:
    string title;
    string author;
    string ISBN;
    bool isAvailable;

    Book(const string& title, const string& author, const string& ISBN)
        : title(title), author(author), ISBN(ISBN), isAvailable(true) {}
};

class Borrower {
public:
    string name;
    string ID;

    Borrower(const string& name, const string& ID)
        : name(name), ID(ID) {}
};

class Transaction {
public:
    Book* book;
    Borrower* borrower;
    time_t checkoutDate;

    Transaction(Book* book, Borrower* borrower)
        : book(book), borrower(borrower) {
        checkoutDate = time(nullptr);
    }
};

class Library {
private:
    vector<Book> books;
    vector<Borrower> borrowers;
    vector<Transaction> transactions;

public:
    void addBook(const string& title, const string& author, const string& ISBN) {
        books.push_back(Book(title, author, ISBN));
        cout << "Book added successfully.\n";
    }

    void addBorrower(const string& name, const string& ID) {
        borrowers.push_back(Borrower(name, ID));
        cout << "Borrower added successfully.\n";
    }

    void searchBooks(const std::string& searchTerm) {
        cout << "Search Results:\n";
        for (const auto& book : books) {
            if (book.title.find(searchTerm) != string::npos ||
                book.author.find(searchTerm) != string::npos ||
                book.ISBN.find(searchTerm) != string::npos) {
                cout << "Title: " << book.title << ", Author: " << book.author
                    << ", ISBN: " << book.ISBN << ", Availability: " << (book.isAvailable ? "Available" : "Not Available") << "\n";
            }
        }
    }

    void checkoutBook(const std::string& ISBN, const std::string& borrowerID) {
        auto bookIt = find_if(books.begin(), books.end(), [&](const Book& b) {
            return b.ISBN == ISBN && b.isAvailable;
        });

        auto borrowerIt = find_if(borrowers.begin(), borrowers.end(), [&](const Borrower& borrower) {
            return borrower.ID == borrowerID;
        });

        if (bookIt != books.end() && borrowerIt != borrowers.end()) {
            bookIt->isAvailable = false;
            transactions.push_back(Transaction(&(*bookIt), &(*borrowerIt)));
            cout << "Book checked out successfully.\n";
        } else {
            cout << "Book or borrower not found or book is not available.\n";
        }
    }

    void returnBook(const string& ISBN) {
        auto transactionIt = find_if(transactions.begin(), transactions.end(), [&](const Transaction& transaction) {
            return transaction.book->ISBN == ISBN;
        });

        if (transactionIt != transactions.end()) {
            transactionIt->book->isAvailable = true;
            cout << "Book returned successfully.\n";
        } else {
            cout << "Book transaction not found.\n";
        }
    }

    void calculateFine(const std::string& ISBN) {
        auto transactionIt = find_if(transactions.begin(), transactions.end(), [&](const Transaction& transaction) {
            return transaction.book->ISBN == ISBN;
        });

        if (transactionIt != transactions.end()) {
            time_t currentDate = time(nullptr);
            double overdueDays = difftime(currentDate, transactionIt->checkoutDate) / (24 * 3600);

            // Assuming a flat fine rate per overdue day
            const double fineRatePerDay = 0.50;
            double fineAmount = fineRatePerDay * overdueDays;

            cout << "Fine for overdue book: " << fineAmount << " USD\n";
        } else {
            cout << "Book transaction not found.\n";
        }
    }
};

void displayMenu() {
    cout << "\n===== Library Management System =====\n";
    cout << "1. Add Book\n";
    cout << "2. Add Borrower\n";
    cout << "3. Search Books\n";
    cout << "4. Checkout Book\n";
    cout << "5. Return Book\n";
    cout << "6. Calculate Fine\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Library library;

    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                string title, author, ISBN;
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                library.addBook(title, author, ISBN);
                break;
            }
            case 2: {
                string name, ID;
                cout << "Enter borrower name: ";
                getline(cin, name);
                cout << "Enter borrower ID: ";
                getline(cin, ID);
                library.addBorrower(name, ID);
                break;
            }
            case 3: {
                string searchTerm;
                cout << "Enter search term: ";
                getline(cin, searchTerm);
                library.searchBooks(searchTerm);
                break;
            }
            case 4: {
                string ISBN, borrowerID;
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                cout << "Enter borrower ID: ";
                getline(cin, borrowerID);
                library.checkoutBook(ISBN, borrowerID);
                break;
            }
            case 5: {
                string ISBN;
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                library.returnBook(ISBN);
                break;
            }
            case 6: {
                string ISBN;
                cout << "Enter ISBN: ";
                getline(cin, ISBN);
                library.calculateFine(ISBN);
                break;
            }
            case 0:
                cout << "Exiting the Library Management System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
