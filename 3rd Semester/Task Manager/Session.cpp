#include "Session.hpp"

// Конструктор. Во время создания объекта создается подключение. Иначе выбрасывается исключение. 
Database::Database(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
    : driver(sql::mariadb::get_driver_instance()), url("jdbc:mariadb://" + host + "/" + database),
    properties({ {"user", user}, {"password", password} }), conn(driver->connect(url, properties))
{
    if (!conn) {
        throw std::runtime_error("Failed to connect to the database");
    }
}


// Отключение от базы данных. нужно во время выхода.
void Database::disconnect() {

    if (conn) {
        conn->close();
        conn.reset();
    }
}


// Загрузка описания таблицы. Нужна, что бы не подгружать все проекты с их полями из базы данных. нужно для экономии памяти
std::vector<Label> Database::loadProjectsDescription(const User& currentUser) {
    std::vector<Label> projectDescriptions;

    try {
        int userId = currentUser.id;

        sql::PreparedStatement* stmt = conn->prepareStatement(
            "SELECT projects.project_id, projects.name FROM projects "
            "JOIN user_project ON user_project.user_id = ? "
            "WHERE projects.project_id = user_project.project_id"
        );

        stmt->setInt(1, userId);

        sql::ResultSet* res = stmt->executeQuery();

        while (res->next()) {
            int projectId = res->getInt("project_id");
            std::string name = res->getString("name").c_str();

            projectDescriptions.emplace_back(projectId, name);
        }

        delete stmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return projectDescriptions;
}


// Сохранение проекта в таблице. При его сохранении автоматически сохраняются все его задачи
void Database::saveProject(Project& project) {
    try {
        sql::PreparedStatement* pstmt;

        if (project.id == -1) {
            pstmt = conn->prepareStatement("INSERT IGNORE INTO projects (name, date_creation, deadline) VALUES (?, ?, ?)");
            pstmt->setString(1, project.name);
            pstmt->setString(2, project.dateCreation);
            pstmt->setString(3, project.deadLine);
            pstmt->executeUpdate();
            delete pstmt;

            int lastInsertId = -1;
            sql::Statement* stmt = conn->createStatement();
            sql::ResultSet* res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (res->next()) {
                lastInsertId = res->getInt(1);
                addUserToProject("Artemy Sychev", lastInsertId);
            }
            delete stmt;
        }
        else {
            pstmt = conn->prepareStatement("UPDATE projects SET name=?, date_creation=?, deadline=? WHERE project_id=?");
            pstmt->setString(1, project.name);
            pstmt->setString(2, project.dateCreation);
            pstmt->setString(3, project.deadLine);
            pstmt->setInt(4, project.id);
            pstmt->executeUpdate();
        }

        delete pstmt;

        for (int i = 0; i < project.getTasksAmount(); i++) {
            saveTask(project.getTask(i), project.id);
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}



// Загрузка проекта из таблицы по ее id. 
Project Database::loadProject(int id) {
    try {
        sql::Statement* stmt = conn->createStatement();
        std::string query = "SELECT * FROM projects WHERE project_id = " + std::to_string(id);

        sql::ResultSet* res = stmt->executeQuery(query);

        res->next();
        int id = res->getInt(1);
        std::string name = res->getString(2).c_str();
        std::string dateCreation = res->getString(4).c_str();
        std::string deadLine = res->getString(3).c_str();
        Project tempProject = Project(name, dateCreation, deadLine, id);
        std::vector<std::string> tempUsers = loadUsers(tempProject.id);

        for (std::string user : tempUsers)
            tempProject.addUser(user);

        std::vector<Task> tasks = loadTasks(id);
        for (int i = 0; i < tasks.size(); i++) {
            tempProject.addTask(tasks[i]);
        }
        delete stmt;

        return tempProject;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}


// Удаление проекта из таблицы. Так как в таблице настроен auto cascade, при удалении проекта автоматически удалятся и задачи которые относятся к нему. так же таблице связей с пользователями удалит лишнии записи.
void Database::deleteProject(int taskId) {
    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement("DELETE FROM projects WHERE project_id = ?");
        pstmt->setInt(1, taskId);
        pstmt->execute();
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}





// сохранение задачи в таблице. Если id == -1, то задача была создана в программе и ее записываем. Если id != -1, то в таком случае обновляем запись
void Database::saveTask(Task& task, int project_id) {
    try {
        sql::PreparedStatement* pstmt;

        if (task.id == -1) {
            pstmt = conn->prepareStatement("INSERT INTO tasks (name, date_creation, is_completed, deadline, project_id) VALUES (?, ?, ?, ?, ?)");
        }
        else {
            pstmt = conn->prepareStatement("UPDATE tasks SET name=?, date_creation=?, is_completed=?, deadline=?, project_id=? WHERE task_id=?");
            pstmt->setInt(6, task.id);
        }

        pstmt->setString(1, task.name);
        pstmt->setString(2, task.dateCreation);
        pstmt->setString(3, std::to_string(task.getCurrentState()));
        pstmt->setString(4, task.deadLine);
        pstmt->setString(5, std::to_string(project_id));

        pstmt->executeUpdate();

        for (int i = 0; i < task.getCommentsAmount(); i++) {
            saveComment(task.id, task.getComment(i));
        }

        delete pstmt;

    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}


// Загрузка задач из таблицы. Происходит по id проекта, к которому задачи относятся.
std::vector<Task> Database::loadTasks(int id) {
    std::vector<Task> tasks;

    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement("SELECT * FROM tasks WHERE project_id = ?");
        pstmt->setInt(1, id);

        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            int task_id = res->getInt("task_id");
            std::string name = res->getString("name").c_str();
            std::string dateCreation = res->getString("date_creation").c_str();
            bool isCompleted = res->getBoolean("is_completed");
            std::string deadLine = res->getString("deadline").c_str();
            Task tempTask = Task(name, dateCreation, deadLine, isCompleted, task_id);
            
            std::vector<Comment> tempComments = loadComments(tempTask.id);

            for (Comment& comment : tempComments) {
                tempTask.addComment(comment);
            }

            tasks.push_back(tempTask);

        }

        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return tasks;
}



// Удаление задачи из таблицы. Так как в таблице настроен auto cascade, при удалении задачи автоматически удалятся и комментарии, которые относятся к ней.
void Database::deleteTask(int taskId) {
    try {
        sql::PreparedStatement* stmt = conn->prepareStatement("DELETE FROM tasks WHERE task_id = ?");
        stmt->setInt(1, taskId);
        stmt->execute();
        delete stmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}



// Загрузка комментариев из таблицы. Так как сущность комментария принадлежит задаче, поэтому связываем ее с таблицей задаче с помощью Primary Key.
std::vector<Comment> Database::loadComments(int id) {
    std::vector<Comment> comments;

    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement("SELECT name, text, comment_id FROM task_comments WHERE task_id = ?");
        pstmt->setInt(1, id);

        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            std::string author = res->getString("name").c_str();
            std::string text = res->getString("text").c_str();
            int id = res->getInt("comment_id");
            comments.push_back(Comment(author, text, id));
        }

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return comments;
}

void Database::saveComment(int id, const Comment& comment) {
    try {
        if (comment.id == -1) {
            sql::PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO task_comments (task_id, name, text) VALUES (?, ?, ?)");
            pstmt->setInt(1, id);
            pstmt->setString(2, comment.author);
            pstmt->setString(3, comment.text);
            pstmt->executeUpdate();
            delete pstmt;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}



// Этот метод нужен для подключения пользователя. В случае того, если пользователь с такими данными отсутвтует, выбрасывается исключение. Иначе создается объект с id пользователя
User Database::ConnectUser1(std::string name, std::string password) {
    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement("SELECT * FROM users WHERE name = ? AND password = ?");
        pstmt->setString(1, name);
        pstmt->setString(2, password);

        sql::ResultSet* res = pstmt->executeQuery();

        User currentUser;

        if (res->next()) {
            int user_id = res->getInt(1);
            std::string name = res->getString(2).c_str();
            currentUser = User(user_id, name);
        }
        else {
            std::cout << "User not found." << std::endl;
        }

        delete pstmt;

        return currentUser;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}



std::string Database::addUserToProject(std::string name, int project_id) {
    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement("SELECT * FROM users WHERE name = ?");
        pstmt->setString(1, name);

        sql::ResultSet* res = pstmt->executeQuery();

        int user_id = -1;

        if (res->next()) {
            user_id = res->getInt(1);
        }
        else {
            return "User not found.";
        }

        pstmt = conn->prepareStatement("SELECT * FROM user_project WHERE user_id = ? AND project_id = ?");
        pstmt->setString(1, std::to_string(user_id));
        pstmt->setString(2, std::to_string(project_id));

        res = pstmt->executeQuery();

        if (!(res->next())) {
            pstmt = conn->prepareStatement("INSERT INTO user_project (user_id, project_id) VALUES (?, ?)");
            pstmt->setString(1, std::to_string(user_id));
            pstmt->setString(2, std::to_string(project_id));
            pstmt->executeUpdate();
        }
        return "Operation Completed";
        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}


std::vector<std::string> Database::loadUsers(int projectId) {
    std::vector<std::string> projectUsers;

    try {
        sql::PreparedStatement* pstmt = conn->prepareStatement(
            "SELECT u.name FROM users u "
            "JOIN user_project up ON u.id = up.user_id "
            "WHERE up.project_id = ?"
        );
        pstmt->setInt(1, projectId);

        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            std::string userName = res->getString("name").c_str();
            projectUsers.push_back(userName);
        }

        delete pstmt;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return projectUsers;
}
