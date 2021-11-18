/**
*  This example demonstrate how we can use peg_parser::parser to define a
* command-line calculator and use a visitor pattern to evaluate the result.
*/

#include <peg_parser/generator.h>

#include <cmath>
#include <iostream>
#include <unordered_map>

int main() {
 using namespace std;

 struct Visitor;

 using Expression = peg_parser::Interpreter<void, Visitor &>::Expression;

 struct Visitor {

   float result;
   unordered_map<string, float> variables;

   float getValue(Expression e) {
     e.evaluate(*this);
     return result;
   }

   void visitAddition(Expression l, Expression r) {
     result = getValue(l) + getValue(r);
   }

   void visitSubtraction(Expression l, Expression r) {
     result = getValue(l) - getValue(r);
   }

   void visitMultiplication(Expression l, Expression r) {
     result = getValue(l) * getValue(r);
   }

   void visitDivision(Expression l, Expression r) {
     result = getValue(l) / getValue(r);
   }

   void visitPower(Expression l, Expression r) {
     result = pow(getValue(l), getValue(r));
   }

   void visitVariable(Expression name) {
     result = variables[name.string()];
   }

   void visitAssignment(Expression name, Expression value) {
     variables[name.string()] = getValue(value);
   }
   void visitDecimalNumber(Expression value) {
     result = stod(value.string());
   }

   void visitHexadecimalNumber(Expression value) {
     result = stod(value.string());
   }

   void visitBinaryNumber(Expression value) {
     string binary = value.string();
     binary.pop_back();
     result = stoi(binary, 0, 2);
   }

   void visitSin(Expression value) {
     result = sin(getValue(value));
   }
   void visitCos(Expression value) {
     result = cos(getValue(value));
   }
   void visitHeader() {
     result = 0;
     variables = unordered_map<string, float>();
   }
 };

 peg_parser::ParserGenerator<void, Visitor &> calculator;

 auto &g = calculator;
 g.setSeparator(g["Whitespace"] << "[\t ]");

  g["Session"] << "Expression | Header";

  g["Header"] << "'-'+" >> [](auto e, auto &v) { v.visitHeader(); };

  g["Expression"] << "Assign | Sum";

 g["Assign"] << "Name '=' Sum" >> [](auto e, auto &v) { v.visitAssignment(e[0], e[1]); };

 g["Sum"] << "Add | Subtract | Product";

 g["Product"] << "Multiply | Divide | Exponent";

 g["Exponent"] << "Power | Atomic";

 g["Atomic"] << "Number | Brackets | Functions | Variable";

 g["Brackets"] << "'(' Sum ')'";

 g["Functions"] << "Sin | Cos";

 g["Sin"] << "'sin' Brackets" >>
     [](auto e, auto &v) { v.visitSin(e[0]); };

 g["Cos"] << "'cos' Brackets" >>
     [](auto e, auto &v) { v.visitCos(e[0]); };

 g["Add"] << "Sum '+' Product" >>
     [](auto e, auto &v) { v.visitAddition(e[0], e[1]); };

 g["Subtract"] << "Sum '-' Product" >>
     [](auto e, auto &v) { v.visitSubtraction(e[0], e[1]); };

 g["Multiply"] << "Product '*' Exponent" >>
     [](auto e, auto &v) { v.visitMultiplication(e[0], e[1]); };

 g["Divide"] << "Product '/' Exponent" >>
     [](auto e, auto &v) { v.visitDivision(e[0], e[1]); };

 g["Power"] << "Atomic ('^' Exponent)" >>
     [](auto e, auto &v) { v.visitPower(e[0], e[1]); };

 g["Variable"] << "Name" >>
     [](auto e, auto &v) { v.visitVariable(e); };

 g["Name"] << "[a-zA-Z]+";

 g["Number"] << "HexadecimalNumber | BinaryNumber | DecimalNumber";

 g["DecimalNumber"] << "'-'? [0-9]+ ('.' [0-9]+)?" >>
     [](auto e, auto &v) { v.visitDecimalNumber(e); };

 g["HexadecimalNumber"] << "'0x' [0-9a-fA-F]+" >>
     [](auto e, auto &v) { v.visitHexadecimalNumber(e); };

 g["BinaryNumber"] << "[0-1]+ 'b'" >>
     [](auto e, auto &v) { v.visitBinaryNumber(e); };

 g.setStart(g["Session"]);

 Visitor visitor;

 while (true) {
   string input;

   cout << "Input: ";
   getline(cin, input);

   try {
     calculator.run(input, visitor);
     cout << "Output: " << visitor.result << endl;
   } catch (peg_parser::SyntaxError &error) {
     auto syntax = error.syntax;
     cout << "  ";
     cout << string(syntax->begin, ' ');
     cout << string(syntax->length(), '~');
     cout << "^\n";
     cout << "  " << "Syntax error while parsing " << syntax->rule->name << endl;
   }
 }

 return 0;
}
