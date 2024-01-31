#ifndef UI_HPP
#define UI_HPP

#include <nanogui/nanogui.h>
#include "Module.hpp"
#include "Session.hpp"

// Класс, который нужен для работы с окном авторизации пользователя. 
class LoginForm : public nanogui::Screen {
private:
    std::string enteredUsername;
    std::string enteredPassword;

public:
    LoginForm();
    void createLoginFormUI(); // Отрисовывает графический интерфейс нашего окна
    std::string getEnteredUsername() const;
    std::string getEnteredPassword() const;
};


// Класс, который нужен для работы с окном выбора доступных ему проектов у конкретного пользователя (или создание нового). Удаление происходит только в таблице базы данных привелегированым пользователем
class ProjectSelectionWindow : public nanogui::Screen {
private:
    void createProjectSelectionWindowUI(); // Отрисовывает графический интерфейс нашего окна
    void renderProjectsLayout(); // Отрисовывает задачи в нашем окне. Метод вынесен отдельно по причине перерисовки во время добавления новой задачи. так мы перерисовываем только одну часть
    Database& coursore; // Курсор базы данных. Нужен что бы работать с базой данных. Передается по ссылке начиная с начала main 
    std::vector<Label> projectsLabel; // Содержит в себе названия и id проектов. После выбора подгружается конкретный проект из базы данных по этому id
    int selectedProjectId; // Выбраный id
    User& user; // Пользователь, который был авторизован в программу(не путать с пользователем базы данных)

public:
    ProjectSelectionWindow(User& user, Database& coursore); // Стандартный конструктор
    void openAddProjectWindow(); // Открыть окно для добавления проекта
    int getSelectedProjectIndex() const; // Получить выбраный идендефикатор после выбора
};


// Стандартный класс Window имеет виртуальный метод mouseDragEvent. Поэтому я отнаследовал свой класс, в котором я запретил двигать это окно.
class FixedWindow : public nanogui::Window {
public:
    FixedWindow(nanogui::Widget* parent, const std::string& title = "")
        : nanogui::Window(parent, title) {}

    bool mouseDragEvent(const Eigen::Vector2i&, const Eigen::Vector2i&, int button, int modifiers) override {
        return true;
    }
};

// Окно в котором представлен конкретный проект. Открывается после выбора проекта
class ProjectWindow : public nanogui::Screen {
private:
    Project& project; // Сам проект, по ссылке
    Database& coursore;// Курсор базы данных для работы с ней 
    User& user; // конкретный пользователь

    FixedWindow* completedTasks; // подокно выполненых задач
    FixedWindow* uncompletedTasks; // подокно невыполненых задач


    std::string projectName; // имя проекта
    std::string dateOfCreation; // дата создания проекта
    std::string deadLine;  // сроки сдачи проекта
    void renderTaskWindows(); //  отрисовка задач в окнах. Сделана отдельно что бы удобнее было перерисовывать окно
    void renderComments(Task& task, const FixedWindow& window); // Отрисовка комментариев в задаче. Сделана отдельно что бы было удобнее перерисовывать.

public:
    ProjectWindow(Project& _project, Database&, User& user); //  стандартынй конструктор
    void createUI(); // отрисовка основного графического интерфейса окна
    void openInfoAboutProject(); // отрисовка окна, которое открывается при нажатии кнопки открыть информацию
    void openAddUserWindow(); // отрисовка окна, которое открываектся при добавлении пользоваетля 
    void openTaskInfoWindow(Task& task); // отрисовка окна которое открывается при выборе задачи
    void openAddTaskWindow(); // отрисовка окна, которое открывается при добавлении задачи


};




#endif