#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <pthread.h> // Librería para hilos
// Constantes ---------------------------------------------------------------------------------------------------
#define PORT 8080         // Puerto de conexión
#define MAX_CLIENTS 10    // Número máximo de clientes
#define MAX_QUESTIONS 10  // Número máximo de preguntas
#define MAX_PROFESSORS 10 // Número máximo de profesores

// Estructuras ---------------------------------------------------------------------------------------------------

struct Question
{
    char text[150];
    int answer;
};
// Estructura para almacenar la información de un profesor
struct Professor
{
    char name[50];
    int score;
};
// Arreglos de preguntasS
struct Question fquestions[30];
struct Question pquestions[30];
struct Question hquestions[30];
struct Question personalquestions[10];
int bandera = 0;
// Arreglos de profesores
struct Professor fullTimeProfessors[MAX_PROFESSORS];
struct Professor partTimeProfessors[MAX_PROFESSORS];
struct Professor hourlyProfessors[MAX_PROFESSORS];

// Profesores Ordenados
struct Professor FullOrdenedProfessors[MAX_PROFESSORS];
struct Professor PartOrdenedProfessors[MAX_PROFESSORS];
struct Professor HourlyOrdenedProfessors[MAX_PROFESSORS];

// Profesores ---------------------------------------------------------------------------------------------------

// Función para inicializar los arreglos de profesores
void initializeProfessors()
{
    // Arreglo de profesores de tiempo completo
    struct Professor p1[MAX_PROFESSORS] = {
        {"ProfesorF 1", 0},
        {"ProfesorF 2", 0},
        {"ProfesorF 3", 0},
        {"ProfesorF 4", 0},
        {"ProfesorF 5", 0},
        {"ProfesorF 6", 0},
        {"ProfesorF 7", 0},
        {"ProfesorF 8", 0},
        {"ProfesorF 9", 0},
        {"ProfesorF 10", 0}};
    // Arreglo de profesores de medio tiempo
    struct Professor p2[MAX_PROFESSORS] = {
        {"ProfesorP 1", 0},
        {"ProfesorP 2", 0},
        {"ProfesorP 3", 0},
        {"ProfesorP 4", 0},
        {"ProfesorP 5", 0},
        {"ProfesorP 6", 0},
        {"ProfesorP 7", 0},
        {"ProfesorP 8", 0},
        {"ProfesorP 9", 0},
        {"ProfesorP 10", 0}};
    // Arreglo de profesores por hora
    struct Professor p3[MAX_PROFESSORS] = {
        {"ProfesorH 1", 0},
        {"ProfesorH 2", 0},
        {"ProfesorH 3", 0},
        {"ProfesorH 4", 0},
        {"ProfesorH 5", 0},
        {"ProfesorH 6", 0},
        {"ProfesorH 7", 0},
        {"ProfesorH 8", 0},
        {"ProfesorH 9", 0},
        {"ProfesorH 10", 0}};
    // Inicializar el arreglo de profesores de tiempo completo
    for (int i = 0; i < 10; i++)
    {
        strcpy(fullTimeProfessors[i].name, p1[i].name); // Copia el nombre del profesor
        fullTimeProfessors[i].score = p1[i].score;
    }

    // Inicializar el arreglo de profesores de medio tiempo
    for (int i = 0; i < 10; i++)
    {
        strcpy(partTimeProfessors[i].name, p2[i].name); // Copia el nombre del profesor
        partTimeProfessors[i].score = p2[i].score;
    }

    // Inicializar el arreglo de profesores por hora
    for (int i = 0; i < 10; i++)
    {
        strcpy(hourlyProfessors[i].name, p3[i].name); // Copia el nombre del profesor
        hourlyProfessors[i].score = p3[i].score;
    }
}

