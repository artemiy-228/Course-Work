#ifndef SESSION_H
#define SESSION_H

#include <conncpp.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Module.hpp"


// class USER --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ����� User ����� ��� ����, ��� �� ������� ���������� � ������. ���������� ��� �������� �������� �� id ������������, ������� �� ��������

struct User {
    std::string name; // ��� ������������
    int id; // ���������� id ������������, ������� ����� ��� �������� ��� ��������.  
    User() : id(-1), name("User") {}
    User(int id, std::string name) : id(id), name(name) {}
};

// class DATABASE --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*���� ����� ����� ��� ������ � ����� ������. �������� ������������� � BaseTemplate, �� ��������� �������� � ��� ������.*/

class Database {
private:
    sql::Driver* driver; // ������� ���� ������, ���������� �������� ��������� ������ ��� ������ � ���
    sql::SQLString url; // ����� ����������� � ���� ������
    sql::Properties properties; // �������� �����������, �������� ������ ������������
    std::unique_ptr<sql::Connection> conn; // "������" ���� ������. ���������� - �� ����� "�������� �������" � ���������, ������ ���������� ���� ��������

    // ���� ����� ���������� ������ loadProject
    std::vector<Task> loadTasks(int id); // �������� ����� �� ���� ������ �� id �������, � �������� ��� �����������. �������� � ������� �������� ����� �� ������� � id 15.

public:

    Database(const std::string&, const std::string&, const std::string&, const std::string&);

    std::string addUserToProject(std::string, int);

    bool connect(); // ����������� ������������. ���������� ���������� ���, ��� �� �������� ��������� �� �����������. ����������� � ������� ������������
    void disconnect(); // ���������� �� ���� ������

    std::vector<Label> loadProjectsDescription(const User& currUser); // �������������� �������� �������� ��� �� ����������� ������������ ��������� ��� �������.

    void deleteProject(int id); // �������� ������� �� ���� ������ �� ������ id �������.
    void saveProject(Project&); // ���������� ������� � ���� ������. ���� ������ ��� ���������� ���������� ���������� ������� � �������. ���� ���, �� ��������
    Project loadProject(int id); // �������� ������� �� ���� ������. �������� ���������� �� id �������.

    void saveTask(Task&, int); // ���������� ������ � �������. ���� ������ ����, �� ���������� ����������, � ���� ������ ���������� ����� ������.
    void deleteTask(int id); // �������� ������ �� ���� ������ �� ������ id ������.

    void saveComment(int id, const Comment& comment);
    std::vector<Comment> loadComments(int id);
    std::vector<std::string> loadUsers(int projectId);
    User ConnectUser1(std::string name, std::string password);

};




#endif
