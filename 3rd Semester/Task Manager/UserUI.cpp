#include "UserUI.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>



// Функция для получения текущего времени. Нужна что бы получать время создания проектов или задач для записи в базу данных

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;

    localtime_s(&localTime, &currentTime);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%y-%m-%d %H:%M:%S");
    return oss.str();
}


// Конструктор создающий само окно.
LoginForm::LoginForm() : nanogui::Screen(Eigen::Vector2i(200, 200), "Login Window", false) {
    createLoginFormUI();
}
// Отрисовка самого окна
void LoginForm::createLoginFormUI() {

    nanogui::Window* window = new nanogui::Window(this, "Login");
    window->setLayout(new nanogui::GroupLayout());

    nanogui::Label* usernameLabel = new nanogui::Label(window, "Username:", "sans-bold");
    nanogui::TextBox* usernameTextBox = new nanogui::TextBox(window);
    usernameTextBox->setEditable(true);
    usernameTextBox->setPlaceholder("Enter your username");
    usernameTextBox->setValue("");

    nanogui::Label* passwordLabel = new nanogui::Label(window, "Password:", "sans-bold");
    nanogui::TextBox* passwordTextBox = new nanogui::TextBox(window);
    passwordTextBox->setEditable(true);
    passwordTextBox->setPlaceholder("Enter your password");
    passwordTextBox->setValue("");

    nanogui::Button* loginButton = new nanogui::Button(window, "Login");
    loginButton->setCallback([this, usernameTextBox, passwordTextBox] {
        enteredUsername = usernameTextBox->value();
        enteredPassword = passwordTextBox->value();
        setVisible(false);
        });

    window->setPosition(Eigen::Vector2i(0, 0));
    window->setFixedSize(Eigen::Vector2i(200, 200));
    performLayout();
}

// Получение значения из формы
std::string LoginForm::getEnteredUsername() const {
    return enteredUsername;
}
// Получение значения из формы
std::string LoginForm::getEnteredPassword() const {
    return enteredPassword;
}



// Конструктор для окна, в котором мы выбираем проекты.
ProjectSelectionWindow::ProjectSelectionWindow(User& user, Database& coursore)
    : nanogui::Screen(Eigen::Vector2i(400, 600), "Project Selection", false), 
    selectedProjectId(-1),user(user), coursore(coursore) {
    projectsLabel = coursore.loadProjectsDescription(user);
    createProjectSelectionWindowUI();

}





// Окно, которое открывается во время нажатия кнопки Add Project. 
void ProjectSelectionWindow::openAddProjectWindow() {
    nanogui::Window* taskFormWindow = new nanogui::Window(this, "Add Project");
    taskFormWindow->setSize(Eigen::Vector2i(300, 200));


    nanogui::Label* taskNameLabel = new nanogui::Label(taskFormWindow, "Project Name:", "sans-bold");
    nanogui::TextBox* taskNameTextBox = new nanogui::TextBox(taskFormWindow);
    taskNameTextBox->setEditable(true);
    taskNameTextBox->setPlaceholder("Enter Project Name");
    taskNameTextBox->setSize(Eigen::Vector2i(220, 25));
    taskNameTextBox->setPosition(Eigen::Vector2i(50, 50));
    taskNameTextBox->setValue("");

    nanogui::Label* deadlineLabel = new nanogui::Label(taskFormWindow, "Deadline:", "sans-bold");
    nanogui::TextBox* deadlineTextBox = new nanogui::TextBox(taskFormWindow);
    deadlineTextBox->setEditable(true);
    deadlineTextBox->setPlaceholder("Example: 2023-09-08 07:06:08");
    deadlineTextBox->setSize(Eigen::Vector2i(220, 25));
    deadlineTextBox->setPosition(Eigen::Vector2i(50, 90));
    deadlineTextBox->setValue("");


    nanogui::Button* addButton = new nanogui::Button(taskFormWindow, "Add");
    addButton->setPosition(Eigen::Vector2i(170, 170));
    addButton->setSize(Eigen::Vector2i(100, 20));
    addButton->setCallback([this, taskNameTextBox, deadlineTextBox, taskFormWindow] {
        std::string projectName = taskNameTextBox->value();
        std::string deadline = deadlineTextBox->value();
        std::string dateCreation = getCurrentDateTime();

        int projectId = -1;

        Project tempProject = Project(projectName, deadline, dateCreation, projectId);
        coursore.saveProject(tempProject);

        createProjectSelectionWindowUI();

        taskFormWindow->dispose();
        });

    taskFormWindow->setModal(true);
}




