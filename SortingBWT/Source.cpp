#include<stdio.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>

using namespace std;

class LCPInterval
{
public:
	int startInt;
	int endInt;
	int LCPIntVal;
	LCPInterval()
	{
		startInt = 0;
		endInt = 0;
		LCPIntVal = 0;
	}
};
class BWT
{
public:
	vector<char> BWTString;
	vector<int> LCPVal;
	vector<int> componentIds;
	vector<LCPInterval> LCPintervals;
	string origString;
	int compSize;
	vector<string> LCPArray;
	void QuickSort(int start, int n);
	int Partition(int start, int n);
	void swap(int &s1, int &s2);
	void findBWT();
	void findLCPArray();
};
void BWT::findBWT()
{
	for (int i = 0; i < origString.size(); i++)
	{
		if (componentIds[i]>0)
			BWTString.push_back(origString[componentIds[i] - 1]);
		else
			BWTString.push_back(origString[origString.size() - 1]);
	}
}

void BWT::findLCPArray()
{
	LCPArray.push_back("\0");
	LCPVal.push_back(-1);
	for (int i = 1; i < componentIds.size(); i++)
	{
		string s1 = origString.substr(componentIds[i-1]);
		string s2 = origString.substr(componentIds[i]);
		int j = 0;
		string LCPStr="";
		while (s1[j] == s2[j])
		{
			LCPStr+=s1[j];
			j++;
		}
		LCPStr[j] += '\0';
		LCPArray.push_back(LCPStr);
		LCPVal.push_back(j);

	}
}

void BWT::QuickSort(int start, int n)
{
	if(n > 1){
		int pivotIndex = Partition(start, n);
		int n1 = pivotIndex - start;
		int n2 = n - n1 - 1;
		QuickSort(start, n1);
		QuickSort(pivotIndex + 1, n2);
	}
}

int BWT::Partition(int start, int n)
{
	string pivot = origString.substr(componentIds[start],compSize);
	int i = start + 1;
	int j = start + n - 1;
	while (i <= j){
		string iComp = origString.substr(componentIds[i], compSize);
		string jComp = origString.substr(componentIds[j], compSize);
		string iPivot = pivot, jPivot = pivot;
		int pivotPoint = componentIds[start];
		int iInc = componentIds[i], jInc=componentIds[j];
		while (iComp == iPivot)
		{
			iInc += compSize;
			pivotPoint += compSize;

			iComp = origString.substr(iInc, compSize);
			iPivot = origString.substr(pivotPoint, compSize);
		}
		//iPivot = pivot;
		pivotPoint = componentIds[start];
		while (jComp == jPivot)
		{
			jInc += compSize;
			pivotPoint += compSize;
			iPivot = origString.substr(pivotPoint, compSize);
			jComp = origString.substr(jInc, compSize);
		}
		if (iComp < pivot)
			i++;
		else if (jComp > pivot)
			j--;
		else{
			swap(componentIds[i], componentIds[j]);
			i++;
  			j--;
		}//else
	}//while
	swap(componentIds[start], componentIds[j]);
	return j;
}//end Partition

void BWT::swap(int &s1,int &s2)
{
	int temp = s1;
	s1 = s2;
	s2 = temp;
}

int main()
{
	string s1 = "ATATATTAG$";
	string s ="";//"ATATATTAG$";
	fstream myFile;
	stringstream ss;
	myFile.open("genome.txt");
	if (myFile.is_open())
	{
		string line;
		int i = 0;
		while (getline(myFile, line))
		{
			ss<<line;
			i++;
		}

		//cout << endl << i << endl;
		myFile.close();
	}
	
	s = s1;//ss.str()+"$";
	//s.erase(remove(s.begin(), s.end(), '\n'), s.end());
	cout << s;
	BWT bwt;
	bwt.origString = s1;
	int compSize = 0;
	cout << "Enter the component size:";
	cin >> bwt.compSize; 
	for (int i = 0; i < s1.size(); i++)
	{
		bwt.componentIds.push_back(i);
	}
	bwt.QuickSort(0,s1.size());

	for (int i = 0; i < s.size(); i++)
	{
		cout << endl << bwt.origString.substr(bwt.componentIds[i], bwt.compSize) << "   " << bwt.componentIds[i];
	}
	// find BWT from sorted component ids
	bwt.findBWT();
	cout << endl;
	for (int i = 0; i < bwt.BWTString.size(); i++)
	{
		cout << bwt.BWTString[i] << "  ";
	}
	// find LCPs from sorted component ids
	bwt.findLCPArray();
	for (int i = 0; i < bwt.LCPArray.size(); i++)
	{
		cout << endl<< bwt.LCPArray[i] << "  "<<bwt.LCPVal[i];
	}

    //Start: compute LCP intervals for super maximal repeats.
	for (int i = 0; i < s.size(); i++)
	{
		LCPInterval interval;
		bool first = true;
		for (int j = 0; j < s.size()-i; j++)
		{
			if (i + j == 0)
			{
				continue;
			}
			if (first)
			{
				if (bwt.LCPVal[i + j] != bwt.LCPVal[i + j + 1])
				{
					interval.startInt = i + j;
					interval.LCPIntVal++;
					interval.endInt = i + j + 1;
					first = false;
				}
			}
			else
			{
				if(i+j == s.size()-1)// if j = s.size() .. worst case
				{
					bwt.LCPintervals.push_back(interval);
					break;
				}
				if (bwt.LCPVal[i + j] > 0)
				{
					interval.LCPIntVal = bwt.LCPVal[i+j];
					interval.endInt = i + j;
					if (bwt.LCPVal[i+j+1]!=bwt.LCPVal[i+j])
					{
						
					}
				}
				else if (bwt.LCPVal[i + j]==0)
				{
					bwt.LCPintervals.push_back(interval);
					break;
				}
			}
		}
	}
	// End : Compute LCP intervals
	fstream fout;
	fout.open("out.txt", ios::out);
	if (fout.is_open())
	{
		for (int i = 0; i < bwt.componentIds.size(); i++)
		{
			fout << " "<<bwt.componentIds[i];
		}
		fout.close();
	}
	cout << "over";
	_getch();
	return 0;

}