#include "scanner.h"

int Scanner::scan(string& input, Token& token) {
    token.lineNumber = currentLineNumber;
    int currentState = 0;
    int nextState = 0;
    int nextCategory = 0;
    string readValue;
    char nextCharacter;

    while (currentPointerPlacement <= input.length()) {
        if (currentPointerPlacement < input.length()) {
            if (currentPointerPlacement == (input.length() - 1)) {
                nextCharacter = checkComment(currentLineNumber, input.at(currentPointerPlacement), ' ');
            }
            else {
                nextCharacter = checkComment(currentLineNumber, input.at(currentPointerPlacement), input.at(currentPointerPlacement + 1));
            }
        } else {
            nextCharacter = SCANNER_DELIMITER;
        }

        nextCategory = getCategory(nextCharacter);
        nextState = FSA_TABLE[currentState][nextCategory];

        if (nextState < 0) {
            getError(currentLineNumber, nextState, nextCharacter);
            exit(EXIT_FAILURE);

        } else if (nextState > FINAL_STATE) {
            token.tokenInstance = readValue;

            switch (nextState) {
                case FINAL_ID:
                    if (getKeyword(token) != -1) {
                        token.tokenID = keywordTk;
                        token.tokenData.assign(readValue);
                        token.tokenInstance.assign(readValue);

                    } else {
                        token.tokenID = idTk;
                        token.tokenData.assign(readValue);
                        token.tokenInstance.assign("idToken " + readValue);
                    }
                    break;

                case FINAL_INT:
                    token.tokenID = intTk;
                    token.tokenData.assign(readValue);
                    token.tokenInstance.assign("intToken " + readValue);
                    break;

                case FINAL_OP:
                    token.tokenID = opTk;

                    if (currentPointerPlacement < input.length()) {
                        string stringOperator = readValue + input.at(currentPointerPlacement);
                            if (isStringOperator(stringOperator) != -1) {
                                readValue = stringOperator;
                                currentPointerPlacement++;
                            }
                    }

                    token.tokenData.assign(readValue);
                    token.tokenInstance.assign(readValue);
                    getOperator(token);
                    token.tokenData.assign(readValue);
                    token.tokenInstance.assign(readValue);
                    break;
            }

            if (isCommenting) {
                currentPointerPlacement++;
            }
            return 0;
        }


        currentState = nextState;
        currentPointerPlacement++;

        if (!isspace(nextCharacter)) {
            readValue.push_back(nextCharacter);
        }
    }

    currentPointerPlacement = 0;
    currentLineNumber++;
    return -1;
}

void Scanner::filterScan(string inputFile, string outputFile) {
    ifstream sourceFile(inputFile.c_str());
    ofstream newOutputFile(outputFile.c_str());
    Token token;

    if (sourceFile.is_open()) {
        string input;
        while (getline(sourceFile, input)) {

            string scan_input;
            while (scan(input, token) == 0) {
                scan_input.append(token.tokenData);

                if (currentPointerPlacement < input.length()) {
                    scan_input.append(" ");
                }
            }

            newOutputFile << scan_input << endl;
        }
    }
    else {
        cout << "[ERROR] Can't open file!" << endl;
        exit(EXIT_FAILURE);
    }

    sourceFile.close();
    newOutputFile.close();
    isCommenting = false;
    currentPointerPlacement = 0;
    currentLineNumber = 1;
}

char Scanner::checkComment(int currentLine, char currentCharacter, char nextCharacter) {
    if (currentCharacter == COMMENT_DELIMITER && nextCharacter == COMMENT_DELIMITER) {
        isCommenting = !isCommenting;
        commentRecentChange = true;

        if (isCommenting) {
            ostringstream temp;
            temp << currentLine << ":" << currentPointerPlacement;
            lastCommentPosition = temp.str();
        }
        return SCANNER_DELIMITER;

    } else if ((currentCharacter == COMMENT_DELIMITER) && (nextCharacter != COMMENT_DELIMITER) && (isCommenting == false) && (commentRecentChange == false)) {
        getError(currentLine, ERROR_COM, COMMENT_DELIMITER);
        exit(EXIT_FAILURE);
    }

    if ((isCommenting == true) || (commentRecentChange == true)) {
        commentRecentChange = false;
        return SCANNER_DELIMITER;
    } else {
        return currentCharacter;
    }
}

int Scanner::getCategory(char ch) {
    if (isupper(ch)) {
        return 0;
    } else if (islower(ch)) {
        return 1;
    } else if (ch == '$') {
        return 2;
    } else if (isdigit(ch)) {
        return 3;
    } else if (isCharOperator(ch) != -1) {
        return 4;
    } else if (isspace(ch)) {
        return 5;
    } else {
        return 7;
    }
}

string Scanner::getScannerPosition() {
    ostringstream temp;
    temp << "(L" << currentLineNumber << ":C" << currentPointerPlacement << ")";
    return temp.str();
}

void Scanner::invokeEOF(Token& tk) {
    tk.lineNumber = (currentLineNumber > 1) ? currentLineNumber - 1 : currentLineNumber;
    tk.tokenID = eofTk;
    tk.tokenInstance = "EOF";
}

void Scanner::getError(int currentLine, int state, char currentCharacter) {
    cout << "SCANNER ERROR: at (L" << currentLine << ":C" << currentPointerPlacement << ") -> " << currentCharacter << ": ";

    if (state == ERROR_ID) {
        cout << "all identifiers must begin with a lowercase letter or dollar sign ($)." << endl;
        cout << "SCANNER ERROR CODE: " << ERROR_ID << endl;

    } else if (state == ERROR_INT) {
        cout << "all integer tokens can only have digits." << endl;
        cout << "SCANNER ERROR CODE: " << ERROR_INT << endl;

    } else if (state == ERROR_COM) {
        cout << "comments must be used with &&, not a single &." << endl;
        cout << "SCANNER ERROR CODE: " << ERROR_COM << endl;

    } else if (state == ERROR_UNK) {
        cout << "unknown token not allowed." << endl;
        cout << "SCANNER ERROR CODE: " << ERROR_UNK << endl;

    }
}