#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;
enum class Token(int value){
   tok_eof = -1,
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5
}
static string IdentifierStr;
static double NumVal;
static int gettok(){
    static int LastChar = ' ';
    while (isspace(LastChar))
        LastChar = getchar();
    if (isalpha(LastChar)){
        IdentifierStr = LastChar;
        while (isalunum((LastChar = getchar())))
            IdentifierStr += LastChar;
        if (IdentifierStr == "def")
          return toK_def;
        if (IdentifierStr == "extern")
          return toK_extern;
        return toK_identifier;
    }
    if(isdigit(LastChar) || LastChar == '.'){
        string NumStr;
        do{
            NumStr += LastChar;
            LastChar = getchar();
        }while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), nullptr);
        return toK_number;
    }
    if (LastChar == '#'){
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if (LastChar != EOF)
            return gettok();
    }
    if(LastChar == EOF)
        return toK_eof;
    int ThisChar = LastChar;
        return ThisChar;
}
namespace {
    class ExprAST {
        public:
        virtual ~ExprAST() = default;

};
class NumberExprAST : public ExprAST {
    double Val;
    public:
    NumberExprAST(double Val) : Val(Val) {}
};
class VariableExprAST : public ExprAST {
    string Name;
    public:
    VariableExprAST(const string &Name) : Name(Name) {}
};
class BinaryExprAST : public ExprAST {
    char Op;
    unique_ptr<ExprAST> LHS, RHS;
    public:
    BinaryExprAST(char Op, unique_ptr<ExprAST> LHS,
                  unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(move(LHS)), RHS(move(RHS)) {}
};
class CallExprAST : public ExprAST {
    string Callee;
    vector<unique_ptr<ExprAST>> Args;
    public:
    CallExprAST(const string &Callee,
                vector<unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(move(Args)) {}
};
class PrototypeAST {
    string Name;
    vector<string> Args;
    public:
    PrototypeAST(const string &Name, vector<string> Args)
        : Name(Name), Args(move(Args)) {}
        const string &getName() const { return Name; }
};
class FunctionAST {
    unique_ptr<PrototypeAST> Proto;
    unique_ptr<ExprAST> Body;
    public:
    FunctionAST(unique_ptr<PrototypeAST> Proto,
                unique_ptr<ExprAST> Body)
        : Proto(move(Proto)), Body(move(Body)) {}
};
}
int main() {
  return 0;
}