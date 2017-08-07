#ifndef _NODE_H
#define _NODE_H
#include<vector>
#include<algorithm>
using namespace std;
class Node            //node类型
{
public:
	enum TYPE{ CHAR, RANGE,AND, OR, REPEAT_0, REPEAT_1 };
	TYPE type;
	Node(TYPE _type) :type(_type){}   //构造  读type
	virtual ~Node() = 0{}
};

typedef vector<Node*> node_pool;     //vector<node*>

class Char_Node :public Node        //字符
{
public:
	Char_Node(char _c) :Node(CHAR), c(_c){}        
	~Char_Node(){}
	char c;                      //有个字符型变量  
};
class Range_Node :public Node     //范围
{
public:
	Range_Node(int _i, int _j) :Node(RANGE),i(_i), j(_j){}
	~Range_Node(){}
	int i, j;                        //有两个int型变量
};

class And_Node :public Node        //并
{
public:
	And_Node(node_pool* _pool) :Node(AND), pool(_pool){}
	~And_Node()    //and_node的析构
	{
		pool->erase(unique(pool->begin(), pool->end()), pool->end()); //去除重复元素，因为对于{n}的处理，一个pool内放置了多个重复地址。
		for (auto node : *(pool))
			delete node;
	}
	node_pool* pool;        //声明了一个pool        
};

class Or_Node :public Node       //或
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
	Node* node; //重复对象
};
#endif