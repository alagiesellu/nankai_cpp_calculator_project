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

   void visitAddition(Expression l, Expression r) { result = getValue(l) + getValue(r); }
   void visitSubtraction(Expression l, Expression r) { result = getValue(l) - getValue(r); }
   void visitMultiplication(Expression l, Expression r) { result = getValue(l) * getValue(r); }
   void visitDivision(Expression l, Expression r) { result = getValue(l) / getValue(r); }
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

   void visitParentheses(Expression name) { result = variables[name.string()]; }
   void visitExpressionPlus(Expression l, Expression r) { result = getValue(l) + getValue(r); }
   void visitExpressionMinus(Expression l, Expression r) { result = getValue(l) - getValue(r); }
   void visitMultiplyingExpressionTimes(Expression l, Expression r) { result = getValue(l) * getValue(r); }
   void visitMultiplyingExpressionDivision(Expression l, Expression r) { result = getValue(l) / getValue(r); }
 };

 peg_parser::ParserGenerator<void, Visitor &> calculator;

 auto &g = calculator;
 g.setSeparator(g["Whitespace"] << "[\t ]");

  g["Session"] << "Header | Equation";

  g["Equation"] << "(Variable EQ)? Expression";

  g["Variable"] << "Name" >> [](auto e, auto &v) { v.visitVariable(e); };

  g["Expression"] << "ExpressionPlus | ExpressionMinus";

  g["ExpressionPlus"] << "MultiplyingExpression (Plus MultiplyingExpression)*"
      >> [](auto e, auto &v) { v.visitExpressionPlus(e[0], e[1]); };

  g["ExpressionMinus"] << "MultiplyingExpression (Minus MultiplyingExpression)*"
      >> [](auto e, auto &v) { v.visitExpressionMinus(e[0], e[1]); };

  g["MultiplyingExpression"] << "MultiplyingExpressionTimes | MultiplyingExpressionDivision";

  g["MultiplyingExpressionTimes"] << "PowerExpression (Times PowerExpression)*"
      >> [](auto e, auto &v) { v.visitMultiplyingExpressionTimes(e[0], e[1]); };

  g["MultiplyingExpressionDivision"] << "PowerExpression (Division PowerExpression)*"
      >> [](auto e, auto &v) { v.visitMultiplyingExpressionDivision(e[0], e[1]); };

  g["PowerExpression"] << "SignedAtom (Power SignedAtom)*";

  g["SignedAtom"] << "FunctionExpression | Atom";

  g["FunctionExpression"] << "Function Parentheses";

  g["Atom"] << "Number | Parentheses";

  g["Function"] << "Cos | Sin";

  g["Parentheses"] << "Lparen Expression Rparen" >>
      [](auto e, auto &v) { v.visitParentheses(e); };

  g["Variable"] << "Name" >> [](auto e, auto &v) { v.visitVariable(e); };

  g["Number"] << "DecimalNumber | HexadecimalNumber | BinaryNumber";

  g["DecimalNumber"] << "Minus? [0-9]+ ('.' [0-9]+)?" >>
      [](auto e, auto &v) { v.visitDecimalNumber(e); };

  g["HexadecimalNumber"] << "'0x' ('0' | '9' | 'A' | 'F')+" >>
      [](auto e, auto &v) { v.visitHexadecimalNumber(e); };

  g["BinaryNumber"] << "('0' | '1')+ 'b'" >>
      [](auto e, auto &v) { v.visitBinaryNumber(e); };

  g["Name"      ] << "[a-zA-Z]+";
  g["Header"    ] << "'----'";
  g["Eq"        ] << "'='";
  g["Cos"       ] << "'cos'";
  g["Sin"       ] << "'sin'";
  g["Lparen"    ] << "'('";
  g["Rparen"    ] << "')'";
  g["Plus"      ] << "'+'";
  g["Minus"     ] << "'-'";
  g["Times"     ] << "'*'";
  g["Division"  ] << "'/'";
  g["Power"     ] << "'^'";

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
