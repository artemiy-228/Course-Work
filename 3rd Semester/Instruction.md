# Instruction to my programm


1. Download MariaDB server from [here](https://mariadb.org/download/?t=mariadb&p=mariadb&r=11.0.4&os=windows&cpu=x86_64&pkg=msi&m=icam) and install it on your computer. In my program, there is one superuser for simplicity. The username is root with the password 1234. However, you can set your own password if you wish. After downloading and installing, and creating a user, you need to import my database, which is located in the file task_manager.sql. Here is an example command for the root user: `mysql -u root -p task_manager < task_manager.sql`. The `-p` flag indicates the password. Note: MySQL/MariaDB are interchangeable during server installation.

2. After installing the server, you will need to install the MariaDB C++ Connector from [here](https://mariadb.com/downloads/connectors/connectors-data-access/cpp-connector). Once installed, navigate to Visual Studio. The library is set up by default for MSVC. Next, you'll need to link the library. You can follow an example from [this video](https://www.youtube.com/watch?v=NLv8WV43TuA&t=48s).

3. After installing the database connector library, you will need to install the library package for nanogui. For your convenience, I have provided them in the CPP_Libraries archive. However, if needed, you can find all cmake files for building library files on the nanogui GitHub page. The version of the MariaDB Connector is 1.02. I didn't include it as it is large.

4. Similarly, connect the rest of the files to your Microsoft Visual Studio project.

After completing these steps, you can run the program.
