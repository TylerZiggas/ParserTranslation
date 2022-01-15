#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include "parser.h"
#include "tree.h"
using namespace std;

int main(int argc, char* argv[]) {
    string fileName;
    string outputFileName;
    string userInput;

    switch (argc) {
        case 1: {
            ofstream tempFile;
            fileName = "input.fs";
            tempFile.open(fileName.c_str());

            cout << "SCANNER: Please input tokens and enter an empty line when you are finished." << endl;

            do {
                getline(cin, userInput);
                tempFile << userInput << endl;

            } while (!userInput.empty());

            cout << "SCANNER: Done reading from keyboard..." << endl << endl;
            tempFile.close();
            outputFileName = "output.fs";
            break;
        }

        case 2: {
            cout << "SCANNER: Reading from file." << endl << endl;

            fileName = argv[1];
            size_t fileType = fileName.find(".");

            if (fileType == string::npos) {
                outputFileName = fileName;
                fileName.append(".fs");
            }
            else {
                outputFileName = fileName.substr(0, fileType);

            }
            break;
        }

        default: {
            cout << "SCANNER ERROR: Too many arguments received!" << endl;
            cout << "Use by: " << argv[0] << " [file]" << endl;
            cout << "With [file] is an optional parameter." << endl;
            return EXIT_FAILURE;

        }
    }

    Parser parser(fileName);
    NodeT* root = parser.parse();
    traversePreorder(root);
    
    return 0;
}
