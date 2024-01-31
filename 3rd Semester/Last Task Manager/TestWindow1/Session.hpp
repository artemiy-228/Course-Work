#ifndef SESSION_H
#define SESSION_H

#include <conncpp.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Module.hpp"


// class USER --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  ласс User нужен дл€ того, что бы хранить информацию о сессии. ‘актически дл€ загрузки проектов по id пользовател€, который ее вызывает

struct User {
    std::string name; // »м€ пользовател€
    int id; // ”никальный id пользовател€, который нужен дл€ загрузок его проектов.  
    User() : id(-1), name("User") {}
    User(int id, std::string name) : id(id), name(name) {}
};

// class DATABASE --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*Ётот класс нужен дл€ работы с базой данных. явл€етс€ дружественным к BaseTemplate, тк посто€нно работает с его пол€ми.*/

class Database {
private:
    sql::Driver* driver; // ƒрайвер базы данных, фактически €вл€етс€ отправной точкой дл€ работы с ней
    sql::SQLString url; // адрес подключени€ к базе данных
    sql::Properties properties; // свойства подключени€, например данные пользовател€
    std::unique_ptr<sql::Connection> conn; // "курсор" базы данных. ‘актически - та сама€ "мигающа€ палочка" в терминале, откуда происходит ввод запросов

    // Ётот метод вызываетс€ внутри loadProject
    std::vector<Task> loadTasks(int id); // «агрузка задач из базы данных по id проекта, к которому они принадлежат. Ќапример € вызываю загрузка задач из проекта с id 15.

public:

    Database(const std::string&, const std::string&, const std::string&, const std::string&);

    std::string addUserToProject(std::string, int);

    bool connect(); // ѕодключение пользовател€. ¬озвращает логические тип, что бы понимать произошло ли подключение. ѕримен€етс€ в сесси€х пользовател€
    void disconnect(); // ќтключение от базы данных

    std::vector<Label> loadProjectsDescription(const User& currUser); // ѕервоночальна€ загрузка проектов что бы представить пользователю доступные ему проекты.

    void deleteProject(int id); // ”даление проекта из базы данных на основе id проекта.
    void saveProject(Project&); // —охранение проекта в базе данных. ≈сли проект уже существует происходит перизапись проекта в таблице. ≈сли нет, то создание
    Project loadProject(int id); // «агрузка проекта из базы данных. «агрузка происходит по id проекта.

    void saveTask(Task&, int); // —охранение задачи в таблице. ≈сли задача есть, то произойдет перезапись, в ином случае добавление новой записи.
    void deleteTask(int id); // ”даление задачи из базы данных на основе id задачи.

    void saveComment(int id, const Comment& comment);
    std::vector<Comment> loadComments(int id);
    std::vector<std::string> loadUsers(int projectId);
    User ConnectUser1(std::string name, std::string password);

};




#endif
