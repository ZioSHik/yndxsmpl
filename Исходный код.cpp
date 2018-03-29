#include <iostream> 
#include <string>
#include <stack>
#include <fstream>
#include <sstream>

using namespace std;
int getPrior(string a);
void parseStr(stack<string> &opz, string &oper, int &m, stack<int> &ch);
void FileLoad(string &str, char *f);
char eSynth[] = { '(', ')', ';', '[', ']' };

void main() {
	stack <string> opz;
	stack <int> ch;
	string str, oper;
	char f[256];
	cin >> f;
	FileLoad(str, f);
	cout << str << endl;
	bool flag;
	int m = 1;
	for (int i = 0; i <= str.length(); i++) {
		flag = true;
		if (str[i] != ' ') {
			for (int j = 0; j < strlen(eSynth); j++) {
				if (str[i] == eSynth[j]) {
					if (!oper.empty() && getPrior(oper) != -1) parseStr(opz, oper, m, ch);
					else cout << oper;
					oper.clear();
					parseStr(opz, string(1, str[i]), m, ch);
					flag = false;
					break;
				}
			}
			if (flag) oper += str[i];
		}
		else
		{
			if (!oper.empty() && getPrior(oper) != -1) parseStr(opz, oper, m, ch);
			else cout << oper;
			oper.clear();
		}
	}

	// IF const1 otn const2 THEN oper
	// ab>m1UPL...m2BPm1:... m2:
}

int getPrior(string a) {
	if (a == "if" || a == "begin" || a == "while" || a == "for" || a == "(" || a == "{" || a == "[") return 0;
	else if (a == "end" || a == "then" || a == "else" || a == "do" || a == ")" || a == "}" || a == "]" || a == ":=" || a == ";") return 1;
	else if (a == "goto") return 2;
	else if (a == "or") return 3;
	else if (a == "and") return 4;
	else if (a == "not") return 5;
	else if (a == "<=" || a == ">=" || a == ">" || a == "<" || a == "=") return 6;
	else if (a == "+" || a == "-") return 7;
	else if (a == "*" || a == "/" || a == "div" || a == "mod") return 8;
	else if (a == "^") return 9;
	else return -1;
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

void parseStr(stack<string> &opz, string &oper, int &m, stack<int> &ch) {
	string metka;
	if (oper == ")") {
		while (opz.top() != "(") {
			if (opz.top() != ";" && opz.top() != "if" && opz.top() != "then"  && opz.top() != "else" && opz.top() != "and") cout << opz.top();
			opz.pop();
		}
		opz.pop();
	}
	else if (oper == ";") {
		while (opz.top() != "begin" && opz.top() != "if") {
			if (opz.top() != ";" && opz.top() != "if" && opz.top() != "then"  && opz.top() != "else" && opz.top() != "and") cout << opz.top();
			opz.pop();
		}
		if (opz.top() == "if") {
			cout << "M" << ch.top() << ":";
			ch.pop();
			opz.pop();
		}
	}
	else if (oper == "end") {
		while (opz.top() != "begin") {
			cout << opz.top();
			opz.pop();
		}
		if (opz.top() == "if") {
			cout << "M" << ch.top() << ":";
			ch.pop();
			opz.pop();
		}
		opz.pop();
	}
	else if (oper == "else") {
		while (opz.top() != "then") {
			if (opz.top() != ";" && opz.top() != "if" && opz.top() != "then"  && opz.top() != "else" && opz.top() != "and") cout << opz.top();
			opz.pop();
		}
		cout << "M" << m << "[BP]" << "M" << ch.top() << ":";
		ch.pop();
		ch.push(m);
		m++;
		opz.pop();
	}
	else if (opz.empty() || getPrior(opz.top()) < getPrior(oper)) {
		opz.push(oper);
		if (opz.top() == "if") ch.push(m);
		if (opz.top() == "then") {
			cout << "M" << m << "[UPL]";
			m++;
		}
	}
	// if e1 then begin if e2 then s1 else s2 end;
	else
	{
		while (!opz.empty() && (getPrior(opz.top()) >= getPrior(oper)))
		{
			if (oper != "(" && oper != "if" && oper != "begin") {
				if (opz.top() != ";" && opz.top() != "if" && opz.top() != "then"  && opz.top() != "else" && opz.top() != "and") cout << opz.top();
				opz.pop();
			}
			else break;
		}
		opz.push(oper);
		if (opz.top() == "if") ch.push(m);
		if (opz.top() == "then") {
			cout << "M" << m << "[UPL]";
			m++;
		}
	}
}

void FileLoad(string &str, char *f) {
	ifstream fin(f);
	string buf;
	while (!fin.eof()) {
		fin >> buf;
		str += buf + " ";
	}
	fin.close();
}