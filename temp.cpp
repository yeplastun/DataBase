#include <stdio.h>
#include <iostream>

using namespace std;

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
						printed[req.client_].back() += (*it)->name_;
						//for (int i = 15 - (*it)->name_.length(); i >= 0; --i)
						//	printed[req.client_].back() += ' ';
					}
					if (*lit == "group") {
						char buff[16];
						sprintf(buff, "%d", (*it)->group_);
						printed[req.client_].back() += buff + ' ';
						//for (int i = 15 - strlen(buff); i >= 0; --i)
						//	printed[req.client_].back() += ' ';
					}
					if (*lit == "rating") {
						char buff[16];
						sprintf(buff, "%f", (*it)->rating_);
						printed[req.client_].back() += buff + ' ';
						//for (int i = 15 - strlen(buff); i >= 0; --i)
						//	printed[req.client_].back() += ' ';
					}
					if (*lit == "info") {
						printed[req.client_].back() += (*it)->info_ + ' ';
						//for (int i = 15 - (*it)->info_.length(); i >= 0; --i)
						//	printed[req.client_].back() += ' ';
					}
				}
			}
		}
		return;
	}


int main () {
	string tmp;
	getline(cin, tmp);
	cout << tmp << endl;
	return 0;
}