//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Preprocessor
// Author：AndersJiang(蒋政)
//---------------------------------------------------------------------------------------------------------

#include "Scanner.h"

Token::Token() {
    _token_type = enmToken_Unknown;
}

void Token::print() {
    switch (_token_type) {
        case enmToken_Unknown:
            printf("Unknown");
            break;

        case enmToken_SharpDefine:
        case enmToken_SharpUnDef:
        case enmToken_SharpIfDef:
        case enmToken_SharpIfNDef:
        case enmToken_SharpIf:
        case enmToken_SharpElIf:
        case enmToken_SharpElse:
        case enmToken_SharpEndIf:
        case enmToken_SharpError:
        case enmToken_SharpPragma:
        case enmToken_SharpExtension:
        case enmToken_SharpVersion:
        case enmToken_SharpLine:
        case enmToken_SharpInclude:
        case enmToken_Defined:
            printf("Keyword: %s", _token_string.c_str());
            break;

        case enmToken_Identifier:
            printf("Identifier: %s", _token_string.c_str());
            break;
        case enmToken_ConstInteger:
            printf("ConstInteger: %s", _token_string.c_str());
            break;

        case enmToken_OpenParen:
            printf("OpenParen: (");
            break;
        case enmToken_CloseParen:
            printf("CloseParen: )");
            break;
        case enmToken_Comma:
            printf("Comma: ,");
            break;
        case enmToken_Colon:
            printf("Colon: :");
            break;

        case enmToken_Plus:
            printf("Plus: +");
            break;
        case enmToken_Minus:
            printf("Minus: -");
            break;
        case enmToken_Times:
            printf("Times: *");
            break;
        case enmToken_Divide:
            printf("Divide: /");
            break;
        case enmToken_Mod:
            printf("Mod: %%");
            break;

        case enmToken_Equal:
            printf("Equal: ==");
            break;
        case enmToken_Inequal:
            printf("Inequal: !=");
            break;
        case enmToken_Less:
            printf("Less: <");
            break;
        case enmToken_LessEqual:
            printf("LessEqual: <=");
            break;
        case enmToken_Greater:
            printf("Greater: >");
            break;
        case enmToken_GreaterEqual:
            printf("GreaterEqual: >=");
            break;

        case enmToken_BitwiseAnd:
            printf("BitwiseAnd: &");
            break;
        case enmToken_BitwiseOr:
            printf("BitwiseOr: |");
            break;
        case enmToken_BitwiseNot:
            printf("BitwiseNot: ~");
            break;
        case enmToken_BitwiseXor:
            printf("BitwiseXor: ^");
            break;

        case enmToken_LogicalAnd:
            printf("LogicalAnd: &&");
            break;
        case enmToken_LogicalOr:
            printf("LogicalOr: ||");
            break;
        case enmToken_LogicalNot:
            printf("LogicalNot: !");
            break;

        case enmToken_LineWrap:
            printf("LineWrap: \\\n");
            break;
        case enmToken_NewLine:
            printf("NewLine");
            break;            
        case enmToken_EndOfFile:
            printf("EndOfFile");
            break;
    }
}

Macro::Macro() {

}

int Macro::find_parameter(const char *parameter) {
    int count = (int)_parameters.size();
    for (int i = 0; i < count; i++) {
        if (strcmp(parameter, _parameters[i].c_str()) == 0) {
            return i;
        }
    }
    return -1;
}

void Macro::print() {
    printf("macro: %s", _name.c_str());
    int count = (int)_parameters.size();
    if (count) {
        printf("(");
        for (int i = 0; i < count; i++) {
            std::string &parameter = _parameters[i];
            printf("%s", parameter.c_str());
            if (i != count - 1) {
                printf(", ");
            }
        }
        printf(")");
    }
    count = (int)_tokens.size();
    if (count) {
        printf(" = ");
        for (int i = 0; i < count; i++) {
            TokenPtr token = _tokens[i];
            token->print();
            if (i != count - 1) {
                printf(", ");
            }
        }
    }
}

MacroParameter::MacroParameter() {
}

Scanner::Scanner() {
    init_keywords();
    init_system_macros();
}

Scanner::~Scanner() {
}

void Scanner::open_source(const char *source) {
    _source = source;
}

