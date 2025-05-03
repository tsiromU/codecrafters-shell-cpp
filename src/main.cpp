#include <iostream>

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
      std::cout << input.substr(5) << std::endl;
      continue;
    }
    if(input.find_first_of("type ") == 0){
      input = input.substr(5);
      if(input == "echo"){
        std::cout << "echo is a shell builtin" << std::endl;
      } else if (input == "exit"){
        std::cout << "exit is a shell builtin" << std::endl;
      } else{
        std::cout << input << ": not found" << std::endl;
      }
      continue;
    }
    std::cout << input << ": command not found" << std::endl;
  }
}
