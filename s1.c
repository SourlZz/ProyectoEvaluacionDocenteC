#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080        // Puerto de conexión
#define MAX_CLIENTS 10   // Número máximo de clientes
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
struct Question questions[30];

// inializar profesores----------------------------------------------------------------------------------------
void initializeProfessors()
{
    struct Professor sampleProfessors[10] = {
        {"Profesor 1", "Tiempo completo", 0},
        {"Profesor 2", "Medio tiempo", 0},
        {"Profesor 3", "Por hora", 0},
        {"Profesor 4", "Medio tiempo", 0},
        {"Profesor 5", "Por hora", 0},
        {"Profesor 6", "Tiempo completo", 0},
        {"Profesor 7", "Medio tiempo", 0},
        {"Profesor 8", "Tiempo completo", 0},
        {"Profesor 9", "Por hora", 0},
        {"Profesor 10", "Medio tiempo", 0}};

    for (int i = 0; i < 10; i++)
    {
        strcpy(professors[i].name, sampleProfessors[i].name);         // Copia el nombre del profesor
        strcpy(professors[i].schedule, sampleProfessors[i].schedule); // Copia el horario del profesor
        professors[i].score = sampleProfessors[i].score;              // Copia el puntaje del profesor
    }
}
// inicializar preguntas----------------------------------------------------------------------------------------
void initializeQuestions()
{
    struct Question sampleQuestions[30] = {
        {"¿Cómo calificarías la claridad de las explicaciones del profesor?", 0},
        {"¿El profesor fomenta un ambiente de respeto en el aula?", 0},
        {"¿El profesor utiliza una variedad de recursos didácticos en sus clases?", 0},
        {"¿El profesor demuestra un conocimiento sólido del tema que enseña?", 0},
        {"¿El profesor proporciona retroalimentación constructiva sobre el desempeño de los estudiantes?", 0},
        {"¿Las evaluaciones del profesor son justas y apropiadas para medir el conocimiento de los estudiantes?", 0},
        {"¿El profesor utiliza estrategias efectivas para mantener el interés de los estudiantes en el tema?", 0},
        {"¿El profesor es puntual y cumple con las fechas de entrega de las calificaciones?", 0},
        {"¿El profesor está disponible para consultas y aclaraciones fuera de las horas de clase?", 0},
        {"¿Cómo calificarías la organización de las clases del profesor?", 0},
        {"¿El profesor fomenta la participación activa de los estudiantes en las clases?", 0},
        {"¿El profesor promueve el pensamiento crítico y la resolución de problemas?", 0},
        {"¿El profesor se comunica de manera efectiva con los estudiantes y responde a sus preguntas?", 0},
        {"¿El profesor adapta su enseñanza a las necesidades individuales de los estudiantes?", 0},
        {"¿El profesor utiliza tecnología de manera efectiva en sus clases?", 0},
        {"¿El profesor promueve el trabajo en equipo y la colaboración entre los estudiantes?", 0},
        {"¿El profesor sigue un plan de estudios coherente y relevante?", 0},
        {"¿El profesor está al tanto de las tendencias y avances en su campo de estudio?", 0},
        {"¿El profesor fomenta el desarrollo de habilidades de investigación y análisis?", 0},
        {"¿El profesor brinda oportunidades para que los estudiantes expresen sus opiniones y perspectivas?", 0},
        {"¿El profesor demuestra empatía y comprensión hacia las dificultades de los estudiantes?", 0},
        {"¿El profesor promueve la ética académica y la integridad?", 0},
        {"¿El profesor ofrece oportunidades para la retroalimentación de los estudiantes sobre su enseñanza?", 0},
        {"¿Cómo calificarías la disponibilidad del profesor para reuniones y consultas individuales?", 0},
        {"¿El profesor utiliza ejemplos prácticos y aplicados en sus clases?", 0},
        {"¿El profesor fomenta la creatividad y la innovación en el aprendizaje?", 0},
        {"¿El profesor establece expectativas claras en cuanto a los requisitos del curso?", 0},
        {"¿El profesor evalúa de manera justa el progreso de los estudiantes a lo largo del curso?", 0},
        {"¿El profesor brinda orientación académica y apoyo a los estudiantes que lo necesitan?", 0},
        {"¿El profesor demuestra pasión y entusiasmo por la materia que enseña?", 0}};

    for (int i = 0; i < 30; i++)
    {
        strcpy(questions[i].text, sampleQuestions[i].text); // Copia el texto de la pregunta
        questions[i].answer = sampleQuestions[i].answer;    // Copia la respuesta de la pregunta
    }
}

