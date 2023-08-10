#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <Windows.h>

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

std::string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

std::vector<std::string> format_strings;
std::vector<std::string> code_strings;

size_t sizeI;

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

struct datafile
{
    std::string name;
    std::string type;
    std::vector<std::string> value;
};

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

bool read_code();
bool read_data();
bool read_formats();
void expand_pseudo_commands();

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

int main()
{
    std::cout << "Assembler Prototype. Ported from A to M" << std::endl;

    for (int i = 0; i < 4000; i++)
        datamemory[i] = "00";
    read_data();
    read_formats();
    read_code();
    expand_pseudo_commands();

    std::cout << "Assembler Prototype Application Terminating." << std::endl;

    return 0;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

std::string get_exe_path()
{
    // Get path to executable
    CHAR buffer[_MAX_PATH] = { 0 };
    int count = GetModuleFileNameA(NULL, buffer, _MAX_PATH);
    // Create a path string. Inludes the filename.
    std::string path = std::string((char*)buffer, count);
    // Find the path to the execution directory
    size_t length = path.find_last_of("/\\") + 1;
    path.resize(length);
    return path;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// read code or text section.
bool read_code()
{
    code_strings.clear();

    std::string filename = get_exe_path() + "test.asm";
    std::ifstream file;

    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open: " << filename << std::endl;
        return false;
    }

    std::string line;
    int flag = 0;
    int start = 0;
    while (getline(file, line))
    {
        if (line == ".data")
        {
            flag = 1;
        }
        if (line == ".text")
        {
            flag = 0;
            continue;
        }
        if (flag != 1)
        {
            code_strings.push_back(line);
        }
    }
    file.close();
    return true;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// Read data section.
bool read_data()
{
    std::vector<datafile> stored;
   
    std::string filename = get_exe_path() + "test.asm";
    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open: " << filename << std::endl;
        return false;
    } 
    else
    {
        std::cout << "File Opened : " << filename << std::endl;
        int flag;
        int start = 0;

        std::string word;
        while (!file.eof())
        {
            file >> word;
            if (start > 1)
            {
                continue;
            }
            else if (start == 1)
            {
                if (word == ".text")
                {
                    // data part ends
                    start = 2;
                }
                else
                {
                    datafile temp;
                    flag = 0;
                    int index;
                    for (int i = 0; i < word.size() - 1; i++)
                    {
                        if (word[i] == ':' && word[i + 1] == '.')
                        {
                            flag = 1;
                            index = i;
                        }
                    }
                    if (flag == 1)
                    {
                        std::string nameT = "\0", typeT = "\0";
                        for (int i = 0; i < index; i++)
                        {
                            nameT += word[i];
                        }
                        for (int i = index + 2; i < word.size(); i++)
                        {
                            typeT += word[i];
                        }
                        temp.name = nameT;
                        temp.type = typeT;
                        getline(file, word);
                        std::stringstream ss(word);
                        while (ss >> word)
                        {
                            temp.value.push_back(word);
                        }
                    }
                    else
                    {
                        word.erase(word.end() - 1);
                        temp.name = word;
                        file >> word;
                        word.erase(word.begin());
                        temp.type = word;
                        getline(file, word);
                        std::stringstream ss(word);
                        while (ss >> word)
                        {
                            temp.value.push_back(word);
                        }
                    }
                    stored.push_back(temp);
                }
            }
            else if (start == 0)
            {
                if (word == ".data")
                {   //data part starts
                    start = 1;
                }
            }
        }
    }
    file.close();








    return true;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

bool read_formats()
{
    std::string filename = get_exe_path() + "formats.txt";
    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (getline(file, line))
    {
        format_strings.push_back(line);
    }
    file.close();

    sizeI = format_strings.size();
    return true;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

void expand_pseudo_commands()
{
    size_t string_count = code_strings.size();
    for (int i = 0; i < string_count; i++)
    {
        // Use reference to code line string.
        std::string& line = code_strings[i];

        for (int index = 0; index < line.size(); index++)
        {
            // Replace tab with space character.
            if (line[index] == 9)
            {
                line[index] = 32;
            }
        }

        int j = 0;
        int start = -1;
            
        while (j < line.size() && line[j] == ' ')
        {
            // Ignore space characters
            j++;
        }
        start = j;
        
        std::string instruction;

        while (j < line.size() && line[j] != ' ')
        {
            instruction += line[j];
            j++;
            if (j < line.size() && line[j] == ':')
            {
                instruction += line[j];
                j++;
                break;
            }
        }

        while (j < line.size() && line[j] == ' ')
        {
            // Ignore space characters
            j++;
        }

        size_t ins_size = instruction.size();
        
        if (instruction[ins_size - 1] == ':' && line.size() > ins_size)
        {
            instruction.clear();
            while (j < line.size() && line[j] == ' ')
            {
                // Ignore space characters
                j++;
            }
            start = j;
            // Add characters to the instruction string
            while (line[j] != ' ')
            {
                instruction += line[j++];
            }
        }



        if (instruction == "la")
        {
            //processla(i);

            continue;
        }
        else if (instruction == "lw" || instruction == "lb" || instruction == "lhw")
        {

            continue;
        }
       
        for (int k = 0; k < sizeI; k++)
        {
        }

        
    }
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------



