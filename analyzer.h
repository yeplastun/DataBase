#ifndef DataBase_analyzer_h
#define DataBase_analyzer_h

#include "hash_set.h"
#include <sstream>
#include <set>
#include <typeinfo>

using std::string;
using std::stringstream;
using std::set;
using std::ostream;
using std::endl;

set<string> fields() {
	set<string> s;
	s.insert("name");
	s.insert("group");
	s.insert("rating");
	s.insert("info");
	return s;
}

set<string> types() {
	set<string> s;
	s.insert("select");
	s.insert("insert");
	s.insert("remove");
	s.insert("reselect");
	s.insert("print");
	s.insert("import");
	s.insert("export");
	return s;
}

void transformation(string& req) {
	for (int i = 0; i < (int)req.length() - 1; i++) {
		if (req[i] == '=') {
			if (req[i - 1] != ' ') {
				req.insert(i, " ");
				i++;
			}
			if (req[i + 1] != ' ') {
				req.insert(i + 1, " ");
				i++;
			}
		}
		if (req[i] == '-') {
			if (req[i - 1] != ' ') {
				req.insert(i, " ");
				i++;
			}
			if (req[i + 1] != ' ') {
				req.insert(i + 1, " ");
				i++;
			}
		}
		if (req[i] == ' ' && req[i + 1] == ' ') {
			req.erase(i, 1);
			i--;
		}
	}
	for (int i = 0; i < (int)req.length(); i++) {
		if (req[i] == '=') {
			int k = 0;
			for (int j = i; j < (int)req.length(); j++) {
				if (k == 2) {
					// std::cout << req[j] << std::endl;
					if (req[j] == '-') {
						i = j;
						j = (int)req.length();
					}
					else {
						req.insert(j, ",");
						j++;
						req.insert(j, " ");
						j++;
						i = j;
						j = (int)req.length();
					}
				}
				if (req[j] == ' ')
					k++;
			}
		}
	}
	// std::cout << req << endl;
}

template <typename T>
class cond {
public:
	T from;
	T to;
	int type_;
	bool empty_;
	bool empty() {
		return empty_;
	}
	int type() {
		return type_;
	}
	cond() {
		type_ = -1;
		empty_ = 1;
	}
	cond(stringstream& ss) {
		get_condition(ss);
	}
	~cond() {};
	bool get_condition(stringstream& ss) {
		string tmp;
		T buff;
		empty_ = 0;
		ss >> tmp;
		if (!ss)
			return 0;
		if (tmp[0] != '=')
			return 0;
		if (typeid(string) == typeid(T)) {
			ss >> buff;
			if (!ss)
				return 0;
			from = buff;
			char c;
			ss >> c;
			if (ss && c != ',')
				ss.unget();
			type_ = 0;
			return 1;
		}
		else {
			char c;
			ss >> c;
			if (c == '*') {
				ss >> tmp;
				if (!ss || tmp != "-")
					return 0;
				ss >> to;
				if (!ss)
					return 0;
				type_ = 3;
				return 1;
			}
			else {
				ss.unget();
				ss >> from;
				if (!ss)
					return 0;
				ss >> tmp;
				if (!ss) {
					type_ = 1;
					return 1;
				}
				if (tmp == "," || tmp == "end") {
					type_ = 1;
					return 1;
				}
				if (tmp != "-")
					return 0;
				ss >> c;
				if (!ss)
					return 0;
				if (c == '*') {
					type_ = 2;
					return 1;
				}
				else {
					ss.unget();
					ss >> to;
					if (!ss)
						return 0;
					type_ = 4;
					return 1;
				}
			}

		}
		return 1;
	}
};

