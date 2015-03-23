#include<stdio.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
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
		LCPIntVal = -1;
	}
	bool isNULLInterval()
	{
		return (startInt == 0 && endInt == 0 && LCPIntVal == -1);
	}

	void MergeInterval(LCPInterval intervalToMerge)
	{
		startInt = this->startInt;
		endInt = intervalToMerge.endInt;
		LCPIntVal = min(this->LCPIntVal, intervalToMerge.LCPIntVal);
	}
	int min(int a, int b)
	{
		if (a < b)
		{
			return a;
		}
		return b;
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
			jPivot = origString.substr(pivotPoint, compSize);
			jComp = origString.substr(jInc, compSize);
		}
		if (iComp < iPivot)
			i++;
		else if (jComp > jPivot)
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
	string s1 = "mississippi$";
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
	int bwtLCPArraySize = bwt.LCPArray.size();
	for (int i = 1; i < bwtLCPArraySize; i++)
	{
		LCPInterval interval;
		int LCPval = -1;
		bool first = true;
		for (int j = 0; j < 5; j++)
		{
			/**/
			if (first)
			{
				if (i + j + 1>bwtLCPArraySize - 1)
				{
					break;
				}
				if (bwt.LCPVal[i + j] != bwt.LCPVal[i + j + 1] && bwt.LCPVal[i + j + 1] > 0)
				{
					interval.startInt = i + j;
					interval.LCPIntVal = bwt.LCPVal[i + j + 1];
					LCPval = bwt.LCPVal[i + j + 1];;
					interval.endInt = i + j + 1;
					first = false;
				}
				else
					break;
			}
			else
			{
				if (i + j + 1 >= bwtLCPArraySize - 1)// if j = s.size() .. worst case
				{
					bwt.LCPintervals.push_back(interval);
					break;
				}
				if (bwt.LCPVal[i + j + 1] == LCPval)
				{
					interval.LCPIntVal = LCPval;
					interval.endInt = i + j+1;
				}
				else 
				{
					bwt.LCPintervals.push_back(interval);
					break;
				}
			}
		}
	}
	// End : Compute LCP intervals

	// Merge intervals
	int lcpIntervalsSize = bwt.LCPintervals.size();
	vector<LCPInterval> tempVector;
	LCPInterval previousInterval;
	for (int i = 0; i < lcpIntervalsSize-1; i++)
	{
		int j = i;
		LCPInterval interval;
		
		bool first = true;
		while (j!=lcpIntervalsSize-1 && bwt.LCPintervals[j].endInt == bwt.LCPintervals[j+1].startInt)
		{
			if (first)
				interval = bwt.LCPintervals[j];
			
			LCPInterval tempInterval = interval;
			interval.MergeInterval(bwt.LCPintervals[j+1]);

			if (tempInterval.LCPIntVal>interval.LCPIntVal)
				tempVector.push_back(tempInterval);

			j++;
			first = false;
		}

		if (j > i) 
		{
			if (tempVector.size()==0)
				tempVector.push_back(interval);
			else if(previousInterval.LCPIntVal != interval.LCPIntVal)
				tempVector.push_back(interval);

			previousInterval = interval;
		}
	}

	//removing duplicates
	/*for (int i = 0; i < tempVector.size(); i++)
	{
		if (te)
	}*/
	//end: removing duplicates
	// End : Merge intervals
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