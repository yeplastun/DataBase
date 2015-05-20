#ifndef DataBase_data_h
#define DataBase_data_h

#include "analyzer.h"
#include "tester.h"
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

#define EPS 0.00001

using std::map;
using std::multimap;
using std::string;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::list;
using std::vector;
using std::sort;
using std::find;

void reverse(string s) {
	unsigned long int i, j;
	char c;
	for (i = 0, j = s.size() - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	return;
}

string itoa(int n) {
	string s;
	s.clear();
	char c;
	int i = 0, sign;
	(void)i;
	(void) sign;
	do {
		c = n % 10 + '0';
		s.push_back(c);
	} while ((n /= 10) > 0);
	c = '\0';
	s.push_back(c);
	reverse(s);
	return s;
}


bool cmp_name (cell* first, cell* second) {
	if (first->name_ > second->name_)
		return 1;
	else
		return 0;
}
bool cmp_group (cell* first, cell* second) {
	if (first->group_ > second->group_)
		return 1;
	else
		return 0;
}
bool cmp_rating (cell* first, cell* second) {
	if (first->rating_ > second->rating_)
		return 1;
	else
		return 0;
}
bool cmp_info (cell* first, cell* second) {
	if (first->info_ > second->info_)
		return 1;
	else
		return 0;
}

bool cmp (int a, int b) {
	if (a > b)
		return 1;
	else
		return 0;
}

bool p_name (cell* first, cell* second) {
	if (first->name_ >= second->name_)
		return 0;
	else
		return 1;

}

bool p_group (cell* first, cell* second) {
	if (first->group_ >= second->group_)
		return 0;
	else
		return 1;

}

bool p_rating (cell* first, cell* second) {
	if (first->rating_ >= second->rating_)
		return 0;
	else
		return 1;

}

bool p_info (cell* first, cell* second) {
	if (first->info_ >= second->info_)
		return 0;
	else
		return 1;

}

class database {
private:
	multimap<string, cell*> name_base_;
	multimap<double, cell*> rating_base_;
	list<cell> data_;
	// vector<cell*> clients_[req.client_];
	hash_set hash_base_;
	int size_;
	map<int, vector<cell*> > clients_;
public:
	map<int, vector<string> > printed;
	int temp_size(int client) {
		return int(clients_[client].size());
	}
	database() {
		size_ = 0;
	}
	database(istream& in) {
		import(in);
	}
	bool import(istream& in) {
		cell tmp;
		while (!in.eof()) {
			in >> tmp;
			if (!in)
				return 0;
			add(tmp);
		}
		return 1;
	};
	list<cell*>* get_group(int number) const {
		list<cell*>* res;
		res = hash_base_.get_group(number);
		return res;
	}
	int size() const {
		return int(rating_base_.size());
	}
	void add(cell& tmp) {
		++size_;
		data_.push_back(tmp);
		name_base_.insert
		(pair<string, cell*>(tmp.name_, &data_.back()));
		rating_base_.insert
		(pair<double, cell*>(tmp.rating_, &data_.back()));
		hash_base_.add(&data_.back());
	}
	bool remove(cell& tmp) {
		list<cell>::iterator it =
		    find(data_.begin(), data_.end(), tmp);
		if (it == data_.end())
			return 0;
		--size_;
		hash_base_.remove(tmp);
		multimap<string, cell*>::iterator it_name =
		    name_base_.find(tmp.name_);
		while (it_name->second->name_ == tmp.name_) {
			if (tmp == *(it_name->second)) {
				name_base_.erase(it_name);
				break;
			}
			++it_name;
		}
		multimap<double, cell*>::iterator it_rate =
		    rating_base_.find(tmp.rating_);
		while (fabs(it_name->second->rating_ - tmp.rating_) < EPS) {
			if (tmp == *(it_rate->second)) {
				rating_base_.erase(it_rate);
				break;
			}
			++it_rate;
		}
		data_.erase(it);
		return 1;
	}
	void Execute(request req) {
		if (req.type_ == "select") {
			Select(req);
		}
		if (req.type_ == "reselect") {
			Reselect(req);
		}
		if (req.type_ == "remove") {
			Remove(req);
		}
		if (req.type_ == "insert") {
			Insert(req); // добавить тип запроса
		}
		if (req.type_ == "import") {
			Import(req); // добавить тип запроса в анализатор
		}
		if (req.type_ == "print") {
			Print(req);
		}
		if (req.type_ == "export") {
			Export(req);
		}
		if (req.type_ == "insert") {
			Insert(req);
		}
	}
	void DeleteClient(int n) {
		clients_[n].clear();
		clients_.erase(n);
		return;
	}
	void Select(request req) {
		// vector<cell*> temp
		// порядок проверки name -> group -> rating -> info
		if (!req.name_.empty() &&
		        req.name_.from[req.name_.from.length() - 1] != '*') {
			for (multimap<string, cell*>::iterator it =
			            name_base_.find(req.name_.from); it !=
			        name_base_.end() && (*it->second).name_ ==
			        req.name_.from; ++it) {
				if (!req.group_.empty()) {
					if (req.group_.type() == 1)
						if (req.group_.from != it->second->group_)
							continue;
					if (req.group_.type() == 2)
						if (req.group_.from >= it->second->group_)
							continue;
					if (req.group_.type() == 3)
						if (req.group_.to <= it->second->group_)
							continue;
					if (req.group_.type() == 4)
						if (req.group_.from >= it->second->group_ ||
						        req.group_.to <= it->second->group_)
							continue;
				}
				if (!req.rating_.empty()) {
					if (req.rating_.type() == 1)
						if (fabs(req.rating_.from - it->second->rating_) >= EPS)
							continue;
					if (req.group_.type() == 2)
						if (req.rating_.from >= it->second->rating_)
							continue;
					if (req.rating_.type() == 3)
						if (req.rating_.to <= it->second->rating_)
							continue;
					if (req.rating_.type() == 4)
						if (req.rating_.from >= it->second->rating_ ||
						        req.rating_.to <= it->second->rating_)
							continue;
				}
				if (!req.info_.empty()) {
					if (req.info_.from[req.info_.from.size() - 1]
					        == '*') {
						if (req.info_.from.substr(0, req.info_.from.size() - 1)
						        != (*it->second).info_.substr(0, req.info_.from.size() - 1))
							continue;
					}
					else {
						if (req.info_.from != (*it->second).info_)
							continue;
					}
				}
				clients_[req.client_].push_back(it->second);
			}
			return;
		}
		if (!req.group_.empty()) {
			if (req.group_.type() == 1) {
				if (find(hash_base_.groups_.begin(), hash_base_.groups_.end(),
				         req.group_.from) == hash_base_.groups_.end())
					return;
				list<cell*>* buff =
				    hash_base_.get_group(req.group_.from);
				if (buff == NULL)
					return;
				for (list<cell*>::iterator it = buff->begin();
				        it != buff->end(); ++it) {
					if (!req.name_.empty()) {
						if (req.name_.from[req.name_.from.length() - 1]
						        == '*') {
							if (req.name_.from.substr(0, req.name_.from.size() - 1)
							        != (*it)->name_.substr(0, req.name_.from.size() - 1))
								continue;
						}
						else {
							if (req.name_.from != (*it)->name_)
								continue;
						}
					}
					if (!req.info_.empty()) {
						if (req.info_.from[req.info_.from.length() - 1]
						        == '*') {
							if (req.info_.from.substr(0, req.info_.from.size() - 1)
							        != (*it)->info_.substr(0, req.info_.from.size() - 1))
								continue;
						}
						else {
							if (req.info_.from != (*it)->info_)
								continue;
						}
					}
					if (!req.rating_.empty()) {
						if (req.rating_.type() == 1)
							if (fabs(req.rating_.from - (*it)->rating_) > EPS)
								continue;
						if (req.group_.type() == 2)
							if (req.rating_.from >= (*it)->rating_)
								continue;
						if (req.rating_.type() == 3)
							if (req.rating_.to <= (*it)->rating_)
								continue;
						if (req.rating_.type() == 4)
							if (req.rating_.from >= (*it)->rating_ ||
							        req.rating_.to <= (*it)->rating_)
								continue;
					}
					clients_[req.client_].push_back(*it);

				}
			}
			if (req.group_.type() == 2) {
				hash_base_.groups_.sort();
				for (list<int>::iterator gr_it = find(hash_base_.groups_.begin(), hash_base_.groups_.end(), req.group_.from);
				        gr_it != hash_base_.groups_.end(); gr_it++) {
					list<cell*>* buff = hash_base_.get_group(*gr_it);
					if (buff == NULL)
						continue;
					for (list<cell*>::iterator it = buff->begin();
					        it != buff->end(); ++it) {
						if (!req.name_.empty()) {
							if (req.name_.from[req.name_.from.length() - 1]
							        == '*') {
								if (req.name_.from.substr(0, req.name_.from.size() - 1)
								        != (*it)->name_.substr(0, req.name_.from.size() - 1))
									continue;
							}
							else {
								if (req.name_.from != (*it)->name_)
									continue;
							}
						}
						if (!req.info_.empty()) {
							if (req.info_.from[req.info_.from.length() - 1]
							        == '*') {
								if (req.info_.from.substr(0, req.info_.from.size() - 1)
								        != (*it)->info_.substr(0, req.info_.from.size() - 1))
									continue;
							}
							else {
								if (req.info_.from != (*it)->info_)
									continue;
							}
						}
						if (!req.rating_.empty()) {
							if (req.rating_.type() == 1)
								if (fabs(req.rating_.from - (*it)->rating_) > EPS)
									continue;
							if (req.group_.type() == 2)
								if (req.rating_.from >= (*it)->rating_)
									continue;
							if (req.rating_.type() == 3)
								if (req.rating_.to <= (*it)->rating_)
									continue;
							if (req.rating_.type() == 4)
								if (req.rating_.from >= (*it)->rating_ ||
								        req.rating_.to <= (*it)->rating_)
									continue;
						}
						clients_[req.client_].push_back(*it);
					}
				}
			}
			if (req.group_.type() == 3) {
				hash_base_.groups_.sort(cmp);
				for (list<int>::iterator gr_it = find(hash_base_.groups_.begin(), hash_base_.groups_.end(), req.group_.to);
				        gr_it != hash_base_.groups_.end(); gr_it++) {
					list<cell*>* buff = hash_base_.get_group(*gr_it);
					if (buff == NULL)
						continue;
					for (list<cell*>::iterator it = buff->begin();
					        it != buff->end(); ++it) {
						if (!req.name_.empty()) {
							if (req.name_.from[req.name_.from.length() - 1]
							        == '*') {
								if (req.name_.from.substr(0, req.name_.from.size() - 1)
								        != (*it)->name_.substr(0, req.name_.from.size() - 1))
									continue;
							}
							else {
								if (req.name_.from != (*it)->name_)
									continue;
							}
						}
						if (!req.info_.empty()) {
							if (req.info_.from[req.info_.from.length() - 1]
							        == '*') {
								if (req.info_.from.substr(0, req.info_.from.size() - 1)
								        != (*it)->info_.substr(0, req.info_.from.size() - 1))
									continue;
							}
							else {
								if (req.info_.from != (*it)->info_)
									continue;
							}
						}
						if (!req.rating_.empty()) {
							if (req.rating_.type() == 1)
								if (fabs(req.rating_.from - (*it)->rating_) > EPS)
									continue;
							if (req.group_.type() == 2)
								if (req.rating_.from >= (*it)->rating_)
									continue;
							if (req.rating_.type() == 3)
								if (req.rating_.to <= (*it)->rating_)
									continue;
							if (req.rating_.type() == 4)
								if (req.rating_.from >= (*it)->rating_ ||
								        req.rating_.to <= (*it)->rating_)
									continue;
						}
						clients_[req.client_].push_back(*it);
					}
				}
			}
			if (req.group_.type() == 4) {
				hash_base_.groups_.sort();
				for (list<int>::iterator gr_it = find(hash_base_.groups_.begin(), hash_base_.groups_.end(), req.group_.from);
				        gr_it != hash_base_.groups_.end() && *gr_it <= req.group_.to; gr_it++) {
					list<cell*>* buff = hash_base_.get_group(*gr_it);
					if (buff == NULL)
						continue;
					for (list<cell*>::iterator it = buff->begin();
					        it != buff->end(); ++it) {
						if (!req.name_.empty()) {
							if (req.name_.from[req.name_.from.length() - 1]
							        == '*') {
								if (req.name_.from.substr(0, req.name_.from.size() - 1)
								        != (*it)->name_.substr(0, req.name_.from.size() - 1))
									continue;
							}
							else {
								if (req.name_.from != (*it)->name_)
									continue;
							}
						}
						if (!req.info_.empty()) {
							if (req.info_.from[req.info_.from.length() - 1]
							        == '*') {
								if (req.info_.from.substr(0, req.info_.from.size() - 1)
								        != (*it)->info_.substr(0, req.info_.from.size() - 1))
									continue;
							}
							else {
								if (req.info_.from != (*it)->info_)
									continue;
							}
						}
						if (!req.rating_.empty()) {
							if (req.rating_.type() == 1)
								if (fabs(req.rating_.from - (*it)->rating_) > EPS)
									continue;
							if (req.group_.type() == 2)
								if (req.rating_.from >= (*it)->rating_)
									continue;
							if (req.rating_.type() == 3)
								if (req.rating_.to <= (*it)->rating_)
									continue;
							if (req.rating_.type() == 4)
								if (req.rating_.from >= (*it)->rating_ ||
								        req.rating_.to <= (*it)->rating_)
									continue;
						}
						clients_[req.client_].push_back(*it);
					}
				}
			}
			//after all types description return should be done
			return;
		}
		if (!req.rating_.empty()) {
			if (req.rating_.type() == 1) {
				for (multimap<double, cell*>::iterator it = rating_base_.find(req.rating_.from);
				        it != rating_base_.end() && fabs(it->first - req.rating_.from) < eps; ++it) {
					if (!req.name_.empty()) {
						if (req.name_.from[req.name_.from.length() - 1]
						        == '*') {
							if (req.name_.from.substr(0, req.name_.from.size() - 1)
							        != (it->second->name_.substr(0, req.name_.from.size() - 1)))
								continue;
						}
						else {
							if (req.name_.from != it->second->name_)
								continue;
						}
					}
					if (!req.info_.empty()) {
						if (req.info_.from[req.info_.from.length() - 1]
						        == '*') {
							if (req.info_.from.substr(0, req.info_.from.size() - 1)
							        != (it->second->info_.substr(0, req.info_.from.size() - 1)))
								continue;
						}
						else {
							if (req.info_.from != it->second->info_)
								continue;
						}
					}
					clients_[req.client_].push_back(it->second);
				}
			}
			if (req.rating_.type() == 2) {
				for (multimap<double, cell*>::iterator it = --rating_base_.end();
				        it->first >= req.rating_.from; --it) {
					if (!req.name_.empty()) {
						if (req.name_.from[req.name_.from.length() - 1]
						        == '*') {
							if (req.name_.from.substr(0, req.name_.from.size() - 1)
							        != (it->second->name_.substr(0, req.name_.from.size() - 1))) {
								if (it == rating_base_.begin()) {
									break;
								}
								continue;
							}
						}
						else {
							if (req.name_.from != it->second->name_) {
								if (it == rating_base_.begin()) {
									break;
								}
								continue;
							}
						}
					}
					if (!req.info_.empty()) {
						if (req.info_.from[req.info_.from.length() - 1]
						        == '*') {
							if (req.info_.from.substr(0, req.info_.from.size() - 1)
							        != (it->second->info_.substr(0, req.info_.from.size() - 1))) {
								if (it == rating_base_.begin()) {
									break;
								}
								continue;
							}
						}
						else {
							if (req.info_.from != it->second->info_) {
								if (it == rating_base_.begin()) {
									break;
								}
								continue;
							}
						}
					}
					clients_[req.client_].push_back(it->second);
					if (it == rating_base_.begin()) {
						break;
					}
				}
			}
			if (req.rating_.type() == 3) {
				for (multimap<double, cell*>::iterator it = rating_base_.begin();
				        it != rating_base_.end() && it->first <= req.rating_.to; ++it) {
					if (!req.name_.empty()) {
						if (req.name_.from[req.name_.from.length() - 1]
						        == '*') {
							if (req.name_.from.substr(0, req.name_.from.size() - 1)
							        != (it->second->name_.substr(0, req.name_.from.size() - 1)))
								continue;
						}
						else {
							if (req.name_.from != it->second->name_)
								continue;
						}
					}
					if (!req.info_.empty()) {
						if (req.info_.from[req.info_.from.length() - 1]
						        == '*') {
							if (req.info_.from.substr(0, req.info_.from.size() - 1)
							        != (it->second->info_.substr(0, req.info_.from.size() - 1)))
								continue;
						}
						else {
							if (req.info_.from != it->second->info_)
								continue;
						}
					}
					clients_[req.client_].push_back(it->second);
				}
			}
			if (req.rating_.type() == 4) {
				for (multimap<double, cell*>::iterator it = rating_base_.begin();
				        it != rating_base_.end() && it->first <= req.rating_.to; ++it) {
					if (!req.name_.empty()) {
						if (req.name_.from[req.name_.from.length() - 1]
						        == '*') {
							if (req.name_.from.substr(0, req.name_.from.size() - 1)
							        != (it->second->name_.substr(0, req.name_.from.size() - 1)))
								continue;
						}
						else {
							if (req.name_.from != it->second->name_)
								continue;
						}
					}
					if (!req.info_.empty()) {
						if (req.info_.from[req.info_.from.length() - 1]
						        == '*') {
							if (req.info_.from.substr(0, req.info_.from.size() - 1)
							        != (it->second->info_.substr(0, req.info_.from.size() - 1)))
								continue;
						}
						else {
							if (req.info_.from != it->second->info_)
								continue;
						}
					}
					clients_[req.client_].push_back(it->second);
				}
			}
			//after all types description return should be done
			return;
		}
		if (!req.info_.empty()) {
			for (list<cell>::iterator it = data_.begin(); it != data_.end(); ++it) {
				if (req.info_.from[req.info_.from.size() - 1]
				        == '*') {
					if (req.info_.from.substr(0, req.info_.from.size() - 1)
					        != it->info_.substr(0, req.info_.from.size() - 1))
						continue;
				}
				else {
					if (req.info_.from != it->info_)
						continue;
				}
				if (!req.name_.empty()) {
					if (req.name_.from.substr(0, req.name_.from.size() - 1)
					        != it->name_.substr(0, req.name_.from.size() - 1))
						continue;
				}
				clients_[req.client_].push_back(&*it);
			}
		}
		if (!req.name_.empty()) {
			for (list<cell>::iterator it = data_.begin(); it != data_.end(); ++it) {
				if (!req.name_.empty()) {
					if (req.name_.from.substr(0, req.name_.from.size() - 1)
					        != it->name_.substr(0, req.name_.from.size() - 1))
						continue;
				}
				clients_[req.client_].push_back(&*it);
			}
		}
	}
	void Reselect(request req) {
		if (clients_[req.client_].size() == 0)
			return;
		for (vector<cell*>::iterator it = clients_[req.client_].begin();
		        it != clients_[req.client_].end(); ++it) {
			if (!req.name_.empty()) {
				if (req.name_.from[req.name_.from.length() - 1] != '*') {
					if ((*it)->name_ != req.name_.from) {
						clients_[req.client_].erase(it);
						// if (it != clients_[req.client_].begin())
						it--;
						continue;
					}
				}
				else {
					if (req.name_.from.substr(0, req.name_.from.size() - 1)
					        != ((*it)->name_.substr(0, req.name_.from.size() - 1))) {
						clients_[req.client_].erase(it);
						// if (it != clients_[req.client_].begin())
						--it;
						continue;
					}
				}
			}
			if (!req.info_.empty()) {
				if (req.info_.from[req.info_.from.length() - 1] != '*') {
					if ((*it)->info_ != req.info_.from) {
						clients_[req.client_].erase(it);
						// if (it != clients_[req.client_].begin())
						it--;
						continue;
					}
				}
				else {
					if (req.info_.from.substr(0, req.info_.from.size() - 1)
					        != ((*it)->info_.substr(0, req.info_.from.size() - 1))) {
						clients_[req.client_].erase(it);
						// if (it != clients_[req.client_].begin())
						--it;
						continue;
					}
				}
			}
			if (!req.group_.empty()) {
				if (req.group_.type() == 1) {
					if (req.group_.from != (*it)->group_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.group_.type() == 2) {
					if (req.group_.from > (*it)->group_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.group_.type() == 3) {
					if (req.group_.to < (*it)->group_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.group_.type() == 4) {
					if (req.group_.from > (*it)->group_
					        || req.group_.to < (*it)->group_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
			}
			if (!req.rating_.empty()) {
				if (req.rating_.type() == 1) {
					if (fabs(req.rating_.from - (*it)->rating_) > EPS) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.rating_.type() == 2) {
					if (req.rating_.from > (*it)->rating_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.rating_.type() == 3) {
					if (req.rating_.to < (*it)->rating_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
				if (req.rating_.type() == 4) {
					if (req.rating_.from > (*it)->rating_
					        || req.rating_.to < (*it)->rating_) {
						clients_[req.client_].erase(it);
						--it;
						continue;
					}
				}
			}

		}
	}
	bool Remove(request req) {
		clients_[req.client_].clear();
		Select(req);
		for (vector<cell*>::iterator it = clients_[req.client_].begin(); it != clients_[req.client_].end(); ++it) {
			remove(**it);
		}
		clients_[req.client_].clear();
		return 1;
	}
	void Insert(request req) {
		add(req.insertStudent);
	}
	bool Import(request req) {
		ifstream in(req.fileName.c_str());
		return import(in);
	}
	void Export(request req) {
		ofstream out(req.fileName.c_str());
		if (clients_[req.client_].empty()) {
			for (list<cell>::iterator it = data_.begin();
			        it != data_.end(); ++it)
				out << *it;
		}
		else {
			for (unsigned int i = 0; i < clients_[req.client_].size(); ++i)
				out << *clients_[req.client_][i];
		}
	}
	void Print(request req) {
		if (clients_[req.client_].empty())
			for (list<cell>::iterator it = data_.begin();
			        it != data_.end(); ++it)
				clients_[req.client_].push_back(&*it);
		if (!clients_[req.client_].empty()) {
			if (req.sort_field_ == "name")
				sort(clients_[req.client_].begin(), clients_[req.client_].end(), p_name);
			if (req.sort_field_ == "group")
				sort(clients_[req.client_].begin(), clients_[req.client_].end(), p_group);
			if (req.sort_field_ == "rating")
				sort(clients_[req.client_].begin(), clients_[req.client_].end(), p_rating);
			if (req.sort_field_ == "info")
				sort(clients_[req.client_].begin(), clients_[req.client_].end(), p_info);
			for (vector<cell*>::iterator it = clients_[req.client_].begin();
			        it != clients_[req.client_].end(); ++it) {
				string tmp = "";
				printed[req.client_].push_back(tmp);
				for (list<string>::iterator lit = req.print_fields_.begin();
				        lit != req.print_fields_.end(); ++lit) {
					if (*lit == "name") {
						printed[req.client_].back() += ((*it)->name_);
						printed[req.client_].back() += ' ';
						for (int i = 15 - (*it)->name_.length(); i >= 0; --i)
							printed[req.client_].back() += ' ';

					}
					if (*lit == "group") {
						char buff[16];
						sprintf(buff, "%d", (*it)->group_);
						printed[req.client_].back() += (buff);
						printed[req.client_].back() += ' ';
						for (int i = 15 - strlen(buff); i >= 0; --i)
							printed[req.client_].back() += ' ';


					}
					if (*lit == "rating") {
						char buff[16];
						sprintf(buff, "%f", (*it)->rating_);
						printed[req.client_].back() += (buff);
						printed[req.client_].back() += ' ';
						for (int i = 15 - strlen(buff); i >= 0; --i)
							printed[req.client_].back() += ' ';

					}
					if (*lit == "info") {
						printed[req.client_].back() += ((*it)->info_);
						printed[req.client_].back() += ' ';
						for (int i = 15 - (*it)->info_.length(); i >= 0; --i)
							printed[req.client_].back() += ' ';

					}
				}
				char cTmp = 0;
				printed[req.client_].back() += cTmp;
			}
		}
		return;
	}
};

#endif