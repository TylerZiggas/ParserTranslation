#include "token.h"

int ScannerLanguage::isCharOperator(char currentCharOperator) {
    for (unsigned int charCounter = 0; charCounter < charOperators.size(); charCounter++) {
        if (currentCharOperator == charOperators[charCounter]) {
            return 1;
        }
    }

    return -1;
}

int ScannerLanguage::isStringOperator(string currentStringOperator) {
    for (unsigned int stringCounter = 0; stringCounter < stringOperators.size(); stringCounter++) {
        if (currentStringOperator.compare(stringOperators[stringCounter]) == 0) {
            return 1;
        }
    }

    return -1;
}

int ScannerLanguage::getOperator(Token& token) {
    for (unsigned int charCounter = 0; charCounter < charOperators.size(); charCounter++) {
        string op(1, charOperators[charCounter]);
        if (token.tokenInstance.compare(op) == 0) {
            token.tokenInstance = allOperators[token.tokenInstance];
            return charCounter;
        }
    }

    for (unsigned int stringCounter = 0; stringCounter < stringOperators.size(); stringCounter++) {
        if (token.tokenInstance.compare(stringOperators[stringCounter]) == 0) {
            token.tokenInstance = allOperators[token.tokenInstance];
            return stringCounter;
        }
    }

    return -1;
}

int ScannerLanguage::getKeyword(Token& token) {
    for (unsigned int keyCounter = 0; keyCounter < keywords.size(); keyCounter++) {
        if (token.tokenInstance.compare(keywords[keyCounter]) == 0) {
            token.tokenInstance = allKeywords[token.tokenInstance];
            return keyCounter;
        }
    }

    return -1;
}
