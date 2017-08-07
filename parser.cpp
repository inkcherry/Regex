#include "parser.h"


Node* Parser::DealChar()   //读取字符
{
	GetChar();
	if (curr == -1)         //如果当前没有字符串  返回
		return nullptr;
	return new Char_Node(curr);   //不然 用char_node解析这个字符
}


Node* Parser::DealUnit()     //最低级A处理每个单元 主要处理括号的转义字符
{
	GetChar();               //读字符
	if (curr == -1)
		return nullptr;
	if (curr == '(')           //如果读取的为"（"
	{
		Node* or_node = DealOr();      //Dealor处理
		GetChar();           //       吃掉继续) 
		return or_node;            //返回一个or节点对象
	}
	else if (curr == ')')          //如果读取的为"("
	{
		Throw_Char();           //回退index
		return nullptr;
	}
	else if (curr == '[')      //如果读到的是[
	{
		char c = GetChar();           //c d 分别为范围
		GetChar(); //-             //分别吃掉-和]
		char d = GetChar();
		GetChar(); //]
		return new Range_Node(c, d);      //返回一个范围节点
	}
	else if (curr == '\\')                             //如果是\  这里进行了转义
	{
		char c = GetChar(); // \w \d       //吃了这个字符
		switch (c)                            //进行转义
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
	else if (curr == '.')        //当时.的时候  意为匹配任何字符
	{
		return new Range_Node(1, 255);
	}
	else //固定字符
	{
		return new Char_Node(curr);   //直接建node
	}
	return nullptr;
}

Node* Parser::DealBlock()        //次级分析机B级  处理特殊字符
{
	Node* unit_node = DealUnit();   //给出一个分析curr状态的A级分析
	GetChar();
	if (curr == '+')              //读取字符  重复一个以上
	{
		return new Repeat_Node(unit_node, Node::REPEAT_1);
	}
	else if (curr == '*')     //重复0或者以上
	{
		return new Repeat_Node(unit_node, Node::REPEAT_0);
	}
	else if (curr == '{')   //进行自定义匹配重复次数
	{
		char c;
		int sum = 0;
		while ((c = GetChar()) != '}')    //一直搜索到}
			sum = 10 * sum + (c - '0');    //计数重复次数
		auto pool = new node_pool;
		for (int i = 0; i < sum; ++i)    //进行连接节点
		{
			pool->push_back(unit_node);
		}
		return new And_Node(pool);       //返回一个用于串联的node
	}
	if (curr != -1)           //结束了就回退
		Throw_Char();
	return unit_node;
}

Node* Parser::DealAnd()           //与级分析器C级别 and的话会一直运行 知道碰到一个|
{
	node_pool* pool = new node_pool;    //声明node
	while (true)                               
	{
		Node* block = DealBlock();        //进行B级操作
		if (!block)                          //如果block为nullptr 则跳出
			break;

		pool->push_back(block);          //把block放进pool里

		if (LookChar() == '|')        //如果发生了或字符 
			break;                     //跳出

	}
	if (pool->empty())          //如果pool是空的  返回nullptr
	{
		delete pool;
		return nullptr;
	}
	else if (pool->size() == 1)       //如果pool的大小为1
	{
		Node* tmp = *(pool->begin());
		delete pool;
		return tmp;
	}
	else
	{
		return new And_Node(pool);    //返回pool
	}
}

Node* Parser::DealOr()      //D级别分析器 处理或
{
	node_pool *pool = new node_pool;    //放到一个大型池子中
	while (true)
	{
		Node* and_node = DealAnd();   //分析and 
		if (!and_node)                
			break;

		pool->push_back(and_node);      //把一个and放进去

		if (LookChar() != '|')       //不是这个代表结束了  那么就跳出
			break;
		else                          //如果是|这个 那么继续循环
			GetChar(); //eat |
	}
	if (pool->empty())               //如果整个池子是空的 那么返回一个空指针
	{
		delete pool;
		return nullptr;
	}
	else if (pool->size() == 1)      //如果pool的大小是1 
	{
		Node* tmp = *(pool->begin());       //返回一个 单个的node
		delete pool;
		return tmp;
	}
	else                            //如果size>1
	{
		return new Or_Node(pool);   //那么返回一个node池
	}
}

void Parser::Deallocate()   //删除tree
{
	delete Tree;
}