// Отрисовка главного окна, в котором мы выбираем проект. В конструкторе мы просто его создаем и даем ему параметры.а здесь мы добавляем в него различные виджеты, кнопки и тд.
void ProjectSelectionWindow::createProjectSelectionWindowUI() {
    nanogui::Label* greetingLabel = new nanogui::Label(this, "Current User: " + user.name, "sans-bold");
    greetingLabel->setFontSize(20);

    nanogui::Button* createProjectButton = new nanogui::Button(this, "Create Project");
    createProjectButton->setPosition(Eigen::Vector2i(270, 0));
    createProjectButton->setCallback([this] {
        openAddProjectWindow();
        });

    nanogui::Widget* buttonPanel = new nanogui::Widget(this);
    buttonPanel->setPosition(Eigen::Vector2i(0, 30));
    buttonPanel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, 0, 5));

    for (const Label& label : projectsLabel) {
        nanogui::Button* projectButton = new nanogui::Button(buttonPanel, label.name);
        projectButton->setFixedHeight(30);
        projectButton->setFixedWidth(400);
        projectButton->setCallback([this, label] {
            selectedProjectId = label.id;
            setVisible(false);
            });
    }

    performLayout();
}

int ProjectSelectionWindow::getSelectedProjectIndex() const {
    return selectedProjectId;
}


// Конструктор для окна, в котором мы работает с конкретным проектом. Внес completedTasks и uncompletedTasks по причине того, чтобы проще было отчищать их

ProjectWindow::ProjectWindow(Project& _project, Database& coursore, User& user)
    : nanogui::Screen(Eigen::Vector2i(1000, 600), _project.getName(), false), project(_project), coursore(coursore), user(user) {
    completedTasks = new FixedWindow(this, "Completed Tasks");
    uncompletedTasks = new FixedWindow(this, "Uncompleted Tasks");
    renderTaskWindows();

    projectName = "Project Name: " + project.getName();
    dateOfCreation = "Date of Creation: " + project.getDateCreation();
    deadLine = "Deadline: " + project.getDeadline();
    createUI();
}



// Открыть окно, в котором показывается информация о конкретном проекте

void ProjectWindow::openInfoAboutProject() {
    FixedWindow* infoWindow = new FixedWindow(this, "Project Information");
    infoWindow->setPosition(Eigen::Vector2i(0, 0));
    infoWindow->setSize(Eigen::Vector2i(500, 500));

    nanogui::Label* nameLabel = new nanogui::Label(infoWindow, projectName, "sans-bold");
    nameLabel->setFontSize(24);
    nameLabel->setPosition(Eigen::Vector2i(5, 30));
    nameLabel->setSize(Eigen::Vector2i(300, 30));

    nanogui::Label* dateLabel = new nanogui::Label(infoWindow, deadLine, "sans-bold");
    dateLabel->setFontSize(24);
    dateLabel->setPosition(Eigen::Vector2i(5, 90));
    dateLabel->setSize(Eigen::Vector2i(300, 30));

    nanogui::Label* deadlineLabel = new nanogui::Label(infoWindow, dateOfCreation, "sans-bold");
    deadlineLabel->setFontSize(24);
    deadlineLabel->setPosition(Eigen::Vector2i(5, 60));
    deadlineLabel->setSize(Eigen::Vector2i(300, 30));


    for (int i = 0; i < project.getUsersAmount(); i++) {
        nanogui::TextBox* userBox = new nanogui::TextBox(infoWindow);
        userBox->setEditable(false);
        userBox->setValue("User: " + project.getUser(i));
        userBox->setSize(Eigen::Vector2i(200, 30));
        userBox->setPosition(Eigen::Vector2i(5, 120 + 50 * i));
        infoWindow->addChild(userBox);
    }

    nanogui::Button* closeButton = new nanogui::Button(infoWindow, "Close");
    closeButton->setPosition(Eigen::Vector2i(100, 450));
    closeButton->setSize(Eigen::Vector2i(300, 40));

    closeButton->setCallback([infoWindow] { 
        infoWindow->dispose(); 
        });


    infoWindow->addChild(nameLabel);
    infoWindow->addChild(dateLabel);
    infoWindow->addChild(deadlineLabel);
    infoWindow->addChild(closeButton);

    infoWindow->setVisible(true);
    infoWindow->setModal(true);
    infoWindow->center();
}