void Scanner::init_keywords() {
    _keywords["#define"] = enmToken_SharpDefine;
    _keywords["#undef"] = enmToken_SharpUnDef;
    _keywords["#ifdef"] = enmToken_SharpIfDef;
    _keywords["#ifndef"] = enmToken_SharpIfNDef;
    _keywords["#if"] = enmToken_SharpIf;
    _keywords["#elif"] = enmToken_SharpElIf;
    _keywords["#else"] = enmToken_SharpElse;
    _keywords["#endif"] = enmToken_SharpEndIf;
    _keywords["#error"] = enmToken_SharpError;
    _keywords["#pragma"] = enmToken_SharpPragma;
    _keywords["#extension"] = enmToken_SharpExtension;
    _keywords["#version"] = enmToken_SharpVersion;
    _keywords["#line"] = enmToken_SharpLine;
    _keywords["#include"] = enmToken_SharpInclude;

    _keywords["defined"] = enmToken_Defined;
}

void Scanner::error(const char *info) {
    printf("%s\n", info);
    int i = 0;
    i = i / i;
}

void Scanner::get_char() {
    _current_char = *_source;
    if (_current_char != 0) {
        _source++;
    }
}

void Scanner::reset_current_token_string() {
    _current_token_string[0] = 0;
    _current_token_string_index = 0;
}

void Scanner::save_current_char() {
    _current_token_string[_current_token_string_index++] = _current_char;
    _current_token_string[_current_token_string_index] = 0;
}

bool Scanner::char_is_identifier(char ch) {
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

TokenPtr Scanner::get_current_token() {
    TokenPtr token = TokenPtr(new Token());
    token->_token_type = _current_token_type;
    token->_token_string = _current_token_string;
    return token;
}

void Scanner::add_current_token() {
    TokenPtr token = get_current_token();
    _token_list.push_back(token);
}

bool Scanner::token_queue_empty() {
    return _token_queue.size() == 0;
}

void Scanner::enqueue_token(TokenPtr token) {
    _token_queue.push_back(token);
}

TokenPtr Scanner::dequeue_token() {
    int count = (int)_token_queue.size();
    if (count == 0) {
        return nullptr;
    }
    TokenPtr token = _token_queue[0];
    _token_queue.erase(_token_queue.begin());
    return token;
}

void Scanner::insert_tokens(std::vector<TokenPtr> &tokens_to_insert) {
    int count_new = (int)tokens_to_insert.size();
    if (count_new == 0) {
        return;
    }
    int count_old = (int)_token_queue.size();
    _token_queue.resize(count_old + count_new);
    int i;
    for (i = count_old - 1; i >= 0; i--) {
        _token_queue[count_new + i] = _token_queue[i];
    }
    for (i = 0; i < count_new; i++) {
        _token_queue[i] = tokens_to_insert[i];
    }
}

void Scanner::scan_token() {
    if (!token_queue_empty()) {
        TokenPtr token = dequeue_token();
        _current_token_type = token->_token_type;
        strcpy(_current_token_string, token->_token_string.c_str());
        return;
    }
    _current_token_type = enmToken_Unknown;
    reset_current_token_string();
redo:
    switch (_current_char) {
        case '\0':
            _current_token_type = enmToken_EndOfFile;
            break;
        case ' ':
        case '\t':
        case '\r':
            get_char();
            goto redo;
        case '\\':
            get_char();
            if (_current_char == '\r') {
                get_char();
            }
            if (_current_char != '\n') {
                error("反斜杆\\后应该接换行字符\\n");
            }
            else {
                _current_token_type = enmToken_LineWrap;
            }
            break;
        case '\n':
            get_char();
            _current_token_type = enmToken_NewLine;
            break;
        case '(':
            get_char();
            _current_token_type = enmToken_OpenParen;
            break;
        case ')':
            get_char();
            _current_token_type = enmToken_CloseParen;
            break;
        case ',':
            get_char();
            _current_token_type = enmToken_Comma;
            break;
        case ':':
            get_char();
            _current_token_type = enmToken_Colon;
            break;
        case '+':
            get_char();
            _current_token_type = enmToken_Plus;
            break;
        case '-':
            get_char();
            _current_token_type = enmToken_Minus;
            break;
        case '*':
            get_char();
            _current_token_type = enmToken_Times;
            break;
        case '/':
            get_char();
            _current_token_type = enmToken_Divide;
            break;
        case '%':
            get_char();
            _current_token_type = enmToken_Mod;
            break;            
        case '=':
            get_char();
            if (_current_char == '=') {
                get_char();
                _current_token_type = enmToken_Equal;
            }
            else {
                error("词法错误");
            }
            break;
        case '!':
            get_char();
            if (_current_char == '=') {
                get_char();
                _current_token_type = enmToken_Inequal;
            }
            else {
                _current_token_type = enmToken_LogicalNot;
            }
            break;
        case '<':
            get_char();
            if (_current_char == '=') {
                get_char();
                _current_token_type = enmToken_LessEqual;
            }
            else {
                _current_token_type = enmToken_Less;
            }
            break;
        case '>':
            get_char();
            if (_current_char == '=') {
                get_char();
                _current_token_type = enmToken_GreaterEqual;
            }
            else {
                _current_token_type = enmToken_Greater;
            }
            break;
        case '&':
            get_char();
            if (_current_char == '&') {
                get_char();
                _current_token_type = enmToken_LogicalAnd;
            }
            else {
                _current_token_type = enmToken_BitwiseAnd;
            }
            break;
        case '|':
            get_char();
            if (_current_char == '|') {
                get_char();
                _current_token_type = enmToken_LogicalOr;
            }
            else {
                _current_token_type = enmToken_BitwiseOr;
            }
            break;
        case '~':
            get_char();
            _current_token_type = enmToken_BitwiseNot;
            break;
        case '^':
            get_char();
            _current_token_type = enmToken_BitwiseXor;
            break;
        case '#':
            save_current_char();
            get_char();
            while (_current_char == ' ' ||
                   _current_char == '\t') {
                get_char();
            }
            if (!isalpha(_current_char)) {
                error("#字符之后应该跟预处理指令");
            }
            goto scan_id;
        case '_':
        case 'a': case 'b': case 'c': case 'd':
        case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D':
        case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z': {
        scan_id:
                save_current_char();
                get_char();
                while (char_is_identifier(_current_char)) {
                    save_current_char();
                    get_char();
                }
                auto it = _keywords.find(_current_token_string);
                if (it != _keywords.end()) {
                    _current_token_type = it->second;
                }
                else {
                    _current_token_type = enmToken_Identifier;
                }
            }
            break;
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9': {
                save_current_char();
                get_char();
                while (isdigit(_current_char)) {
                    save_current_char();
                    get_char();
                }
                _current_token_type = enmToken_ConstInteger;
            }
            break;
        default: {
                char buf[1000];
                sprintf(buf, "无法解析的字符: 0x%02X=%c", _current_char, _current_char);
                error(buf);
            }
            break;
    }
}

