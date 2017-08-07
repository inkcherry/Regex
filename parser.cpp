#include "parser.h"


Node* Parser::DealChar()   //��ȡ�ַ�
{
	GetChar();
	if (curr == -1)         //�����ǰû���ַ���  ����
		return nullptr;
	return new Char_Node(curr);   //��Ȼ ��char_node��������ַ�
}


Node* Parser::DealUnit()     //��ͼ�A����ÿ����Ԫ ��Ҫ�������ŵ�ת���ַ�
{
	GetChar();               //���ַ�
	if (curr == -1)
		return nullptr;
	if (curr == '(')           //�����ȡ��Ϊ"��"
	{
		Node* or_node = DealOr();      //Dealor����
		GetChar();           //       �Ե�����) 
		return or_node;            //����һ��or�ڵ����
	}
	else if (curr == ')')          //�����ȡ��Ϊ"("
	{
		Throw_Char();           //����index
		return nullptr;
	}
	else if (curr == '[')      //�����������[
	{
		char c = GetChar();           //c d �ֱ�Ϊ��Χ
		GetChar(); //-             //�ֱ�Ե�-��]
		char d = GetChar();
		GetChar(); //]
		return new Range_Node(c, d);      //����һ����Χ�ڵ�
	}
	else if (curr == '\\')                             //�����\  ���������ת��
	{
		char c = GetChar(); // \w \d       //��������ַ�
		switch (c)                            //����ת��
		{  
		case 'w':                           
			return new Range_Node('A', 'z');
			break;
		case 'd':
			return new Range_Node('0', '9');
			break;
		case '(':
			return new Range_Node('(', '(');
			break;
		case ')':
			return new Range_Node(')', ')');
			break;
		case '.':
			return new Range_Node('.', '.');
			break;
		default:
			throw;
		}
	}
	else if (curr == '.')        //��ʱ.��ʱ��  ��Ϊƥ���κ��ַ�
	{
		return new Range_Node(1, 255);
	}
	else //�̶��ַ�
	{
		return new Char_Node(curr);   //ֱ�ӽ�node
	}
	return nullptr;
}

Node* Parser::DealBlock()        //�μ�������B��  ���������ַ�
{
	Node* unit_node = DealUnit();   //����һ������curr״̬��A������
	GetChar();
	if (curr == '+')              //��ȡ�ַ�  �ظ�һ������
	{
		return new Repeat_Node(unit_node, Node::REPEAT_1);
	}
	else if (curr == '*')     //�ظ�0��������
	{
		return new Repeat_Node(unit_node, Node::REPEAT_0);
	}
	else if (curr == '{')   //�����Զ���ƥ���ظ�����
	{
		char c;
		int sum = 0;
		while ((c = GetChar()) != '}')    //һֱ������}
			sum = 10 * sum + (c - '0');    //�����ظ�����
		auto pool = new node_pool;
		for (int i = 0; i < sum; ++i)    //�������ӽڵ�
		{
			pool->push_back(unit_node);
		}
		return new And_Node(pool);       //����һ�����ڴ�����node
	}
	if (curr != -1)           //�����˾ͻ���
		Throw_Char();
	return unit_node;
}

Node* Parser::DealAnd()           //�뼶������C���� and�Ļ���һֱ���� ֪������һ��|
{
	node_pool* pool = new node_pool;    //����node
	while (true)                               
	{
		Node* block = DealBlock();        //����B������
		if (!block)                          //���blockΪnullptr ������
			break;

		pool->push_back(block);          //��block�Ž�pool��

		if (LookChar() == '|')        //��������˻��ַ� 
			break;                     //����

	}
	if (pool->empty())          //���pool�ǿյ�  ����nullptr
	{
		delete pool;
		return nullptr;
	}
	else if (pool->size() == 1)       //���pool�Ĵ�СΪ1
	{
		Node* tmp = *(pool->begin());
		delete pool;
		return tmp;
	}
	else
	{
		return new And_Node(pool);    //����pool
	}
}

Node* Parser::DealOr()      //D��������� �����
{
	node_pool *pool = new node_pool;    //�ŵ�һ�����ͳ�����
	while (true)
	{
		Node* and_node = DealAnd();   //����and 
		if (!and_node)                
			break;

		pool->push_back(and_node);      //��һ��and�Ž�ȥ

		if (LookChar() != '|')       //����������������  ��ô������
			break;
		else                          //�����|��� ��ô����ѭ��
			GetChar(); //eat |
	}
	if (pool->empty())               //������������ǿյ� ��ô����һ����ָ��
	{
		delete pool;
		return nullptr;
	}
	else if (pool->size() == 1)      //���pool�Ĵ�С��1 
	{
		Node* tmp = *(pool->begin());       //����һ�� ������node
		delete pool;
		return tmp;
	}
	else                            //���size>1
	{
		return new Or_Node(pool);   //��ô����һ��node��
	}
}

void Parser::Deallocate()   //ɾ��tree
{
	delete Tree;
}