// inicializar preguntas----------------------------------------------------------------------------------------
void initializeQuestions()
{
    // Arreglo de preguntas de tiempo completo
    struct Question FQuestions[50] = {
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
        {"¿El profesor demuestra pasión y entusiasmo por la materia que enseña?", 0},
        {"¿Cómo evalúas la distribución de las horas de trabajo del profesor a lo largo de la semana?", 0},
        {"¿El profesor demuestra un buen manejo del tiempo durante las clases y actividades programadas?", 0},
        {"¿Sientes que el profesor utiliza eficientemente el tiempo disponible para cubrir los temas asignados?", 0},
        {"¿El profesor es consistente en su disponibilidad durante las horas de oficina y para reuniones programadas?", 0},
        {"¿Cómo calificarías la organización general de las tareas y responsabilidades del profesor a tiempo completo?", 0},
        {"¿El profesor muestra flexibilidad en la programación de actividades fuera del horario regular de clases?", 0},
        {"¿Cómo percibes la carga de trabajo asignada por el profesor en relación con el tiempo disponible?", 0},
        {"¿El profesor establece expectativas claras sobre la dedicación de tiempo requerida fuera de las clases?", 0},
        {"¿Sientes que el profesor equilibra adecuadamente sus responsabilidades académicas con otras obligaciones laborales?", 0},
        {"¿El profesor ofrece un ambiente de trabajo que facilita la colaboración y el intercambio de ideas?", 0}};
    // Inicializar el arreglo de preguntas medio tiempo
    struct Question PQuestions[30] = {
        {"¿Cómo calificarías la claridad de las explicaciones del profesor de medio tiempo?", 0},
        {"¿El profesor fomenta un ambiente de respeto en el aula?", 0},
        {"¿El profesor utiliza una variedad de recursos didácticos en sus clases?", 0},
        {"¿El profesor demuestra un conocimiento sólido del tema que enseña?", 0},
        {"¿El profesor proporciona retroalimentación constructiva sobre el desempeño de los estudiantes?", 0},
        {"¿Las evaluaciones del profesor son justas y apropiadas para medir el conocimiento de los estudiantes?", 0},
        {"¿El profesor utiliza estrategias efectivas para mantener el interés de los estudiantes en el tema?", 0},
        {"¿El profesor es puntual y cumple con las fechas de entrega de las calificaciones?", 0},
        {"¿El profesor está disponible para consultas y aclaraciones fuera de las horas de clase?", 0},
        {"¿Cómo calificarías la organización de las clases del profesor de medio tiempo?", 0},
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
        {"¿Cómo calificarías la disponibilidad del profesor de medio tiempo para reuniones y consultas individuales?", 0},
        {"¿El profesor utiliza ejemplos prácticos y aplicados en sus clases?", 0},
        {"¿El profesor fomenta la creatividad y la innovación en el aprendizaje?", 0},
        {"¿El profesor establece expectativas claras en cuanto a los requisitos del curso?", 0},
        {"¿El profesor evalúa de manera justa el progreso de los estudiantes a lo largo del curso?", 0},
        {"¿El profesor brinda orientación académica y apoyo a los estudiantes que lo necesitan?", 0},
        {"¿El profesor demuestra pasión y entusiasmo por la materia que enseña?", 0}};
    // Inicializar el arreglo de preguntas por hora
    struct Question HQuestions[30] = {
        {"¿Cómo calificarías la claridad de las explicaciones del profesor en la única hora que imparte?", 0},
        {"¿El profesor fomenta un ambiente de respeto en el aula durante la única hora de clase?", 0},
        {"¿El profesor utiliza una variedad de recursos didácticos en esa hora de clase?", 0},
        {"¿El profesor demuestra un conocimiento sólido del tema que enseña en la única hora de clase?", 0},
        {"¿El profesor proporciona retroalimentación constructiva sobre el desempeño de los estudiantes durante la clase?", 0},
        {"¿La evaluación del profesor en esa hora es justa y apropiada para medir el conocimiento de los estudiantes?", 0},
        {"¿El profesor utiliza estrategias efectivas para mantener el interés de los estudiantes durante la clase?", 0},
        {"¿El profesor es puntual y cumple con las fechas de entrega de las calificaciones para esa hora de clase?", 0},
        {"¿El profesor está disponible para consultas y aclaraciones relacionadas con esa hora fuera del horario de clase?", 0},
        {"¿Cómo calificarías la organización de esa única clase del profesor?", 0},
        {"¿El profesor fomenta la participación activa de los estudiantes durante esa única hora de clase?", 0},
        {"¿El profesor promueve el pensamiento crítico y la resolución de problemas en esa hora de clase?", 0},
        {"¿El profesor se comunica de manera efectiva con los estudiantes y responde a sus preguntas durante esa hora?", 0},
        {"¿El profesor adapta su enseñanza a las necesidades individuales de los estudiantes durante esa única hora?", 0},
        {"¿El profesor utiliza tecnología de manera efectiva en esa hora de clase?", 0},
        {"¿El profesor promueve el trabajo en equipo y la colaboración entre los estudiantes en esa hora de clase?", 0},
        {"¿El profesor sigue un plan de estudios coherente y relevante durante esa única hora de clase?", 0},
        {"¿El profesor está al tanto de las tendencias y avances en su campo de estudio durante esa hora?", 0},
        {"¿El profesor fomenta el desarrollo de habilidades de investigación y análisis durante esa única hora de clase?", 0},
        {"¿El profesor brinda oportunidades para que los estudiantes expresen sus opiniones y perspectivas durante esa hora?", 0},
        {"¿El profesor demuestra empatía y comprensión hacia las dificultades de los estudiantes durante esa única hora?", 0},
        {"¿El profesor promueve la ética académica y la integridad durante esa hora de clase?", 0},
        {"¿El profesor ofrece oportunidades para la retroalimentación de los estudiantes sobre esa hora de enseñanza?", 0},
        {"¿Cómo calificarías la disponibilidad del profesor para reuniones y consultas individuales relacionadas con esa hora de clase?", 0},
        {"¿El profesor utiliza ejemplos prácticos y aplicados durante esa única hora de clase?", 0},
        {"¿El profesor fomenta la creatividad y la innovación en el aprendizaje durante esa hora?", 0},
        {"¿El profesor establece expectativas claras en cuanto a los requisitos de esa única clase?", 0},
        {"¿El profesor evalúa de manera justa el progreso de los estudiantes durante esa única hora de clase?", 0},
        {"¿El profesor brinda orientación académica y apoyo a los estudiantes que lo necesitan durante esa hora?", 0},
        {"¿El profesor demuestra pasión y entusiasmo por la materia que enseña durante esa hora de clase?", 0}};

    struct Question personalQuestions[10] =
        {
            {"¿Cómo describirías el ambiente general en el salón de clases?", 0},
            {"¿Te sientes cómodo/a con la disposición del mobiliario en el salón?", 0},
            {"¿Cómo afecta la iluminación y ventilación en tu experiencia en el salón?", 0},
            {"¿El entorno físico del salón de clases contribuye a tu concentración y enfoque?", 0},
            {"¿Cómo evalúas la comodidad de las instalaciones, como sillas, mesas y pizarras?", 0},
            {"¿Te sientes motivado/a por la decoración o diseño visual del salón de clases?", 0},
            {"¿Cómo impacta la limpieza y orden del salón en tu experiencia de aprendizaje?", 0},
            {"¿El espacio del salón de clases favorece la interacción y colaboración entre estudiantes?", 0},
            {"¿Sientes que hay suficientes recursos disponibles en el salón para apoyar tu aprendizaje?", 0},
            {"¿Cómo describirías la atmósfera general de la escuela en términos de amistad y comunidad?", 0}

        };
    for (int i = 0; i < 30; i++)
    {
        strcpy(fquestions[i].text, FQuestions[i].text); // Copia el texto de la pregunta
        fquestions[i].answer = FQuestions[i].answer;    // Copia la respuesta de la pregunta
    }
    for (int i = 0; i < 30; i++)
    {
        strcpy(pquestions[i].text, PQuestions[i].text); // Copia el texto de la pregunta
        pquestions[i].answer = PQuestions[i].answer;    // Copia la respuesta de la pregunta
    }
    for (int i = 0; i < 30; i++)
    {
        strcpy(hquestions[i].text, HQuestions[i].text); // Copia el texto de la pregunta
        hquestions[i].answer = HQuestions[i].answer;    // Copia la respuesta de la pregunta
    }
    for (int i = 0; i < 10; i++)
    {
        strcpy(personalquestions[i].text, personalQuestions[i].text); // Copia el texto de la pregunta
        personalquestions[i].answer = personalQuestions[i].answer;    // Copia la respuesta de la pregunta
    }
}
// Estructura para almacenar la información del cliente
struct ClientInfo
{
    int socket;
    struct sockaddr_in address;
};