void Scanner::match_token(enmToken token_expect) {
    if (_current_token_type == token_expect) {
        scan_token();
    }
    else {
        char buf[1000];
        sprintf(buf, "当前标记%d与期望不符,期望%d", _current_token_type, token_expect);
        error(buf);
    }
}

void Scanner::init_system_macros() {
    add_system_macro("__LINE__", new_integer_token(1));
    add_system_macro("__FILE__", new_integer_token(1));
    add_system_macro("__VERSION__", new_integer_token(100));
}

TokenPtr Scanner::new_integer_token(int integer) {
    char buffer[100];
    itoa(integer, buffer, 10);
    TokenPtr token = TokenPtr(new Token());
    token->_token_type = enmToken_ConstInteger;
    token->_token_string = buffer;
    return token;
}

MacroPtr Scanner::add_system_macro(const char *macro_name, TokenPtr token) {
    MacroPtr macro = MacroPtr(new Macro());
    macro->_name = macro_name;
    macro->_tokens.push_back(token);
    _macros[macro->_name] = macro;
    return macro;
}

MacroPtr Scanner::find_macro(const char *macro_name) {
    auto it = _macros.find(macro_name);
    if (it != _macros.end()) {
        return it->second;
    }
    return nullptr;
}

void Scanner::undefine_macro(const char *macro_name) {
    _macros.erase(macro_name);
}

