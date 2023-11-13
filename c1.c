#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <ncurses.h>

#define PORT 8080        // Puerto de conexión
#define MAX_QUESTIONS 10 // Número máximo de preguntas
int errorControl;
// Estructuras ---------------------------------------------------------------------------------------------------
struct Professor
{
    char name[50];
    char schedule[20];
    int score;
};

struct Question
{
    char text[150];
    int answer;
};

struct Professor professors[10];

// Reconección ---------------------------------------------------------------------------------------------------
int establishConnection()
{
    int client_socket;
    struct sockaddr_in server_addr;

    // Crear socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error al crear el socket del cliente");
        scanf("%d", &errorControl);
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Cambia la dirección IP según tu configuración.

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)

    {
        perror("Error al conectar al servidor");
        scanf("%d", &errorControl);
        exit(1);
    }

    return client_socket;
}

// MAIN ----------------------------------------------------------------------------------------------------------
int main()
{
    initscr();     // Inicializa la pantalla de ncurses
    start_color(); // Habilita el uso de colores

    init_pair(1, COLOR_BLACK, COLOR_WHITE); // Define un par de colores: texto blanco sobre fondo negro
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // Define otro par de colores: texto amarillo sobre fondo rojo

    // Establece el color de fondo y el color del texto
    bkgd(COLOR_PAIR(1));   // Establece el color de fondo en blanco (par de colores 1)
    attron(COLOR_PAIR(2)); // Establece el color del texto en amarillo (par de colores 2)

    refresh(); // Actualiza la pantalla

    int client_socket;     // Socket del cliente
    int selectedProfessor; // Profesor seleccionado por el cliente
    int client_choice;
    char pasarPantallar; // Opción elegida por el cliente
    int errorControl;    // Control de errores

    printw("Conectado al servidor. Elija una opción:\n");

    while (1)
    {

        // Loop principal del cliente
        client_socket = establishConnection(); // Establece la conexión con el servidor
        clear();                               // Limpia la pantalla
        // Muestra las opciones disponibles al cliente
        // Get the dimensions of the terminal window
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);

        // Calculate the position of the menu
        int menu_y = max_y / 2 - 5;  // 5 is the number of lines in the menu
        int menu_x = max_x / 2 - 15; // 15 is half the width of the menu

        // Print the menu at the calculated position
        printw("\n");
        printw("%*s------------------------------\n", menu_x, "");
        printw("%*s|       MENU PRINCIPAL       |\n", menu_x, "");
        printw("%*s------------------------------\n", menu_x, "");
        printw("%*s| 1. Evaluación docente      |\n", menu_x, "");
        printw("%*s| 2. Mostrar lista de profes |\n", menu_x, "");
        printw("%*s|    -ores mejor evaluados   |\n", menu_x, "");
        printw("%*s| 3. Cerrar sesión          |\n", menu_x, "");
        printw("%*s------------------------------\n", menu_x, "");
        printw("\n");
        printw("\n");
        printw("\n");
        printw("%*sIngrese el número de la opción: ", menu_x, "");
        scanw("%d", &client_choice); // Lee la opción elegida por el cliente

        // Envía la elección del cliente al servidor
        if (send(client_socket, &client_choice, sizeof(int), 0) == -1)
        {
            perror("Error al enviar la elección al servidor");
            close(client_socket);
            exit(1);
        }
        // Evalúa la elección del cliente
        switch (client_choice)
        {
        case 1:

            // Opción 1: Evaluación docente
            if (recv(client_socket, professors, sizeof(professors), 0) == -1)
            {
                perror("Error al recibir la lista de profesores del servidor");
                close(client_socket);
                exit(1);
            }

            // Muestra la lista de profesores al cliente y permite que el cliente elija uno
            clear(); // Limpia la pantalla
            // Muestra la lista de profesores en una tabla centrada
            int table_width = 50;
            int table_height = 12;
            int table_x = (max_x - table_width) / 2;
            int table_y = ((max_y - 10) - table_height) / 2;
            mvprintw(table_y, table_x, "+------------------------------------------------+");
            mvprintw(table_y + 1, table_x, "| Lista de profesores                             |");
            mvprintw(table_y + 2, table_x, "+------------------------------------------------+");
            for (int i = 0; i < 10; i++)
            {
                mvprintw(table_y + 3 + i, table_x, "| %d. %-40s |\n", i + 1, professors[i].name);
            }
            mvprintw(table_y + 13, table_x, "+------------------------------------------------+");

            // Muestra el mensaje "Elija un profesor" centrado
            int message_width = 30;
            int message_x = (max_x - message_width) / 2;
            mvprintw(table_y + 15, message_x, "Elija un profesor (1-10): ");
            scanw("%d", &selectedProfessor); // Lee la elección del profesor
            while (selectedProfessor > 10 || selectedProfessor < 1)
            {
                refresh(); // Actualiza la pantalla
                clear();   // Limpia la entrada anterior
                mvprintw(table_y, table_x, "+------------------------------------------------+");
                mvprintw(table_y + 1, table_x, "| Lista de profesores                             |");
                mvprintw(table_y + 2, table_x, "+------------------------------------------------+");
                for (int i = 0; i < 10; i++)
                {
                    mvprintw(table_y + 3 + i, table_x, "| %d. %-40s |\n", i + 1, professors[i].name);
                }
                mvprintw(table_y + 13, table_x, "+------------------------------------------------+");
                mvprintw(table_y + 16, message_x -10, "Profesor no válido. Por favor, Elija un profesor  (1-10): ");
                scanw("%d", &selectedProfessor); // Lee la elección del profesor
            }
            // Envía la elección del profesor al servidor
            if (send(client_socket, &selectedProfessor, sizeof(int), 0) == -1)
            {
                perror("Error al enviar la elección del profesor al servidor");
                close(client_socket);
                exit(1);
            }

            // Implementa la lógica para recibir las preguntas del servidor
            struct Question receivedQuestions[MAX_QUESTIONS]; // Preguntas recibidas del servidor
            // Recibe las preguntas del servidor
            if (recv(client_socket, receivedQuestions, sizeof(receivedQuestions), 0) == -1)
            {
                perror("Error al recibir las preguntas del servidor");
                close(client_socket);
                exit(1);
            }

            // Realiza la evaluación respondiendo a las preguntas
            for (int i = 0; i < MAX_QUESTIONS; i++)
            {
                clear(); // Limpia la pantalla
                printw("%*sProfesor: %s\n", (int)((max_x - strlen(professors[selectedProfessor - 1].name)) / 2), "", professors[selectedProfessor - 1].name);
                printw("\n");
                printw("%*sPregunta %d: %s\n", (int)((max_x - strlen(receivedQuestions[i].text)) / 2), "", i + 1, receivedQuestions[i].text);

                printw("\n");
                printw("%*sSeleccione una respuesta (1-Mala, 2-Buena, 3-Excelente): ", (max_x - 45) / 2, ""); // Pide la respuesta al cliente
                int userAnswer;                                                                               // Respuesta del cliente
                scanw("%d", &userAnswer);                                                                     // Lee la respuesta del cliente

                // Envía la respuesta al servidor
                while (userAnswer > 3 || userAnswer < 1)
                {
                    refresh(); // Actualiza la pantalla
                    clear(); // Limpia la pantalla
                     printw("%*sProfesor: %s\n", (int)((max_x - strlen(professors[selectedProfessor - 1].name)) / 2), "", professors[selectedProfessor - 1].name);
                    printw("%*sPregunta %d: %s\n", (int)((max_x - strlen(receivedQuestions[i].text)) / 2), "", i + 1, receivedQuestions[i].text);
                     printw("\n");
                    printw("%*sRespuesta no válida. Por favor, seleccione una respuesta válida.\n", (max_x - 56) / 2, "");
                    printw("\n");
                    printw("%*sSeleccione una respuesta (1-Mala, 2-Buena, 3-Excelente): ", (max_x - 45) / 2, ""); // Pide la respuesta al cliente                                                    // Respuesta del cliente
                    scanw("%d", &userAnswer);                                                                     // Lee la respuesta del cliente
                }
                if (send(client_socket, &userAnswer, sizeof(int), 0) == -1)
                {
                    perror("Error al enviar la respuesta al servidor");
                    close(client_socket);
                    exit(1);
                }
            }
            clear();
            printw("%*sEvaluación completada. Gracias por participar.\n", (max_x - 48) / 2, "");
            printw("\n");
            printw("%*sPresione 1 y enter para continuar: ", (max_x - 36) / 2, "");
            scanw("%hhd", &pasarPantallar); // Lee la opción elegida por el cliente
            close(client_socket);           // Cierra la conexión con el servidor
            break;
        case 2:
            // Mostrar lista de profesores mejor evaluados
            clear(); // Limpia la pantalla

            if (recv(client_socket, professors, sizeof(professors), 0) == -1)
            {
                perror("Error al recibir la lista de profesores del servidor");
                close(client_socket);
                exit(1);
            }
            // aqui se imprime en pantalla la lista de los profesores
            // mvprintw(table_y, message_x, "Lista de profesores:");

            // Calcula el ancho de cada columna de la tabla
            int nameWidth = 20;
            int scoreWidth = 10;
            int scheduleWidth = 20;

            // Calcula el ancho total de la tabla
            int tableWidth = nameWidth + scoreWidth + scheduleWidth;

            // Calcula la posición x de la tabla para centrarla
            int tableX = (COLS - tableWidth) / 2;

            // Calcula la posición y de la tabla para centrarla
            int tableY = (LINES - 18) / 2;

            printw("%*s+---------------------------------------------+\n", menu_x, "");
            printw("%*s|    Lista de profesores mejores evaluados    |\n", menu_x, "");
            printw("%*s+---------------------------------------------+\n", menu_x, "");

            // Imprime los encabezados de la tabla
            mvprintw(tableY, tableX, "+%-*s+%-*s+%-*s+", nameWidth, "--------------------", scoreWidth, "----------", scheduleWidth, "--------------------");
            mvprintw(tableY + 1, tableX, "|%-*s|%-*s|%-*s|", nameWidth, "Nombre", scoreWidth, "Puntuación", scheduleWidth - 1, "Horario");
            mvprintw(tableY + 2, tableX, "+%*s+%*s+%*s+", nameWidth, "--------------------", scoreWidth, "----------", scheduleWidth, "--------------------");

            // Imprime cada fila de la tabla con el formato adecuado
            for (int i = 0; i < 10; i++)
            {
                mvprintw(tableY + 3 + i, tableX, "|%-*s|%-*d|%-*s|", nameWidth, professors[i].name, scoreWidth, professors[i].score, scheduleWidth, professors[i].schedule);
                mvprintw(tableY + 4 + i, tableX, "+%*s+%*s+%*s+", nameWidth, "--------------------", scoreWidth, "----------", scheduleWidth, "--------------------");
            }
            printw("\n");
            printw("\n");
            printw("\n");

            printw("\t \t \t \t \t Presione 1 y enter para continuar: ");
            scanw("%hhd", &pasarPantallar); // Lee la opción elegida por el cliente
            close(client_socket);           // Cierra la conexión con el servidor
            break;
        case 3:
            // Opción 3: Cerrar sesión
            // Cierra la conexión con el servidor
            printw("Sesión cerrada. Hasta luego.\n");
            clear();
            close(client_socket);
            endwin(); // Cierra la pantalla de ncurses
            exit(0);
        default:
            printw("Opción no válida. Por favor, seleccione una opción válida.\n");
        }
    }
    close(client_socket); // Cierra la conexión con el servidor
    endwin();             // Cierra la pantalla de ncurses
    return 0;
}
