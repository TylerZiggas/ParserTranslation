#include "parser.h"

Parser::Parser(string fileName) {
	string outputFileName = "parsertemp.dat";
	scanner.filterScan(fileName, outputFileName);

	file.open(outputFileName.c_str());
	if (!file.is_open()) {
		cout << "[ERROR] Can't open file!" << endl;
		exit(EXIT_FAILURE);
	}

	emptyToken.tokenInstance = "EMPTY";
	scannerStatus = 0;
	getline(file, fileInput);
}

NodeT* Parser::parse() {
	NodeT* root = NULL;
	nextScan();
	root = program();

	if (tk.tokenID != eofTk) {
		expectedToken.assign("eofTk");
		parserError();
		exit(EXIT_FAILURE);
	} else {
		cout << "PARSER: No errors found while parsing, displaying tree..." << endl;
		file.close();
		return root;
	}
}

void Parser::nextScan() {
	do {
		if (scannerStatus == -1) {
			if (!getline(file, fileInput)) {
				if (file.eof()) {
					scanner.invokeEOF(tk);
					return;
				}
			}
		}

		scannerStatus = scanner.scan(fileInput, tk);

	} while (scannerStatus == -1);
}

NodeT* Parser::createNode(string labelName) {
	NodeT* node = new NodeT;
	node->label = labelName;
	node->nodeOne = NULL;
	node->nodeTwo = NULL;
	node->nodeThree = NULL;
	node->nodeFour = NULL;
	node->nodeFive = NULL;
	return node;
}

NodeT* Parser::program() {
	NodeT* node = createNode("<program>");

	node->nodeOne = vars();

	if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "programToken")) {
		nextScan();

	} else {
		expectedToken.assign("program");
		parserError();
	}

	node->nodeTwo = block();

	return node;
}

NodeT* Parser::block() {
	NodeT* node = createNode("<block>");

	if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "startToken")) {
		nextScan();

		node->nodeOne = vars();
		node->nodeTwo = stats();

		if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "stopToken")) {
			nextScan();
			return node;

		} else {
			expectedToken.assign("stop");
			parserError();
		}
	} else {
		expectedToken.assign("start");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::vars() {
	NodeT* node = createNode("<vars>");

	if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "declareToken")) {
		nextScan();

		if (tk.tokenID == idTk) {
			node->tokens.push_back(tk);  
			nextScan();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "equalToken")) {
				nextScan();

				if (tk.tokenID == intTk) {
					node->tokens.push_back(tk);
					nextScan();

					if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
						nextScan();

						node->nodeOne = vars();
						return node;

					} else {
						expectedToken.assign(";");
						parserError();
					}
				} else {
					expectedToken.assign("Integer");
					parserError();
				}
			} else {
				expectedToken.assign("=");
				parserError();
			}
		} else {
			expectedToken.assign("Identifier");
			parserError();
		}
	} else {
		node->tokens.push_back(emptyToken);
		return node;
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::expr() {
	NodeT* node = createNode("<expr>");

	node->nodeOne = N();

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "plusToken")) {
		node->tokens.push_back(tk);
		nextScan();

		node->nodeTwo = expr();
	}

	return node;
}

NodeT* Parser::N() {
	NodeT* node = createNode("<N>");

	node->nodeOne = A();

	if (tk.tokenID == opTk) {
		if (allOperators[tk.tokenInstance] == "divideToken") {
			node->tokens.push_back(tk);  
			nextScan();

			node->nodeTwo = N();

		} else if (allOperators[tk.tokenInstance] == "multiplyToken") {
			node->tokens.push_back(tk);
			nextScan();

			node->nodeTwo = N();
		}
	}

	return node;
}

NodeT* Parser::A() {
	NodeT* node = createNode("<A>");

	node->nodeOne = M();

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "minusToken")) {
		node->tokens.push_back(tk);
		nextScan();

		node->nodeTwo = A();
	}

	return node;
}

NodeT* Parser::M() {
	NodeT* node = createNode("<M>");

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "dotToken")) {
		node->tokens.push_back(tk);
		nextScan();

		node->nodeOne = M();
		return node;
	}

	node->nodeOne = R();
	return node;
}

