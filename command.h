
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <deque>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <typeinfo>

using namespace std;

struct Entity {
	int id;
	string line;
};

class Shell {

public:
	Shell (void) {
		m_username = getlogin();
		m_cnt = 1;
	}

	vector<string> Tokenizer(string line, char del) {
		vector<string> tokens;
		stringstream ss(line);
  		string token;
  		while(getline(ss, token, del)) {
  			tokens.push_back(token);
  		}
  		return tokens;
  	}

	void Quit() {
		exit(0);
	}

	string Date() { 
		time_t rawtime;
  		struct tm * timeinfo;
  		char buffer [80];

  		time (&rawtime);
  		timeinfo = localtime (&rawtime);

  		strftime (buffer,80,"%a %b %d %T %Z %Y",timeinfo);
  		string ret_str(buffer);
    	return ret_str;
	}

	string Current() {
		char* cwd;
    	char buff[PATH_MAX + 1];
    	cwd = getcwd(buff, PATH_MAX + 1);

    	string ret_str(cwd);
    	return ret_str;
	}

	int ChangeDir(string dir) {
		int ret;
		string dir_t = "~";
		dir_t.append(m_username);
		if (dir == dir_t || dir == "~") {
			string dir_str = "/user/";
			dir_str.append(m_username);
			const char * hdir_ch = dir_str.c_str();
			ret = chdir(hdir_ch);
		}
		else if (dir.length() > 0 && dir.at(0) == '#') {
			string num_str = dir;
			num_str.erase(0, 1);
			int num = atoi(num_str.c_str());
			bool flag = this->CheckDirEntity(num);
			if (flag) {
				string dir_str = this->GetDirEntity(num);
				const char * hdir_ch = dir_str.c_str();
				ret = chdir(hdir_ch);
			}
			else {
				ret = -1;
			}
		}
		else {
			const char * dir_ch = dir.c_str();
			ret = chdir(dir_ch);
		}
		return ret;
	}

	void HistoryList() {
		for (int i = 0; i < m_hlist.size() ; i++) {
			cout << m_hlist[i].id << " " << m_hlist[i].line << endl;
		}
	}

	void DirList() {
		for (int i = 0; i < m_dlist.size(); i++) {
			cout << i+1 << " " << m_dlist[i].line << endl;
		}
	}

	void CountInc() {
		m_cnt++;
	}

	string GetUsername() {
		return m_username;
	}

	int GetCount() {
		return m_cnt;
	}

	// hlist operations
	void AddHistoryEntity(string line) {
		Entity entity;
		entity.id = m_cnt;
		entity.line = line;

		if (m_hlist.size() < 10) {
			m_hlist.push_back(entity);
		}
		else {
			m_hlist.pop_front();
			m_hlist.push_back(entity);
		}
	}

	int CheckHistoryEntity(int id) {
		int idx = -1;
		for (int i = 0; i < m_hlist.size(); i++) {
			if (m_hlist[i].id == id) {
				idx = i;
				break;
			}
		}
		return idx;
	}

	string GetHistoryEntity(int idx) {
		return m_hlist[idx].line;
	}

	// dlist opertaions
	void AddDirEntity() {
		Entity entity;
		entity.id = m_cnt;
		entity.line = this->Current();

		if (m_dlist.size() < 10) {
			m_dlist.push_front(entity);
		}
		else {
			m_dlist.pop_back();
			m_dlist.push_front(entity);
		}
	}

	bool CheckDirEntity(int idx) {
		if (idx < m_dlist.size() && idx > 0) {
			return true;
		}
		else {
			return false;
		}
	}

	string GetDirEntity(int idx) {
		return m_dlist[idx].line;
	}

	void Parse(char *line, char **argv) {
		while (*line != '\0') {
			while (*line == ' ' || *line == '\t' || *line == '\n') {
				*line++ = '\0';
			}

			*argv++ = line;

			while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
				line++;
			}
		}
		*argv = '\0';
	}

	void Execute(char **argv) {
		pid_t pid;
		int status;
		int result;

		int cnt = 0;
		char **temp = argv;
		while (*temp != NULL) {
			cnt++;
			temp++;
		}

		const char *amp;
		amp = "&";
		bool amp_flag = false;

		if (strcmp(argv[cnt-1], amp) == 0) {
			amp_flag = true;
			argv[cnt-1] = NULL;
			cnt--;
		}

		if ((pid = fork()) < 0) {
			cout << "fork initial error" << endl;
			exit(1);
		}
		else if (pid == 0) {
			if (execvp(*argv, argv) < 0) {
				cout << "fork child process error" << endl;
				exit(1);
			}
		}
		else if (!amp_flag) {
			if (wait(&status) != pid) {
				perror( "wait error" );
			}
		}

		if (amp_flag) {
			cout << "wait... child, pid = " << pid << endl;
			result = waitpid(pid, &status, 0);
		}
	}

	void ExternalCommand(char line[]) {
		char  *argv[64];

		this->Parse(line, argv);
		this->Execute(argv);
	}

private:
	string m_username;
	deque<Entity> m_hlist;
	deque<Entity> m_dlist;
	int m_cnt;
};