void ProjectWindow::renderComments(Task& task, const FixedWindow& window) {
    nanogui::Widget* commentsWidget = new nanogui::Widget(window);
    commentsWidget->setPosition(Eigen::Vector2i(20, 250));
}




// Открыть окно, в котором показывается информация о конкретной ЗАДАЧЕ


void ProjectWindow::openTaskInfoWindow(Task& task) {
    FixedWindow* taskInfoWindow = new FixedWindow(this, "Task Information: " + task.getName());
    taskInfoWindow->setSize(Eigen::Vector2i(width() / 2 - 20, height() - 100));

    // Выше мы создали наше окно, которое нельзя двигать и сейчас мы создаем для него дочерние объекта. различные надписи и текстовые поля.

    nanogui::Label* titleLabel = new nanogui::Label(taskInfoWindow, "Task Name:", "sans-bold");
    titleLabel->setFontSize(36);
    titleLabel->setSize(Eigen::Vector2i(500, 20));
    titleLabel->setPosition(Eigen::Vector2i(5, 37));

    nanogui::TextBox* tasknameTextBox = new nanogui::TextBox(taskInfoWindow);
    tasknameTextBox->setEditable(true);
    tasknameTextBox->setValue(task.getName());
    tasknameTextBox->setSize(Eigen::Vector2i(250, 25));
    tasknameTextBox->setPosition(Eigen::Vector2i(80, 35));

    nanogui::Label* dateLabel = new nanogui::Label(taskInfoWindow, "Date of Creation: ", "sans-bold");
    dateLabel->setSize(Eigen::Vector2i(500, 20));
    dateLabel->setPosition(Eigen::Vector2i(5, 70));

    nanogui::TextBox* dateTextBox = new nanogui::TextBox(taskInfoWindow);
    dateTextBox->setEditable(false);
    dateTextBox->setValue(task.getDateCreation());
    dateTextBox->setSize(Eigen::Vector2i(250, 25));
    dateTextBox->setPosition(Eigen::Vector2i(110, 67));



    nanogui::Label* deadlineLabel = new nanogui::Label(taskInfoWindow, "Deadline: ", "sans-bold");
    deadlineLabel->setSize(Eigen::Vector2i(500, 20));
    deadlineLabel->setPosition(Eigen::Vector2i(5, 100));

    nanogui::TextBox* deadlineTextBox = new nanogui::TextBox(taskInfoWindow);
    deadlineTextBox->setEditable(true);
    deadlineTextBox->setValue(task.getDeadline());
    deadlineTextBox->setSize(Eigen::Vector2i(250, 25));
    deadlineTextBox->setPosition(Eigen::Vector2i(70, 97));

    nanogui::Label* commentsLabel = new nanogui::Label(taskInfoWindow, "Comments", "sans-bold");
    commentsLabel->setFontSize(36);
    commentsLabel->setSize(Eigen::Vector2i(500, 20));

    commentsLabel->setPosition(Eigen::Vector2i(20, 180));
    nanogui::TextBox* addCommentTextBox = new nanogui::TextBox(taskInfoWindow);
    addCommentTextBox->setEditable(true);
    addCommentTextBox->setValue("");
    addCommentTextBox->setSize(Eigen::Vector2i(350, 30));
    addCommentTextBox->setPosition(Eigen::Vector2i(20, 200));

    // Создаем объект класса кнопки на добавление окмментария и привязываем к нему коллбек
    nanogui::Button* addCommentButton = new nanogui::Button(taskInfoWindow, "Add");
    addCommentButton->setSize(Eigen::Vector2i(80, 30));
    addCommentButton->setPosition(Eigen::Vector2i(375, height() - 400));
    addCommentButton->setCallback([this, taskInfoWindow, addCommentTextBox, &task] {
        if (addCommentTextBox->value() != "") {
            std::string User = user.name;
            std::string commentText = addCommentTextBox->value();
            int id = -1;

            Comment tempComment(User, commentText, id);
            task.addComment(tempComment);
            addCommentTextBox->setValue("");
            taskInfoWindow->dispose();
            openTaskInfoWindow(task);
        }
        });



    nanogui::Label* statusLabel = new nanogui::Label(taskInfoWindow, "Completed: ", "sans-bold");
    statusLabel->setSize(Eigen::Vector2i(500, 20));
    statusLabel->setPosition(Eigen::Vector2i(5, 130));

    nanogui::CheckBox* statusCheckBox = new nanogui::CheckBox(taskInfoWindow, "");
    statusCheckBox->setChecked(task.getCurrentState());

    statusCheckBox->setSize(Eigen::Vector2i(20, 20));
    statusCheckBox->setPosition(Eigen::Vector2i(75, 130));

    for (int i = 0; i < task.getCommentsAmount(); i++) {
        nanogui::TextBox* commentBox = new nanogui::TextBox(taskInfoWindow);
        commentBox->setEditable(false);
        commentBox->setValue(task.getComment(i).author + " | " + task.getComment(i).text);
        commentBox->setSize(Eigen::Vector2i(350, 30));
        commentBox->setPosition(Eigen::Vector2i(20, 250 + 50 * i));
        taskInfoWindow->addChild(commentBox);
    }


    nanogui::Button* saveTaskButton = new nanogui::Button(taskInfoWindow, "Save Task");
    saveTaskButton->setSize(Eigen::Vector2i(100, 40));
    saveTaskButton->setPosition(Eigen::Vector2i(333, height() - 150));
    saveTaskButton->setCallback([this, taskInfoWindow, tasknameTextBox, deadlineTextBox, statusCheckBox, &task] {
        task.setName(tasknameTextBox->value());
        task.setDeadLine(deadlineTextBox->value());
        task.setCurrentState(statusCheckBox->checked());
        coursore.saveProject(project);
        taskInfoWindow->dispose();
        createUI();
        });


    nanogui::Button* closeButton = new nanogui::Button(taskInfoWindow, "Cancel");
    closeButton->setSize(Eigen::Vector2i(100, 40));
    closeButton->setPosition(Eigen::Vector2i(33, height() - 150));
    closeButton->setCallback([taskInfoWindow,&task] {
        taskInfoWindow->dispose();
        });

    nanogui::Button* deleteTaskButton = new nanogui::Button(taskInfoWindow, "Delete Task");
    deleteTaskButton->setSize(Eigen::Vector2i(100, 40));
    deleteTaskButton->setPosition(Eigen::Vector2i(183, height() - 150));
    deleteTaskButton->setCallback([this, taskInfoWindow, &task] {
        project.deleteTask(task.id);
        coursore.deleteTask(task.id);
        createUI();
        taskInfoWindow->dispose();
        });

    taskInfoWindow->addChild(titleLabel);
    taskInfoWindow->addChild(tasknameTextBox);
    taskInfoWindow->addChild(dateLabel);
    taskInfoWindow->addChild(dateTextBox);
    taskInfoWindow->addChild(deadlineLabel);
    taskInfoWindow->addChild(deadlineTextBox);
    taskInfoWindow->addChild(statusLabel);
    taskInfoWindow->addChild(statusCheckBox);
    taskInfoWindow->addChild(addCommentTextBox);
    taskInfoWindow->addChild(closeButton);
    taskInfoWindow->addChild(addCommentButton);
    taskInfoWindow->addChild(commentsLabel);
    taskInfoWindow->center();
    taskInfoWindow->setVisible(true);
    taskInfoWindow->setModal(true);
}


