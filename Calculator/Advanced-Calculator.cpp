#include <iostream> // The standard I/O library
#include <limits>   // For dealing with cursed input
#include <vector>   // For storing multiple numbers

using namespace std; // Using the namespace directly to avoid the std::cout mess

double add(const vector<double> &args)
{
    // Adding all the numbers together
    double answer{};

    for (double num : args)
    {
        answer += num;
    }

    return answer;
}

double sub(const vector<double> &args)
{
    // Subtracting every number from the first one
    double answer{args[0]};

    for (size_t i = 1; i < args.size(); i++)
    {
        answer -= args[i];
    }

    return answer;
}

double mul(const vector<double> &args)
{
    // Multiplying all the numbers together
    double answer{1};

    for (double num : args)
    {
        answer *= num;
    }

    return answer;
}

double div(const vector<double> &args)
{
    // Dividing the first number by the rest in order
    double answer{args[0]};

    for (size_t i = 1; i < args.size(); i++)
    {
        answer /= args[i];
    }

    return answer;
}

int main()
{
    // Initial Vars
    vector<double> nums{};
    int terms{};
    char op{};

    cout << "\nWelcome to Advanced Calculator!" << endl;

    cout << "\nEnter the number of terms: ";

    if (!(cin >> terms) || terms <= 0)
    {
        // Checking if the number of terms makes sense
        cout << "Invalid number of terms!" << endl;
        return 1;
    }

    cout << "Enter " << terms << " numbers separated by spaces: ";

    for (int i = 0; i < terms; i++)
    {
        double input{};

        if (!(cin >> input))
        {
            // Making sure the user actually enters a number
            cout << "Invalid number entered!" << endl;
            return 1;
        }

        nums.push_back(input);
    }

    cout << "Choose the operation (+, -, *, /): ";
    cin >> op;

    // Actually doing the math
    switch (op)
    {
    case '+':
        // Adding everything up
        cout << "Sum: " << add(nums) << endl;
        break;

    case '-':
        // Taking numbers away from the first one
        cout << "Difference: " << sub(nums) << endl;
        break;

    case '*':
        // Multiplying everything like a math machine
        cout << "Product: " << mul(nums) << endl;
        break;

    case '/':
        // Checking for division by zero because apparently math has limits 😭
        for (size_t i = 1; i < nums.size(); i++)
        {
            if (nums[i] == 0)
            {
                cout << "Error: Division by zero!" << endl;
                return 1;
            }
        }

        // All clear, calculating the quotient
        cout << "Quotient: " << div(nums) << endl;
        break;

    default:
        // Invalid operator. Bro forgot the basic math symbols 😭
        cout << "Invalid operator!" << endl;
        return 1;
    }

    return 0;
}
