#ifndef _PARSER_H
#define _PARSER_H
#include<iostream>
#include "node.h"
using namespace std;




class Parser   //分析器
{
public:
	Parser(const string s) :reg(s),index(0){ }                //传入Parser的reg
	Node* GenTree() { Tree = DealOr(); return Tree; }         //返回值是dealor
	Node* GetTree(){ return Tree; }
	~Parser(){ Deallocate(); }
private:
	string reg;
	size_t index;
	char curr;
	Node* Tree;

	int GetChar(){ if (index < reg.size()){ curr = reg[index++]; } else curr = -1; return curr; }   //读取字符 然后index+1  如果没有字符 返回curr-1
	void Throw_Char(){ --index; }                             //回退index
	int LookChar(){ auto c = GetChar(); if (c != -1)Throw_Char(); return c; }   //读取字符 index不变化
	//一系列处理函数
	Node* DealChar();
	Node* DealRange();
	Node* DealUnit();
	Node* DealAnd();
	Node* DealOr();
	Node* DealBlock();
	void Deallocate();
};


#endif