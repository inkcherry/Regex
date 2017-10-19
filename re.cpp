#include "re.h"
string Re::match_all(const string& source)
{
	string result = "";
	while (curr_index < source.size())
	{
		result += match(source);
		result += " ";
	}
	return result;
}
string Re::match(const string& source)
{
	unsigned i = curr_index, i_bak = i - 1; //source 游标
	int start_pos = state_list.get_start();  //start_pos从start开始
	int curr_status = start_pos;
	if (curr_status == -1)
		throw;
	string result = "";
	while (true)
	{
		if (curr_status == start_pos) //位于开始状态，游标恢复到原备份+1
		{
			result = "";
			i = ++i_bak;
		}
		if (i >= source.size()) //游标移动到结尾
			break;
		curr_status = state_list.get_status(curr_status, source[i]);
		if (curr_status == -1) //到达末尾状态，并接受一个不能使其转移的字符
			break;
		result += source[i];
		++i;
	}
	if (state_list.is_in_end_pos(curr_status) || curr_status == -1) //处于结束状态(仅会出现在*+匹配到最后一个字符的情况) || 末尾未转移
	{
		curr_index = i_bak + result.size();
		return result;
	}
	else //直到结尾都没有匹配到 
	{
		curr_index = source.size();
	}
	return "";
}