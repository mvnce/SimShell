
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <deque>
#include <string.h>
#include <ctime>
#include <sstream>

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
		for (int i = 0; i < m_dlist.size() ; i++) {
			cout << m_hlist[i].id << " " << m_hlist[i].line << endl;
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

	void AddDirEntity() {
		Entity entity;
		entity.id = m_cnt;
		entity.line = this->Current();

		if (m_dlist.size() < 10) {
			m_dlist.push_back(entity);
		}
		else {
			m_dlist.pop_front();
			m_dlist.push_back(entity);
		}

		for (int i = 0; i < m_dlist.size(); i++) {
			cout << m_dlist[i].id << m_dlist[i].line << endl;
		}
	}

private:
	string m_username;
	deque<Entity> m_hlist;
	deque<Entity> m_dlist;
	int m_cnt;
};