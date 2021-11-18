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

   void visitPower(Expression l, Expression r) { result = pow(getValue(l), getValue(r)); }
   void visitVariable(Expression name) { result = variables[name.string()]; }

   void visitAssignment(Expression name, Expression value) {
     result = (variables[name.string()] = getValue(value));
   }

   void visitDecimalNumber(Expression value) {
     result = stod(value.string());
   }
   void visitHexadecimalNumber(Expression value) {
     result = stod(value.string());
   }
   void visitBinaryNumber(Expression value) {
     result = stod(value.string());
   }

   void visitExpressionPlus(Expression l, Expression r) { result = getValue(l) + getValue(r); }
   void visitExpressionMinus(Expression l, Expression r) { result = getValue(l) - getValue(r); }
   void visitMultiplyingExpressionTimes(Expression l, Expression r) { result = getValue(l) * getValue(r); }
   void visitMultiplyingExpressionDivision(Expression l, Expression r) { result = getValue(l) / getValue(r); }

   void visitCos(Expression e) { result = cos(getValue(e)); }
   void visitSin(Expression e) { result = sin(getValue(e)); }
 };

 peg_parser::ParserGenerator<void, Visitor &> calculator;

 auto &g = calculator;
 g.setSeparator(g["Whitespace"] << "[\t ]");

  g["Session"] << "Header | Expression";

  g["Expression"] << "Assignment | AlgebraicExpression";

  g["Assignment"] << "Variable '=' AlgebraicExpression"
      >> [](auto e, auto &v) { v.visitAssignment(e[0], e[1]); };

  g["Variable"] << "[a-zA-Z]+"
      >> [](auto e, auto &v) { v.visitVariable(e); };

  g["AlgebraicExpression"] << "ExpressionPlus | ExpressionMinus";

  g["ExpressionPlus"] << "MultiplyingExpression ('+' MultiplyingExpression)"
      >> [](auto e, auto &v) { v.visitExpressionPlus(e[0], e[1]); };

  g["ExpressionMinus"] << "MultiplyingExpression ('-' MultiplyingExpression)"
      >> [](auto e, auto &v) { v.visitExpressionMinus(e[0], e[1]); };

  g["MultiplyingExpression"] << "MultiplyingExpressionTimes | MultiplyingExpressionDivision";

  g["MultiplyingExpressionTimes"] << "PowerExpression ('*' PowerExpression)"
      >> [](auto e, auto &v) { v.visitMultiplyingExpressionTimes(e[0], e[1]); };

  g["MultiplyingExpressionDivision"] << "PowerExpression ('/' PowerExpression)"
      >> [](auto e, auto &v) { v.visitMultiplyingExpressionDivision(e[0], e[1]); };

  g["PowerExpression"] << "SignedAtom ('^' SignedAtom)"
      >> [](auto e, auto &v) { v.visitPower(e[0], e[1]); };

  g["SignedAtom"] << "FunctionExpression | Atom";

//  g["FunctionExpression"] << "Function Parentheses";

  g["Atom"] << "Number | Parentheses";

  g["FunctionExpression"] << "CosExpression | SinExpression";

  g["Parentheses"] << "'(' Expression ')'";

  g["Number"] << "DecimalNumber | HexadecimalNumber | BinaryNumber";

  g["DecimalNumber"] << "'-'? [0-9]+ ('.' [0-9]+)?" >>
      [](auto e, auto &v) { v.visitDecimalNumber(e); };

  g["HexadecimalNumber"] << "'0x' ('0' | '9' | 'A' | 'F')+" >>
      [](auto e, auto &v) { v.visitHexadecimalNumber(e); };

  g["BinaryNumber"] << "('0' | '1')+ 'b'" >>
      [](auto e, auto &v) { v.visitBinaryNumber(e); };

  g["CosExpression"       ] << "'cos' Parentheses" >>
      [](auto e, auto &v) { v.visitCos(e); };

  g["SinExpression"       ] << "'sin' Parentheses" >>
      [](auto e, auto &v) { v.visitSin(e); };

  g["Header"    ] << "'----'";

  g.setStart(g["Session"]);

//  g["Assign"] << "Name '=' Sum" >> [](auto e, auto &v) { v.visitAssignment(e[0], e[1]); };
//  g["Sum"] << "Add | Subtract | Product";
//  g["Product"] << "Multiply | Divide | Exponent";
//  g["Exponent"] << "Power | Atomic";
//  g["Atomic"] << "Number | Brackets | Variable";
//  g["Brackets"] << "'(' Sum ')'";
//  g["Add"] << "Sum '+' Product" >> [](auto e, auto &v) { v.visitAddition(e[0], e[1]); };
//  g["Subtract"] << "Sum '-' Product" >> [](auto e, auto &v) { v.visitSubtraction(e[0], e[1]); };
//  g["Multiply"] << "Product '*' Exponent" >>
//  [](auto e, auto &v) { v.visitMultiplication(e[0], e[1]); };
//  g["Divide"] << "Product '/' Exponent" >> [](auto e, auto &v) { v.visitDivision(e[0], e[1]); };
//  g["Power"] << "Atomic ('^' Exponent)" >> [](auto e, auto &v) { v.visitPower(e[0], e[1]); };

 cout << "Enter an expression to be evaluated.\n";

 while (true) {
   string str;
   cout << "> ";
   getline(cin, str);
   if (str == "q" || str == "quit") {
     break;
   }
   try {
     Visitor visitor;
     calculator.run(str, visitor);
     cout << str << " = " << visitor.result << endl;
   } catch (peg_parser::SyntaxError &error) {
     auto syntax = error.syntax;
     cout << "  ";
     cout << string(syntax->begin, ' ');
     cout << string(syntax->length(), '~');
     cout << "^\n";
     cout << "  "
          << "Syntax error while parsing " << syntax->rule->name << endl;
   }
 }

 return 0;
}
