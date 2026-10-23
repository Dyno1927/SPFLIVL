// Manager.cpp — a CLI password manager, my C++ project.
// Stores app logins (app name, username, password) in a plain-text vault
// file (Output/Vault.txt). Reads it on start, rewrites it on every create.
// No encryption here — this is a learning project, don't put real
// passwords in it.
//
// I wrote this one to feel the difference from C: std::string instead of
// fixed char buffers, and streams (ifstream/ofstream) instead of fopen.
// The logic is the same as Manager.c on purpose, so the two can be read
// side by side.

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// One saved login: app name, username, password.
// Unlike C, I use std::string here, so the text grows as needed and I
// never worry about a 32-char cap or a missing '\0'.
typedef struct
{
    string app;
    string user;
    string pass;
} Account;

int main()
{
    int running = 1;
    string cmd;

    Account vault[1024];
    int accountCount = 0;
    int choice;

    //  Load the vault from disk
    // ifstream's constructor opens the file; if it doesn't exist yet (first
    // run) is_open() is false and I just skip loading. Like C's fopen
    // returning NULL, but RAII closes it for me when fp goes out of scope.

    ifstream fp("Output/Vault.txt");

    if (fp.is_open())
    {
        int loadedCount;

        if (fp >> loadedCount)
        {
            // I validate the count before touching the array — a negative or
            // oversized count in a tampered file must not drive me past the
            // 1024 slots.
            if (loadedCount >= 0 && loadedCount <= 1024)
            {
                accountCount = 0;

                for (int i = 0; i < loadedCount; ++i)
                {
                    if (fp >> vault[i].app >> vault[i].user >> vault[i].pass)
                    {
                        ++accountCount;
                    }
                    else
                    {
                        cout << "Warning: Vault file is incomplete.\n";
                        break;
                    }
                }
            }
            else
            {
                cout << "Warning: Invalid account count in vault file.\n";
            }
        }
        else
        {
            cout << "Warning: Could not read vault file.\n";
        }

        fp.close();
    }

    cout << "Hello, this is a Password Manager CLI "
         << "(but in C++ rather than C).\n";

    //  Main menu loop

    while (running)
    {
        cout << "\nPlease choose an option [Manage | Create | Exit]: ";

        if (!(cin >> cmd))
        {
            cout << "\nInput ended. Exiting...\n";
            break;
        }

        // EXIT OPTION
        if (cmd == "exit" || cmd == "Exit" || cmd == "e" || cmd == "E")
        {
            cout << "Alright, see you again!\n";
            running = 0;
        }

        // MANAGE OPTION
        else if (cmd == "M" || cmd == "m" || cmd == "Manage" || cmd == "manage")
        {
            cout << "\n-> Selected: Manage\n\n";

            if (accountCount == 0)
            {
                cout << "No accounts stored yet!\n";
            }
            else
            {
                // I display the list of saved accounts, numbered 1..n for humans.
                for (int i = 0; i < accountCount; ++i)
                {
                    cout << i + 1 << ". " << vault[i].app << '\n';
                }

                cout << "\n-> Please select which account to view: ";

                if (cin >> choice)
                {
                    int index = choice - 1;

                    // I validate the selected account before indexing — reading
                    // past the end is undefined behavior, same as in C.
                    if (index >= 0 && index < accountCount)
                    {
                        cout << "\n-> App name: " << vault[index].app << '\n';

                        cout << "-> Username: " << vault[index].user << '\n';

                        cout << "-> Password: " << vault[index].pass << '\n';
                    }
                    else
                    {
                        cout << "Please select a valid option!\n";
                    }
                }
                else
                {
                    // I recover if the user types text instead of a number,
                    // so one bad input doesn't kill the whole session.
                    cin.clear();
                    cin.ignore(10000, '\n');

                    cout << "Invalid input! Please enter a number.\n";
                }
            }
        }

        // CREATE OPTION
        else if (cmd == "C" || cmd == "c" || cmd == "Create" || cmd == "create")
        {
            cout << "\n-> Selected: Create\n";

            if (accountCount >= 1024)
            {
                cout << "Vault is full!\n";
                continue;
            }

            // I collect the account details.
            cout << "\n-> Please choose the app's name: ";
            cin >> vault[accountCount].app;

            cout << "-> Please choose the username or email: ";
            cin >> vault[accountCount].user;

            cout << "-> Please choose a password: ";
            cin >> vault[accountCount].pass;

            // I confirm the new account.
            cout << "\n-> App name: " << vault[accountCount].app << '\n';

            cout << "-> Username: " << vault[accountCount].user << '\n';

            // I echo the entry back so I can confirm it's what I entered.
            cout << "-> Password: " << vault[accountCount].pass << '\n';

            // The account is now part of the vault.
            ++accountCount;

            //  Save the entire vault to disk

            ofstream saveFp("Output/Vault.txt");

            if (saveFp.is_open())
            {
                // First line stores the account count.
                saveFp << accountCount << '\n';

                // I write each account on its own line.
                for (int i = 0; i < accountCount; ++i)
                {
                    saveFp << vault[i].app << ' ' << vault[i].user << ' ' << vault[i].pass << '\n';
                }

                saveFp.close();

                if (saveFp)
                {
                    cout << "Account saved successfully!\n";
                }
                else
                {
                    cout << "Error: Failed while writing the vault.\n";
                }
            }
            else
            {
                cout << "Error: Could not save to disk.\n";
                cout << "Make sure the output directory exists.\n";
            }
        }

        // INVALID OPTION
        else
        {
            cout << "Invalid option! Please try again.\n";
        }
    }

    return 0;
}
