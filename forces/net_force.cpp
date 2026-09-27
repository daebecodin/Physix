#include "pch.h"

using std::cin, std::cout;

std::string getStr()
{
    std::string prompt("Enter a valid string: ");
    std::string str;

    cout << prompt;

    while (std::cin >> str) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid String. Try Again";
        }
    }

    return str;
}

int getNum()
{
    int num{};
    return num;

}


int main() 
{

    std::string test = getStr();
    cout << '\n' << test;
    return 0;

}
