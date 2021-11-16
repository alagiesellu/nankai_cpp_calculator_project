/*
BSD License
Copyright (c) 2013, Tom Everett
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of Tom Everett nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#####################################################################################

Reference:
    https://github.com/antlr/grammars-v4/blob/master/calculator/Calculator.g4

Grammar copied from the above mentioned reference.
But modified to project specification.

*/

grammar Calculator;

session
    : header
    | equation
    ;

header
    : HEADER
    ;

equation
   : (variable EQ)? expression
   ;

expression
   : multiplyingExpression ((PLUS | MINUS) multiplyingExpression)*
   ;

multiplyingExpression
   : powExpression ((TIMES | DIV) powExpression)*
   ;

powExpression
   : signedAtom (POW signedAtom)*
   ;

signedAtom
   : func_
   | atom
   ;

atom
   : scientific
   | parentices
   ;

variable
   : VARIABLE
   ;

scientific
   : SCIENTIFIC_NUMBER
   ;

parentices
   : LPAREN expression RPAREN
   ;

func_
   : funcname parentices
   ;

funcname
   : COS
   | SIN
   ;

HEADER
   : '----'
   ;

EQ
   : '='
   ;

COS
   : 'cos'
   ;

SIN
   : 'sin'
   ;

LPAREN
   : '('
   ;

RPAREN
   : ')'
   ;

PLUS
   : '+'
   ;

MINUS
   : '-'
   ;

TIMES
   : '*'
   ;

DIV
   : '/'
   ;

POW
   : '^'
   ;

VARIABLE
   : VALID_ID_CHAR+
   ;

fragment VALID_ID_CHAR
   : ('a' .. 'z') | ('A' .. 'Z')
   ;

SCIENTIFIC_NUMBER
   : NUMBER | HEXADECIMAL_NUMBER | BINARY_NUMBER
   ;

fragment NUMBER
   : ('0' .. '9')+ ('.' ('0' .. '9') +)?
   ;

fragment HEXADECIMAL_NUMBER
   : '0x' ('0' | '9' | 'A' | 'F') +
   ;

fragment BINARY_NUMBER
   : ('0' | '1')+ 'b'
   ;

WS
   : [ \r\n\t] + -> skip
   ;