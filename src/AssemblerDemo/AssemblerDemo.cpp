// AssemblerDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

//namespace fs = std::filesystem;

// ---------------------------------------------------------------------------------------------

#pragma region Data Structures

struct datafile
{
    std::string name;
    std::string type;
    std::vector<std::string> value;
};

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region Global Variables

std::string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region Function Forward Declarations

void expand_pseudo_instructions(std::vector<std::string>& instructions);
void load_instrution_formats(std::vector<std::string>& instruction_formats);
void read_data_segment();
void read_text_segment(std::vector<std::string>& instructions);


#pragma endregion    

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

int main()
{
    std::cout << "-- Assembler C++ Demo! ------\n";

    // Initialize the data memory.
    for (int i = 0; i < 4000; i++)
        datamemory[i] = "00";

    // Read data segment and populate the data memory.
    read_data_segment();


    std::vector<std::string> instruction_formats;
    load_instrution_formats(instruction_formats);

    std::vector<std::string> instructions;
    read_text_segment(instructions);
    expand_pseudo_instructions(instructions);

    std::cout << "Exiting Assembler C++ Demo\n";
}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------

void load_instrution_formats(std::vector<std::string>& instruction_formats)
{
    std::string filename = get_exe_path() + "formats.txt";

    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open instruction formats file: " << filename << std::endl;
        return;
    }
    else
    {
        std::string instruction_format;
        while (std::getline(file, instruction_format))
        {
            instruction_formats.push_back(instruction_format);
        }
    }
    file.close();
}

// ---------------------------------------------------------------------------------------------

// Read the data segment.
void read_data_segment()
{
    std::string filename = get_exe_path() + "test.asm";

    std::ifstream file;
    file.open(filename); 
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open: " << filename << std::endl;
        return;
    }
    else
    {
        std::cout << "Opened Test Assembly Source File: " << filename << std::endl;

        int start = 0;
        std::string word;
        while (!file.eof())
        {
            file >> word;
            std::cout << word << std::endl;

            if (start > 1)
            {
                continue;
            }
            else if (start == 1)
            {
                if (word == ".text")
                {
                    // Reached the end of the data section.
                    start = 2;
                }
                else
                {
                    // Read the data section.

                }
            }
            else if (start == 0)
            {
                if (word == ".data")
                {
                    // Found the data section.
                    start = 1;
                }
            }
        }
    }
    // Close the source code file.
    file.close();
}

// ---------------------------------------------------------------------------------------------

// Read instructions from the test file.
void read_text_segment(std::vector<std::string>& instructions)
{
    std::string filename = get_exe_path() + "test.asm";

    std::ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Failed to open: " << filename << std::endl;
        return;
    }
    else
    {
        int flag = 0;
        std::string instruction;
        while (std::getline(file, instruction))
        {
            if (instruction == ".data")
                flag = 1;
            if (instruction == ".text")
            {
                flag = 0;
                continue;
            }
            if (flag != 1)
            {
                instructions.push_back(instruction);
            }
        }
    }
    file.close();
}

// ---------------------------------------------------------------------------------------------

void expand_pseudo_instructions(std::vector<std::string>& instructions)
{
    size_t instruction_count = instructions.size();
    for (int instruction_index = 0; instruction_index < instruction_count; instruction_index++)
    {
        std::string instruction = instructions[instruction_index];
        
        // Itterate the characters of the instruction.
        for (int index = 0; index < instruction.size(); index++)
        {
            // Replace tabs with space characters.
            if (instructions[instruction_index][index] == 9)
            {
                instructions[instruction_index][index] = 32;
            }
        }
         
        int char_index = 0;

        // Ignore leading space characters.
        while (char_index < instruction.size() && instruction[char_index] == ' ')
        {
            char_index++;
        }

        // Read the first token of the command.
        std::string token;
        while (char_index < instruction.size() && instruction[char_index] != ' ')
        {
            token += instruction[char_index];
            char_index++;
            if (char_index < instruction.size() && instruction[char_index] == ':')
            {
                // The instruction is a label.  
                token += instruction[char_index];
                // Goto the next character.
                char_index++;
                // Token reader completed. 
                break;
            }
        }

        // Ignore spaces after the first token characters.
        while (char_index < instruction.size() && instruction[char_index] == ' ')
        {
            char_index++;
        }

        // Is the current token a label with trailing characters?
        size_t token_size = token.size();
        if (token[token_size - 1] == ':' && instruction.size() > token_size)
        {
            // 
        }





    }
}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
