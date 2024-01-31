#ifndef UI_HPP
#define UI_HPP

#include <nanogui/nanogui.h>
#include "Module.hpp"
#include "Session.hpp"

// �����, ������� ����� ��� ������ � ����� ����������� ������������. 
class LoginForm : public nanogui::Screen {
private:
    std::string enteredUsername;
    std::string enteredPassword;

public:
    LoginForm();
    void createLoginFormUI(); // ������������ ����������� ��������� ������ ����
    std::string getEnteredUsername() const;
    std::string getEnteredPassword() const;
};


// �����, ������� ����� ��� ������ � ����� ������ ��������� ��� �������� � ����������� ������������ (��� �������� ������). �������� ���������� ������ � ������� ���� ������ ���������������� �������������
class ProjectSelectionWindow : public nanogui::Screen {
private:
    void createProjectSelectionWindowUI(); // ������������ ����������� ��������� ������ ����
    void renderProjectsLayout(); // ������������ ������ � ����� ����. ����� ������� �������� �� ������� ����������� �� ����� ���������� ����� ������. ��� �� �������������� ������ ���� �����
    Database& coursore; // ������ ���� ������. ����� ��� �� �������� � ����� ������. ���������� �� ������ ������� � ������ main 
    std::vector<Label> projectsLabel; // �������� � ���� �������� � id ��������. ����� ������ ������������ ���������� ������ �� ���� ������ �� ����� id
    int selectedProjectId; // �������� id
    User& user; // ������������, ������� ��� ����������� � ���������(�� ������ � ������������� ���� ������)

public:
    ProjectSelectionWindow(User& user, Database& coursore); // ����������� �����������
    void openAddProjectWindow(); // ������� ���� ��� ���������� �������
    int getSelectedProjectIndex() const; // �������� �������� ������������� ����� ������
};


// ����������� ����� Window ����� ����������� ����� mouseDragEvent. ������� � ������������ ���� �����, � ������� � �������� ������� ��� ����.
class FixedWindow : public nanogui::Window {
public:
    FixedWindow(nanogui::Widget* parent, const std::string& title = "")
        : nanogui::Window(parent, title) {}

    bool mouseDragEvent(const Eigen::Vector2i&, const Eigen::Vector2i&, int button, int modifiers) override {
        return true;
    }
};

// ���� � ������� ����������� ���������� ������. ����������� ����� ������ �������
class ProjectWindow : public nanogui::Screen {
private:
    Project& project; // ��� ������, �� ������
    Database& coursore;// ������ ���� ������ ��� ������ � ��� 
    User& user; // ���������� ������������

    FixedWindow* completedTasks; // ������� ���������� �����
    FixedWindow* uncompletedTasks; // ������� ������������ �����


    std::string projectName; // ��� �������
    std::string dateOfCreation; // ���� �������� �������
    std::string deadLine;  // ����� ����� �������
    void renderTaskWindows(); //  ��������� ����� � �����. ������� �������� ��� �� ������� ���� �������������� ����
    void renderComments(Task& task, const FixedWindow& window); // ��������� ������������ � ������. ������� �������� ��� �� ���� ������� ��������������.

public:
    ProjectWindow(Project& _project, Database&, User& user); //  ����������� �����������
    void createUI(); // ��������� ��������� ������������ ���������� ����
    void openInfoAboutProject(); // ��������� ����, ������� ����������� ��� ������� ������ ������� ����������
    void openAddUserWindow(); // ��������� ����, ������� ������������ ��� ���������� ������������ 
    void openTaskInfoWindow(Task& task); // ��������� ���� ������� ����������� ��� ������ ������
    void openAddTaskWindow(); // ��������� ����, ������� ����������� ��� ���������� ������


};




#endif