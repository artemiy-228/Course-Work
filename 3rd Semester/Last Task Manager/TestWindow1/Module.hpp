#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>



class Task;

//class LABEL --------------------------------------------------------------------------------------------------------------------------------

/* Этот класс используется для того, что бы во время выбора проекта в памяти не хранились все объекты из базы данных. Благодаря чему, мы экономим память. А для выбора нам хватит названия. id же нужен, что бы потом загрузить объект класса Project из базы данных. Куда мы передаем id и загружаем проект по его project_id из таблицы. */


struct Label {
    int id;
    std::string name;
    Label(int _id, std::string _name) : id(_id), name(_name) {}
    void printLabels() const { std::cout << "Project ID: " << id << " Name: " << name << '\n'; }
};

//class COMMENT --------------------------------------------------------------------------------------------------------------------------------

/*Этот класс нужен, что бы хранить информацию о комментарие. У любого комментария есть автор, текст и дата создания. Пришла идея реализовать через класс, так как так легче и лучше всего собирать разные сущности в одно. Раньше вы сказали, что нельзя делать комментарий только текстом.*/

struct Comment {
    std::string author;  // Имя автора комментария
    std::string text;   //  текст комментария
    int id;
    Comment(std::string author, std::string text, int id) : author(author), text(text), id(id){}
};

// class BASETEMPLATE -----------------------------------------------------------------------------------------------------------------------

/*Этот класс нужен для описания стандартных полей для Проекта и Задачи*/

class BaseTemplate : public Label {
protected:
    std::string dateCreation; // Дата создания. Пробовал сделать через время, но там слишком муторно + все равно используется к приведению к строке для вывода.
    std::string deadLine; // Дедлайн. Пробовал сделать через время, но там слишком муторно + все равно используется к приведению к строке для вывода.
    std::vector<Comment> comments; // вектор объектов класса Комментарий
    std::vector<std::string> users; // вектор пользователей. Нужны только имена пользователей, что бы было понятно кто к нему принадлежит. хранить тут объекты класса нельзя
    int commentsAmount; // количество комментариев
    int usersAmount; // количество пользователей

public:
    friend class Database; // тк Database постоянно работает с объектами его дочерних классов, объявляем его классом дружественным

    BaseTemplate(std::string _name, std::string _dateCreation, std::string _deadLine, int _id);

    // аксессор и мутато для имени. Мы можем поменять имя проекта
    std::string getName() const { return name; }
    void setName(std::string _name) { name = _name; }


    // аксессор для даты создания проекта. Менять его нельзя
    std::string getDateCreation() const { return dateCreation; }

    // аксессор и мутатор для срока выполнения. Срок выполнения можно сдвигать
    std::string getDeadline() const { return deadLine; }
    void setDeadLine(std::string date) { deadLine = date; }

    // Методы для работы с сущностями комментариев. Получить количество комментариев, получить комментарий по индексу, добавить комментарий и удалить комментарий
    int getCommentsAmount() { return commentsAmount; }
    Comment getComment(int index);
    void addComment(const Comment& comment) { comments.push_back(comment); commentsAmount++; }
    void deleteComment(int index);


    // Методы для работы с пользователями. Добавить и удалить пользователя ИЗ ПРОЕКТА, получить количество пользователей и получить конкретного пользователя по индексу.
    void addUser(std::string user);
    void deleteUser(std::string user);
    int getUsersAmount() { return usersAmount; }
    std::string getUser(int index);

};

// class TASK -----------------------------------------------------------------------------------------------------------------------

/*Этот класс нужен для представление задач. Как правило - задачу сущность атомарная.*/

class Task : public BaseTemplate {
private:
    bool isCompleted; // Поле обозначающее то, что задача завершена.

public:


    Task(std::string _name, std::string _dateCreation, std::string _deadline, bool _isCompleted, int _id);

    void showInfo() const; // Выводит информацию о задаче. Нужна только для отладки

    // Аксессор и мутатор для поля завершенности
    bool getCurrentState() const { return isCompleted; }
    void setCurrentState(bool flag) { isCompleted = flag; }
};

// class PROJECT -----------------------------------------------------------------------------------------------------------------------

/*Этот класс нужен для объединения задач по общему признаку выполнения. Например разбиение алгоритма наливания чая на задачи. Вскипятить воду, и так далее.*/

class Project : public BaseTemplate {
private:
    std::vector<Task> tasks; // Вектор задач. Нужен что бы мы могли работать с задачами относящимися к одному проекту
    int tasksAmount; // Количество задач в проекте
    void copyFrom(const Project& other);

public:

    Project(std::string _name, std::string _dateCreation, std::string _deadLine, int id);


    // Методы для работы с Задачами. Добавление, удаление, получение количества и получения по индексу.
    void addTask(const Task& task);
    void deleteTask(int index);
    int getTasksAmount() const { return tasksAmount; }
    Task& getTask(int index);

    void showInfo(); // Существует только для промежуточной работы.
    Project& operator=(const Project& other);
    

};

#endif