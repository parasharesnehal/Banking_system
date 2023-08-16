#include <iostream>
#include <fstream>
#include <map> // To store account information

using namespace std;
#define MINIMUM_BALANCE 500

class InsufficientFunds {}; // Exception 

class Account {
private:
    long accNumber;
    string fullName;
    float balance;
    static long nextAccNumber; // Automatically generates next account number

public:
    // Constructors
    Account() {} // Default constructor
    Account(string name, float bal); // Parameterized constructor

    // Accessor methods
    long getAccNumber() const { return accNumber; }
    string getFullName() const { return fullName; }
    float getBalance() const { return balance; }

    // Account operations
    void deposit(float amount);
    void withdraw(float amount);

    // Static methods for managing account numbers
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    // Operator overloads
    friend ofstream & operator<<(ofstream &ofs, Account &acc);
    friend ifstream & operator>>(ifstream &ifs, Account &acc);
    friend ostream & operator<<(ostream &os, Account &acc);
};

long Account::nextAccNumber = 0;

class Bank {
private:
    map<long, Account> accounts; 

public:
    // Constructor to load existing accounts from file
    Bank();

    // Account operations
    Account openAccount(string name, float balance);
    Account balanceEnquiry(long accNumber);
    Account deposit(long accNumber, float amount);
    Account withdraw(long accNumber, float amount);
    void closeAccount(long accNumber);
    void showAccounts();

    // Destructor saves accounts to file
    ~Bank();
};

int main() {
    Bank bank;
    int choice;
    Account acc;
    string name;
    long accNumber;
    float balance;
    float amount;

    cout << "*** Banking System ***" << endl;
    //handles every option

    do {
        cout << "\nSelect an option:";
        cout << "\n1. Open an Account";
        cout << "\n2. Balance Enquiry";
        cout << "\n3. Deposit";
        cout << "\n4. Withdrawal";
        cout << "\n5. Close an Account";
        cout << "\n6. Show All Accounts";
        cout << "\n7. Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1://creation of account
                cout << "Enter Full Name (use underscores): ";
                cin >> name;
                cout << "Enter Initial Balance: ";
                cin >> balance;
                acc = bank.openAccount(name, balance);
                cout << "\nAccount Created Successfully" << endl;
                cout << acc; // Display account details
                break;
            case 2:// balance display
                cout << "Enter Account Number: ";
                cin >> accNumber;
                acc = bank.balanceEnquiry(accNumber);
                cout << "\nAccount Details" << endl;
                cout << acc;
                break;
            case 3:// money in acc
                cout << "Enter Account Number: ";
                cin >> accNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = bank.deposit(accNumber, amount);
                cout << "\nAmount Deposited Successfully" << endl;
                cout << acc;
                break;
            case 4:// remove money
                cout << "Enter Account Number: ";
                cin >> accNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = bank.withdraw(accNumber, amount);
                cout << "\nAmount Withdrawn Successfully" << endl;
                cout << acc;
                break;
            case 5://close acc
                cout << "Enter Account Number: ";
                cin >> accNumber;
                bank.closeAccount(accNumber);
                cout << "\nAccount Closed Successfully" << endl;
                break;
            case 6://display acc
                bank.showAccounts();
                break;
            case 7://exit 
                cout << "Quitting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 7);

    return 0;
}

Account::Account(string name, float bal) {
    nextAccNumber++;
    accNumber = nextAccNumber;
    fullName = name;
    balance = bal;
}

void Account::deposit(float amount) {
    balance += amount;
}

void Account::withdraw(float amount) {
    if (balance - amount < MINIMUM_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber) {
    nextAccNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return nextAccNumber;
}

ofstream & operator<<(ofstream &ofs, Account &acc) {
    ofs << acc.accNumber << endl;
    ofs << acc.fullName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs, Account &acc) {
    ifs >> acc.accNumber;
    ifs >> acc.fullName;
    ifs >> acc.balance;
    return ifs;
}

ostream & operator<<(ostream &os, Account &acc) {
    os << "Full Name: " << acc.getFullName() << endl;
    os << "Account Number: " << acc.getAccNumber() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

Bank::Bank() {
    Account account;
    ifstream infile;
    infile.open("BankData.data");
    if (!infile) {
        return;
    }
    while (!infile.eof()) {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNumber(), account));
    }
    Account::setLastAccountNumber(account.getAccNumber());
    infile.close();
}

Account Bank::openAccount(string name, float balance) {
    ofstream outfile;
    Account account(name, balance);
    accounts.insert(pair<long, Account>(account.getAccNumber(), account));

    outfile.open("BankData.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceEnquiry(long accNumber) {
    map<long, Account>::iterator itr = accounts.find(accNumber);
    return itr->second;
}

Account Bank::deposit(long accNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accNumber);
    itr->second.deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accNumber);
    itr->second.withdraw(amount);
    return itr->second;
}

void Bank::closeAccount(long accNumber) {
    map<long, Account>::iterator itr = accounts.find(accNumber);
    cout << "Account Deleted" << itr->second;
    accounts.erase(accNumber);
}

void Bank::showAccounts() {
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        cout << "Account " << itr->first << endl << itr->second << endl;
    }
}

Bank::~Bank() {
    ofstream outfile;
    outfile.open("BankData.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
}