// Función para manejar la conexión de un cliente
void *handleClient(void *arg)
{
    struct ClientInfo *clientInfo = (struct ClientInfo *)arg;
    int client_socket = clientInfo->socket;
    struct sockaddr_in client_addr = clientInfo->address;
    int selectedProfessor; // Profesor seleccionado por el cliente

    // Variables para almacenar la opción del cliente
    int client_choice;
    int typeProfessor;
    int typeProfessor2;
    int clientAnswer; // Respuesta del cliente
    // Recibe la elección del cliente
    if (send(client_socket, &bandera, sizeof(bandera), 0) == -1)
    {
        //perror("Error al enviar la lista de profesores al cliente");
        close(client_socket);
        pthread_exit(NULL);
    }
    if (recv(client_socket, &client_choice, sizeof(int), 0) == -1)
    {
        //perror("Error al recibir la opción del cliente");
        close(client_socket);
        pthread_exit(NULL);
    }
    // Evalúa la elección del cliente
    switch (client_choice)
    {
    case 1:
        // Opción 1: Evaluación docente
        // Recibe el tipo de profesor

        if (recv(client_socket, &typeProfessor, sizeof(int), 0) == -1)
        {
           // perror("Error al recibir la elección del profesor");
            close(client_socket);
            pthread_exit(NULL);
        }
        switch (typeProfessor)
        {
            // Tiempo completo
        case 1:
         bandera++;
            if (send(client_socket, fullTimeProfessors, sizeof(fullTimeProfessors), 0) == -1)
            {
                // perror("Error al enviar la lista de profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Recibe la elección del profesor por parte del cliente
            if (recv(client_socket, &selectedProfessor, sizeof(int), 0) == -1)
            {
               // perror("Error al recibir la elección del profesor");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Verifica que la elección del profesor sea válida
            if (selectedProfessor < 1 || selectedProfessor > 10)
            {
                printf("Profesor no válido. Cerrando la conexión.\n");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            struct Question randomQuestions[MAX_QUESTIONS];
            for (int i = 0; i < MAX_QUESTIONS; i++)
            {
                // Genera preguntas aleatorias
                int randomIndex = rand() % 30;                                 // 30 es el número total de preguntas
                strcpy(randomQuestions[i].text, fquestions[randomIndex].text); // Copia el texto de la pregunta
                randomQuestions[i].answer = fquestions[randomIndex].answer;    // Copia la respuesta de la pregunta
            }

            // Envía las preguntas aleatorias al cliente
             if (send(client_socket, randomQuestions, sizeof(randomQuestions), 0) == -1)
            {
                // perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            
            // Implementa la lógica para seleccionar y enviar las preguntas al cliente
            if (send(client_socket, personalquestions, sizeof(personalquestions), 0) == -1)
            {
               // perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            for (int i = 0; i < 10; i++)
            {
                // Selecciona una pregunta aleatoria del arreglo de preguntas
                int randomIndex = rand() % 10;                            // 10 es el número total de preguntas
                struct Question randomQuestion = fquestions[randomIndex]; // Copia la pregunta aleatoria

                // Envía la pregunta al cliente
                if (send(client_socket, &randomQuestion, sizeof(struct Question), 0) == -1)
                {
                    //perror("Error al enviar la pregunta al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }

                // Recibe la respuesta del cliente (1, 2 o 3)

                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                   // perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
                // Actualiza el puntaje del profesor según la respuesta del cliente
                fullTimeProfessors[selectedProfessor - 1].score += clientAnswer; // Suma la respuesta del cliente al puntaje del profesor
            }
            // Envía un mensaje de agradecimiento al cliente
            printf("Evaluación docente completada. Gracias por participar.\n");
            for (int i = 0; i < 10; i++)
            {
                if (send(client_socket, personalquestions[i].text, sizeof(personalquestions[i].text), 0) == -1)
                {
                    //perror("Error al enviar las preguntas al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                   // perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
            }
            break;
            // Medio tiempo
        case 2:
         bandera++;
            if (send(client_socket, partTimeProfessors, sizeof(partTimeProfessors), 0) == -1)
            {
                //perror("Error al enviar la lista de profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Recibe la elección del profesor por parte del cliente
            if (recv(client_socket, &selectedProfessor, sizeof(int), 0) == -1)
            {
                // perror("Error al recibir la elección del profesor");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Verifica que la elección del profesor sea válida
            if (selectedProfessor < 1 || selectedProfessor > 10)
            {
                printf("Profesor no válido. Cerrando la conexión.\n");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            struct Question randomQuestions1[MAX_QUESTIONS];
            for (int i = 0; i < MAX_QUESTIONS; i++)
            {
                // Genera preguntas aleatorias
                int randomIndex = rand() % 30;                                  // 30 es el número total de preguntas
                strcpy(randomQuestions1[i].text, pquestions[randomIndex].text); // Copia el texto de la pregunta
                randomQuestions1[i].answer = pquestions[randomIndex].answer;    // Copia la respuesta de la pregunta
            }

            // Envía las preguntas aleatorias al cliente
            if (send(client_socket, randomQuestions1, sizeof(randomQuestions1), 0) == -1)
            {
                // perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }

            // Implementa la lógica para seleccionar y enviar las preguntas al cliente
            if (send(client_socket, personalquestions, sizeof(personalquestions), 0) == -1)
            {
                // perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            for (int i = 0; i < 10; i++)
            {
                // Selecciona una pregunta aleatoria del arreglo de preguntas
                int randomIndex = rand() % 10;                             // 10 es el número total de preguntas
                struct Question randomQuestion1 = pquestions[randomIndex]; // Copia la pregunta aleatoria

                // Envía la pregunta al cliente
                if (send(client_socket, &randomQuestion1, sizeof(struct Question), 0) == -1)
                {
                    // perror("Error al enviar la pregunta al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }

                // Recibe la respuesta del cliente (1, 2 o 3)
                int clientAnswer; // Respuesta del cliente
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                    // perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }

                // Actualiza el puntaje del profesor según la respuesta del cliente
                partTimeProfessors[selectedProfessor - 1].score += clientAnswer; // Suma la respuesta del cliente al puntaje del profesor
            }
            // Envía un mensaje de agradecimiento al cliente
            printf("Evaluación docente completada. Gracias por participar.\n");
            for (int i = 0; i < 10; i++)
            {
                if (send(client_socket, personalquestions[i].text, sizeof(personalquestions[i].text), 0) == -1)
                {
                    //perror("Error al enviar las preguntas al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                    //perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
            }
            break;
        case 3:
         bandera++;
            if (send(client_socket, hourlyProfessors, sizeof(hourlyProfessors), 0) == -1)
            {
                //perror("Error al enviar la lista de profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Recibe la elección del profesor por parte del cliente
            if (recv(client_socket, &selectedProfessor, sizeof(int), 0) == -1)
            {
                //perror("Error al recibir la elección del profesor");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Verifica que la elección del profesor sea válida
            if (selectedProfessor < 1 || selectedProfessor > 10)
            {
                printf("Profesor no válido. Cerrando la conexión.\n");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            struct Question randomQuestions2[MAX_QUESTIONS];
            for (int i = 0; i < MAX_QUESTIONS; i++)
            {
                // Genera preguntas aleatorias
                int randomIndex = rand() % 30;                                  // 30 es el número total de preguntas
                strcpy(randomQuestions2[i].text, hquestions[randomIndex].text); // Copia el texto de la pregunta
                randomQuestions2[i].answer = hquestions[randomIndex].answer;    // Copia la respuesta de la pregunta
            }

            // Envía las preguntas aleatorias al cliente
            if (send(client_socket, randomQuestions2, sizeof(randomQuestions2), 0) == -1)
            {
                //perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }

            // Implementa la lógica para seleccionar y enviar las preguntas al cliente
            if (send(client_socket, personalquestions, sizeof(personalquestions), 0) == -1)
            {
                //perror("Error al enviar las preguntas al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            // Genera preguntas aleatorias y envía cada pregunta al cliente
            for (int i = 0; i < 10; i++)
            {
                // Selecciona una pregunta aleatoria del arreglo de preguntas
                int randomIndex = rand() % 10;                             // 10 es el número total de preguntas
                struct Question randomQuestion2 = hquestions[randomIndex]; // Copia la pregunta aleatoria

                // Envía la pregunta al cliente
                if (send(client_socket, &randomQuestion2, sizeof(struct Question), 0) == -1)
                {
                    perror("Error al enviar la pregunta al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }

                // Recibe la respuesta del cliente (1, 2 o 3)
                int clientAnswer; // Respuesta del cliente
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                    //perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }

                // Actualiza el puntaje del profesor según la respuesta del cliente
                hourlyProfessors[selectedProfessor - 1].score += clientAnswer; // Suma la respuesta del cliente al puntaje del profesor
            }
            // Envía un mensaje de agradecimiento al cliente
            printf("Evaluación docente completada. Gracias por participar.\n");
            for (int i = 0; i < 10; i++)
            {
                if (send(client_socket, personalquestions[i].text, sizeof(personalquestions[i].text), 0) == -1)
                {
                    // perror("Error al enviar las preguntas al cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
                if (recv(client_socket, &clientAnswer, sizeof(int), 0) == -1)
                {
                    // perror("Error al recibir la respuesta del cliente");
                    close(client_socket);
                    pthread_exit(NULL);
                }
            }
            break;
        default:
            break;
        }

        break;
    case 2:
        // Opción 2: Ver resultados
        if (recv(client_socket, &typeProfessor2, sizeof(int), 0) == -1)
        {
            // perror("Error al recibir la elección del profesor");
            close(client_socket);
            pthread_exit(NULL);
        }
        switch (typeProfessor2)
        {
        case 1:
            // Enviar la lista de profesores al cliente
            for (int i = 0; i < 10; i++)
            {
                strcpy(FullOrdenedProfessors[i].name, fullTimeProfessors[i].name); // Copia el nombre del profesor
                FullOrdenedProfessors[i].score = fullTimeProfessors[i].score;
            }
            // Bubble sort implementation to sort professors array based on their score
            void bubbleSort(struct Professor professors[], int n)
            {
                int i, j;
                struct Professor temp;
                for (i = 0; i < n - 1; i++)
                {
                    for (j = 0; j < n - i - 1; j++)
                    {
                        if (professors[j].score < professors[j + 1].score)
                        {
                            // swap professors[j] and professors[j+1]
                            temp = professors[j];
                            professors[j] = professors[j + 1];
                            professors[j + 1] = temp;
                        }
                    }
                }
            }

            // Sort and send the sorted professors to the client
            bubbleSort(FullOrdenedProfessors, 10);
            if (send(client_socket, FullOrdenedProfessors, sizeof(FullOrdenedProfessors), 0) == -1)
            {
                // perror("Error al enviar los profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }

            break;
        case 2:
            // Enviar la lista de profesores al cliente
            for (int i = 0; i < 10; i++)
            {
                strcpy(PartOrdenedProfessors[i].name, partTimeProfessors[i].name); // Copia el nombre del profesor
                PartOrdenedProfessors[i].score = partTimeProfessors[i].score;
            }
            // Bubble sort implementation to sort professors array based on their score
            void bubbleSort2(struct Professor professors[], int n)
            {
                int i, j;
                struct Professor temp;
                for (i = 0; i < n - 1; i++)
                {
                    for (j = 0; j < n - i - 1; j++)
                    {
                        if (professors[j].score < professors[j + 1].score)
                        {
                            // swap professors[j] and professors[j+1]
                            temp = professors[j];
                            professors[j] = professors[j + 1];
                            professors[j + 1] = temp;
                        }
                    }
                }
            }
            bubbleSort2(PartOrdenedProfessors, 10);
            if (send(client_socket, PartOrdenedProfessors, sizeof(PartOrdenedProfessors), 0) == -1)
            {
               // perror("Error al enviar los profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            break;
        case 3:
            // Enviar la lista de profesores al cliente
            for (int i = 0; i < 10; i++)
            {
                strcpy(HourlyOrdenedProfessors[i].name, hourlyProfessors[i].name); // Copia el nombre del profesor
                HourlyOrdenedProfessors[i].score = hourlyProfessors[i].score;
            }
            // Bubble sort implementation to sort professors array based on their score
            void bubbleSort3(struct Professor professors[], int n)
            {
                int i, j;
                struct Professor temp;
                for (i = 0; i < n - 1; i++)
                {
                    for (j = 0; j < n - i - 1; j++)
                    {
                        if (professors[j].score < professors[j + 1].score)
                        {
                            // swap professors[j] and professors[j+1]
                            temp = professors[j];
                            professors[j] = professors[j + 1];
                            professors[j + 1] = temp;
                        }
                    }
                }
            }
            bubbleSort3(HourlyOrdenedProfessors, 10);
            if (send(client_socket, HourlyOrdenedProfessors, sizeof(HourlyOrdenedProfessors), 0) == -1)
            {
                // perror("Error al enviar los profesores al cliente");
                close(client_socket);
                pthread_exit(NULL);
            }
            break;

        default:
            break;
        }
        /* Bubble sort implementation to sort professors array based on their score
        void bubbleSort(struct Professor professors[], int n)
        {
            int i, j;
            struct Professor temp;
            for (i = 0; i < n - 1; i++)
            {
                for (j = 0; j < n - i - 1; j++)
                {
                    if (professors[j].score < professors[j + 1].score)
                    {
                        // swap professors[j] and professors[j+1]
                        temp = professors[j];
                        professors[j] = professors[j + 1];
                        professors[j + 1] = temp;
                    }
                }
            }
        }
        bubbleSort(professors, 10);
        if (send(client_socket, professors, sizeof(professors), 0) == -1)
        {
            perror("Error al enviar la lista de profesores al cliente");
            close(client_socket);
            pthread_exit(NULL);
        }
        */

        break;
    case 3:
        // Opción 2: Cerrar sesión del cliente
        // Cierra la conexión con el cliente

        close(client_socket);
        printf("\x1b[31mCliente desconectado.\n");
        break;

    default:
        // Opción no válida
        printf("Opción no válida. Cerrando la conexión.\n");
        close(client_socket);
        pthread_exit(NULL);
        break;
    }

    // Cierra la conexión con el cliente y libera la memoria de la estructura de información del cliente
    close(client_socket);
    free(clientInfo);
    pthread_exit(NULL);
}

int main()
{
    int server_socket;              // Socket del servidor
    struct sockaddr_in server_addr; // Estructura para almacenar la información del servidor
    int typeProfessor;              // Tipo de profesor
    int selectedProfessor;          // Profesor seleccionado por el cliente
    initializeProfessors();         // Inicializa la lista de profesores
    initializeQuestions();          // Inicializa la lista de preguntas

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
        struct sockaddr_in client_addr;             // Estructura para almacenar la información del cliente
        socklen_t client_len = sizeof(client_addr); // Tamaño de la estructura del cliente
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        // Verifica si hubo un error al aceptar la conexión
        if (client_socket == -1)
        {
            perror("Error al aceptar la conexión");
            continue;
        }
        // Imprime un mensaje de conexión exitosa
        printf("\x1b[32mCliente conectado.\n");

        // Crea una estructura para almacenar la información del cliente y pasa esa estructura como argumento a la función handleClient
        struct ClientInfo *clientInfo = malloc(sizeof(struct ClientInfo));
        clientInfo->socket = client_socket;
        clientInfo->address = client_addr;
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleClient, (void *)clientInfo) != 0)
        {
            perror("Error al crear el hilo");
            close(client_socket);
            continue;
        }
        // El hilo se encargará de manejar la conexión con el cliente
        pthread_detach(thread);
    }

    // Cerrar el socket del servidor (esto no se alcanza porque el bucle es infinito)
    // close(server_socket);
    return 0;
}
