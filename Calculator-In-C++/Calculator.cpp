#include <iostream>  // The standard I/O library
#include <limits>    // For clearing cursed input
#include <stdexcept> // The error exception library
#include <string>    // For handling the Exit command

using namespace std; // Using the namespace directly to avoid the std::cout mess

double add(double a, double b)
{
    return a + b;
}

double sub(double a, double b)
{
    return a - b;
}

double mul(double a, double b)
{
    return a * b;
}

double div(double a, double b)
{
    if (b == 0) // Checking if the denominator is 0 cuz if you did something called "Pay attention in school" you know
                // it ain't possible
    {
        throw runtime_error(
            "Division by Zero. Gng first you using a calculator then dividing by zero; Go study SONion 😭😭");
    }

    return a / b;
}

int main()
{
    // Initial Vars
    bool running = true;
    string command;
    char op;
    double firstNumber;
    double secondNumber;

    cout << "\nWelcome to the calculator." << endl;

    while (running)
    {
        cout << "\nType Enter to calculate or Exit to leave: ";
        cin >> command;

        if (command == "Exit" || command == "exit")
        {
            running = false;
            cout << "Leaving the calculator. Lock in next time, SONion 😭" << endl;
            break;
        }
        else if (command != "Enter" && command != "enter")
        {
            cout << "SONion 😭 Invalid command. Type Enter or Exit." << endl;
            continue;
        }

        // Getting the first number
        cout << "Enter first number: ";

        if (!(cin >> firstNumber))
        {
            if (cin.eof())
                break;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cerr << "SONion 😭 That's not a valid number." << endl;
            continue;
        }

        // Getting the second number
        cout << "\nEnter second number: ";

        if (!(cin >> secondNumber))
        {
            if (cin.eof())
                break;

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cerr << "SONion 😭 That's not a valid number." << endl;
            continue;
        }

        // Getting the operator
        cout << "\nWhat you wanna do [+, -, *, /]: ";
        cin >> op;

        // Actually doing the math
        try
        {
            switch (op)
            {
            case '+':
                cout << "Result: " << add(firstNumber, secondNumber) << endl;
                break;

            case '-':
                cout << "Result: " << sub(firstNumber, secondNumber) << endl;
                break;

            case '*':
                cout << "Result: " << mul(firstNumber, secondNumber) << endl;
                break;

            case '/':
                cout << "Result: " << div(firstNumber, secondNumber) << endl;
                break;

            default:
                cerr << "SONion 😭 Invalid operator. "
                     << "Learn your symbols." << endl;
                break;
            }
        }
        catch (const runtime_error &e)
        {
            // Catching the division-by-zero violation
            cerr << "ERROR: " << e.what() << endl;
        }
    }

    return 0;
}
