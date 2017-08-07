#ifndef _PARSER_H
#define _PARSER_H
#include<iostream>
#include "node.h"
using namespace std;




class Parser   //������
{
public:
	Parser(const string s) :reg(s),index(0){ }                //����Parser��reg
	Node* GenTree() { Tree = DealOr(); return Tree; }         //����ֵ��dealor
	Node* GetTree(){ return Tree; }
	~Parser(){ Deallocate(); }
private:
	string reg;
	size_t index;
	char curr;
	Node* Tree;

	int GetChar(){ if (index < reg.size()){ curr = reg[index++]; } else curr = -1; return curr; }   //��ȡ�ַ� Ȼ��index+1  ���û���ַ� ����curr-1
	void Throw_Char(){ --index; }                             //����index
	int LookChar(){ auto c = GetChar(); if (c != -1)Throw_Char(); return c; }   //��ȡ�ַ� index���仯
	//һϵ�д�����
	Node* DealChar();
	Node* DealRange();
	Node* DealUnit();
	Node* DealAnd();
	Node* DealOr();
	Node* DealBlock();
	void Deallocate();
};


#endif