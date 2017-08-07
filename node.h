#ifndef _NODE_H
#define _NODE_H
#include<vector>
#include<algorithm>
using namespace std;
class Node            //node����
{
public:
	enum TYPE{ CHAR, RANGE,AND, OR, REPEAT_0, REPEAT_1 };
	TYPE type;
	Node(TYPE _type) :type(_type){}   //����  ��type
	virtual ~Node() = 0{}
};

typedef vector<Node*> node_pool;     //vector<node*>

class Char_Node :public Node        //�ַ�
{
public:
	Char_Node(char _c) :Node(CHAR), c(_c){}        
	~Char_Node(){}
	char c;                      //�и��ַ��ͱ���  
};
class Range_Node :public Node     //��Χ
{
public:
	Range_Node(int _i, int _j) :Node(RANGE),i(_i), j(_j){}
	~Range_Node(){}
	int i, j;                        //������int�ͱ���
};

class And_Node :public Node        //��
{
public:
	And_Node(node_pool* _pool) :Node(AND), pool(_pool){}
	~And_Node()    //and_node������
	{
		pool->erase(unique(pool->begin(), pool->end()), pool->end()); //ȥ���ظ�Ԫ�أ���Ϊ����{n}�Ĵ���һ��pool�ڷ����˶���ظ���ַ��
		for (auto node : *(pool))
			delete node;
	}
	node_pool* pool;        //������һ��pool        
};

class Or_Node :public Node       //��
{
public:
	Or_Node(node_pool* _pool) :Node(OR), pool(_pool){}
	~Or_Node()
	{
		for (auto node : *(pool))
			delete node;
	}
	node_pool* pool;
};

class Repeat_Node :public Node
{
public:
	Repeat_Node(Node* _node, TYPE type) :Node(type), node(_node){}
	~Repeat_Node()
	{
		delete node;
	}
	Node* node; //�ظ�����
};
#endif