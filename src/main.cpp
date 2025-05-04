#include <iostream>
#include <filesystem>

std::string execute_command_echo(std::string input);
std::string execute_command_type(std::string input);

std::string find_in_path(std::string file){
  std::string s;
  std::string stringPath(getenv("PATH"));
  std::stringstream ss(stringPath);
  while(getline(ss, s, ':')){
      std::filesystem::path path(s + '/' + file);
      if(std::filesystem::exists(path)){
          if(std::filesystem::is_regular_file(path));
              return path;
      }
  }
  return "";
}


int main() {
  // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;


    while(true){
        std::string input;
        std::cout << "$ ";
        std::getline(std::cin, input);
        if(input == "exit 0"){
        break;
        }

        if(input.find_first_of("echo ") == 0){
        input = input.substr(5);
        std::cout << execute_command_echo(input) << std::endl;
        continue;
        }

        if(input.find_first_of("type ") == 0){
        input = input.substr(5);
        std::cout << execute_command_type(input) << std::endl;
        continue;
        }

        std::cout << input << ": command not found" << std::endl;
    }
}


std::string execute_command_echo(std::string input){
    return input;
};


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
    std::string ans = find_in_path(input);
    if(ans != "")
        return ans;
    return input + ": not found";
};
