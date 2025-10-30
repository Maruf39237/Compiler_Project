#include <bits/stdc++.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <windows.h>
#include <conio.h>
#include <stdio.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

#define COLOR_BLUE       FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define COLOR_GREEN      FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_RED        FOREGROUND_RED | FOREGROUND_INTENSITY
#define COLOR_YELLOW     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_WHITE      FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define COLOR_AQUA       FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

struct BalanceError
{
    string message;
    char expected;
    char found;
    int line;
    int column;
};

struct BracketInfo
{
    char bracket;
    int line;
    int column;
};

typedef struct
{
    stack<BracketInfo> bracket_stack;
    vector<BalanceError> errors;
} BalancedParenthesesChecker_t;

bool is_open_bracket(char c)
{
    return c == '(' || c == '{' || c == '[';
}

bool is_close_bracket(char c)
{
    return c == ')' || c == '}' || c == ']';
}

char get_matching_close(char open_bracket)
{
    switch (open_bracket)
    {
    case '(':
        return ')';
    case '{':
        return '}';
    case '[':
        return ']';
    default:
        return '\0';
    }
}

void balancer_init(BalancedParenthesesChecker_t *checker)
{
    (void)checker;
}

void balancer_free(BalancedParenthesesChecker_t *checker)
{
    (void)checker;
}

void add_balance_error(BalancedParenthesesChecker_t *checker, const string &msg, char expected, char found, int line, int column)
{
    BalanceError new_error;
    new_error.message = msg;
    new_error.expected = expected;
    new_error.found = found;
    new_error.line = line;
    new_error.column = column;
    checker->errors.push_back(new_error);
}

bool check_balance(BalancedParenthesesChecker_t *checker, const string &code)
{
    while (!checker->bracket_stack.empty())
    {
        checker->bracket_stack.pop();
    }
    checker->errors.clear();

    int current_line = 1;
    int current_column = 0;

    for (char c : code)
    {
        current_column++;

        if (c == '\n')
        {
            current_line++;
            current_column = 0;
            continue;
        }

        if (is_open_bracket(c))
        {
            checker->bracket_stack.push({c, current_line, current_column});
        }
        else if (is_close_bracket(c))
        {
            if (checker->bracket_stack.empty())
            {
                add_balance_error(checker, "Unmatched closing bracket", '\0', c, current_line, current_column);
            }
            else
            {
                BracketInfo top_info = checker->bracket_stack.top();
                if (get_matching_close(top_info.bracket) == c)
                {
                    checker->bracket_stack.pop();
                }
                else
                {
                    add_balance_error(checker, "Mismatched closing bracket", get_matching_close(top_info.bracket), c, current_line, current_column);
                    checker->bracket_stack.pop();
                }
            }
        }
    }

    while (!checker->bracket_stack.empty())
    {
        BracketInfo unclosed_info = checker->bracket_stack.top();
        checker->bracket_stack.pop();
        add_balance_error(checker, "Unclosed opening bracket", '\0', unclosed_info.bracket, unclosed_info.line, unclosed_info.column);
    }

    return checker->errors.empty();
}

void print_errors(const BalancedParenthesesChecker_t *checker)
{
    if (checker->errors.empty())
    {
        SetColor(COLOR_GREEN);
        cout << "\t\t\tNo balance errors found." << endl;
        SetColor(COLOR_WHITE);
    }
    else
    {
        SetColor(COLOR_RED);
        cerr << "\t\t\t--- Balance Errors ---" << endl;
        for (const BalanceError &err : checker->errors)
        {
            cerr << "\t\t\tError (Line " << err.line << ", Col " << err.column << "): " << err.message;
            if (err.expected != '\0')
            {
                cerr << " Expected '" << err.expected << "', found '" << err.found << "'";
            }
            else if (err.found != '\0')
            {
                cerr << " Found '" << err.found << "'";
            }
            cerr << endl;
        }
        cerr << "\t\t\t----------------------" << endl;
        SetColor(COLOR_WHITE);
    }
}

