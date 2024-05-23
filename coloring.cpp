#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <vector>
#include <algorithm>

enum Token{
    INCLUDE,
    STRING_TEXT,
    KEYWORD,
    PARENTHESES,
    CURLY_BRACK,
    COMMENT,
    WHITE_CHAR_OR_NAME,
    DIGIT,
    ARITHMETIC
};

std::vector<std::string> keyWords = {"int", "return", "for", "printf"};

std::string getTokName(int idx){
    if(idx==0){return "INCLUDE";}
    else if(idx==1){return "STRING_TEXT";}
    else if(idx==2){return "KEYWORD";}
    else if(idx==3){return "PARENTHESES";}
    else if(idx==4){return "CURLY_BRACK";}
    else if(idx==5){return "COMMENT";}
    else if(idx==6){return "WHITE_CHAR_OR_NAME";}
    else if(idx==7){return "DIGIT";}
    else if(idx==8){return "ARITHMETIC";}
    return "";
}

void emptyCharHandler(std::string & word, std::vector<std::string> &res, char & x){
    if(!word.empty()){res.push_back(word);}
    word = "";
}

void spaceHandler(std::vector<std::string> &res, std::string &word, char & x){
    if(x == ' '){
        emptyCharHandler(word, res, x);
    }
    else{
        word += x;
    }
}

std::vector<std::string> removeSpaces(std::string param){
    std::string word = "";
    std::vector<std::string> res;
    for (auto x : param) 
    {
        spaceHandler(res, word, x);
    }
    if(!word.empty()){
        res.push_back(word);
    }
    return res;
}

void includeScannedHandler(std::string & str, std::vector<std::pair<std::string, Token>> & res){
    res.push_back({"#include", Token::INCLUDE});
    std::string tempStr = "#include";
    std::size_t temp = tempStr.length()+1;
    res.push_back({str.substr(temp), Token::STRING_TEXT});    
}

void noKeyWordOrBracketFoundHandler(std::string & str, std::string & word, std::vector<std::pair<std::string, Token>> & res){
    for(int i=0; i<word.length(); i++){
        if(word[i] == '=' || word[i] == '+' || word[i] == '>' || word[i] == '<' || word[i] == '-' || word[i] == '/' || word[i] == '*'){
            std::string temp = "";
            temp += word[i];
            res.push_back({temp, Token::ARITHMETIC});
        }
        else if(isdigit(word[i])){
            std::string temp = "";
            temp += word[i];
            res.push_back({temp, Token::DIGIT});
        }
        else if(word[i+1] == '=' || word[i+1] == '+' || word[i+1] == '>' || word[i+1] == '<' || word[i+1] == '-' || word[i+1] == '/' || word[i+1] == '*'){
            std::string temp = "";
            temp += word[i];
            res.push_back({temp, Token::WHITE_CHAR_OR_NAME});
        }
        else if(word[i-1] == '=' || word[i-1] == '+' || word[i-1] == '>' || word[i-1] == '<' || word[i-1] == '-' || word[i-1] == '/' || word[i-1] == '*'){
            std::string temp = "";
            temp += word[i];
            res.push_back({temp, Token::WHITE_CHAR_OR_NAME});
        }
        else if(isdigit(word[i-1])){
            std::string temp = "";
            temp += word[i];
            res.push_back({temp, Token::WHITE_CHAR_OR_NAME});
        }
        else{
            res.push_back({word, Token::WHITE_CHAR_OR_NAME});
            break;
        }
    }   
}

void noIncludeScannedHandler(std::string & str, std::vector<std::pair<std::string, Token>> & res){
    auto words = removeSpaces(str);
    for(auto word: words){
        if(std::find(keyWords.begin(), keyWords.end(), word) != keyWords.end()){
            res.push_back({word, Token::KEYWORD});
        }
        else if(word == "()" || word == "(" || word == ")"){
            res.push_back({word, Token::PARENTHESES});
        }
        else if(word == "{" || word == "}"){
            res.push_back({word, Token::CURLY_BRACK});
        }
        else if(word.substr(0, 2) == "//"){
            res.push_back({str, Token::COMMENT});
            goto outer;
        }
        else if(word.substr(0, 1) == "\""){
            res.push_back({word, Token::STRING_TEXT});
        }
        else{
            noKeyWordOrBracketFoundHandler(str, word, res);
        }
    }
    outer:;
}

std::vector<std::pair<std::string, Token>> scanner(std::string str){
    std::vector<std::pair<std::string, Token>> res;
    if(str.find("#include") != std::string::npos){
        includeScannedHandler(str, res);
    }
    else{
        noIncludeScannedHandler(str, res);
    }
    return res;
}

void htmlOutPutHandler(std::string & html_code, std::pair<std::string, Token> & el){
    if(getTokName(el.second) == "INCLUDE" || getTokName(el.second) == "KEYWORD"){
        auto addition = "<span style=\"color: blue\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
    else if(getTokName(el.second) == "STRING_TEXT"){
        auto addition = "<span style=\"color: #ff542f\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
    else if(getTokName(el.second) == "PARENTHESES" || getTokName(el.second) == "CURLY_BRACK"){
        auto addition = "<span style=\"color: yellow\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
    else if(getTokName(el.second) == "COMMENT"){
        auto addition = "<span style=\"color: green\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
    else if(getTokName(el.second) == "DIGIT"){
        auto addition = "<span style=\"color: #00b2ff\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
    else{
        auto addition = "<span style=\"color: white\">";
        html_code += addition;
        html_code += el.first;
        html_code += "</span>\"";
    }
}

int main(){
    std::ifstream file("input.txt");
    std::ofstream out("output.html");
    std::string str;
    std::vector<std::pair<std::string, Token>> output;
    std::string html_code = "";
    while(std::getline(file, str)){
        if(str.empty()){continue;}
        output = scanner(str);
        for(auto el: output){
            htmlOutPutHandler(html_code, el);
        }
        out << html_code;
        html_code.clear();
        html_code += "\n";
    }
    out.close();
    return 0;
}