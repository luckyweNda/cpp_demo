#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string line;
    std::cout << "enter the file name" << std::endl;
    std::getline(std::cin, line);

    if(line[0] == '\0')
    {
        std::cout << "Please enter a valid file name." << std::endl;
        return false;
    }

    std::ifstream file;
    file.open(line);

    if(!file.is_open())
    {
        std::cout << "The file cannot be opened." << std::endl;
        return false;
    }

    while(std::getline(file, line))
    {
        std::cout << line << std::endl;
    }

    file.close();

    return 0;
}