#ifndef DataBase_hash_set_h
#define DataBase_hash_set_h

#include <string.h>
#include <list>
#include <iostream>
#include <fstream>
#include <cmath>

#define HASH_SIZE 100000
#define eps 0.0001

using std::string;
using std::list;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::endl;

class cell {
  public:
	string name_;
	int group_;
	double rating_;
	string info_;
	cell() {
		group_ = 0;
		rating_ = 0.0;
	}
	cell(string name, int gr, double rate) {
		name_ = name;
		group_ = gr;
		rating_ = rate;
	}
	cell(string name, int gr, double rate, string str) {
		name_ = name;
		group_ = gr;
		rating_ = rate;
		info_ = str;
	}
	friend istream& operator >>(istream& in, cell& st) {
		string name;
		int gr;
		double rate;
		string info;
		in >> name >> gr >> rate >> info;
		st.name_ = name;
		st.group_ = gr;
		st.rating_ = rate;
		st.info_ = info;
		return in;
	}
	friend ostream& operator <<(ostream& out, cell& st) {
		out << st.name_ << ' ' << st.group_ << ' ';
		out << st.rating_ <<  ' ' << st.info_ << endl;
		return out;
	}
	bool operator ==(const cell& st) {
		if (!(name_ == st.name_))
			return 0;
		if (!(group_ == st.group_))
			return 0;
		if (!(fabs(rating_ - st.rating_) < eps))
			return 0;
		if (!(info_ == st.info_))
			return 0;
		return 1;
	}
	~cell() {}
};

typedef list<list<cell*> >::iterator list_it;
typedef list<cell*>::iterator cell_it;

class hash_set {
	friend class database;
  private:
	int size_;
	int real_size_;
	list<list<cell*> >* data_;
	list<int> groups_;
  public:
	static int HF(const int key, const int& size) {
		int res;
		res = key;
		res += ~(res << 16);
		res ^=  (res >>  5);
		res +=  (res <<  3);
		res ^=  (res >> 13);
		res += ~(res <<  9);
		res ^=  (res >> 17);
		return res % size;
	}
	hash_set() {
		size_ = HASH_SIZE;
		real_size_ = 0;
		data_ = new list<list<cell*> >[HASH_SIZE];
	};
	hash_set(int size) {
		size_ = size;
		data_ = new list<list<cell*> >[size];
	}
	bool add(cell* student) {
		int index = HF(student->group_, size_);
		for (list_it it = data_[index].begin(); it != data_[index].end(); it++) {
			if (it->front()->group_ == student->group_) {
				for (cell_it c_it = it->begin(); c_it != it->end(); c_it++) {
					if (*c_it == student)
						return 0;
				}
				it->push_back(student);
				real_size_++;
				return 1;
			}
		}
		groups_.push_back(student->group_);
		list<cell*> tmp;
		tmp.push_back(student);
		data_[index].push_back(tmp);
		real_size_++;
		return 1;
	}
	bool remove(cell& student) {
		int index = HF(student.group_, size_);
		for (list_it it = data_[index].begin();
		        it != data_[index].end(); it++) {
			if (it->front()->group_ == student.group_) {
				unsigned long size = it->size();
				for (cell_it c_it = it->begin(); c_it != it->end();
				        c_it++) {
					if (student == **c_it) {
						it->erase(c_it);
						c_it = it->begin();
						if (it->size() == 0)
							break;
					}
				}

				// it->remove(student);
				if (size != it->size()) {
					real_size_--;
					return 1;
				}
			}
		}
		return 0;
	}
	list<cell*>* get_group(int number) const  {
		list<cell*>* res;
		int index = HF(number, size_);
		for (list_it it = data_[index].begin(); it != data_[index].end(); it++)
			if (number == it->front()->group_)
				res = &(*it);
		return res;
	}
	int size() const {
		return real_size_;
	}
};

#endif