int main()
{
    SetColor(COLOR_BLUE);
    cout << "\n\n\n\t\t\t--- Balanced Parentheses Checker (C/C++ Language) ---" << endl;
    cout << "\t\t\tChecks for proper nesting of (), {}, []." << endl;
    cout << "\t\t\tType your code. Type 'EOF' or 'DONE' on a new line to finish input." << endl;
    cout << "\t\t\tType 'exit' to quit the program." << endl << endl;
    SetColor(COLOR_YELLOW);

    string line;
    string input_code;

    BalancedParenthesesChecker_t checker;
    balancer_init(&checker);

    while (true)
    {
        system("cls");
        printf("\e[?25l");

        SetColor(COLOR_BLUE);
        cout << "\n\n\n\t\t\t--- Balanced Parentheses Checker (C/C++ Language) ---" << endl;
        cout << "\t\t\tChecks for proper nesting of (), {}, []." << endl;
        cout << "\t\t\tType your code. Type 'EOF' or 'DONE' on a new line to finish input." << endl;
        cout << "\t\t\tType 'exit' to quit the program." << endl << endl;
        SetColor(COLOR_YELLOW);

        cout << "\t\t\t>>> Enter code (type 'EOF' or 'DONE' on a new line to submit):" << endl;

        input_code.clear();

        bool reading_input = true;
        while (reading_input)
        {
            SetColor(COLOR_WHITE);
            cout << "\t\t\t> ";
            SetColor(COLOR_YELLOW);
            if (!getline(cin, line))
            {
                reading_input = false;
                break;
            }
            SetColor(COLOR_WHITE);

            if (line == "EOF" || line == "DONE" || line == "eof" || line == "done")
            {
                reading_input = false;
            }
            else if (line == "exit" || line == "EXIT")
            {
                balancer_free(&checker);
                system("cls");
                SetColor(COLOR_BLUE);
                cout << "\n\n\n\t\t\t--- Balanced Parentheses Checker (C/C++ Language) ---" << endl;
                cout << "\t\t\tChecks for proper nesting of (), {}, []." << endl;
                cout << "\t\t\tType your code. Type 'EOF' or 'DONE' on a new line to finish input." << endl;
                cout << "\t\t\tType 'exit' to quit the program." << endl << endl;

                cout << "\t\t\t------------------------" << endl;
                cout << "\t\t\tExiting program as requested." << endl;
                cout << "\t\t\t------------------------" << endl << endl;
                SetColor(COLOR_WHITE);
                char ch; cout << "\t\t\t"; cin >> ch;
            }
            else
            {
                input_code += line + '\n';
            }
        }

        if (!input_code.empty() && input_code.back() == '\n')
        {
            input_code.pop_back();
        }

        if (input_code.empty())
        {
            SetColor(COLOR_YELLOW);
            cout << "\t\t\tNo code entered. Please try again or type 'exit'." << endl;
            SetColor(COLOR_WHITE);
            continue;
        }

        cout << endl;
        SetColor(COLOR_AQUA);
        cout << "\t\t\t--- Checking Balance ---" << endl;
        SetColor(COLOR_WHITE);

        bool is_balanced = check_balance(&checker, input_code);

        if (is_balanced)
        {
            SetColor(COLOR_GREEN);
            cout << "\t\t\tParentheses, braces, and brackets are balanced." << endl;
        }
        else
        {
            SetColor(COLOR_RED);
            cout << "\t\t\tParentheses, braces, and brackets are NOT balanced." << endl;
            print_errors(&checker);
        }
        SetColor(COLOR_AQUA);
        cout << "\t\t\t------------------------" << endl << endl;
        SetColor(COLOR_WHITE);

        Sleep(10000);
    }

    balancer_free(&checker);

    char ch; cout << "\t\t\t"; cin >> ch;

    return 0;
}