// Открыть окно, в котором можно добавить пользователя в ПРОЕКТ


void ProjectWindow::openAddUserWindow() {
    nanogui::Window* window = new nanogui::Window(this, "Add User");

    nanogui::Label* usernameLabel = new nanogui::Label(window, "Add User", "sans-bold");
    usernameLabel->setPosition(Eigen::Vector2i(10, 30));
    usernameLabel->setSize(Eigen::Vector2i(200, 20));

    nanogui::TextBox* usernameTextBox = new nanogui::TextBox(window);
    usernameTextBox->setEditable(true);
    usernameTextBox->setPlaceholder("Enter username");
    usernameTextBox->setValue("");
    usernameTextBox->setSize(Eigen::Vector2i(180, 30));
    usernameTextBox->setPosition(Eigen::Vector2i(10, 60));



    nanogui::Button* loginButton = new nanogui::Button(window, "Add");
    loginButton->setSize(Eigen::Vector2i(80, 30));
    loginButton->setPosition(Eigen::Vector2i(110, 110));
    loginButton->setCallback([this, usernameTextBox, window] {
        window->setModal(false);
        std::string answer = coursore.addUserToProject(usernameTextBox->value(), project.id);
        if(answer == "This user succesfully added.")
            project.addUser(usernameTextBox->value());
        nanogui::Window* messageWindow = new nanogui::Window(this, "Message");
        messageWindow->setSize(Eigen::Vector2i(200, 120));

        nanogui::Label* messageLabel = new nanogui::Label(messageWindow, answer, "sans-bold");
        messageLabel->setPosition(Eigen::Vector2i(10, 30));
        messageLabel->setSize(Eigen::Vector2i(180, 20));

        nanogui::Button* okButton = new nanogui::Button(messageWindow, "OK");
        okButton->setPosition(Eigen::Vector2i(20, 80));
        okButton->setSize(Eigen::Vector2i(160, 30));
        okButton->setCallback([messageWindow, window] {
            messageWindow->dispose();
            window->dispose();
            });

        messageWindow->setModal(true);
        messageWindow->setPosition(Eigen::Vector2i(500, 300));
        });

    nanogui::Button* closeButton = new nanogui::Button(window, "Cancel");
    closeButton->setSize(Eigen::Vector2i(80, 30));
    closeButton->setPosition(Eigen::Vector2i(10, 110));
    closeButton->setCallback([window] {
        window->dispose();
        });

    window->setSize(nanogui::Vector2i(200, 150));
    window->addChild(closeButton);
    window->addChild(loginButton);
    window->setModal(true);
    window->center();
}



