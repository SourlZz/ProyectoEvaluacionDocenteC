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

// Estructuras ---------------------------------------------------------------------------------------------------
struct Professor
{
    char name[50];
    char schedule[20];
    int score;
};

struct Question
{
    char text[100];
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
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Cambia la dirección IP según tu configuración.

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error al conectar al servidor");
        exit(1);
    }

    return client_socket;
}

// MAIN ----------------------------------------------------------------------------------------------------------
int main()
{
    int client_socket;         // Socket del cliente
    int selectedProfessor = 0; // Profesor seleccionado por el cliente
    int client_choice = 0;     // Opción elegida por el cliente
    printf("Conectado al servidor. Elija una opción:\n");

    while (1)
    {                    // Loop principal
        system("clear"); // Limpia la pantalla
        // Muestra las opciones disponibles al cliente
        printf("1. Evaluación docente\n");
        printf("2. Cerrar sesión\n");
        printf("Ingrese el número de la opción: ");
        scanf("%d", &client_choice);           // Lee la opción elegida por el cliente
        client_socket = establishConnection(); // Establece la conexión con el servidor

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
            printf("Elija un profesor (1-10): ");
            scanf("%d", &selectedProfessor); // Lee la elección del profesor

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
                printf("Pregunta %d: %s\n", i + 1, receivedQuestions[i].text); // Muestra la pregunta
                printf("Seleccione una respuesta (1-Mala, 2-Buena, 3-Excelente): "); // Pide la respuesta al cliente
                int userAnswer; // Respuesta del cliente
                scanf("%d", &userAnswer); // Lee la respuesta del cliente

                // Envía la respuesta al servidor
                if (send(client_socket, &userAnswer, sizeof(int), 0) == -1)
                {
                    perror("Error al enviar la respuesta al servidor");
                    close(client_socket);
                    exit(1);
                }
            }

            printf("Evaluación completada. Gracias por participar.\n");
            close(client_socket); // Cierra la conexión con el servidor
            break;

        case 2:
            // Opción 2: Cerrar sesión
            // Cierra la conexión con el servidor
            close(client_socket);
            printf("Sesión cerrada. Hasta luego.\n");
            system("clear");
            exit(0);
        default:
            printf("Opción no válida. Por favor, seleccione una opción válida.\n");
        }
    }

    close(client_socket); // Cierra la conexión con el servidor
    return 0;
}
