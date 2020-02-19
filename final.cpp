#include <iostream>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <exception>

using namespace std;

class Date {
public:
	Date(){
		m_year = 0;
		m_month = 1;
		m_day = 1;
	}
	Date(int year, int month, int day) {
		if (month > 12 || month < 1) {
			string error = "Month value is invalid: " + to_string(month);
			throw invalid_argument(error);
		}
		if (day > 31 || day < 1) {
			string error = "Day value is invalid: " + to_string(day);
			throw invalid_argument(error);		
		}
		m_year = year;
		m_month = month;
		m_day = day;
	}
	int GetYear() const{
		return m_year;
	};
	int GetMonth() const{
		return m_month;
	};
	int GetDay() const{
		return m_day;
	};
	int get_total() const{
	//	int tmp = total();
		return total();
	}

private:
	int m_year;
	int m_month;
	int m_day;
	int total() const {
		return m_day + m_month * 31 + m_year * 365;
	};
};

istream& operator >> (istream& is, Date& d) {
	int year, month, day;
	// if (is >> year && is.ignore(1) && is >> month && is.ignore(1) && is >> day) {
	// 	d = Date(year, month, day);
	// }
	
	string error, q;
	is >> error;
	stringstream tmp(error);
	tmp >> year;
	if (tmp.peek() != '-') {
		throw invalid_argument("Wrong date format: " + error);
	}
	tmp.ignore(1);
	tmp >> month;
	if (tmp.peek() != '-') {
		throw invalid_argument("Wrong date format: " + error);
	}
	tmp.ignore(1);
	if (tmp >> day) {
		d = Date(year, month, day);
	} else {
		throw invalid_argument("Wrong date format: " + error);
	}
	if (tmp >> q) {
		throw invalid_argument("Wrong date format: " + error);
	}
	return is;
}

ostream& operator << (ostream& os, const Date& d) {
	os << setw(4);
	os << setfill('0');
	return os << d.GetYear() << '-' << setw(2) << d.GetMonth() 
				<< '-' << setw(2) << d.GetDay();
}

bool operator < (const Date& lhs, const Date& rhs) {
	return lhs.get_total() < rhs.get_total();
};

bool operator == (const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() 
		&& lhs.GetDay() == rhs.GetDay()) {
		return true;
	} else {
		return false;
	}
}

class Database {
public:
	void check_null(const Date& date) {
		if (date_to_event[date].size() == 0) {
			map<Date, set<string>> :: iterator is_null;
			is_null = date_to_event.find(date);
			date_to_event.erase(is_null);
		}
	};

	void AddEvent(const Date& date, const string& event){
		date_to_event[date].insert(event);
		check_null(date);
	};
	bool DeleteEvent(const Date& date, const string& event){
		if (date_to_event[date].count(event)){
			date_to_event[date].erase(event);
			check_null(date);
			cout << "Deleted successfully" << endl;
			return true;
		} else {
			cout << "Event not found" << endl;
			return false;
		}
	};
	int DeleteDate(const Date& date){
		int n = date_to_event[date].size();
		date_to_event[date].clear();
		check_null(date);
		//cout << "Deleted " << n << " events" << endl;
		return n;
	};

	vector<string> Find(const Date& date) const{
		if (date_to_event.count(date) > 0) {
			vector<string> events;
			for (const auto& i : date_to_event.at(date)) {
				events.push_back(i);
			}
			sort(begin(events), end(events));
			return events;
		} else {
			return {};
		}
	};
	void Print() const{
		for (const auto& i : date_to_event) {
			for (const auto& j : i.second) {
				if (i.first.GetYear() >= 0) {
					cout << i.first << " " << j << endl;
				}
			}
		}
		//for (const auto& i : date_to_event) {
		//	string tmp_event;
		//	if (i.first.GetYear() >= 0) {
		//		for (const auto& j : i.second) {
		//				tmp_event += j + " ";
		//		}
		//	int length = tmp_event.length();
		//	tmp_event.resize(length - 1);
		//	cout << i.first << " " << tmp_event << endl;
		//	}	
		//}
	};
private:
	map<Date, set<string>> date_to_event;
};

int main() {
	Database db;
	string command;
		try { while (getline(cin, command)) {
					Date d;
					string tmp_event, op;
					stringstream tmp_stream(command);
					tmp_stream >> op;
					if (op == "Add") {
						try {
							if (tmp_stream >> d >> tmp_event){ 
								db.AddEvent(d, tmp_event);
							}
						} catch(invalid_argument& ex) {
							cout << ex.what() << endl;
							return -1;
						}
						
					}
					if (op == "Del") {
						if (tmp_stream >> d >> tmp_event) {
							if (db.DeleteEvent(d, tmp_event)) {
							}
						} else {
							int n = db.DeleteDate(d);
							cout << "Deleted " << n << " events\n";
						}
					}
					if (op == "Find") {
						tmp_stream >> d;
						vector<string> v = db.Find(d);
						for (auto& i : v) {
							cout << i << endl;
						}
						
					}
					if (op == "Print") {
						db.Print();
					}
					if (op != "Add" && op != "Del" && op != "Find" && op != "Print" && op != "") {
						cout << "Unknown command: " << op << endl;
					}
				}
		} catch(invalid_argument& ex) {
			cout << ex.what() << endl;
			return -1;
		}
  	return 0;
}