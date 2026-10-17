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
    int account_count = 0;
    int choice;

    // --- Load the vault from disk -------------------------------

    ifstream fp("output/vault.txt");

    if (fp.is_open())
    {
        int loaded_count;

        if (fp >> loaded_count)
        {
            // Validate the count before accessing the array.
            if (loaded_count >= 0 && loaded_count <= 1024)
            {
                account_count = 0;

                for (int i = 0; i < loaded_count; ++i)
                {
                    if (fp >> vault[i].app >> vault[i].user >> vault[i].pass)
                    {
                        ++account_count;
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

            if (account_count == 0)
            {
                cout << "No accounts stored yet!\n";
            }
            else
            {
                // Display the list of saved accounts.
                for (int i = 0; i < account_count; ++i)
                {
                    cout << i + 1 << ". " << vault[i].app << '\n';
                }

                cout << "\n-> Please select which account to view: ";

                if (cin >> choice)
                {
                    int index = choice - 1;

                    // Validate the selected account.
                    if (index >= 0 && index < account_count)
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

            if (account_count >= 1024)
            {
                cout << "Vault is full!\n";
                continue;
            }

            // Collect account details.
            cout << "\n-> Please choose the app's name: ";
            cin >> vault[account_count].app;

            cout << "-> Please choose the username or email: ";
            cin >> vault[account_count].user;

            cout << "-> Please choose a password: ";
            cin >> vault[account_count].pass;

            // Confirm the new account.
            cout << "\n-> App name: " << vault[account_count].app << '\n';

            cout << "-> Username: " << vault[account_count].user << '\n';

            cout << "-> Password: [hidden]\n";

            // Account is now part of the vault.
            ++account_count;

            // --- Save the entire vault to disk -------------------

            ofstream save_fp("output/vault.txt");

            if (save_fp.is_open())
            {
                // First line stores the account count.
                save_fp << account_count << '\n';

                // Write each account on its own line.
                for (int i = 0; i < account_count; ++i)
                {
                    save_fp << vault[i].app << ' ' << vault[i].user << ' ' << vault[i].pass << '\n';
                }

                save_fp.close();

                if (save_fp)
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