class request {
public:
	int client_;
	string type_; // select reselect insert remove print
	list<string> print_fields_;
	string sort_field_;
	cond<string> name_;
	cond<int> group_;
	cond<double> rating_;
	cond<string> info_;
	cell insertStudent;
	string fileName;
	bool empty_;
	bool empty() {
		return empty_;
	}
	request() { empty_ = 1;};
	void clear() {
		type_ = "";
		print_fields_.clear();
		name_.empty_ = 1;
		group_.empty_ = 1;
		rating_.empty_ = 1;
		info_.empty_ = 1;
		empty_ = 2;
		fileName = "";
	}
	request(string req, int client) {
		if (get_request(req, client))
			empty_ = 0;
		else
			empty_ = 1;
	}
	bool get_request(string req, int client) {
		client_ = client;
		transformation(req);
		stringstream ss(req);
		string tmp;
		ss >> type_;
		if (!ss)
			return 0;
		if (type_ == "print") {
			while (ss && tmp != "sort" && tmp != "to") {
				ss >> tmp;
				if (fields().find(tmp) != fields().end())
					print_fields_.push_back(tmp);
				else
					return 0;
			}
			if (tmp == "sort") {
				ss >> tmp;
				if (!ss)
					return 0;
				if (fields().find(tmp) == fields().end())
					return 0;
				sort_field_ = tmp;
				ss >> tmp;
				if (!ss)
					return 1;
			}
			if (print_fields_.empty())
				return 0;
			return 1;
}
		if (type_ == "insert") {
			ss >> insertStudent;
			if (!ss)
				return 0;
			return 1;
		}
		if (type_ == "import") {
			ss >> fileName;
			if (!ss)
				return 0;
			return 1;
		}
		if (type_ == "export") {
			ss >> fileName;
			if (!ss)
				return 0;
			return 1;
		}
		if (types().find(type_) == types().end())
			return 0;
		while (ss) {
			ss >> tmp;
			if (!ss) {
				if (name_.empty_ == 1 && group_.empty_ == 1 &&
					rating_.empty_ == 1 && info_.empty_ == 1)
					return 0;
				else 
					return 1;
			}
			if (fields().find(tmp) != fields().end()) {
				if (tmp == "name")
					if (name_.get_condition(ss) == 0) {
						name_.empty_ = 1;
						return 0;
					}
				if (tmp == "group")
					if (group_.get_condition(ss) == 0) {
						group_.empty_ = 1;
						return 0;
					}
				if (tmp == "rating")
					if (rating_.get_condition(ss) == 0) {
						rating_.empty_ = 1;
						return 0;
					}
				if (tmp == "info")
					if (info_.get_condition(ss) == 0) {
						info_.empty_ = 1;
						return 0;
					}
			}
			else
				return 1;
		}
		return 1;
	}

	friend ostream& operator <<(ostream& out, request& req) {
		//if (req.empty())
		//	return out;
		out << "Request type: " << req.type_ << endl;
		if (req.type_ == "print") {
			out << "Fields: ";
			for (list<string>::const_iterator it = req.print_fields_.begin();
			        it != req.print_fields_.end(); it++)
				out << *it << ' ';
			out << endl;
			if (req.sort_field_.length() > 0)
				out << "Sort by: " << req.sort_field_ << endl;
			//if (req.output.length() > 0)
			//	out << "Print to: " << req.output << endl;
			return out;
		}
		if (req.name_.empty_ == 0) {
			out << "name" << ": " ;
			if (req.name_.type() == 0)
				out << req.name_.from << endl;
			else
				out << req.name_.from << " - " <<
				    req.name_.to << endl;
		}
		if (req.group_.empty_ == 0) {
			out << "group" << ": " ;
			if (req.group_.type() == 1)
				out << req.group_.from << endl;
			if (req.group_.type() == 2)
				out << req.group_.from << " - "
				    << '*' << endl;
			if (req.group_.type() == 3)
				out << '*' << " - "
				    << req.group_.to << endl;
			if (req.group_.type() == 4)
				out << req.group_.from << " - "
				    << req.group_.to << endl;
		}
		if (req.rating_.empty_ == 0) {
			out << "rating" << ": " ;
			if (req.rating_.type() == 1)
				out << req.rating_.from << endl;
			if (req.rating_.type() == 2)
				out << req.rating_.from << " - "
				    << '*' << endl;
			if (req.rating_.type() == 3)
				out << '*' << " - "
				    << req.rating_.to << endl;
			if (req.rating_.type() == 4)
				out << req.rating_.from << " - "
				    << req.rating_.to << endl;
		}
		if (req.info_.empty_ == 0) {
			out << "info" << ": " ;
			if (req.info_.type() == 0)
				out << req.info_.from << endl;
		}
		return out;
	}
};

#endif
