// AssemblerDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define NOMINMAX 

#include <bitset>
#include <map>
#include <iostream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <sstream>
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


typedef struct
{
    std::string name;
    int index;
} label;

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region Global Variables

std::string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

#pragma endregion

// ---------------------------------------------------------------------------------------------

#pragma region ---- Function Forward Declarations ----------------------------------------------

void assemble_source_code(std::vector<std::string>& source_code,
                          std::vector<std::string> format_strings,
                          std::map<std::string, int> register_map);

std::map<std::string, int> create_register_map();

void expand_pseudo_instructions(std::vector<std::string> instruction_formats,
                                std::vector<std::string>& instruction_lines,
                                std::vector<std::string> instruction_params);

void encode_instruction(std::string instruction_type, std::string instruction_string, 
                                                      std::string format_string,
                                                      std::map<std::string, int> register_map);

void encode_I_instruction(std::string instruction_string, std::string format_string, std::map<std::string, int> register_map);

std::string get_format_string(std::string mnemonic, std::vector<std::string> format_strings);

void load_instrution_formats(std::vector<std::string>& instruction_formats);

void read_data_segment(std::istream& input);
void read_text_segment(std::istream& input, std::vector<std::string>& instruction_lines);
void read_source_stream(std::istream& input, std::vector<std::string>& source_code);



void parse_source_line(std::string& source_line, std::vector<std::string>& instruction_lines);

// Read instructions and labels from the source file
void read_labels(std::vector<std::string>& instruction_lines, std::vector<label>& labels);
void convert_sp_params(std::vector<std::string> instruction_params);
void process(std::vector<std::string> instruction_params);

std::string get_exe_path();
std::string& replace_tabs(std::string& str);
// Remove leading whitespace characters
std::string& ltrim(std::string& str);
// Split li
std::vector<std::string> split(std::string str, char delimiter);

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
    ///read_data_segment();

    std::map<std::string, int> register_map = create_register_map();

    // Get all instruction format strings;
    std::vector<std::string> format_strings;
    load_instrution_formats(format_strings);

    // Read and format the source code file.
    std::vector<std::string> source_code;
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
        std::cout << "Opened Test Assembly Source File: " << filename << std::endl;
        read_source_stream(file, source_code);
    }
    file.close();

    assemble_source_code(source_code, format_strings, register_map);

    /******************
    // Read instructions and labels from the source file
    std::vector<std::string> instruction_lines;
    read_text_segment(instruction_lines);

    for (int i = 0; i < instruction_lines.size(); i++)
    {
        std::cout << "Instruction: " << instruction_lines[i] << std::endl;
    }


    std::vector<std::string> instruction_params;
    expand_pseudo_instructions(instruction_formats, instruction_lines, instruction_params);


    std::vector<label> labels;
    read_labels(instruction_lines, labels);
    for (int i = 0; i < labels.size(); i++)
    {
        std::cout << labels[i].index << "    " << labels[i].name << std::endl;
    }

    convert_sp_params(instruction_params);
    process(instruction_params);
    *********************/

    std::cout << "Exiting Assembler C++ Demo\n";
}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

std::map<std::string, int> create_register_map()
{
    std::map<std::string, int> register_map
    {
        { "x0",    0 },
        { "x1",    1 },
        { "x2",    2 },
        { "x3",    3 },
        { "x4",    4 },
        { "x5",    5 },
        { "x6",    6 },
        { "x7",    7 },
        { "x8",    8 },
        { "x9",    9 },
        { "x10",  10 },
        { "x11",  11 },
        { "x12",  12 },
        { "x13",  13 },
        { "x14",  14 },
        { "x15",  15 },
        { "x16",  16 },
        { "x17",  17 },
        { "x18",  18 },
        { "x19",  19 },
        { "x20",  20 },
        { "x21",  21 },
        { "x22",  22 },
        { "x23",  23 },
        { "x24",  24 },
        { "x25",  25 },
        { "x26",  26 },
        { "x27",  27 },
        { "x28",  28 },
        { "x29",  29 },
        { "x30",  30 },
        { "x31",  31 }
    };
    return register_map;
}

