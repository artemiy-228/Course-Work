#include "Module.hpp"


// BASETEMPLATE IMPLEMENTATION ---------------------------------------------------------------------------------------------------------------------

// ����������� �������� �������� �������
BaseTemplate::BaseTemplate(std::string _name, std::string _dateCreation, std::string _deadLine, int _id)
    : Label(_id, _name), dateCreation(_dateCreation), deadLine(_deadLine), comments(), users() {
}

Comment BaseTemplate::getComment(int index) {
    if (index >= 0 && index < comments.size()) {
        return comments[index];
    }
    else {
        std::cout << "Index out of range\n";
    }
}

// ������� ����������� �� �������� �������(����������� ������ � �������)
void BaseTemplate::deleteComment(int index) {
    if (index >= 0 && index < comments.size()) {
        comments.erase(comments.begin() + index);
    }
    else {
        std::cout << "This element doesn't exist\n";
    }
}

// �������� ������������ � ������ �������� �������(����������� ������ � �������)
void BaseTemplate::addUser(std::string user) {
    if (std::find(users.begin(), users.end(), user) == users.end()) {
        users.push_back(user);
        usersAmount++;
    }
    else {
        std::cout << "User already exists";
    }
}

std::string BaseTemplate::getUser(int index) {
    if (index >= 0 && index < usersAmount) {
        return users[index];
    }
    else {
        std::cout << "User already exists";
    }
}

// ������� ������������ �� ������� �������� �������(����������� ������ � �������)
void BaseTemplate::deleteUser(std::string user) {
    auto it = std::find(users.begin(), users.end(), user);
    if (it != users.end()) {
        users.erase(it);
        usersAmount--;
    }
    else {
        std::cout << "User doesn't exist";
    }

}



// PROJECT IMPLEMENTATION ---------------------------------------------------------------------------------------------------------------------

// ����������� �������. 
Project::Project(std::string _name, std::string _dateCreation, std::string _deadLine, int _id)
    : BaseTemplate(_name, _dateCreation, _deadLine, _id) {
    tasks = std::vector<Task>();
}

// ���������� ���������� � �������, �� ����� ������ ��� �������. � ��������� ������ ������ ����� �� �������.
void Project::showInfo() {
    std::cout << "\nProject name: " << name << '\n';
    std::cout << "Users: ";
    for (std::string user : users) {
        std::cout << user << ' ';
    }
    std::cout << '\n';
    std::cout << "Project id: " << id << '\n';
    for (const Task& task : tasks) {
        task.showInfo();
        std::cout << '\n';
    }
}

// ��������� ������ � ������.
void Project::addTask(const Task& task) {
    tasks.push_back(task);
    tasksAmount++;
}

// �������� ������ �� �������, �� �������.
void Project::deleteTask(int index) {
    for (int i = 0; i < tasksAmount; i++) {
        if (tasks[i].id == index) {
            tasks.erase(tasks.begin() + i);
            tasksAmount = tasks.size();
            return;
        }
    }
    std::cout << "Task with id " << index << " not found." << std::endl;
}

// ��������� ������ �� �� �������.
Task& Project::getTask(int index) {
    if (index >= 0 && index < tasksAmount) {
        return tasks[index];
    }
}

// ����������� �������. 
void Project::copyFrom(const Project& other) {
    name = other.name;
    dateCreation = other.dateCreation;
    deadLine = other.deadLine;
    users = other.users;
    usersAmount = other.usersAmount;

    tasks.clear();
    for (const Task& task : other.tasks) {
        tasks.push_back(task);
    }
    tasksAmount = other.tasksAmount;

}

Project& Project::operator=(const Project& other) {
    if (this != &other) {
        copyFrom(other);
    }
    return *this;
}



// TASK IMPLEMENTATION ---------------------------------------------------------------------------------------------------------------------

// ����������� �����
Task::Task(std::string _name, std::string _dateCreation, std::string _deadLine, bool _isCompleted, int _id)
    : BaseTemplate(_name, _dateCreation, _deadLine, _id), isCompleted(_isCompleted) {
}

// ���������� ���������� � ������. ����� ������ ��� ������� ����������, � ��������� ������, ������ ����� �� �������.
void Task::showInfo() const {
    system("cls");
    std::cout << "Task name: " << name << '\n';
    std::cout << "Task Completed: " << isCompleted << '\n';
    std::cout << "Task date's: " << dateCreation << ":   Until: " << deadLine << '\n';
    std::cout << "Task id: " << id << '\n';
}
