//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Preprocessor
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <windows.h>

#pragma warning(disable : 4996)

#define ASSERT(x) if ((x)) { int i = 0; i = i / i; }

enum enmToken {
    enmToken_Unknown,
    
    enmToken_SharpDefine,
    enmToken_SharpUnDef,
    enmToken_SharpIfDef,
    enmToken_SharpIfNDef,
    enmToken_SharpIf,
    enmToken_SharpElIf,
    enmToken_SharpElse,
    enmToken_SharpEndIf,
    enmToken_SharpError,
    enmToken_SharpPragma,
    enmToken_SharpExtension,
    enmToken_SharpVersion,
    enmToken_SharpLine,
    enmToken_SharpInclude,

    enmToken_Defined,

    enmToken_Identifier,
    enmToken_ConstInteger,

    enmToken_OpenParen,
    enmToken_CloseParen,
    enmToken_Comma,
    enmToken_Colon,

    enmToken_Plus,
    enmToken_Minus,
    enmToken_Times,
    enmToken_Divide,
    enmToken_Mod,

    enmToken_Equal,
    enmToken_Inequal,
    enmToken_Less,
    enmToken_LessEqual,
    enmToken_Greater,
    enmToken_GreaterEqual,

    enmToken_BitwiseAnd,
    enmToken_BitwiseOr,
    enmToken_BitwiseNot,
    enmToken_BitwiseXor,

    enmToken_LogicalAnd,
    enmToken_LogicalOr,
    enmToken_LogicalNot,

    enmToken_LineWrap,
    enmToken_NewLine,
    enmToken_EndOfFile,
};

class Token
{
public:
    Token();
    void print();

public:
    enmToken _token_type;
    std::string _token_string;
};

typedef std::shared_ptr<Token> TokenPtr;

class Macro
{
public:
    Macro();
    int find_parameter(const char *parameter);
    void print();

public:
    std::string _name;
    std::vector<std::string> _parameters;
    std::vector<TokenPtr> _tokens;
};

typedef std::shared_ptr<Macro> MacroPtr;

class MacroParameter
{
public:
    MacroParameter();

public:
    std::vector<TokenPtr> _tokens;
};

typedef std::shared_ptr<MacroParameter> MacroParameterPtr;

class Scanner {
public:
    Scanner();
    ~Scanner();

public:
    void open_source(const char *source);
    void preprocess();
    void print_token_list();
    void print_macro_list();

private:
    void init_keywords();
    void error(const char *info);

private:
    void get_char();
    void reset_current_token_string();
    void save_current_char();
    bool char_is_identifier(char ch);
    TokenPtr get_current_token();
    void add_current_token();

private:
    bool token_queue_empty();
    void enqueue_token(TokenPtr token);
    TokenPtr dequeue_token();
    void insert_tokens(std::vector<TokenPtr> &tokens_to_insert);

private:
    void scan_token();
    void match_token(enmToken token_expect);

private:
    void init_system_macros();
    TokenPtr new_integer_token(int integer);
    MacroPtr add_system_macro(const char *macro_name, TokenPtr token);
    MacroPtr find_macro(const char *macro_name);
    void undefine_macro(const char *macro_name);
    void apply_macro(MacroPtr macro);

private:
    int preprocess_defined();
    int preprocess_primitive_expression();
    bool is_unary_operator();
    int preprocess_unary_expression();
    int preprocess_multicative_expression();
    int preprocess_additive_expression();
    int preprocess_relational_expression();
    int preprocess_equality_expression();    
    int preprocess_bitwise_and_expression();
    int preprocess_bitwise_xor_expression();
    int preprocess_bitwise_or_expression();
    int preprocess_logical_and_expression();
    int preprocess_logical_or_expression();
    int preprocess_expression();

private:
    void skip_macro_block();
    bool preprocess_condition();
    void preprocess_define();
    void preprocess_undef();
    void preprocess_if_common(bool condition);
    void preprocess_if_def(bool ifndef);
    void preprocess_if();
    void preprocess_recursive(bool in_macro_block);

private:
    const char *_source;

private:
    char _current_char = '\0';
    enmToken _current_token_type = enmToken_Unknown;
    char _current_token_string[10000];
    int _current_token_string_index = 0;

private:
    std::map<std::string, enmToken> _keywords;
    std::map<std::string, MacroPtr> _macros;

private:
    std::vector<TokenPtr> _token_list;
    std::vector<TokenPtr> _token_queue;
};