void Scanner::apply_macro(MacroPtr macro) {
    std::vector<TokenPtr> tokens_to_insert;
    int count = (int)macro->_parameters.size();
    if (count > 0) {
        std::vector<MacroParameterPtr> real_parameters;
        match_token(enmToken_Identifier);
        match_token(enmToken_OpenParen);
        for (int i = 0; i < count; i++) {
            MacroParameterPtr macro_parameter = MacroParameterPtr(new MacroParameter());
            int paren_level = 0;
            while (true) {
                if (_current_token_type == enmToken_OpenParen) {
                    paren_level++;
                }
                if(paren_level > 0) {
                    if (_current_token_type == enmToken_CloseParen) {
                        paren_level--;
                    }
                }
                if (paren_level == 0) {
                    if (i < count - 1) {
                        if (_current_token_type == enmToken_Comma) {
                            break;
                        }
                    }
                    else {
                        if (_current_token_type == enmToken_CloseParen) {
                            break;
                        }
                    }
                }
                TokenPtr token = get_current_token();
                macro_parameter->_tokens.push_back(token);
                match_token(_current_token_type);
            }
            real_parameters.push_back(macro_parameter);
            if (i != count - 1) {
                match_token(enmToken_Comma);
            }
        }
        //match_token(enmToken_CloseParen);  //这里不能有
        for (auto token : macro->_tokens) {
            int parameter_index = macro->find_parameter(token->_token_string.c_str());
            if (parameter_index != -1) {
                MacroParameterPtr macro_parameter = real_parameters[parameter_index];
                for (auto token1 : macro_parameter->_tokens) {
                    tokens_to_insert.push_back(token1);
                }
            }
            else {
                tokens_to_insert.push_back(token);
            }
        }
    }
    else {
        for (auto token : macro->_tokens) {
            tokens_to_insert.push_back(token);
        }
    }
    insert_tokens(tokens_to_insert);
}

int Scanner::preprocess_defined() {
    int condition = 0;
    match_token(enmToken_Defined);
    bool match_paren = false;
    if (_current_token_type == enmToken_OpenParen) {
        match_paren = true;
    }
    if (match_paren) {
        match_token(enmToken_OpenParen);
    }
    if (_current_token_type != enmToken_Identifier) {
        error("应对defined表达式指定标识符");
    }
    MacroPtr macro = find_macro(_current_token_string);
    if (macro) {
        condition = 1;
    }
    match_token(enmToken_Identifier);
    if (match_paren) {
        match_token(enmToken_CloseParen);
    }
    return condition;
}

int Scanner::preprocess_primitive_expression() {
    int val = 0;
    switch (_current_token_type) {
        case enmToken_Defined:
            val = preprocess_defined();
            break;
        case enmToken_Identifier: {
                MacroPtr macro = find_macro(_current_token_string);
                if (macro) {
                    apply_macro(macro);
                    scan_token();
                    val = preprocess_expression();
                }
                else {
                    error("未定义或者非常量的标识符");
                }
            }
            break;
        case enmToken_ConstInteger:
            val = atoi(_current_token_string);
            match_token(enmToken_ConstInteger);
            break;
        case enmToken_OpenParen:
            match_token(enmToken_OpenParen);
            val = preprocess_expression();
            match_token(enmToken_CloseParen);
            break;
        default:
            error("语法错误");
            match_token(_current_token_type);
            break;
    }
    return val;
}

bool Scanner::is_unary_operator() {
    switch (_current_token_type) {
        case enmToken_Plus:
        case enmToken_Minus:
        case enmToken_LogicalNot:
        case enmToken_BitwiseNot:
            return true;
        default:
            return false;
    }
}

int Scanner::preprocess_unary_expression() {
    int val = 0;
    if (is_unary_operator()) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        val = preprocess_unary_expression();
        switch (token_type) {
            case enmToken_Plus:
                val = val;
                break;
            case enmToken_Minus:
                val = -val;
                break;
            case enmToken_LogicalNot:
                val = val ? 0 : 1;
                break;
            case enmToken_BitwiseNot:
                val = ~val;
                break;
        }
    }
    else {
        val = preprocess_primitive_expression();
    }
    return val;
}

