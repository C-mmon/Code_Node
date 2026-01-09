//Designing a system with operation
//(i) incPop(tag): Increment the popularity of a tag
//(ii) decPop(tag):  Decrement the popularity of a tag
//(iii) getMostPop(tag): Return tag with higest popularity
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

class TagManagementSystem
{
private:
	int maxPop;
	unordered_map <int, unordered_set <string>> popToTags;
	unordered_map <string, int> tagToPop;

public:
	void incPop(const string& tag)
	{
		//fetch the tag to pop
		int pop = tagToPop[tag]++;
		//erase the given tag
		popToTags[pop].erase(tag);
		//insert the tag back 
		popToTags[pop+1].insert(tag);
		//if the pop To Tags is empty
		if(popToTags.empty())
		{
			//we erase the previous tag position
			popToTags.erase(pop);
		}
		//Update the maxPop, if the current pop+1, becomes the next pop
		maxPop = max(maxPop, pop+1);
	}

	void decPop(const string& tag)
	{
		int pop = tagToPop[tag];
		if(pop == 0)
		{	
			//the given pop is not found
			return ;
		}
		tagToPop[tag]--;
		//only insert if during the decrement the value becomes positive
		// otherwise this can be problematic
		popToTags[pop-1].insert(tag);
		if(popToTags[pop].empty())
		{
			popToTags.erase(pop);
		}
		if(pop == maxPop && popToTags.find(pop) == popToTags.end())
		{
			maxPop--;
		}
	}

	string getMostPop()
	{
		if(popToTags[maxPop].empty())
		{
			return "";
		}

	}
}