NodeT* Parser::R() {
	NodeT* node = createNode("<R>");

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "leftParenToken")) {
		nextScan();

		node->nodeOne = expr();

		if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "rightParenToken")) {
			nextScan();
			return node;
		} else {
			expectedToken.assign(")");
			parserError();
		}

	} else if (tk.tokenID == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;

	} else if (tk.tokenID == intTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;

	} else {
		expectedToken.assign("( or Identifier or Integer");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::stats() {
	NodeT* node = createNode("<stats>");

	node->nodeOne = stat();
	node->nodeTwo = mStat();

	return node;
}

NodeT* Parser::mStat() {
	NodeT* node = createNode("<mStat>");

	if (((tk.tokenID == keywordTk) && (
			(allKeywords[tk.tokenInstance] == "listenToken") || (allKeywords[tk.tokenInstance] == "talkToken") ||
			(allKeywords[tk.tokenInstance] == "ifToken") || (allKeywords[tk.tokenInstance] == "loopToken") ||
			(allKeywords[tk.tokenInstance] == "labelToken") || (allKeywords[tk.tokenInstance] == "jumpToken") || 
			(allKeywords[tk.tokenInstance] == "assignToken") || (allKeywords[tk.tokenInstance] == "startToken")))) {
		node->nodeOne = stat();
		node->nodeTwo = mStat();
		return node;

	} else {
		node->tokens.push_back(emptyToken);
		return node;
	}
}

NodeT* Parser::stat() {
	NodeT* node = createNode("<stat>");

	if (tk.tokenID == keywordTk && !(allKeywords[tk.tokenInstance] == "startToken")) {
		if (allKeywords[tk.tokenInstance] == "listenToken") {
			nextScan();

			node->nodeOne = in();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
		} else if (allKeywords[tk.tokenInstance] == "talkToken") {
			nextScan();

			node->nodeOne = out();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
		} else if (allKeywords[tk.tokenInstance] == "ifToken") {
			nextScan();

			node->nodeOne = iff();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
			return node;
		} else if (allKeywords[tk.tokenInstance] == "whileToken") {
			nextScan();

			node->nodeOne = loop();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
			return node;
		} else if (allKeywords[tk.tokenInstance] == "labelToken") {
			nextScan();

			node->nodeOne = label();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
			return node;
		} else if (allKeywords[tk.tokenInstance] == "jumpToken") {
			nextScan();

			node->nodeOne = gotoo();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
			return node;
		} else if (allKeywords[tk.tokenInstance] == "assignToken") {
			nextScan();

			node->nodeOne = assign();

			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "semiColonToken")) {
				nextScan();
				return node;

			} else {
				expectedToken.assign(";");
				parserError();
			}
			return node;
		} else {
			expectedToken.assign("listen or talk or if or while or label or jump or start");
			parserError();
		}
	} else if (tk.tokenID == keywordTk && allKeywords[tk.tokenInstance] == "startToken") {
		node->nodeOne = block();
		return node;

	} else {
		expectedToken.assign("listen or talk or if or while or label or jump or start");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::in() {
	NodeT* node = createNode("<in>");

	if (tk.tokenID == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;

	} else {
		expectedToken.assign("Identifier");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::out() {
	NodeT* node = createNode("<out>");

	node->nodeOne = expr();
	return node;
}

NodeT* Parser::iff() {
	NodeT* node = createNode("<if>");

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "leftBracketToken")) {
		nextScan();

		node->nodeOne = expr();
		node->nodeTwo = RO();
		node->nodeThree = expr();

		if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "rightBracketToken")) {
			nextScan();

			if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "thenToken")) {
				nextScan();

				node->nodeFour = stat();

				if ((tk.tokenID == keywordTk) && (allKeywords[tk.tokenInstance] == "elseToken")) {
					nextScan();

					node->nodeFive = stat();
					return node;

				} else {
					return node;
				}
			} else {
				expectedToken.assign("then");
				parserError();
			}
		} else {
			expectedToken.assign("]");
			parserError();
		}
	} else {
		expectedToken.assign("[");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::loop() {
	NodeT* node = createNode("<loop>");

	if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "leftBracketToken")) {
		nextScan();

		node->nodeOne = expr();
		node->nodeTwo = RO();
		node->nodeThree = expr();

		if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "rightBracketToken")) {
			nextScan();

			node->nodeFour = stat();
			return node;

		} else {
			expectedToken.assign("]");
			parserError();
		}
	} else {
		expectedToken.assign("[");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::assign() {
	NodeT* node = createNode("<assign>");

	if (tk.tokenID == idTk) {
		node->tokens.push_back(tk);
		nextScan();

		if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "equalToken")) {
			nextScan();
			node->nodeOne = expr();
			return node;

		} else {
			expectedToken.assign("=");
			parserError();
		}
	} else {
		expectedToken.assign("Identifier");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::RO() {
	NodeT* node = createNode("<RO>");

	if (tk.tokenID == opTk) {
		if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "lessThanToken")) {
			node->tokens.push_back(tk);
			nextScan();

			return node;

		} else if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "greaterThanToken")) {
			node->tokens.push_back(tk);
			nextScan();

			return node;

		} else if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "equalEqualToken")) {
			node->tokens.push_back(tk);
			nextScan();

			return node;

		} else if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "leftCurlyToken")) {
			nextScan();
			
			if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "equalEqualToken")) {
				nextScan();

				if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "rightCurlyToken")) {
					tk.tokenInstance = "{==}";
					node->tokens.push_back(tk);
					nextScan();

					return node;
				} else {
					expectedToken.assign("}");
					parserError();
				}

			} else {
				expectedToken.assign("==");
				parserError();
			}
		} else if ((tk.tokenID == opTk) && (allOperators[tk.tokenInstance] == "modulusToken")) {
			node->tokens.push_back(tk);
			nextScan();
			return node;

		} else {
			expectedToken.assign("> or < or == or {==} or %");
			parserError();
		}
	} else {
		expectedToken.assign("> or < or == or {==} or %");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::label() {
	NodeT* node = createNode("<label>");

	if (tk.tokenID == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;

	} else {
		expectedToken.assign("Identifier");
		parserError();
	}

	exit(EXIT_FAILURE);
}

NodeT* Parser::gotoo() {
	NodeT* node = createNode("<goto>");

	if (tk.tokenID == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;

	} else {
		expectedToken.assign("Identifier");
		parserError();
	}

	exit(EXIT_FAILURE);
}

void Parser::parserError() {
	cout << "[ERROR] Parsing error at " << scanner.getScannerPosition() << " -> expected [" << expectedToken << "] but received [" << tk.tokenInstance << "]" << endl;
	file.close();
	exit(EXIT_FAILURE);
}