// Открыть окно для добавления ЗАДАЧИ в ПРОЕКТ

void ProjectWindow::openAddTaskWindow() {
    nanogui::Window* taskFormWindow = new nanogui::Window(this, "Add Task");
    taskFormWindow->setSize(Eigen::Vector2i(300, 200));


    nanogui::Label* taskNameLabel = new nanogui::Label(taskFormWindow, "Task Name:", "sans-bold");
    nanogui::TextBox* taskNameTextBox = new nanogui::TextBox(taskFormWindow);
    taskNameTextBox->setEditable(true);
    taskNameTextBox->setPlaceholder("Enter Task Name");
    taskNameTextBox->setSize(Eigen::Vector2i(220, 25));
    taskNameTextBox->setPosition(Eigen::Vector2i(50, 50));
    taskNameTextBox->setValue("");

    nanogui::Label* deadlineLabel = new nanogui::Label(taskFormWindow, "Deadline:", "sans-bold");
    nanogui::TextBox* deadlineTextBox = new nanogui::TextBox(taskFormWindow);
    deadlineTextBox->setEditable(true);
    deadlineTextBox->setPlaceholder("Example: 2023-09-08 07:06:08");
    deadlineTextBox->setSize(Eigen::Vector2i(220, 25));
    deadlineTextBox->setPosition(Eigen::Vector2i(50, 90));
    deadlineTextBox->setValue("");

    nanogui::Button* addButton = new nanogui::Button(taskFormWindow, "Add");
    addButton->setPosition(Eigen::Vector2i(170, 170));
    addButton->setSize(Eigen::Vector2i(100, 20));
    addButton->setCallback([this, taskNameTextBox, deadlineTextBox, taskFormWindow] {
        std::string taskName = taskNameTextBox->value();
        std::string deadline = deadlineTextBox->value();
        bool isCompleted = 0;
        std::string dateCreation = getCurrentDateTime();

        int taskId = -1;
        Task tempTask = Task(taskName, dateCreation, deadline, isCompleted, taskId);
        project.addTask(tempTask);
        coursore.saveProject(project);
        taskFormWindow->dispose();
        createUI();
        });

    nanogui::Button* closeButton = new nanogui::Button(taskFormWindow, "Close");
    closeButton->setPosition(Eigen::Vector2i(30, 170));
    closeButton->setSize(Eigen::Vector2i(100, 20));
    closeButton->setCallback([this, taskFormWindow] {
        taskFormWindow->dispose();
        });

    taskFormWindow->center();
    taskFormWindow->setVisible(true);
    taskFormWindow->addChild(taskNameLabel);
    taskFormWindow->addChild(taskNameTextBox);
    taskFormWindow->addChild(deadlineLabel);
    taskFormWindow->addChild(deadlineTextBox);
    taskFormWindow->addChild(addButton);
    taskFormWindow->setModal(true);
}


