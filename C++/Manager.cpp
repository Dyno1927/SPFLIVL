#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// One saved login: app name, username, password.
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

    // --- Load the vault from disk -------------------------------

    ifstream fp("Output/Vault.txt");

    if (fp.is_open())
    {
        int loadedCount;

        if (fp >> loadedCount)
        {
            // Validate the count before accessing the array.
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

    // --- Main menu loop -----------------------------------------

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
                // Display the list of saved accounts.
                for (int i = 0; i < accountCount; ++i)
                {
                    cout << i + 1 << ". " << vault[i].app << '\n';
                }

                cout << "\n-> Please select which account to view: ";

                if (cin >> choice)
                {
                    int index = choice - 1;

                    // Validate the selected account.
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
                    // Recover if the user enters text instead of a number.
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

            // Collect account details.
            cout << "\n-> Please choose the app's name: ";
            cin >> vault[accountCount].app;

            cout << "-> Please choose the username or email: ";
            cin >> vault[accountCount].user;

            cout << "-> Please choose a password: ";
            cin >> vault[accountCount].pass;

            // Confirm the new account.
            cout << "\n-> App name: " << vault[accountCount].app << '\n';

            cout << "-> Username: " << vault[accountCount].user << '\n';

            cout << "-> Password: [hidden]\n";

            // Account is now part of the vault.
            ++accountCount;

            // --- Save the entire vault to disk -------------------

            ofstream saveFp("Output/Vault.txt");

            if (saveFp.is_open())
            {
                // First line stores the account count.
                saveFp << accountCount << '\n';

                // Write each account on its own line.
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
