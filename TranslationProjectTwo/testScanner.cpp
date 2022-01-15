/*
Author: Tyler Ziggas
Date: October 20, 2021
Project Description: This project either takes in a file and reads from it or takes user input if no file is given and then stores it into a temporary file to read from.
        This file is then scanner over with a custom set of rules acting as a new programming langauge, looking for tokens such as indentifiers, integers,
        keywords, and operators.  It will figure out what token was given and if it is significant (something like comments or whitespace will not show) then it will
        display that to the screen with the token and where it was found.  If an error is given the program will tell you why the error happened and then exit as the scanner
        will not be able to accurately judge what token was given considering it does not apply to rules established.
*/

#include "token.h"
#include "scanner.h"
#include "testScanner.h"

int testScanner(string fileName) {
    Scanner scanner; // Create our scanner
    ifstream file(fileName.c_str());
    unsigned int currentLine = 1; // Our first line is the first one
    string input;
    Token token;

    if (file.is_open()) { // If we are able to ope nthe file
        while (getline(file, input)) { // Get our current line and read it
            while (scanner.scan(currentLine, input, token) == 0) {
                scanner.printToken(token);

            }
            currentLine++; // Increment our line on the next one
        }

    }
    else { // In case the file could not be opened
        cout << "SCANNER: File cannot be opened!" << endl;
        return EXIT_FAILURE;
    }

    if (file.eof()) { // If we reached the end of the file
        if (currentLine > 1) { // Undo our recent increment of current line
            currentLine--;
        }

        token.tokenID = eofTk; // Establish our token for the EOF
        token.tokenInstance = "EOF";
        token.lineNumber = currentLine;
        scanner.printToken(token);
    }

    file.close(); // Close our file an exit
    return EXIT_SUCCESS;
}

