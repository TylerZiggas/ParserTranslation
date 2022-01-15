#pragma once

#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

enum tokenIDEnum { idTk, keywordTk, intTk, opTk, eofTk };

typedef struct token {
    tokenIDEnum tokenID;
    string tokenInstance;
    string tokenData;
    int lineNumber = 0;
} Token;

class ScannerLanguage {
protected:
    map<string, string> allOperators = {
            {"=", "equalToken"}, {">", "greaterThanToken"}, {"<", "lessThanToken"}, {"==", "equalEqualToken"}, {":", "colonToken"}, {":=", "colonEqualToken"}, {"+", "plusToken"},
            {"-", "minusToken"}, {"*", "multiplyToken"}, {"/", "divideToken"}, {"%", "modulusToken"}, {".", "dotToken"}, {"(", "leftParenToken"}, {")", "rightParenToken"},
            {",", "commaToken"}, {"{", "leftCurlyToken"}, {"}", "rightCurlyToken"}, {";", "semiColonToken"}, {"[", "leftBracketToken"}, {"]", "rightBracketToken"}
    };

    map<string, string> allKeywords = {
            {"start", "startToken"}, {"stop", "stopToken"}, {"loop", "loopToken"}, {"while", "whileToken"}, {"for", "forToken"}, {"label", "labelToken"},
            {"exit", "exitToken"}, {"listen", "listenToken"}, {"talk", "talkToken"}, {"program", "programToken"}, {"if", "ifToken"},
            {"then", "thenToken"}, {"assign", "assignToken"}, {"declare", "declareToken"}, {"jump", "jumpToken"}, {"else", "elseToken"}
    };

    const vector<string> tokenTypes = {
            "Identifier", "Keyword", "Integer", "Operator", "End Of File"
    };

    const vector<string> keywords = {
            "start", "stop", "loop", "while", "for", "label", "exit", "listen", "talk", "program", "if", "then", "assign", "declare", "jump", "else"
    };

    const vector<char> charOperators = {
            '=', '>', '<', ':', '+', '-', '*', '/', '%', '.', '(', ')', ',', '{', '}', ';', '[', ']'
    };

    const vector<string> stringOperators = {
            ":=", "=="
    };

    const char COMMENT_DELIMITER = '&';

public:
    int isCharOperator(char);
    int isStringOperator(string);
    int getOperator(Token&);
    int getKeyword(Token&);
};

#endif
