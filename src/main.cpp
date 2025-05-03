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
    std::cout << input << ": command not found" << std::endl;
  }
}
