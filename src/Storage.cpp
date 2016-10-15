#include "Storage.hpp"
#include <iostream>
#include <cstdio>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <sstream>
using namespace std;

std::shared_ptr<Storage> Storage::m_instance = NULL;

Storage::Storage() {
	m_dirty = true;
	readFromFile();
}

Storage::~Storage() {
	sync();
}

std::shared_ptr<Storage> Storage::getInstance(void) {
	 if (m_instance == NULL) {
	 	m_instance = std::shared_ptr<Storage>(new Storage());
	 }
	 return m_instance;
}

void Storage::createUser(const User & t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
}

list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	list<User> result;
	list<User>::const_iterator iter;
	for (iter = m_userList.begin(); iter != m_userList.end(); iter++) {
		if (filter(*iter)) result.push_back(*iter);
	}
	return result;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
						std::function<void(User &)> switcher) {
	int count = 0;
	list<User>::iterator iter;
	for (iter = m_userList.begin(); iter != m_userList.end(); iter++) {
		if (filter(*iter)) {
			switcher(*iter);
			count++;
			m_dirty = true;
		}
	}
	return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int count = 0;
	list<User>::iterator iter;
	for (iter = m_userList.begin(); iter != m_userList.end();) {
		if (filter(*iter)) {
			iter = m_userList.erase(iter);
			count++;
			m_dirty = true;
		} else {
			iter++;
		}
	}
	return count;
}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
}

list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
	list<Meeting> result;
	list<Meeting>::const_iterator iter;
	for (iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
		if (filter(*iter)) result.push_back(*iter);
	}
	return result;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                 function<void(Meeting &)> switcher) {
	int count = 0;
	list<Meeting>::iterator iter;
	for (iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
		if (filter(*iter)) {
			switcher(*iter);
			count++;
			m_dirty = true;
		}
	}
	return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int count = 0;
	list<Meeting>::iterator iter;
	for (iter = m_meetingList.begin(); iter != m_meetingList.end();) {
		if (filter(*iter)) {
			iter = m_meetingList.erase(iter);
			count++;
			m_dirty = true;
		} else {
			iter++;
		}
	}
	return count;
}

bool Storage::readFromFile(void) {
	if (!m_dirty) return true;
	ifstream userfile, meetingfile;
	string buff;
	userfile.open("/home/Administrator/workspace/Agenda/data/users.csv");
	meetingfile.open("/home/Administrator/workspace/Agenda/data/meetings.csv");
	if (userfile) {
		m_userList.clear();
		while (getline(userfile, buff)) {
			string name = "", password = "", email = "", phone = "";
			int count = 0;
			buff += ',';
			while (buff[count] != ',') {
				name += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				password += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				email += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				phone += buff[count++];
			}
			User tmp(name.substr(1, name.length()-2),
				 password.substr(1, password.length()-2),
				 email.substr(1, email.length()-2),
				 phone.substr(1, phone.length()-2));
			m_userList.push_back(tmp);
			buff = "";
		}
		userfile.close();
	} else {
		return false;
	}
	if (meetingfile) {
		m_meetingList.clear();
		while (getline(meetingfile, buff)) {
			string sponsor = "", participators = "", startDate = "", endDate = "", title = "", str = "";
			vector<string> vec;
			int count = 0, counter = 0, flag = 0;
			buff += ',';
			while (buff[count] != ',') {
				sponsor += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				participators += buff[count++];
			}
			count++;
			for (int i = 0; i < participators.length(); i++) {
				if (participators[i] == '&') counter++;
			}
			if (counter == 0) {
				vec.push_back(participators.substr(1, participators.length()-2));
			} else {
				for (int i = 0; i < participators.length();) {
					if (participators[i] == '&') {
						flag++;
						if (flag == 1) {
							vec.push_back(str.substr(1, str.length()-1));
						} else {
							vec.push_back(str);
						}
						str = "";
						i++;
					} else {
						str += participators[i];
						i++;
					}
				}
				vec.push_back(str.substr(0, str.length()-1));
			}
			while (buff[count] != ',') {
				startDate += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				endDate += buff[count++];
			}
			count++;
			while (buff[count] != ',') {
				title += buff[count++];
			}
			Meeting m(sponsor.substr(1, sponsor.length()-2),
				vec,
				startDate.substr(1, startDate.length()-2),
				endDate.substr(1, endDate.length()-2),
				title.substr(1, title.length()-2));
			m_meetingList.push_back(m);
			buff = "";
		}
		meetingfile.close();
	} else {
		return false;
	}
	m_dirty = false;
	return true;
}

bool Storage::writeToFile(void) {
	if (!m_dirty) return true;
	ofstream userfile, meetingfile;
	userfile.open("/home/Administrator/workspace/Agenda/data/users.csv");
	meetingfile.open("/home/Administrator/workspace/Agenda/data/meetings.csv");
	if (userfile) {
		list<User>::iterator iter;
		for (iter = m_userList.begin(); iter != m_userList.end(); iter++) {
			userfile << '\"' << iter->getName() << '\"' << ','
			<< '\"' << iter->getPassword() << '\"' << ','
			<< '\"' << iter->getEmail() << '\"' << ','
			<< '\"' << iter->getPhone() << '\"' << '\n';
		}
		userfile.close();
	} else {
		return false;
	}
	if (meetingfile) {
		list<Meeting>::iterator iter2;
		for (iter2 = m_meetingList.begin(); iter2 != m_meetingList.end(); iter2++) {
			meetingfile << '\"' << iter2->getSponsor() << '\"' << ',' << '\"';
			for (int i = 0; i < iter2->getParticipator().size()-1; i++) {
				meetingfile << iter2->getParticipator()[i] << '&';
			}
			meetingfile << iter2->getParticipator()[iter2->getParticipator().size()-1] << '\"' << ','
			<< '\"' << iter2->getStartDate().dateToString(iter2->getStartDate()) << '\"' << ','
			<< '\"' << iter2->getEndDate().dateToString(iter2->getEndDate()) << '\"' << ','
			<< '\"' << iter2->getTitle() << '\"' << '\n';
		}
		meetingfile.close();
	} else {
		return false;
	}
	m_dirty = false;
	return true;
}

bool Storage::sync(void) {
	return writeToFile();
}
