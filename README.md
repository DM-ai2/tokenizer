# tokenizer
a script tokenizer for c++ projects
# Example : 
```cpp 
#include "tokenize.hpp"
#include <fstream>
int main(int argc, char*args[]){
  std::fstream readFile(args[1]);if(!readFile){std::cout<<"no input file"<<std::endl;}
  std::string script((std::istreambuf_iterator<char>(readFile)),std::istreambuf_iterator<char>());
  readFile.close();
  std::vector<token>tokens = tokenize(script,
    //conditional words
    {
      "class",
      "function",
      "if",
      "else",
      "for",
      "while",
      "in"
    },
    //double symbols
    {
      "++",
      "--",
      "==",
      "!=",
      "<=",
      ">=",
      "&&",
      "||",
      "-=",
      "+=",
      "*=",
      "/="
    }
  );
  printTokens(tokens);
}
```
