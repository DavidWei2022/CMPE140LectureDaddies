#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>

using namespace std;

// Define registers, PC, instruction memory, and data memory
vector<uint32_t> registers(32, 0);  // 32 general-purpose registers
uint32_t pc = 0x00000000;  // Program Counter
vector<uint32_t> instruction_memory;  // Vector to store instructions
unordered_map<uint32_t, uint32_t> data_memory;  // Unordered map to store data

// Load the program from a text file
void load_program(const string& filename) {
    instruction_memory.clear();
    ifstream file(filename);
    if (file) {
        string line;
        while (getline(file, line)) {
            uint32_t instruction = stoul(line, nullptr, 16);  // Assuming hexadecimal instructions
            instruction_memory.push_back(instruction);
        }
        cout << "Program loading is complete." << endl;
        file.close();
    } else {
        cout << "File not found." << endl;
    }
}

// Function to execute a single RV32I instruction
void execute_instruction(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    int32_t imm = instruction >> 20;  // Sign-extended immediate value

    if (opcode == 0x33) {  // R-type instruction (e.g., ADD, SUB, etc.)
        if (funct3 == 0x0) {  // ADD
            registers[rd] = registers[rs1] + registers[rs2];
        }
        // Implement other R-type instructions here
    } else if (opcode == 0x13) {  // I-type instruction (e.g., ADDI, etc.)
        if (funct3 == 0x0) {  // ADDI
            registers[rd] = registers[rs1] + imm;
        }
        // Implement other I-type instructions here
    } else if (opcode == 0x63) {  // B-type instruction (e.g., BEQ, BNE, etc.)
        if (funct3 == 0x0) {  // BEQ
            if (registers[rs1] == registers[rs2]) {
                pc += imm;
            }
        }
        // Implement other B-type instructions here
    }
    // Add support for other instruction types
}

// Display help information
void display_help() {
    cout << "Available commands:" << endl;
    cout << "  r            - Run the entire program" << endl;
    cout << "  s            - Run the next instruction" << endl;
    cout << "  x0-x31       - Display register values (e.g., x0, x1, x2, ..., x31)" << endl;
    cout << "  0x12345678   - Display data from a memory address (e.g., 0x12345678)" << endl;
    cout << "  pc           - Display the current program counter value" << endl;
    cout << "  help         - Display this help information" << endl;
    cout << "  exit         - Exit the simulator" << endl;
}

int main() {
    string command;

    display_help();

    while (true) {
        cout << "Enter a command: ";
        cin >> command;

        if (command == "exit") {
            break;
        }

        if (command == "help") {
            display_help();
        } else if (command == "r") {  // Run the entire program
            while (pc < instruction_memory.size()) {
                execute_instruction(instruction_memory[pc]);
                pc++;
            }
        } else if (command == "s") {  // Run the next instruction
            if (pc < instruction_memory.size()) {
                execute_instruction(instruction_memory[pc]);
                pc++;
            }
        } else if (command == "pc") {  // Display the value of PC
            cout << "PC: 0x" << hex << pc << dec << endl;
        } else if (command[0] == 'x') {  // Display register value
            int reg_num = stoi(command.substr(1));
            if (reg_num >= 0 && reg_num < 32) {
                cout << command << ": " << registers[reg_num] << endl;
            } else {
                cout << "Invalid register number" << endl;
            }
        } else if (command.substr(0, 2) == "0x") {  // Display data memory value
            uint32_t address = stoul(command, nullptr, 16);
            if (data_memory.find(address) != data_memory.end()) {
                cout << command << ": " << "0x" << hex << data_memory[address] << dec << endl;
            } else {
                cout << "Address not found in data memory" << endl;
            }
        } else {
            cout << "Invalid command. Type 'help' for available commands." << endl;
        }
    }

    return 0;
}