// ---------------------------------------------------------------------------------------------

void read_parameter_tokens(std::string instruction_string, std::vector<std::string>& parameter_tokens)
{
    // Ignore the first token. The mnemonic is not required.
    while (!std::isspace(instruction_string[0]))
    {
        instruction_string.erase(0, 1);
    }

    std::string parameter_token;

    int char_index = 0;
    int char_count = instruction_string.size();

    while (char_index <= char_count)
    {
        char c = instruction_string[char_index];

        if (std::isspace(c) || (char_index >= char_count) || c == ',')
        {
            if (!parameter_token.empty())
            {
                parameter_tokens.push_back(parameter_token);
            }
            parameter_token.clear();
        }
        else
        {
            parameter_token.append(1, c);
        }
        char_index++;
    }
}



// ---------------------------------------------------------------------------------------------

void encode_instruction(std::string instruction_type, std::string instruction_string, 
                                                      std::string format_string,
                                                      std::map<std::string, int> register_map)
{
    if (instruction_type.compare("I") == 0)
    {
        // Immediate instruction type.
        encode_I_instruction(instruction_string, format_string, register_map);
    }
    if (instruction_type.compare("R") == 0)
    {

    }
    if (instruction_type.compare("S") == 0)
    {

    }
    if (instruction_type.compare("UJ") == 0)
    {

    }
    if (instruction_type.compare("U") == 0)
    {

    }
    if (instruction_type.compare("SB") == 0)
    {

    }
}

// ---------------------------------------------------------------------------------------------

