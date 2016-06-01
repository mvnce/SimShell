#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "command.h"

using namespace std;

int main(int argc, char **argv, char** env) {
	Shell shell;

	bool flag = false;
	string input_data = "";

	while (1) {
		char input_str[128];
		if (flag) {
			flag = false;
			cout << input_data << endl;

			strcpy(input_str, input_data.c_str());
		}
		else {
			cout << "<" << shell.GetCount() << " " << shell.GetUsername() << ">";
			cin.getline(input_str, 128);
		}
		vector<string> tokens = shell.Tokenizer(input_str , ' ');
		
		string ftoken = "";
		if (tokens.size() > 0) {
			ftoken = tokens[0];
		}

		if (ftoken != "" && ftoken.at(0) == '!') {
			string num_str = ftoken;
			num_str.erase(0, 1);
			int num = atoi(num_str.c_str());
			int ret = shell.CheckHistoryEntity(num);
			if (ret != -1) {
				flag = true;
				input_data = shell.GetHistoryEntity(ret);
			}
			else {
				cout << num << ": Event not found." << endl;
			}
			continue;
		}

		if (ftoken == "quit") {
			shell.Quit();
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "date") {
			string retstr = shell.Date();
			cout << retstr << endl;
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "curr") {
			string retstr = shell.Current();
			cout << retstr << endl;
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "env") {
			while (*env) {
      			cout << *env++ << endl;
			}
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "cd") {
			if (tokens.size() > 1) {
				int ret = shell.ChangeDir(tokens[1]);

				if (ret != 0) {
					cout << tokens[1] << ": No such file or directory." << endl;
				}
			}
			else {
				cout << "missing tokens!" << endl;
			}
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "hlist") {
			shell.HistoryList();
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "dlist") {
			shell.DirList();
			shell.AddHistoryEntity(input_str);
			shell.AddDirEntity();
			shell.CountInc();
		}
		else if (ftoken == "") {
			continue;
		}
		else {
			char input_bu[128];
			strcpy(input_bu, input_str);

			shell.ExternalCommand(input_str);
			shell.AddHistoryEntity(input_bu);
			shell.AddDirEntity();
			shell.CountInc();
		}


	}

	return 0;
}