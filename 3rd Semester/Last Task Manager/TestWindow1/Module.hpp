#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>



class Task;

//class LABEL --------------------------------------------------------------------------------------------------------------------------------

/* ���� ����� ������������ ��� ����, ��� �� �� ����� ������ ������� � ������ �� ��������� ��� ������� �� ���� ������. ��������� ����, �� �������� ������. � ��� ������ ��� ������ ��������. id �� �����, ��� �� ����� ��������� ������ ������ Project �� ���� ������. ���� �� �������� id � ��������� ������ �� ��� project_id �� �������. */


struct Label {
    int id;
    std::string name;
    Label(int _id, std::string _name) : id(_id), name(_name) {}
    void printLabels() const { std::cout << "Project ID: " << id << " Name: " << name << '\n'; }
};

//class COMMENT --------------------------------------------------------------------------------------------------------------------------------

/*���� ����� �����, ��� �� ������� ���������� � �����������. � ������ ����������� ���� �����, ����� � ���� ��������. ������ ���� ����������� ����� �����, ��� ��� ��� ����� � ����� ����� �������� ������ �������� � ����. ������ �� �������, ��� ������ ������ ����������� ������ �������.*/

struct Comment {
    std::string author;  // ��� ������ �����������
    std::string text;   //  ����� �����������
    int id;
    Comment(std::string author, std::string text, int id) : author(author), text(text), id(id){}
};

// class BASETEMPLATE -----------------------------------------------------------------------------------------------------------------------

/*���� ����� ����� ��� �������� ����������� ����� ��� ������� � ������*/

class BaseTemplate : public Label {
protected:
    std::string dateCreation; // ���� ��������. �������� ������� ����� �����, �� ��� ������� ������� + ��� ����� ������������ � ���������� � ������ ��� ������.
    std::string deadLine; // �������. �������� ������� ����� �����, �� ��� ������� ������� + ��� ����� ������������ � ���������� � ������ ��� ������.
    std::vector<Comment> comments; // ������ �������� ������ �����������
    std::vector<std::string> users; // ������ �������������. ����� ������ ����� �������������, ��� �� ���� ������� ��� � ���� �����������. ������� ��� ������� ������ ������
    int commentsAmount; // ���������� ������������
    int usersAmount; // ���������� �������������

public:
    friend class Database; // �� Database ��������� �������� � ��������� ��� �������� �������, ��������� ��� ������� �������������

    BaseTemplate(std::string _name, std::string _dateCreation, std::string _deadLine, int _id);

    // �������� � ������ ��� �����. �� ����� �������� ��� �������
    std::string getName() const { return name; }
    void setName(std::string _name) { name = _name; }


    // �������� ��� ���� �������� �������. ������ ��� ������
    std::string getDateCreation() const { return dateCreation; }

    // �������� � ������� ��� ����� ����������. ���� ���������� ����� ��������
    std::string getDeadline() const { return deadLine; }
    void setDeadLine(std::string date) { deadLine = date; }

    // ������ ��� ������ � ���������� ������������. �������� ���������� ������������, �������� ����������� �� �������, �������� ����������� � ������� �����������
    int getCommentsAmount() { return commentsAmount; }
    Comment getComment(int index);
    void addComment(const Comment& comment) { comments.push_back(comment); commentsAmount++; }
    void deleteComment(int index);


    // ������ ��� ������ � ��������������. �������� � ������� ������������ �� �������, �������� ���������� ������������� � �������� ����������� ������������ �� �������.
    void addUser(std::string user);
    void deleteUser(std::string user);
    int getUsersAmount() { return usersAmount; }
    std::string getUser(int index);

};

// class TASK -----------------------------------------------------------------------------------------------------------------------

/*���� ����� ����� ��� ������������� �����. ��� ������� - ������ �������� ���������.*/

class Task : public BaseTemplate {
private:
    bool isCompleted; // ���� ������������ ��, ��� ������ ���������.

public:


    Task(std::string _name, std::string _dateCreation, std::string _deadline, bool _isCompleted, int _id);

    void showInfo() const; // ������� ���������� � ������. ����� ������ ��� �������

    // �������� � ������� ��� ���� �������������
    bool getCurrentState() const { return isCompleted; }
    void setCurrentState(bool flag) { isCompleted = flag; }
};

// class PROJECT -----------------------------------------------------------------------------------------------------------------------

/*���� ����� ����� ��� ����������� ����� �� ������ �������� ����������. �������� ��������� ��������� ��������� ��� �� ������. ���������� ����, � ��� �����.*/

class Project : public BaseTemplate {
private:
    std::vector<Task> tasks; // ������ �����. ����� ��� �� �� ����� �������� � �������� ������������ � ������ �������
    int tasksAmount; // ���������� ����� � �������
    void copyFrom(const Project& other);

public:

    Project(std::string _name, std::string _dateCreation, std::string _deadLine, int id);


    // ������ ��� ������ � ��������. ����������, ��������, ��������� ���������� � ��������� �� �������.
    void addTask(const Task& task);
    void deleteTask(int index);
    int getTasksAmount() const { return tasksAmount; }
    Task& getTask(int index);

    void showInfo(); // ���������� ������ ��� ������������� ������.
    Project& operator=(const Project& other);
    

};

#endif