#include "AgendaService.hpp"
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <functional>
#include <memory>
#include <vector>
using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const string userName, const string password) {
	list<User> ret;
	auto f = [&] (const User &user) {
		return user.getName() == userName && user.getPassword() == password;
	};
	ret = m_storage->queryUser(f);
	if (!ret.empty()) {
		return true;
	} else {
		return false;
	}
}

bool AgendaService::userRegister(const string userName, string password,
                      			   const string email, string phone) {
	list<User> ret;
	auto f = [&] (const User &user) {
		return user.getName() == userName;
	};
	ret = m_storage->queryUser(f);
	if (!ret.empty()) {
		return false;
	} else {
		User temp(userName, password, email, phone);
		m_storage->createUser(temp);
		return true;
	}
}

bool AgendaService::deleteUser(const string userName, const string password) {
	int count = 0;
	auto f1 = [&] (const User &user) {
		return (user.getName() == userName && user.getPassword() == password);
	};
	count = m_storage->deleteUser(f1);
	if (count != 0) {
		auto f2 = [&] (const Meeting &meeting) {
			return meeting.getSponsor() == userName;
		};
		int temp1 = m_storage->deleteMeeting(f2);
		auto f3 = [&] (const Meeting &meeting) {
			vector<string> vec;
			vec = meeting.getParticipator();
			for (int i = 0; i < vec.size(); i++) {
				if (vec[i] == userName) {
					return true;
				}
			}
			return false;
		};
		int temp = m_storage->deleteMeeting(f3);
		return true;
	} else {
		return false;
	}
}

list<User> AgendaService::listAllUsers(void) const {
	list<User> ret;
	auto f = [&] (const User &user) {
		if (user.getName() != "") {
			return true;
		} else {
			return false;
		}
	};
	ret = m_storage->queryUser(f);
	return ret;
}

bool AgendaService::createMeeting(const string userName, const string title,
                   				  const string startDate, const string endDate,
                   				  const vector<string> participator) {
	Date transfer1(startDate);
	Date transfer2(endDate);
	Date wrong(0, 0, 0, 0, 0);
	if (transfer1 == wrong || transfer2 == wrong) return false;
	if (Date::isValid(transfer1) == false || Date::isValid(transfer2) == false) return false;
	if (transfer1 >= transfer2) return false;
	for (int i = 0; i < participator.size(); i++) {
		if (userName == participator[i]) return false;
	}
	auto f1 = [&] (const User &user) {
		if (user.getName() == userName) {
			return true;
		}
		for (int i = 0; i < participator.size(); i++) {
			if (participator[i] == user.getName()) {
				return true;
			}
		}
		return false;
	};
	list<User> temp = m_storage->queryUser(f1);
	if (temp.size() != (participator.size() + 1) || startDate == endDate) {
		return false;
	}
	auto f2 = [&] (const Meeting &meeting) {
		if ((meeting.getStartDate() >= startDate && meeting.getStartDate() < endDate) ||
		   (meeting.getEndDate() > startDate && meeting.getEndDate() <= endDate) ||
		   (meeting.getStartDate() <= startDate && meeting.getEndDate() >= endDate)) {
			return true;
		} else {
			return false;
		}
	};
	list<Meeting> set = m_storage->queryMeeting(f2);
	list<Meeting>::iterator iter;
	for (iter = set.begin(); iter != set.end(); iter++) {
		for (int i = 0; i < iter->getParticipator().size(); i++) {
			if (iter->getParticipator()[i] == userName) {
				return false;
			}
			if (iter->getSponsor() == userName) return false;
			for (int i = 0; i < participator.size(); i++) {
				if (participator[i] == iter->getSponsor()) return false;
			}
			for (int i = 0; i < participator.size(); i++) {
				for (int j = 0; j < iter->getParticipator().size(); j++) {
					if (participator[i] == iter->getParticipator()[j]) return false;
				}
			}
 		}
	}
	auto f3 = [&] (const Meeting &meeting) {
		if (meeting.getTitle() == title) {
			return true;
		} else {
			return false;
		}
	};
	list<Meeting> titleset = m_storage->queryMeeting(f3);
	if (titleset.size() != 0) return false;
	Meeting ret(userName, participator, startDate, endDate, title);
	m_storage->createMeeting(ret);
	return true;
}

list<Meeting> AgendaService::meetingQuery(const string userName,
                                    	  const string title) const {
	auto f1 = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName && meeting.getTitle() == title) {
			return true;
		}
		for (int i = 0; i < meeting.getParticipator().size(); i++) {
			if (userName == meeting.getParticipator()[i] && title == meeting.getTitle()) {
				return true;
			}
		}
		return false;
	};
	list<Meeting> temp = m_storage->queryMeeting(f1);
	return temp;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    		   const std::string startDate,
                                    		   const std::string endDate) const {
  Date transfer1(startDate);
  Date transfer2(endDate);
  Date wrong(0,0,0,0,0);
	if (transfer1 == wrong || transfer2 == wrong || Date::isValid(transfer1) == false ||
       Date::isValid(transfer2) == false || transfer1 > transfer2) {
		list<Meeting> kong;
		return kong;
	}
	auto f1 = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName &&
		   ((meeting.getStartDate() >= startDate && meeting.getStartDate() <= endDate) ||
		   (meeting.getEndDate() >= startDate && meeting.getEndDate() <= endDate) || (meeting.getStartDate() <= startDate && meeting.getEndDate() >= endDate))) {
			return true;
		}
		for (int i = 0; i < meeting.getParticipator().size(); i++) {
			if (meeting.getParticipator()[i] == userName && ((meeting.getStartDate() >= startDate && meeting.getStartDate() <= endDate) ||
		   (meeting.getEndDate() >= startDate && meeting.getEndDate() <= endDate) || (meeting.getStartDate() <= startDate && meeting.getEndDate() >= endDate))) {
				return true;
			}
		}
		return false;
	};
	list<Meeting> temp = m_storage->queryMeeting(f1);
	return temp;
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
	auto f = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName) {
			return true;
		};
		for (int i = 0; i < meeting.getParticipator().size(); i++) {
			if (meeting.getParticipator()[i] == userName) {
				return true;
			}
		}
		return false;
	};
	list<Meeting> temp = m_storage->queryMeeting(f);
	return temp;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
	auto f = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName) {
			return true;
		} else {
			return false;
		}
	};
	list<Meeting> temp = m_storage->queryMeeting(f);
	return temp;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string userName) const {
	auto f = [&] (const Meeting &meeting) {
		for (int i = 0; i < meeting.getParticipator().size(); i++) {
			if (meeting.getParticipator()[i] == userName) {
				return true;
			}
		}
		return false;
	};
	list<Meeting> temp = m_storage->queryMeeting(f);
	return temp;
}

bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
	auto f = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName && meeting.getTitle() == title) {
			return true;
		} else {
			return false;
		}
	};
	int count = m_storage->deleteMeeting(f);
	if (count != 0) {
		return true;
	} else {
		return false;
	}
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
	auto f = [&] (const Meeting &meeting) {
		if (meeting.getSponsor() == userName) {
			return true;
		} else {
			return false;
		}
	};
	int count = m_storage->deleteMeeting(f);
	if (count != 0) {
		return true;
	} else {
		return false;
	}
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

    /**
     * quit Agenda service
     */
void AgendaService::quitAgenda(void) {
	m_storage->sync();
}
