#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <Windows.h>

// ----------------------------------------------------------------------------------

#define START 268435456;

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

std::string datamemory[4000]; // Initial Size of Data Memory is Fixed 4000 Bytes

std::vector<std::string> format_strings;
std::vector<std::string> code_strings;
std::vector<std::string> code;

size_t sizeI;

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

struct datafile
{
    std::string name;
    std::string type;
    std::vector<std::string> value;

    datafile() = default;
};

// ----------------------------------------------------------------------------------

struct seg
{
    std::string name;
    int64_t position;
};

// ----------------------------------------------------------------------------------

typedef struct
{
    std::string s;
    int index;
} label;

// ----------------------------------------------------------------------------------

std::vector<seg> datalabel;
std::vector<label> labels;

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// Convert a number as a string format to its Hexadecimal
std::string convert(std::string s, int len);
// Get 2's Complement Representation for Immediate Values
uint32_t getinver(int32_t imme, int bit);

bool read_code();
bool read_data();
bool read_formats();
void read_labels();
void expand_pseudo_commands();

// Convert Stack Pointer(sp) to x2
void preprocess();

void processla(int index);
void processlw(std::string type, int index, int64_t pos);



// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

int main()
{
    std::cout << "Assembler Prototype. Ported from A to M" << std::endl;

    for (int i = 0; i < 4000; i++)
    {
        datamemory[i] = "00";
    }

    read_data();
    read_formats();
    read_code();
    expand_pseudo_commands();
    /*
    read_labels();
    preprocess();
    */

    std::cout << "Assembler Prototype Application Terminating." << std::endl;

    return 0;
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

std::string convert(std::string s, int len)
{
    int length = s.size();
    int flag = 1;
    for (int i = 0; i < length; i++)
    {
        if (s[i] == 'x' || s[i] == 'X')
        {
            flag = 0;
            break;
        }
    }
    if (flag)
    {
        int32_t num = 0;
        int32_t mul = 1;
        int last = 0;
        int flag1 = 0;
        
        if (s[0] == '-')
        {
            flag1 = 1;
        }
        
        if (flag1)
        {
            last = 1;
        }

        for (int i = length - 1; i >= last; i--)
        {
            num += (s[i] - 48) * mul;
            mul *= 10;
        }

        if (flag1)
        {
            num = getinver(num, len * 4);
        }

        std::string ans;
        for (int i = 0; i < len; i++)
        {
            int rem = num % 16;
            if (rem <= 9)
            {
                ans += (rem + 48);
            }
            else
            {
                ans += (rem - 10 + 65);
            }
            num /= 16;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
    else
    {
        int32_t x;
        std::stringstream ss;
        ss << std::hex << s;
        ss >> x;
        int32_t num = x;
        if (num < 0)
        {
            num = getinver(num, len * 4);
        }
        std::string ans;
        for (int i = 0; i < len; i++)
        {
            int rem = num % 16;
            if (rem <= 9)
            {
                ans += (rem + 48);
            }
            else
            {
                ans += (rem - 10 + 65);
            }
            num /= 16;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }

}

// ----------------------------------------------------------------------------------

// Get 2's Complement Representation for Immediate Values
uint32_t getinver(int32_t imme, int bit)
{
    int32_t imcopy = -imme;
    std::vector<int>bb;
    imme = -imme;
    for (int i = 0; i < bit; i++)
    {
        bb.push_back(imme % 2);
        imme /= 2;
    }
    reverse(bb.begin(), bb.end());
    for (int i = 0; i < bit; i++)
    {
        if (bb[i])
            bb[i] = 0;
        else
            bb[i] = 1;
    }
    int j = bit - 1;
    while (1)
    {
        if (bb[j] == 0)
        {
            bb[j] = 1;
            break;
        }
        else {
            bb[j] = 0;
        }
        j--;
    }
    int32_t num = 0;
    int32_t mul = 1;
    for (int i = bit - 1; i >= 0; i--)
    {
        num += bb[i] * mul;
        mul *= 2;
    }
    return num;
}

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
                {   
                    // data part starts
                    start = 1;
                }
            }
        }
    }
    file.close();

    int pos = 0;
    for (int i = 0; i < stored.size(); i++)
    {
        seg temp;   
        temp.name = stored[i].name;    
        temp.position = pos + START;

        datalabel.push_back(temp);

        if (stored[i].type == "byte")
        {
            for (int j = 0; j < stored[i].value.size(); j++)
            {
                std::string s = convert(stored[i].value[j], 2);
                datamemory[pos++] = s;
            }
        }
        else if (stored[i].type == "word")
        {
            for (int j = 0; j < stored[i].value.size(); j++)
            {
                std::string s = convert(stored[i].value[j], 8);
                std::string temp;    
                
                temp += s[6]; 
                temp += s[7];
                datamemory[pos++] = temp; 
                temp.clear();

                temp += s[4]; 
                temp += s[5];
                datamemory[pos++] = temp; 
                temp.clear();

                temp += s[2]; 
                temp += s[3];
                datamemory[pos++] = temp; 
                temp.clear();

                temp += s[0]; 
                temp += s[1];
                datamemory[pos++] = temp; 
                temp.clear();
            }
        }
        else if (stored[i].type == "halfword")
        {
            for (int j = 0; j < stored[i].value.size(); j++)
            {
                std::string s = convert(stored[i].value[j], 4);
                std::string temp;

                temp += s[2]; 
                temp += s[3];
                datamemory[pos++] = temp; 
                temp.clear();
                
                temp += s[0]; 
                temp += s[1];
                datamemory[pos++] = temp; 
                temp.clear();
            }
        }
        else
        {
            std::cout << "ERROR: Invalid Data Type: " << stored[i].type << std::endl;
        }
    }

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

// Read the labels from code.
void read_labels()
{
    size_t code_lines = code_strings.size();
    int count = -1;
    for (int i = 0; i < code_lines; i++)
    {
        std::string ins;
        int j = 0;
        int start = -1;

        while (j < code_strings[i].size() && code_strings[i][j] == ' ')
        {
            // Ignore leading space characters.
            j++;
        }
        start = j;

        while (j < code_strings[i].size() && code_strings[i][j] != ' ')
        {
            ins += code_strings[i][j];
            j++;
            if (j < code_strings[i].size() && code_strings[i][j] == ':')
            {
                ins += code_strings[i][j];
                j++;
                break;
            }
        }

        size_t sins = ins.size();

        if (ins[sins - 1] == ':')
        {
            std::string ins1;
            for (int k = 0; k < sins - 1; k++)
            {
                ins1 += ins[k];
            }
            label temp;
            temp.s = ins1;
            temp.index = count + 1;
            labels.push_back(temp);
        }

        if (ins[sins - 1] == ':' && sins < code_strings[i].size())
        {
            while (j < code_strings[i].size() && code_strings[i][j] == ' ')
            {
                j++;
            }
            ins.clear();
            while (j < code_strings[i].size() && code_strings[i][j] != ' ')
            {
                ins += code_strings[i][j];
                j++;
            }
        }

        if (ins == "la")
        {
            count += 2;
            continue;
        }
        else if (ins == "lw" || ins == "lb" || ins == "lhw")
        {
            std::string lab;
            size_t j = code_strings[i].size() - 1;
            while (code_strings[i][j] == ' ')
            {
                j--;
            }
            while (code_strings[i][j] != ' ')
            {
                lab += code_strings[i][j];
                j--;
            }
            reverse(lab.begin(), lab.end());
            int flag = 1;
            for (int j = 0; j < datalabel.size(); j++)
            {
                if (lab.compare(datalabel[j].name) == 0)
                {
                    flag = 0;
                    count += 2;
                    break;
                }
            }
            if (!flag)
            {
                continue;
            }
        }

        for (int k = 0; k < sizeI; k++)
        {
            std::string type;
            int k1 = 0;
            while (format_strings[k][k1] != ' ')
            {
                type += (format_strings[k][k1]);
                k1++;
            }
            if (ins.compare(type) == 0)
            {
                count++;
                break;
            }
        }
    }
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
            processla(i);
            continue;
        }
        else if (instruction == "lw" || instruction == "lb" || instruction == "lhw")
        {
            // Read the last parameter of the instruction 
            std::string lab;
            size_t j = line.size() - 1;
            while (line[j] == ' ')
            {
                j--;
            }
            while (line[j] != ' ')
            {
                lab += line[j];
                j--;
            }
            reverse(lab.begin(), lab.end());

            int flag = 1;
            // TODO: Bug datalabel vector not initialized.
            for (int j = 0; j < datalabel.size(); j++)
            {
                if (lab.compare(datalabel[j].name) == 0)
                {
                    flag = 0;
                    processlw(instruction, i, datalabel[j].position);
                    break;
                }
            }

            continue;
        }
       
        for (int k = 0; k < sizeI; k++)
        {
            std::string instruction_type;
            int k1 = 0;
            while (format_strings[k][k1] != ' ')
            {
                instruction_type += (format_strings[k][k1]);
                k1++;
            }
            if (instruction.compare(instruction_type) == 0)
            {
                std::string add;
                for (int k = start; k < code_strings[i].size(); k++)
                {
                    add += code_strings[i][k];
                }
                code.push_back(add);
                break;
            }
        }
    }
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// Convert Stack Pointer(sp) to x2
void preprocess()
{
    for (int i = 0; i < code.size(); i++)
    {
        std::string& instruction = code[i];
        size_t instruction_size = code[i].size();
        for (size_t j = 1; j < instruction_size; j++)
        {
            if (instruction[j - 1] == ' ' && instruction[j] == 's' && j + 1 < instruction_size && instruction[j + 1] == 'p' && j + 2 < instruction_size && instruction[j + 2] == ' ')
            {
                instruction[j] = 'x';
                instruction[j + 1] = '2';
            }

        }
    }
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

// Process Load Address(la) pseudo command
void processla(int index)
{

}

// ----------------------------------------------------------------------------------

// Process Load Word(lw) pseudo command
void processlw(std::string type, int index, int64_t pos)
{
    int64_t current_program_counter = code.size() * 4 + START;

    int i = 0;
    while (code_strings[index][i] != 'x')
    {
        i++;
    }
    i++;

    std::string s;
    while (code_strings[index][i] != ' ')
    {
        s += code_strings[index][i++];
    }

    current_program_counter = pos - current_program_counter;

    std::string instruction;
    instruction = "auipc x" + s + " 65536";
    code.push_back(instruction);
    instruction.clear();


}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------