int main()
{
    int server_socket, client_socket;            // Sockets del servidor y del cliente
    struct sockaddr_in server_addr, client_addr; // Estructuras para almacenar la información del servidor y del cliente
    socklen_t client_len = sizeof(client_addr);  // Tamaño de la estructura del cliente
    int selectedProfessor;                       // Profesor seleccionado por el cliente
    initializeProfessors();                      // Inicializa la lista de profesores
    initializeQuestions();                       // Inicializa la lista de preguntas

    // Crear socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error al crear el socket del servidor");
        exit(1);
    }

    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_port = htons(PORT);       // Puerto de conexión
    server_addr.sin_addr.s_addr = INADDR_ANY; // Cualquier dirección IP

    // Vincular el socket a una dirección y puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error al vincular el socket");
        exit(1);
    }

    // Escuchar conexiones entrantes
    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Error al escuchar conexiones");
        exit(1);
    }

    printf("Servidor esperando conexiones en el puerto %d...\n", PORT); // Mensaje de espera

    while (1)
    {
        // Aceptar una conexión entrante
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        // Verifica si hubo un error al aceptar la conexión
        if (client_socket == -1)
        {
            perror("Error al aceptar la conexión");
            continue;
        }
        // Imprime un mensaje de conexión exitosa
        printf("Cliente conectado.\n");

        // Variables para almacenar la opción del cliente
        int client_choice;

        // Recibe la elección del cliente
        if (recv(client_socket, &client_choice, sizeof(int), 0) == -1)
        {
            perror("Error al recibir la opción del cliente");
            close(client_socket);
            continue;
        }
    // Evalúa la elección del cliente
        switch (client_choice)
        {
        case 1:
            // Opción 1: Evaluación docente
            // Envía la lista de profesores al cliente
            if (send(client_socket, professors, sizeof(professors), 0) == -1)
            {
                perror("Error al enviar la lista de profesores al cliente");
                close(client_socket);
                break;
            }

            // Recibe la elección del profesor por parte del cliente
            int selectedProfessor;
            if (recv(client_socket, &selectedProfessor, sizeof(int), 0) == -1)
            {
                perror("Error al recibir la elección del profesor");
                close(client_socket);
                break;
            }
            // Verifica que la elección del profesor sea válida
            if (selectedProfessor < 1 || selectedProfessor > 10)
            {
                printf("Selección de profesor no válida. Cerrando la conexión.\n");
                close(client_socket);
                break;
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            struct Question randomQuestions[MAX_QUESTIONS];
            for (int i = 0; i < MAX_QUESTIONS; i++)
            {
                // Genera preguntas aleatorias
                int randomIndex = rand() % 30; // 30 es el número total de preguntas
                strcpy(randomQuestions[i].text, questions[randomIndex].text); // Copia el texto de la pregunta
                randomQuestions[i].answer = questions[randomIndex].answer; // Copia la respuesta de la pregunta
            }

            // Envía las preguntas aleatorias al cliente
            if (send(client_socket, randomQuestions, sizeof(randomQuestions), 0) == -1)
            {
                perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                break;
            }

            // Implementa la lógica para seleccionar y enviar las preguntas al cliente
            if (send(client_socket, questions, sizeof(questions), 0) == -1)
            {
                perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                break;
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            for (int i = 0; i < 10; i++)
            {
                // Selecciona una pregunta aleatoria del arreglo de preguntas
                int randomIndex = rand() % 10; // 10 es el número total de preguntas 
                struct Question randomQuestion = questions[randomIndex]; // Copia la pregunta aleatoria

                // Envía la pregunta al cliente
                if (send(client_socket, &randomQuestion, sizeof(struct Question), 0) == -1)
                {
                    perror("Error al enviar la pregunta al cliente");
                    close(client_socket);
                    break;
                }

                // Recibe la respuesta del cliente (1, 2 o 3)
                int clientAnswer; // Respuesta del cliente
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                    perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    break;
                }

                // Actualiza el puntaje del profesor según la respuesta del cliente
                professors[selectedProfessor - 1].score += clientAnswer; // Suma la respuesta del cliente al puntaje del profesor
            }
            // Envía un mensaje de agradecimiento al cliente
            printf("Evaluación docente completada. Gracias por participar.\n");
            break;

        case 2:
            // Opción 2: Cerrar sesión del cliente
            // Cierra la conexión con el cliente
            close(client_socket);
            printf("Cliente desconectado.\n");
            break;

        default:
            // Opción no válida
            printf("Opción no válida. Cerrando la conexión.\n");
            close(client_socket);
            break;
        }
    }

    // Cerrar el socket del servidor (esto no se alcanza porque el bucle es infinito)
    close(server_socket);
    return 0;
}