// Encode immediate type instruction.
void encode_I_instruction(std::string instruction_string, std::string format_string, std::map<std::string, int> register_map)
{
    std::cout << "Encode instruction type: I" << std::endl;

    std::vector<std::string> parameter_tokens;
    read_parameter_tokens(instruction_string, parameter_tokens);
    for (std::string token : parameter_tokens)
    {
        std::cout << token << "   ";
    }
    std::cout << std::endl;

    /**************

    std::stringstream instruction_stream(instruction_string);

    // Ignore the first token. The mnemonic is not required.
    instruction_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

    std::string argument;
    while (std::getline(instruction_stream, argument, ','))
    {
        // 
        instruction_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

        std::cout << argument << std::endl;
    }
    ******/

    /**
    // Read destination (rd) register.
    if (instruction_stream >> argument)
    {
        std::cout << argument << std::endl;
        int rd = register_map[argument];
        std::cout << "rd: " << rd << std::endl;


    }
    */

    //std::string binary = std::bitset<8>(32).to_string();
    //std::cout << binary << std::endl;
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

void parse_source_line(std::string& source_line, std::vector<std::string>& instruction_lines)
{
    // strip leading spaces.
    ltrim(source_line);
    // Replace tab characters with space characters.
    replace_tabs(source_line);

    // Spit label lines that have instructions after the ":" character.
    std::vector<std::string> lines = split(source_line, ':');
    for (std::string line : lines)
    {
        instruction_lines.push_back(line);
    }

}

// ---------------------------------------------------------------------------------------------

void read_source_stream(std::istream& input, std::vector<std::string>& source_code)
{
    read_data_segment(input);

    // Move to the start of the input stream.
    input.seekg(0);
    read_text_segment(input, source_code);
}

// ---------------------------------------------------------------------------------------------

// Read the data segment.
void read_data_segment(std::istream& input)
{
    int start = 0;
    std::string word;

    while (!input.eof())
    {
        input >> word;
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

// ---------------------------------------------------------------------------------------------

// Read instructions from the test file.
void read_text_segment(std::istream& input, std::vector<std::string>& instruction_lines)
{
    instruction_lines.clear();

    int flag = 0;
    std::string instruction;
        
    while (std::getline(input, instruction))
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
            parse_source_line(instruction, instruction_lines);
        }
    }
}

// ---------------------------------------------------------------------------------------------

void assemble_source_code(std::vector<std::string>& source_code, 
                          std::vector<std::string> format_strings,
                          std::map<std::string, int> register_map)
{
    std::cout << "Assembling Source Code." << std::endl;

    for (int i = 0; i < source_code.size(); i++)
    {
        //std::cout << i << ":     " << source_code[i] << std::endl;

        std::string instruction = source_code[i];

        // TODO: What to do with labels??

        // Get mnemonic of the instruction.
        std::string mnemonic;
        std::stringstream stream(instruction);
        stream >> mnemonic;

                // Find the format of the instruction.
        std::string format_string = get_format_string(mnemonic, format_strings);
        if (!format_string.empty())
        {
            //std::cout << "FOUND THE FORMAT STRING:    " << format_string << std::endl;

            // Get the instruction format type. Last field in the format string;
            std::string instruction_type = format_string.substr(format_string.find_last_of(" ") + 1);
            //std::cout << "Found instruction type: " << mnemonic << "  " << instruction_type << std::endl;

            encode_instruction(instruction_type, instruction, format_string, register_map);

        }

    }

}

// ---------------------------------------------------------------------------------------------

std::string get_format_string(std::string mnemonic, std::vector<std::string> format_strings)
{
    std::string result_format_string;

    for (std::string format_string : format_strings)
    {
        std::string format_mnemonic;
        std::stringstream stream(format_string);
        stream >> format_mnemonic;
        if (format_mnemonic.compare(mnemonic) == 0)
        {
            result_format_string = format_string;
            return result_format_string;
        }
    }
    return result_format_string;
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
        /**
        for (int index = 0; index < instruction.size(); index++)
        {
            // Replace tabs with space characters.
            if (instruction_lines[instruction_index][index] == 9)
            {
                instruction_lines[instruction_index][index] = 32;
            }
        }
        */
         
        int char_index = 0;

        /**
        // Ignore leading space characters.
        while (char_index < instruction.size() && instruction[char_index] == ' ')
        {
            char_index++;
        }
        **/

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
                //std::cout << "Label Found: " << token << std::endl;
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

// Assumes that the source file has been parsed. (Label lines split into 2 statements) 

void read_labels(std::vector<std::string>& instruction_lines, std::vector<label>& labels)
{
    int line_index = 0;
    for (std::string line : instruction_lines)
    {
        // Reverse itterator
        auto itterator = std::find_if(line.rbegin(), line.rend(), [](char ch)
            {
                return (ch == ':');
            });
        line.erase(itterator.base(), line.end());

        if ((line.size() > 0) && (line[line.size() -1] == ':'))
        {
            std::string label_token = line;
            //std::cout << "Label Token:    " << label_token << std::endl;

            label label;
            label.name = label_token;
            label.index = line_index;

            labels.push_back(label);
        }

        line_index++;
    }
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

#pragma region ---- Helper Functions -----------------------------------------------------------

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

// Remove leading whitespace characters
std::string& ltrim(std::string& str)
{
    auto itterator = std::find_if(str.begin(), str.end(), [](char ch) 
    { 
        return !std::isspace<char>(ch, std::locale::classic()); 
    });
    str.erase(str.begin(), itterator);
    return str;
}

// ---------------------------------------------------------------------------------------------

std::string& rtrim(std::string& str)
{
    auto itterator = std::find_if(str.rbegin(), str.rend(), [](char ch)
    { 
        return !std::isspace<char>(ch, std::locale::classic()); 
    });
    str.erase(itterator.base(), str.end());
    return str;
}

// ---------------------------------------------------------------------------------------------

std::string& replace_tabs(std::string& str)
{
    auto find = str.find(9);
    while (find != std::string::npos)
    {
        str.replace(find, 1, (char *)32);
        find = str.find(9);
    }
    return str;
}

// ---------------------------------------------------------------------------------------------

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> result;

    auto end = str.find(delimiter);
    while (end != std::string::npos)
    {
        // Get token.
        std::string token = str.substr(0, end) + delimiter;
        // Erase the token from the input string. 
        str.erase(0, token.length());
        result.push_back(token);
        end = str.find(delimiter);
    }

    // return the remainder of the input string as a token.
    if (str.length() > 0)
    {
        result.push_back(str);
    }

    return result;
}

// ---------------------------------------------------------------------------------------------

#pragma endregion


// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
