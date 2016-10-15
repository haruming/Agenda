#include "AgendaUI.hpp"
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <functional>
#include <memory>
#include <vector>
#include <iomanip>
using namespace std;

	AgendaUI::AgendaUI() {
        m_agendaService.startAgenda();
    }

	void AgendaUI::OperationLoop(void) {
		startAgenda();
        while (executeOperation(getOperation())) {}
        quitAgenda();
	}

    void AgendaUI::startAgenda(void) {
    	m_agendaService.startAgenda();
    }

    /**
     * catch user's operation
     * @return the operation
     */
    std::string AgendaUI::getOperation() {
        if (m_userName == "") {
            for (int i = 0; i < 30; i++) {
                cout << '-';
            }
            cout << " Agenda ";
            for (int i = 0; i < 30; i++) {
                cout << '-';
            }
            cout << '\n';
            cout << "Action :\n";
            cout << "l   - log in Agenda by user name and password\n";
            cout << "r   - register an Agenda account\n";
            cout << "q   - quit Agenda\n";
            for (int i = 0; i < 68; i++) {
                cout << '-';
            }
            cout << endl << endl;
            cout << "Agenda : ~$ ";
            string key;
            cin >> key;
            return key;
        } else {
            for (int i = 0; i < 30; i++) {
                cout << '-';
            }
            cout << " Agenda ";
            for (int i = 0; i < 30; i++) {
                cout << '-';
            }
            cout << '\n';
            cout << "Action :\n";
            cout << "o   - log out Agenda\n";
            cout << "dc  - delete Agenda account\n";
            cout << "lu  - list all Agenda user\n";
            cout << "cm  - create a meeting\n";
            cout << "la  - list all meetings\n";
            cout << "las - list all sponsor meetings\n";
            cout << "lap - list all participate meetings\n";
            cout << "qm  - query meeting by title\n";
            cout << "qt  - query meeting by time interval\n";
            cout << "dm  - delete meeting by title\n";
            cout << "da  - delete all meetings\n";
            for (int i = 0; i < 68; i++) {
                cout << '-';
            }
            cout << "\n\nAgenda@" << m_userName << " :~# ";
            string key1;
            cin >> key1;
            return key1;
        }
    }

    /**
     * execute the operation
     * @return if the operationloop continue
     */
    bool AgendaUI::executeOperation(std::string t_operation) {
    	if (t_operation == "l") {
            userLogIn();
            return true;
        }
    	if (t_operation == "r") {
            userRegister();
            return true;
        }
    	if (t_operation == "q") {
            quitAgenda();
            return false;
        }
    	if (t_operation == "dc") {
            deleteUser();
            return true;
        }
    	if (t_operation == "lu") {
            listAllUsers();
            return true;
        }
     	if (t_operation == "cm") {
            createMeeting();
            return true;
        }
     	if (t_operation == "la") {
            listAllMeetings();
            return true;
        }
     	if (t_operation == "las") {
            listAllSponsorMeetings();
            return true;
        }
     	if (t_operation == "lap") {
            listAllParticipateMeetings();
            return true;
        }
     	if (t_operation == "qm") {
            queryMeetingByTitle();
            return true;
        }
     	if (t_operation == "qt") {
            queryMeetingByTimeInterval();
            return true;
        }
     	if (t_operation == "dm") {
            deleteMeetingByTitle();
            return true;
        }
     	if (t_operation == "da") {
            deleteAllMeetings();
            return true;
        }
     	if (t_operation == "o") {
            userLogOut();
            return true;
        }
        return true;
    }

    /**
     * user Login
     */
    void AgendaUI::userLogIn(void) {
    	cout << "\n[log in] [user name] [password]\n[log in] ";
    	cin >> m_userName >> m_userPassword;
    	if (m_agendaService.userLogIn(m_userName, m_userPassword)) {
    		cout << "[log in] succeed!\n\n";
    	} else {
    		cout << "[error] log in fail!\n\n";
            m_userName = "";
            m_userPassword = "";
    	}
    }

    /**
     * user regist
     */
    void AgendaUI::userRegister(void) {
    	 cout << "\n[register] [user name] [password] [email] [phone]\n";
    	 cout << "[register] ";
    	 string name, password, email, phone;
    	 cin >> name >> password >> email >> phone;
    	 if (m_agendaService.userRegister(name, password, email, phone)) {
    	 	cout << "[register] succeed!\n\n";
    	 } else {
    	 	cout << "[error] register fail!\n\n";
    	 }
         m_userName = "";
         m_userPassword = "";
    }

    /**
     * quit the Agenda
     */
    void AgendaUI::quitAgenda(void) {
    	m_agendaService.quitAgenda();
    }

    /**
     * user logout
     */
    void AgendaUI::userLogOut(void) {
        m_userName = "";
        m_userPassword = "";
        m_agendaService.quitAgenda();
    }

    /**
     * delete a user from storage
     */
    void AgendaUI::deleteUser(void) {
    	if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
    		cout << "\n[delete agenda account] succeed!\n\n";
    	}
        m_agendaService.quitAgenda();
        m_userName = "";
        m_userPassword = "";
    }

    /**
     * list all users from storage
     */
    void AgendaUI::listAllUsers(void) {
    	cout << "\n[list all users]\n\n";
        cout << left << setw(10) << "name";
        cout << left << setw(10) << "email";
        cout << left << setw(10) << "phone" << endl;
        list<User> result = m_agendaService.listAllUsers();
        if (result.size()) {
            list<User>::iterator iter;
            for (iter = result.begin(); iter != result.end(); iter++) {
                cout << left << setw(10) << iter->getName();
                cout << left << setw(10) << iter->getEmail();
                cout << left << setw(10) << iter->getPhone() << endl;
            }
        }
    }

    /**
     * user create a meeting with someone else
     */
    void AgendaUI::createMeeting(void) {
        cout << "\n[create meeting] [the number of participators]\n";
        cout << "[create meeting] ";
        int number;
        cin >> number;
        vector<string> vec;
        for (int i = 1; i <= number; i++) {
            cout << "[create meeting] [please enter the participator " << i << " ]\n";
            cout << "[create meeting] ";
            string temp;
            cin >> temp;
            vec.push_back(temp);
        }
        cout << "[title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n";
        cout << "[create meeting] ";
        string m_title, sdate, edate;
        cin >> m_title >> sdate >> edate;
        if (m_agendaService.createMeeting(m_userName, m_title, sdate, edate, vec)) {
            cout << "[create meeting] succeed!\n\n";
        } else {
            cout << "[create meeting] error!\n\n";
        }
    }

    /**
     * list all meetings from storage
     */
    void AgendaUI::listAllMeetings(void) {
        cout << "\n[list all meetings]\n\n";
        list<Meeting> result = m_agendaService.listAllMeetings(m_userName);
        printMeetings(result);
    }

    /**
     * list all meetings that this user sponsored
     */
    void AgendaUI::listAllSponsorMeetings(void) {
        cout << "\n[list all sponsor meetings]\n\n";
        cout << "title     sponsor     start time     end time     participators\n";
        list<Meeting> result = m_agendaService.listAllSponsorMeetings(m_userName);
        printMeetings(result);
    }

    /**
     * list all meetings that this user take part in
     */
    void AgendaUI::listAllParticipateMeetings(void) {
        cout << "\n[list all participator meetings]\n\n";
        list<Meeting> result = m_agendaService.listAllParticipateMeetings(m_userName);
        printMeetings(result);
    }

    /**
     * search meetings by title from storage
     */
    void AgendaUI::queryMeetingByTitle(void) {
        cout << "\n[query meeting] [title]:\n";
        cout << "[query meeting] ";
        string m_meeting;
        cin >> m_meeting;
        list<Meeting> result = m_agendaService.meetingQuery(m_userName, m_meeting);
        printMeetings(result);
    }

    /**
     * search meetings by timeinterval from storage
     */
    void AgendaUI::queryMeetingByTimeInterval(void) {
        cout << "\n[query meetings] [start time(yyyy-mm-dd/hh:mm) [end time(yyyy-mm-dd/hh:mm)]]\n";
        cout << "[query meetings] ";
        string sdate, edate;
        cin >> sdate >> edate;
        list<Meeting> result = m_agendaService.meetingQuery(m_userName, sdate, edate);
        printMeetings(result);
    }

    /**
     * delete meetings by title from storage
     */
    void AgendaUI::deleteMeetingByTitle(void) {
        cout << "\n[delete meeting] [title]\n[delete meeting] ";
        string m_title;
        cin >> m_title;
        if (m_agendaService.deleteMeeting(m_userName, m_title)) {
            cout << "\n[delete meeting by title] succeed!\n\n";
        } else {
            cout << "\n[error] delete meeting fail!\n\n";
        }
    }

    /**
     * delete all meetings that this user sponsored
     */
    void AgendaUI::deleteAllMeetings(void) {
        cout << "\n[delete all meetings] ";
        if (m_agendaService.deleteAllMeetings(m_userName)) {
            cout << "succeed!\n\n";
        }
    }

    /**
     * show the meetings in the screen
     */
    void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
        cout << left << setw(20) << "title";
        cout << left << setw(20) << "sponsor";
        cout << left << setw(20) << "start time";
        cout << left << setw(20) << "end time";
        cout << left << setw(20) << "participator" << endl;
        list<Meeting>::iterator iter;
        for (iter = t_meetings.begin(); iter != t_meetings.end(); iter++) {
            cout << left << setw(20) << iter->getTitle();
            cout << left << setw(20) << iter->getSponsor();
            cout << left << setw(20) << iter->getStartDate().dateToString(iter->getStartDate());
            cout << left << setw(20) << iter->getEndDate().dateToString(iter->getEndDate());
            for (int i = 0; i < iter->getParticipator().size()-1; i++) {
                cout << iter->getParticipator()[i] << ",";
            }
            cout << iter->getParticipator()[iter->getParticipator().size()-1] << endl;
        }
        cout << endl << endl;
    }
