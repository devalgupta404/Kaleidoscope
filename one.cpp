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
static int CurTok;
static int getNextToken() { return CurTok = gettok(); }
static map<char, int> BinopPrecedence;
static int GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}
unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}
unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}
unique_ptr<ExprAST> ParseExpression();
unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return move(Result);
}
static unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;
    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken();
    return V;
}
static unique_ptr<ExprAST> ParseIdentifierExpr() {
    string IdName = IdentifierStr;
    getNextToken();
    if (CurTok != '(')
        return make_unique<VariableExprAST>(IdName);
    getNextToken();
    vector<unique_ptr<ExprAST>> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(move(Arg));
            else
                return nullptr;
            if (CurTok == ')')
                break;
            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }
    getNextToken();
    return make_unique<CallExprAST>(IdName, move(Args));
}
static unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
    default:
        return LogError("unknown token when expecting an expression");
    case toK_identifier:
        return ParseIdentifierExpr();
    case toK_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}
static unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence();
        if (TokPrec < ExprPrec)
            return LHS;
        int BinOp = CurTok;
        getNextToken();
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, move(RHS));
            if (!RHS)
                return nullptr;
        }
        LHS = make_unique<BinaryExprAST>(BinOp, move(LHS), move(RHS));
    }
}
static unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;
    return ParseBinOpRHS(0, move(LHS));
}
static unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != toK_identifier)
        return LogErrorP("Expected function name in prototype");
    string FnName = IdentifierStr;
    getNextToken();
    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");
    vector<string> ArgNames;
    while (getNextToken() == toK_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");
    getNextToken();
    return make_unique<PrototypeAST>(FnName, move(ArgNames));
}
static unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken();
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;
    if (auto E = ParseExpression())
        return make_unique<FunctionAST>(move(Proto), move(E));
    return nullptr;
}
static unique_ptr<FunctionAST> ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto = make_unique<PrototypeAST>("__anon_expr",
                                              vector<string>());
        return make_unique<FunctionAST>(move(Proto), move(E));
    }
    return nullptr;
}
static unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();
    return ParsePrototype();
}
static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken();
    }
    
}
static void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        getNextToken();
    }
}
static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken();
    }
}
static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
        case toK_eof:
            return;
        case toK_def:
            HandleDefinition();
            break;
        case toK_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}
int main() {
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;
    fprintf(stderr, "ready> ");
    getNextToken();
    MainLoop();
  return 0;
}