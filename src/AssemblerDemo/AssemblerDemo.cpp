// AssemblerDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

// ---------------------------------------------------------------------------------------------

#pragma region Data Structures

struct datafile
{
    std::string name;
    std::string type;
    std::vector<std::string> value;
};


struct segment
{
    std::string name;
    int32_t position;
};

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region Global Variables

std::string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region Function Forward Declarations

void expand_pseudo_instructions(std::vector<std::string> instruction_formats, 
                                std::vector<std::string>& instruction_lines,
                                std::vector<std::string> instruction_params);
void load_instrution_formats(std::vector<std::string>& instruction_formats);
void read_data_segment();
void read_text_segment(std::vector<std::string>& instruction_lines);
void set_labels();
void convert_sp_params(std::vector<std::string> instruction_params);
void process(std::vector<std::string> instruction_params);


#pragma endregion    

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

int main()
{

    std::cout << "--------------------------------------" << std::endl
                                                          << std::endl
              << "    RiscEE C++ Assembler"               << std::endl
                                                          << std::endl
              << "--------------------------------------" << std::endl
                                                          << std::endl;

    std::cout << "TODO: Comments are not parsed." << std::endl;



    // Initialize the data memory.
    for (int i = 0; i < 4000; i++)
        datamemory[i] = "00";

    // Read data segment and populate the data memory.
    read_data_segment();


    std::vector<std::string> instruction_formats;
    load_instrution_formats(instruction_formats);

    std::vector<std::string> instruction_lines;
    read_text_segment(instruction_lines);

    std::vector<std::string> instruction_params;
    expand_pseudo_instructions(instruction_formats, instruction_lines, instruction_params);

    for (int i = 0; i < instruction_lines.size(); i++)
    {
        std::cout << "Instruction: " << instruction_lines[i] << std::endl;
    }

    set_labels();

    convert_sp_params(instruction_params);
    process(instruction_params);

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
            //std::cout << word << std::endl;

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
void read_text_segment(std::vector<std::string>& instruction_lines)
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
            {
                // Data section found.
                flag = 1;
            }

            if (instruction == ".text")
            {
                // Instruction / text section found.
                flag = 0;
                continue;
            }

            if (flag != 1)
            {
                instruction_lines.push_back(instruction);
            }
        }
    }
    file.close();
}

// ---------------------------------------------------------------------------------------------

void expand_pseudo_instructions(std::vector<std::string> instruction_formats, 
                                std::vector<std::string>& instruction_lines,
                                std::vector<std::string> instruction_params)
{
    size_t instruction_count = instruction_lines.size();
    for (int instruction_index = 0; instruction_index < instruction_count; instruction_index++)
    {
        std::string instruction = instruction_lines[instruction_index];
        
        // Itterate the characters of the instruction.
        for (int index = 0; index < instruction.size(); index++)
        {
            // Replace tabs with space characters.
            if (instruction_lines[instruction_index][index] == 9)
            {
                instruction_lines[instruction_index][index] = 32;
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
                std::cout << "Label Found:    " << token << std::endl;
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
            // TODO.
            std::cout << "TODO: Parse text on the same line following a label definition." << std::endl;
        }

        if (token == "la")
        {
            std::cout << "TODO: Parse ""la"" pseudo instruction." << std::endl;
            //processla(i);
            continue;
        }
        else if (token == "lw" || token == "lb" || token == "lhw")
        {
            std::cout << "TODO: Parse ""lw lb lhw"" pseudo instructions." << std::endl;
        }

        //   What is this doing??????
        // Itterate all format strings.
        for (int format_index = 0; format_index < instruction_formats.size(); format_index++)
        {
            //std::cout << instruction_formats[format_index] << std::endl;
            std::string format_string = instruction_formats[format_index];

            // Read the instruction mnemonic from the instruction format strings. 
            std::string mnemonic;
            int k1 = 0;
            while (format_string[k1] != ' ')
            {
                mnemonic += format_string[k1];
                k1++;
            }
            if (token.compare(mnemonic) == 0)
            {
                // Found the command format string.  
                //std::cout << "Format Found:   Instruction: " << token << "  Mnemonic: " << mnemonic << std::endl;

                std::string arguments;
                for (int k = char_index; k < instruction.size(); k++)
                {
                    arguments += instruction[k];
                }
                instruction_params.push_back(arguments);
               
                //std::cout << "Command: " << mnemonic << "  Arguments:  " << arguments << std::endl;
                break;
            }

        }

    }
}

// ---------------------------------------------------------------------------------------------

void set_labels()
{

}

// ---------------------------------------------------------------------------------------------

// Convert Stack Pointer(sp) to x2
void convert_sp_params(std::vector<std::string> instruction_params)
{
    for (int params_index = 0; params_index < instruction_params.size(); params_index++)
    {
        std::string params = instruction_params[params_index];

        size_t char_count = params.size();
        for (int char_index = 0; char_index < char_count; char_index++)
        {
        }
    }
}

// ---------------------------------------------------------------------------------------------

//To extract instruction type and process them independently
void process(std::vector<std::string> instruction_params)
{

}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
