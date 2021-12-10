//
// Created by leber on 12/10/21.
//

#include <cmath>
#include "visitor.h"

float Visitor::getValue(Expression &expression) {
  expression.evaluate(*this);
  return result;
}

void Visitor::visitAddition(Expression left, Expression right) {
  result = getValue(left) + getValue(right);
}

void Visitor::visitSubtraction(Expression left, Expression right) {
  result = getValue(left) - getValue(right);
}

void Visitor::visitMultiplication(Expression left, Expression right) {
  result = getValue(left) * getValue(right);
}

void Visitor::visitDivision(Expression left, Expression right) {
  result = getValue(left) / getValue(right);
}

void Visitor::visitPower(Expression left, Expression right) {
  result = pow(getValue(left), getValue(right));
}

void Visitor::visitVariable(const Expression& name) {
  result = variables[name.string()];
}

void Visitor::visitAssignment(const Expression& name, Expression value) {
  variables[name.string()] = getValue(value);
}

void Visitor::visitDecimalNumber(const Expression& value) {
  result = stof(value.string());
}

void Visitor::visitHexadecimalNumber(const Expression& value) {
  result = stof(value.string());
}

void Visitor::visitBinaryNumber(const Expression& value) {
  string binary = value.string();
  binary.pop_back();
  result = (float) stoi(binary, nullptr, 2);
}

void Visitor::visitSin(Expression value) {
  result = sin(getValue(value));
}

void Visitor::visitCos(Expression value) {
  result = cos(getValue(value));
}

void Visitor::visitHeader() {
  result = {};
  variables = unordered_map<string, float>();
}