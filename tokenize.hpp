#include <vector>
#include <string>
class token{
public:
  int start;
  int end;
  std::string value;
  std::string type;
};
std::vector<token>tokenize(std::string script,std::vector<std::string>conditional_words,std::vector<std::string>multi_symbols){
  std::vector<token>This;
  token Cap;
  bool str1=0;
  bool str2=0;
  bool str3=0;
  bool strEsc=0;
  int letter=0;
  int skip=0;
  for(char&i:script){
    Cap.end=letter;
    if(skip){
      skip--;
    }else if(strEsc){
      strEsc=0;
      Cap.value+=i;
    }else if(i == '"'&&!str1&&!str2&&!str3){
      if(Cap.value.length()){
        Cap.type = "id";
        This.push_back(Cap);
        Cap.value="";
      }
      str1=1;
      Cap.value+=i;
    }else if(i == '"'&&str1){
      str1=0;
      Cap.value+=i;
      Cap.type = "string";
      This.push_back(Cap);
      Cap.value="";
    }else if(i == '\''&&!str1&&!str2&&!str3){
      if(Cap.value.length()){
        Cap.type = "id";
        This.push_back(Cap);
        Cap.value="";
      }
      str2=1;
      Cap.value+=i;
    }else if(i == '\''&&str2){
      str2=0;
      Cap.value+=i;
      Cap.type = "string";
      This.push_back(Cap);
      Cap.value="";
    }else if(i == '`'&&!str1&&!str2&&!str3){
      if(Cap.value.length()){
        Cap.type = "id";
        This.push_back(Cap);
        Cap.value="";
      }
      str3=1;
      Cap.value+=i;
    }else if(i == '`'&&str2){
      str3=0;
      Cap.type = "string";
      This.push_back(Cap);
      Cap.value="";
    }else if(str1||str2||str3){
      if(i == '\\'){
        strEsc=1;
      }
      Cap.value+=i;
    }else{
      if(isspace(i)){
        if(Cap.value.length()){
          Cap.type = "id";
          This.push_back(Cap);
          Cap.value="";
        }
      }else if(!isalpha(i)&&!isdigit(i)){
        if(Cap.value.length()){
          Cap.type = "id";
          This.push_back(Cap);
          Cap.value="";
        }
        Cap.type = "symbol";
        Cap.value=i;
        This.push_back(Cap);
        Cap.value="";
      }else{
        Cap.value+=i;
      }
    }
    letter++;
  }
  auto match_conditional=[conditional_words=conditional_words](std::string val){
    for(auto&condition:conditional_words){
      if(val == condition){
        return true;
      }
    }
    return false;
  };
  if(Cap.value.length()){
    Cap.type = "id";
    if(str1||str2||str3){Cap.type = "unclosed string";}
    This.push_back(Cap);
    Cap.value="";
  }
  for(token&Tk:This){
    Tk.start = Tk.end-Tk.value.length();
    if(Tk.type == "id"){
      if(isdigit(Tk.value[0])){
        Tk.type="int";for(char&ltr:Tk.value){if(!isdigit(ltr)){Tk.type="invalid int";}}
      }else if(match_conditional(Tk.value)){
        Tk.type="condition";
      }
    }
  }
  auto match_multi_symbol=[multi_symbols=multi_symbols](std::string val){
    for(auto&condition:multi_symbols){
      if(val == condition){
        return true;
      }
    }
    return false;
  };
  std::vector<token>RETURN;
  int SKIP=0;
  for(int i=0;i<This.size()-1;i++){
    if(SKIP){
      SKIP--;
    }else if(This[i].type == "symbol"){
      if(This[i+1].type == "symbol"){
        if(match_multi_symbol(This[i].value+This[i+1].value)){
          This[i].value+=This[i+1].value;
          RETURN.push_back(This[i]);
          SKIP++;
        }else{
          RETURN.push_back(This[i]);
        }
      }else{
          RETURN.push_back(This[i]);
      }
    }else{
      RETURN.push_back(This[i]);
    }
  }
  if(!SKIP){
    RETURN.push_back(This[This.size()-1]);
  }
  return RETURN;
}
#include <iostream>
void printTokens(std::vector<token>tokens){int tkn=-1;for (token &tk : tokens){tkn++;std::cout<<tkn<<"\033[93m<\033[0;1m"<<tk.start<<"\033[93m:\033[0;1m"<<tk.end<<"\033[93m>\033[35;1m<"<<tk.type<<">\033[0;1m"<<tk.value<<"\033[0;0m"<<std::endl;}}
