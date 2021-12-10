#include <peg_parser/generator.h>
#include <iostream>
#include "visitor.h"

using namespace std;
using namespace peg_parser;

void parserGenerator(ParserGenerator<void, Visitor &> &calculator);
void checkExitProgram(string &input);

int main() {

  ParserGenerator<void, Visitor &> calculator;
  Visitor visitor;
  string input;

  parserGenerator(calculator);

  cout << "Enter 'exit' to exit a program." << endl;

  while (true) {

    cout << "Input: ";
    getline(cin, input);

    checkExitProgram(input);

    try {

      calculator.run(input, visitor);
      cout << "Output: " << visitor.result << endl;

    } catch (SyntaxError &error) {

      cout << "*** Syntax error while parsing " << error.syntax->rule->name << endl;

    }
  }
}

void checkExitProgram(string &input) {
  if (input == "exit") {

    cout << "Bye bye." << endl;
    exit(EXIT_SUCCESS);
  }
}

void parserGenerator(ParserGenerator<void, Visitor &> &calculator) {

  auto &parserGenerator = calculator;

  parserGenerator.setSeparator(parserGenerator["Whitespace"] << "[\t ]");

  parserGenerator["Session"] << "Expression | Header";

  parserGenerator["Header"] << "'-'+" >>
      [](auto expression, auto &visitor) {
        visitor.visitHeader();
      };

  parserGenerator["Expression"] << "Assignment | Equation";

  parserGenerator["Assignment"] << "Name '=' Equation" >>
      [](auto expression, auto &visitor) {
        visitor.visitAssignment(expression[0], expression[1]);
      };

  parserGenerator["Equation"] << "Add | Subtract | Product | Variable";

  parserGenerator["Product"] << "Multiply | Divide | Exponent";

  parserGenerator["Exponent"] << "Power | Atomic";

  parserGenerator["Atomic"] << "Number | Brackets | Functions | Variable";

  parserGenerator["Brackets"] << "'(' Equation ')'";

  parserGenerator["Functions"] << "Sin | Cos";

  parserGenerator["Sin"] << "'sin' Brackets" >>
      [](auto expression, auto &visitor) {
        visitor.visitSin(expression[0]);
      };

  parserGenerator["Cos"] << "'cos' Brackets" >>
      [](auto expression, auto &visitor) {
        visitor.visitCos(expression[0]);
      };

  parserGenerator["Add"] << "Equation '+' Product" >>
      [](auto expression, auto &visitor) {
        visitor.visitAddition(expression[0], expression[1]);
      };

  parserGenerator["Subtract"] << "Equation '-' Product" >>
      [](auto expression, auto &visitor) {
        visitor.visitSubtraction(expression[0], expression[1]);
      };

  parserGenerator["Multiply"] << "Product '*' Exponent" >>
      [](auto expression, auto &visitor) {
        visitor.visitMultiplication(expression[0], expression[1]);
      };

  parserGenerator["Divide"] << "Product '/' Exponent" >>
      [](auto expression, auto &visitor) {
        visitor.visitDivision(expression[0], expression[1]);
      };

  parserGenerator["Power"] << "Atomic ('^' Exponent)" >>
      [](auto expression, auto &visitor) {
        visitor.visitPower(expression[0], expression[1]);
      };

  parserGenerator["Variable"] << "Name" >>
      [](auto expression, auto &visitor) {
        visitor.visitVariable(expression);
      };

  parserGenerator["Name"] << "[a-zA-Z]+";

  parserGenerator["Number"] << "HexadecimalNumber | BinaryNumber | DecimalNumber";

  parserGenerator["DecimalNumber"] << "'-'? [0-9]+ ('.' [0-9]+)?" >>
      [](auto expression, auto &visitor) {
        visitor.visitDecimalNumber(expression);
      };

  parserGenerator["HexadecimalNumber"] << "'0x' [0-9a-fA-F]+" >>
      [](auto expression, auto &visitor) {
        visitor.visitHexadecimalNumber(expression);
      };

  parserGenerator["BinaryNumber"] << "[0-1]+ 'b'" >>
      [](auto expression, auto &visitor) {
        visitor.visitBinaryNumber(expression);
      };

  parserGenerator.setStart(parserGenerator["Session"]);
}