int Scanner::preprocess_multicative_expression() {
    int val = preprocess_unary_expression();
    while (_current_token_type == enmToken_Times ||
           _current_token_type == enmToken_Divide ||
           _current_token_type == enmToken_Mod) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_unary_expression();
        if (token_type == enmToken_Times) {
            val = val * val2;
        }
        else if (token_type == enmToken_Divide) {
            if (val2 == 0) {
                error("常量求值时发生除0错误");
            }
            val = val / val2;
        }
        else if (token_type == enmToken_Mod) {
            if (val2 == 0) {
                error("常量求值时发生除0错误");
            }
            val = val % val2;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_additive_expression() {
    int val = preprocess_multicative_expression();
    while (_current_token_type == enmToken_Plus ||
           _current_token_type == enmToken_Minus) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_multicative_expression();
        if (token_type == enmToken_Plus) {
            val = val + val2;
        }
        else if (token_type == enmToken_Minus) {
            val = val - val2;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_relational_expression() {
    int val = preprocess_additive_expression();
    while (_current_token_type == enmToken_Less ||
           _current_token_type == enmToken_LessEqual ||
           _current_token_type == enmToken_Greater ||
           _current_token_type == enmToken_GreaterEqual) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_additive_expression();
        if (token_type == enmToken_Less) {
            val = (val < val2) ? 1 : 0;
        }
        else if (token_type == enmToken_LessEqual) {
            val = (val <= val2) ? 1 : 0;
        }
        else if (token_type == enmToken_Greater) {
            val = (val > val2) ? 1 : 0;
        }
        else if (token_type == enmToken_GreaterEqual) {
            val = (val >= val2) ? 1 : 0;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_equality_expression() {
    int val = preprocess_relational_expression();
    while (_current_token_type == enmToken_Equal ||
           _current_token_type == enmToken_Inequal) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_relational_expression();
        if (token_type == enmToken_Equal) {
            val = (val == val2) ? 1 : 0;
        }
        else if (token_type == enmToken_Inequal) {
            val = (val != val2) ? 1 : 0;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_bitwise_and_expression() {
    int val = preprocess_equality_expression();
    while (_current_token_type == enmToken_BitwiseAnd) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_equality_expression();
        if (token_type == enmToken_BitwiseAnd) {
            val = val & val2;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_bitwise_xor_expression() {
    int val = preprocess_bitwise_and_expression();
    while (_current_token_type == enmToken_BitwiseXor) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_bitwise_and_expression();
        if (token_type == enmToken_BitwiseXor) {
            val = val ^ val2;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_bitwise_or_expression() {
    int val = preprocess_bitwise_xor_expression();
    while (_current_token_type == enmToken_BitwiseOr) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_bitwise_xor_expression();
        if (token_type == enmToken_BitwiseOr) {
            val = val | val2;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_logical_and_expression() {
    int val = preprocess_bitwise_or_expression();
    while (_current_token_type == enmToken_LogicalAnd) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_bitwise_or_expression();
        if (token_type == enmToken_LogicalAnd) {
            val = (val && val2) ? 1 : 0;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_logical_or_expression() {
    int val = preprocess_logical_and_expression();
    while (_current_token_type == enmToken_LogicalOr) {
        enmToken token_type = _current_token_type;
        match_token(_current_token_type);
        int val2 = preprocess_logical_and_expression();
        if (token_type == enmToken_LogicalOr) {
            val = (val || val2) ? 1 : 0;
        }
        else {
            error("逻辑错误(不可达代码)");
        }
    }
    return val;
}

int Scanner::preprocess_expression() {
    return preprocess_logical_or_expression();
}

void Scanner::skip_macro_block() {
    int macro_depth = 0;
    while (true) {
        if (_current_token_type == enmToken_EndOfFile) {
            error("遇到非法的文件结束");
        }
        else {
            if (_current_token_type == enmToken_SharpIfDef ||
                _current_token_type == enmToken_SharpIfNDef ||
                _current_token_type == enmToken_SharpIf) {
                macro_depth++;
            }
            else {
                if (macro_depth > 0) {
                    if (_current_token_type == enmToken_SharpEndIf) {
                        macro_depth--;
                    }
                }
                else if (macro_depth == 0) {
                    if (_current_token_type == enmToken_SharpElse ||
                        _current_token_type == enmToken_SharpElIf ||
                        _current_token_type == enmToken_SharpEndIf) {
                        return;
                    }
                }
            }
        }
        scan_token();
    }
}

bool Scanner::preprocess_condition() {
    return (preprocess_expression() != 0);
}

void Scanner::preprocess_define() {
    match_token(enmToken_SharpDefine);
    MacroPtr macro = MacroPtr(new Macro());
    if (_current_token_type == enmToken_Identifier) {
        if (strncmp(_current_token_string, "__", 2) == 0 ||
            strncmp(_current_token_string, "GL_", 3) == 0) {
            error("__及GL_开头的宏名称为保留的宏名称");
        }
        macro->_name = _current_token_string;
    }
    match_token(enmToken_Identifier);
    if (_current_token_type == enmToken_OpenParen) {
        match_token(enmToken_OpenParen);
        if (_current_token_type != enmToken_CloseParen) {
            if (_current_token_type == enmToken_Identifier) {
                macro->_parameters.push_back(_current_token_string);
            }
            match_token(enmToken_Identifier);
            while (_current_token_type == enmToken_Comma) {
                match_token(enmToken_Comma);
                if (_current_token_type == enmToken_Identifier) {
                    macro->_parameters.push_back(_current_token_string);
                }
                match_token(enmToken_Identifier);
            }
        }
        match_token(enmToken_CloseParen);
    }
    while (true) {
        if (_current_token_type == enmToken_LineWrap) {
            match_token(enmToken_LineWrap);
        }
        else if (_current_token_type == enmToken_NewLine) {
            break;
        }
        else {
            TokenPtr token = get_current_token();
            macro->_tokens.push_back(token);
            scan_token();
        }
    }
    _macros[macro->_name] = macro;
}

void Scanner::preprocess_undef() {
    match_token(enmToken_SharpUnDef);
    if (_current_token_type == enmToken_Identifier) {
        undefine_macro(_current_token_string);
    }
    match_token(enmToken_Identifier);
}

void Scanner::preprocess_if_common(bool condition) {
    bool matched = false;
    if (condition) {
        preprocess_recursive(true);
        matched = true;
    }
    else {
        skip_macro_block();
    }
    while (_current_token_type == enmToken_SharpElIf) {
        match_token(enmToken_SharpElIf);
        bool condition1 = preprocess_condition();
        if (!matched && condition1) {
            preprocess_recursive(true);
            matched = true;
        }
        else {
            skip_macro_block();
        }
    }
    if (_current_token_type == enmToken_SharpElse) {
        match_token(enmToken_SharpElse);
        if (!matched) {
            preprocess_recursive(true);
            matched = true;
        }
        else {
            skip_macro_block();
        }
    }
    match_token(enmToken_SharpEndIf);
}

void Scanner::preprocess_if_def(bool ifndef) {
    match_token(_current_token_type);
    if (_current_token_type != enmToken_Identifier) {
        error("#ifdef及#ifndef之后应该接标识符");
    }
    MacroPtr macro = find_macro(_current_token_string);
    match_token(enmToken_Identifier);
    bool condition = (macro != nullptr) ^ ifndef;
    preprocess_if_common(condition);
}

void Scanner::preprocess_if() {
    match_token(_current_token_type);
    bool condition = preprocess_condition();
    preprocess_if_common(condition);
}

void Scanner::preprocess_recursive(bool in_macro_block) {
    while (true) {
        switch (_current_token_type) {
            case enmToken_SharpDefine: {
                    preprocess_define();
                }
                break;
            case enmToken_SharpUnDef: {
                    preprocess_undef();
                }
                break;
            case enmToken_SharpIfDef:
            case enmToken_SharpIfNDef: {
                    bool ifndef = (_current_token_type == enmToken_SharpIfNDef);
                    preprocess_if_def(ifndef);
                }
                break;
            case enmToken_SharpIf: {
                    preprocess_if();
                }
                break;
            case enmToken_SharpElse:
            case enmToken_SharpElIf:
            case enmToken_SharpEndIf:
                if (in_macro_block) {
                    return;
                }
                else {
                    error("检测到无法匹配的预处理指令");
                }
                break;
            case enmToken_SharpError:
            case enmToken_SharpPragma:
            case enmToken_SharpExtension:
            case enmToken_SharpVersion:
            case enmToken_SharpLine:
                match_token(_current_token_type);
                while (_current_token_type != enmToken_NewLine &&
                       _current_token_type != enmToken_EndOfFile) {
                    scan_token();
                }
                break;
            case enmToken_EndOfFile:
                if (in_macro_block) {
                    error("遇到非法的文件结束");
                }
                else {
                    return;
                }
                break;
            default:
                if (_current_token_type != enmToken_NewLine) {
                    if (_current_token_type == enmToken_Identifier) {
                        MacroPtr macro = find_macro(_current_token_string);
                        if (macro) {
                            apply_macro(macro);
                        }
                        else {
                            add_current_token();
                        }
                    }
                    else {
                        add_current_token();
                    }
                }
                scan_token();
                break;
        }
    }
}

void Scanner::preprocess() {
    get_char();
    scan_token();
    preprocess_recursive(false);
}

void Scanner::print_token_list() {
    for (auto & token : _token_list) {
        token->print();
        printf("\n");
    }
}

void Scanner::print_macro_list() {
    for (auto & macro : _macros) {
        macro.second->print();
        printf("\n");
    }
}