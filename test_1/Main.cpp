//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 

//root cern
#include "TApplication.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TThread.h"

//my
#include "A.h"
#include "Cv.h"

using namespace std;

void write()
{
	cout << "Write tree" << endl;
	
	ostringstream file_for_tree_name;
	file_for_tree_name << "E:\\190521\\190521_caen_trees\\test" << ".root";
	TFile file_for_tree(file_for_tree_name.str().c_str(), "RECREATE");

	TTree tree_main("TreeMain", "TreeMain");

	A *a = new A();

	//tree_main.Branch("EventMainCh", &a, 16000 , 0);
	tree_main.Branch("EventMainCh", &a, 16000);

	for (int ev = 0; ev < 2; ev++)
	{
		for (int ch = 0; ch < 3; ch++)
		{
			B *b = new B();
			//Cv *cv = new Cv();
			Cv cv;
			b->peak_amp = cv.GetPeakAmp();
			a->peaks.push_back(b);

			a->id.push_back(ch);
		}

		tree_main.Fill();
		a->Clear();
	}

	cout << endl << "Write tree:" << file_for_tree_name.str().c_str() << endl;
	file_for_tree.Write();
	file_for_tree.Close();
}

void read()
{
	cout << endl << "Read tree" << endl;
	
	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html

	string file_name_tree = "E:\\190521\\190521_caen_trees\\test.root";
	TFile *f = new TFile(file_name_tree.c_str());
	if (!(f->IsOpen()))
	{
		cout << "could not open file: " << file_name_tree << endl;
		exit(1);
	}
	else
	{
		cout << "File has been opened correctly: " << file_name_tree << endl;
	}

	TTree *tree = (TTree*)f->Get("TreeMain");

	A *a = new A();
	TBranch *branch = tree->GetBranch("EventMainCh");
	branch->SetAddress(&a);

	cout << "tree->GetEntries() = " << tree->GetEntries() << endl;
	//ev loop
	cout << endl;
	for (unsigned int ev = 0; ev < tree->GetEntries(); ev++)
	{
		cout << "ev = " << ev << endl;
		branch->GetEntry(ev);

		vector<B*> peaks = a->peaks;

		cout << "\t" << "a->id.size() = " << a->id.size() << endl;
		cout << "\t" << "a->peaks.size() = " << a->peaks.size() << endl;
		for (int ch = 0; ch < 3; ch++)
		{
			cout << "\t \t" << "ch = " << ch << endl;
			cout << "\t \t \t" << "id = " << a->id[ch] << endl;

			cout << "\t \t \t" << "peaks[ch]->peak_amp.size() = " << peaks[ch]->peak_amp.size() << endl;
			for (int peak_id = 0; peak_id < peaks[ch]->peak_amp.size(); peak_id++)
			{
				cout << "\t \t \t \t" << "peaks[ch]->peak_amp[peak_id] = " << peaks[ch]->peak_amp[peak_id] << endl;
			}
		}

		
		//for (int j = 0; j < a->id.size(); j++)
		//{

		//}

		//vector<B*> b = a->peaks;
	}
}

int main(int argc, char **argv)
{
	write();
	read();

	cout << endl;
	cout << "all is ok" << endl;
	system("pause");
}