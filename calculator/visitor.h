//
// Created by Muhammed S. Baldeh on 12/10/21.
//

#include <peg_parser/generator.h>
#include <iostream>
#include <unordered_map>


using namespace std;
using namespace peg_parser;

#ifndef PEGPARSER_VISITOR_H
#define PEGPARSER_VISITOR_H

struct Visitor;

using Expression = Interpreter<void, Visitor &>::Expression;

struct Visitor {

  float result{};
  unordered_map<string, float> variables{};

  float getValue(Expression &expression);

  void visitAddition(Expression left, Expression right);

  void visitSubtraction(Expression left, Expression right);

  void visitMultiplication(Expression left, Expression right);

  void visitDivision(Expression left, Expression right);

  void visitPower(Expression left, Expression right);

  void visitVariable(const Expression& name);

  void visitAssignment(const Expression& name, Expression value);

  void visitDecimalNumber(const Expression& value);

  void visitHexadecimalNumber(const Expression& value);

  void visitBinaryNumber(const Expression& value);

  void visitSin(Expression value);

  void visitCos(Expression value);

  void visitHeader();
};

#endif  // PEGPARSER_VISITOR_H