#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <vector>

std::string execute_command_echo(std::string input);
std::string execute_command_type(std::string input);
void execute_command_cd(std::string input);

std::string find_in_path(std::string file);

void execute_command(std::string input);
void execute_command(std::vector<char*> args);
void execute_command(std::vector<std::string> args);

std::vector<std::string> string_to_vector(std::string input);
std::string vector_to_string(std::vector<std::string>& vec);

int main() {
  // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    
    while(true){
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string> input_vector;
        if(input == "exit 0"){
            break;
        }
        input_vector = string_to_vector(input);
        if(input_vector.size() == 0){
            continue;
        }
        if(input_vector[0] == "echo"){
            input_vector.erase(input_vector.begin());
            input = vector_to_string(input_vector);
            std::cout << execute_command_echo(input) << std::endl;
            continue;
        }

        if(input_vector[0] == "type"){
            input_vector.erase(input_vector.begin());
            input = vector_to_string(input_vector);
            std::cout << execute_command_type(input) << std::endl;
            continue;
        }
        
        if(input_vector[0] == "pwd"){
            std::cout << std::filesystem::current_path().string() << std::endl;
            continue;
        }

        if(input_vector[0] == "cd"){
            execute_command_cd(input_vector[1]);
            continue;
        }

        std::string path = find_in_path(input_vector[0]);
        if(path != ""){
            execute_command(input_vector);
            continue;
        }
        std::cout << input << ": command not found" << std::endl;

    }
}


std::string execute_command_echo(std::string input){
    return input;
};

void execute_command_cd(std::string input){
    if(input == ""){
        std::cout << "cd: no argument" << std::endl;
    }

    if(input == "~"){
        std::filesystem::path home_path(getenv("HOME"));
        std::filesystem::current_path(home_path);
        return;
    }

    std::filesystem::path path(input);
    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)){
        std::filesystem::current_path(path);
        return;
    }
    
    path = std::filesystem::current_path() / input;

    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)){
        std::filesystem::current_path(path);
        return;
    }
    std::cout << "cd: " << input << ": No such file or directory" << std::endl;
};


//needs refactoring(array of available commands, so we can get rid of the multiple if statements)
std::string execute_command_type(std::string input){
    if(input == "type"){
        return "type is a shell builtin";
    }
    if(input == "echo"){
        return "echo is a shell builtin";
    }
    if(input == "exit"){
        return "exit is a shell builtin";
    }
    if(input == "pwd"){
        return "pwd is a shell builtin";
    }

    if(input == "cd"){
        return "cd is a shell builtin";
    }

    std::string ans = find_in_path(input);
    if(ans != "")
        return ans;
    return input + ": not found";
};


std::string find_in_path(std::string file){
    std::string s;
    std::string stringPath(getenv("PATH"));
    std::stringstream ss(stringPath);
    while(getline(ss, s, ':')){
        std::filesystem::path path(s + '/' + file);
        if(std::filesystem::exists(path)){
            if(std::filesystem::is_regular_file(path))
                return path;
        }
    }
    return "";
  }

void execute_command(std::string input){
    auto id = fork();
    if(id != 0){
        int status;
        waitpid(id, &status, 0);
    } else {
        std::istringstream iss(input);
        std::vector<char*> args;
        std::string arg;

        while(iss >> arg){
            char* cstr = new char[arg.length() + 1];
            strcpy(cstr, arg.c_str());
            args.push_back(cstr);
        }
        execvp(args[0], args.data());
    }
}
void execute_command(std::vector<char*> args){
    auto id = fork();
    if(id != 0){
        int status;
        waitpid(id, &status, 0);
    } else {
        execvp(args[0], args.data());
    }
}
void execute_command(std::vector<std::string> args){
    auto id = fork();
    if(id != 0){
        int status;
        waitpid(id, &status, 0);
    } else {
        std::vector<char*> cstr_args;
        for (const auto& arg : args) {
            char* cstr = new char[arg.length() + 1];
            strcpy(cstr, arg.c_str());
            cstr_args.push_back(cstr);
        }
        execvp(cstr_args[0], cstr_args.data());
    }
}

std::string vector_to_string(std::vector<std::string>& vec) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            oss << " ";
        }
        oss << vec[i];
    }
    return oss.str();
}
std::vector<std::string> string_to_vector(std::string input){
    std::string word;
    std::istringstream iss(input);
    std::vector<std::string> ans;
    while(iss >> word){
        ans.push_back(word);
    }
    return ans;
};