#include "Session.hpp"
#include "UserUI.hpp"
#include "Module.hpp"



int main() {
        Database coursore = Database("localhost:3306", "root", "5950", "task_manager");
        nanogui::init();

        std::string username;
        std::string password;
        User currentUser;
        int selectedProjectIndex = -1;

        {
            nanogui::ref<LoginForm> loginWindow = new LoginForm();
            loginWindow->setVisible(true);
            loginWindow->setFixedSize(Eigen::Vector2i(300, 200));
            nanogui::mainloop();

            username = loginWindow->getEnteredUsername();
            password = loginWindow->getEnteredPassword();

            currentUser = coursore.ConnectUser1(username, password);

            if (currentUser.id == -1) {
                std::cout << "Authentication failed. Exiting the program." << std::endl;
                nanogui::shutdown();
                return 1;
            }
            loginWindow->setVisible(false);


        }

        currentUser = coursore.ConnectUser1("Artemy Sychev", "1qaz2wsx");

        {
            std::vector<Label> projectsLabel = coursore.loadProjectsDescription(currentUser);
            nanogui::ref<ProjectSelectionWindow> projectSelectionWindow = new ProjectSelectionWindow(currentUser, coursore);
            projectSelectionWindow->setVisible(true);
            nanogui::mainloop();

            selectedProjectIndex = projectSelectionWindow->getSelectedProjectIndex();


        }

        //currentUser = coursore.ConnectUser1("Artemy Sychev", "1qaz2wsx");
        

        if(selectedProjectIndex != -1)
        {
            Project choosenProject = coursore.loadProject(selectedProjectIndex);
            nanogui::ref<ProjectWindow> projectSelectionWindow = new ProjectWindow(choosenProject, coursore, currentUser);
            projectSelectionWindow->setVisible(true);
            nanogui::mainloop();

        }


     nanogui::shutdown();
    return 0;
}