//Метод который отрисовывает два подокна с задачами(Выполненые и Невыполненые).

void ProjectWindow::renderTaskWindows() {

    completedTasks->dispose();
    uncompletedTasks->dispose();

    completedTasks = new FixedWindow(this, "Completed Tasks");
    uncompletedTasks = new FixedWindow(this, "Uncompleted Tasks");
    completedTasks->setPosition(Eigen::Vector2i(width() / 2 + 10, 90));
    completedTasks->setSize(Eigen::Vector2i(width() / 2 - 20, height() - 100));
    uncompletedTasks->setPosition(Eigen::Vector2i(10, 90));
    uncompletedTasks->setSize(Eigen::Vector2i(width() / 2 - 20, height() - 100));


    int first = 0;
    int second = 0;
    for (int i = 0; i < project.getTasksAmount(); ++i) {
        nanogui::Button* openButton = new nanogui::Button(
            project.getTask(i).getCurrentState() ? completedTasks : uncompletedTasks,
            project.getTask(i).getName()
        );

        openButton->setFixedSize(Eigen::Vector2i(300, 40));
        openButton->setPosition(Eigen::Vector2i(100, 40 + 60 * (project.getTask(i).getCurrentState() ? second : first)));
        openButton->setCallback([this, i] {
            openTaskInfoWindow(project.getTask(i));
            });

        if (project.getTask(i).getCurrentState()) {
            second++;
        }
        else {
            first++;
        }
    }

}



// Создание интерфейса в главном окне ПРОЕКТА. 

void ProjectWindow::createUI() {

    project = coursore.loadProject(project.id);

    nanogui::Label* projectInfo = new nanogui::Label(this, projectName, "sans-bold");
    projectInfo->setFontSize(20);
    projectInfo->setPosition(Eigen::Vector2i(350, 5));
    projectInfo->setSize(Eigen::Vector2i(300, 30));
    int buttonWidth = 100;
    int buttonHeight = 30;
    int margin = 10;
    int rightMargin = 10;

    int buttonPanelX = width() - buttonWidth - rightMargin;
    int buttonPanelY = height() - margin - buttonHeight;

    nanogui::Button* infoButton = new nanogui::Button(this, "Info");
    infoButton->setSize(Eigen::Vector2i(buttonWidth, buttonHeight));
    infoButton->setPosition(Eigen::Vector2i(buttonPanelX, buttonPanelY));

    infoButton->setCallback([this]() {
        openInfoAboutProject();
    });


    nanogui::Button* addUserButton = new nanogui::Button(this, "Add User");
    addUserButton->setCallback([this] {
        openAddUserWindow();
        });


    nanogui::Button* closeButton = new nanogui::Button(this, "Close");
    closeButton->setCallback([this] { setVisible(false); });

    nanogui::Widget* buttonPanel = new nanogui::Widget(this);
    buttonPanel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Minimum, 0, 10));
    buttonPanel->setFixedSize(nanogui::Vector2i(buttonWidth, buttonHeight * 2 + 10));
    buttonPanel->setPosition(nanogui::Vector2i(buttonPanelX, buttonPanelY));


    buttonPanel->addChild(infoButton);
    buttonPanel->addChild(addUserButton);
    buttonPanel->addChild(closeButton);

    buttonPanel->setPosition(Eigen::Vector2i(400, height() - buttonPanel->height()));
    nanogui::Widget* taskWidget = new nanogui::Widget(this);
    taskWidget->setPosition(Eigen::Vector2i(10, 10));

    renderTaskWindows();

    nanogui::Button* addTaskButton = new nanogui::Button(this, "Add Task");
    addTaskButton->setPosition(Eigen::Vector2i(width() - 130, 0));
    addTaskButton->setCallback([this] {
        openAddTaskWindow();
        });

    performLayout();
}


