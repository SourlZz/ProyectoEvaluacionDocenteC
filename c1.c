#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

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

#ifdef _WIN32 // Windows
    system("color f0");
    printf("\033[31m"); // Cambia el color del cursor a rojo
#else
    printf("\033[47;30m"); // Linux
#endif

    // LE MOVI TANTITO Y SE BUGUEO AAAAA
    int client_socket;         // Socket del cliente
    int selectedProfessor; // Profesor seleccionado por el cliente
    int client_choice; 
    char pasarPantallar;    // Opción elegida por el cliente
    int errorControl;     // Control de errores
    printf("Conectado al servidor. Elija una opción:\n");

    while (1)
    {     
        // Loop principal del cliente
        client_socket = establishConnection(); // Establece la conexión con el servidor               
        system("clear"); // Limpia la pantalla
        // Muestra las opciones disponibles al cliente
        printf("1. Evaluación docente\n");
        printf("2. Mostrar lista de profesores mejor evaluados\n");
        printf("3. Cerrar sesión\n");
        printf("Ingrese el número de la opción: ");
        scanf("%d", &client_choice);           // Lee la opción elegida por el cliente
        

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
            system("clear"); // Limpia la pantalla
            printf("Lista de profesores:\n");
            for (int i = 0; i < 10; i++)
            {
                // Muestra la lista de profesores
                printf("%d. %s (Puntaje: %d) - %s\n", i + 1, professors[i].name, professors[i].score, professors[i].schedule);
            }
            printf("Elija un profesor (1-10): \n");
            scanf("%d", &selectedProfessor); // Lee la elección del profesor
           
            while (selectedProfessor > 10 || selectedProfessor < 1)
            {
                printf("Profesor no válido. Por favor, seleccione un profesor válido.\n");
                printf("Elija un profesor (1-10): ");
                scanf("%d", &selectedProfessor); // Lee la elección del profesor
                
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
                system("clear"); // Limpia la pantalla
                printf("Profesor: %s\n", professors[selectedProfessor - 1].name); // Muestra el nombre del profesor")                                                    
                printf("Pregunta %d: %s\n", i + 1, receivedQuestions[i].text);       // Muestra la pregunta
                printf("Seleccione una respuesta (1-Mala, 2-Buena, 3-Excelente): "); // Pide la respuesta al cliente
                int userAnswer;                                                      // Respuesta del cliente
                scanf("%d", &userAnswer);                                            // Lee la respuesta del cliente

                // Envía la respuesta al servidor
                while (userAnswer > 3 || userAnswer < 1)
                {
                    printf("Respuesta no válida. Por favor, seleccione una respuesta válida.\n");
                    printf("Seleccione una respuesta (1-Mala, 2-Buena, 3-Excelente): "); // Pide la respuesta al cliente                                                    // Respuesta del cliente
                    scanf("%d", &userAnswer);                                            // Lee la respuesta del cliente
                }
                if (send(client_socket, &userAnswer, sizeof(int), 0) == -1)
                {
                    perror("Error al enviar la respuesta al servidor");
                    close(client_socket);
                    exit(1);
                }
            }
            system("clear");
            printf("Evaluación completada. Gracias por participar.\n");
            printf("Presione 1 y enter para continuar: ");
            scanf("%hhd", &pasarPantallar); // Lee la opción elegida por el cliente
            close(client_socket); // Cierra la conexión con el servidor
            break;
        case 2:
            // Mostrar lista de profesores mejor evaluados
            system("clear"); // Limpia la pantalla

            if (recv(client_socket, professors, sizeof(professors), 0) == -1)
            {
                perror("Error al recibir la lista de profesores del servidor");
                close(client_socket);
                exit(1);
            }
            // aqui se imprime en pantalla la lista de los profesores
            printf("Lista de profesores:\n");
            for (int i = 0; i < 10; i++)
            {
                // Muestra la lista de profesores
                printf("%d. %s (Puntaje: %d) - %s\n", i + 1, professors[i].name, professors[i].score, professors[i].schedule);
            }
            printf("Presione 1 y enter para continuar: ");
            scanf("%hhd", &pasarPantallar); // Lee la opción elegida por el cliente
            close(client_socket);         // Cierra la conexión con el servidor
            break;
        case 3:
            // Opción 2: Cerrar sesión
            // Cierra la conexión con el servidor
            printf("Sesión cerrada. Hasta luego.\n");
            system("clear");
            close(client_socket);
            #ifdef _WIN32 // Windows
    system("color 0f");
    printf("\033[31m"); // Cambia el color del cursor a rojo
#else
    printf("\033[47;30m"); // Linux
#endif
            exit(0);
        default:
            printf("Opción no válida. Por favor, seleccione una opción válida.\n");
        }
    }
    close(client_socket); // Cierra la conexión con el servidor    
    #ifdef _WIN32 // Windows
    system("color 0f");
    printf("\033[31m"); // Cambia el color del cursor a rojo
#else
    printf("\033[47;30m"); // Linux
#endif
    return 0;
}
