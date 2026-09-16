/*
Curso: Bacharelado em Ciência da Computação
Disciplina: Sistemas Operacionais Ciberfísicos
Período: 4
Turma: A
Integrantes:
- Vicente Migliorini
*/

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "semphr.h"
#include <time.h>
#include "basic_io.h"
#include <string.h>

// declaracao das funcoes que serao passadas nas tasks
void vYaw(void *pvParameters);
void vRoll(void *pvParameters);
void vPitch(void *pvParameters);
void radioFrenquencia(void *pvParameters);

// xSemaphore
SemaphoreHandle_t xSemaphore;

// variáveis globais
int BUFFER_SIZE = 20;

volatile char sentido[20];
volatile char direcao[20];
volatile char orientacao[20];

volatile long motor0;
volatile long motor1;
volatile long motor2;
volatile long motor3;

// valor de acresimo e decresimo e valor padrao das velocidades do motores respectivamente
const int DEFAULT_CHANGE = 1;
const int DEFAULT_VALUE = 100;


// funcao para printar o status dos motores e a instrucao
void print_status(char *pMensagem){
	vPrintString(pMensagem);
 
	vPrintStringAndNumber("motor0: ", motor0);
	vPrintStringAndNumber("motor1: ", motor1);
	vPrintStringAndNumber("motor2: ", motor2);
	vPrintStringAndNumber("motor3: ", motor3);
}

// funcao para guinada
void vYaw(void *pvParameters) {

	for (;;){
		// instrução de guinada no sentido horário
		if (sentido != NULL && strcmp(sentido, "horario") == 0)
		{

			xSemaphoreTake(xSemaphore, portMAX_DELAY);

			motor0 += DEFAULT_CHANGE;
			motor2 += DEFAULT_CHANGE;
			motor1 -= DEFAULT_CHANGE;
			motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Guinada para o sentido horario\n";

			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);
			
			vTaskDelay(portTICK_RATE_MS * 10);

		}
		// instrução de guinada no anti-horário
		else if (sentido != NULL && strcmp(sentido, "anti-horario") == 0)
		{
			xSemaphoreTake(xSemaphore, portMAX_DELAY);
			motor1 += DEFAULT_CHANGE;
			motor3 += DEFAULT_CHANGE;
			motor0 -= DEFAULT_CHANGE;
			motor2 -= DEFAULT_CHANGE;

			char *pMensagem = "Guinada para o sentido anti-horario\n";

			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);

			vTaskDelay(portTICK_RATE_MS * 10);
		}
		else
		{
			char* mensagem_erro = "Problemas na alocação da instrução de guinada";
			vPrintString(mensagem_erro);
		}
	}

	vTaskDelete(NULL);
}

void vPitch(void *pvParameters){
	for (;;)
	{
		// instruções de arfagem para frente
		if (direcao != NULL && strcmp(direcao, "frente") == 0)
		{
			xSemaphoreTake(xSemaphore, portMAX_DELAY);
			motor2 += DEFAULT_CHANGE;
			motor3 += DEFAULT_CHANGE;
			motor0 -= DEFAULT_CHANGE;
			motor1 -= DEFAULT_CHANGE;

			char *pMensagem = "arfagem para frente\n";

			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);

			vTaskDelay(portTICK_RATE_MS * 40);
		}
		// instruções de arfagem para trás
		else if (direcao != NULL && strcmp(direcao, "tras") == 0)
		{
			xSemaphoreTake(xSemaphore, portMAX_DELAY);
			motor0 += DEFAULT_CHANGE;
			motor1 += DEFAULT_CHANGE;
			motor2 -= DEFAULT_CHANGE;
			motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Arfagem para tras\n";

			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);

			vTaskDelay(portTICK_RATE_MS * 40);
		}
		else
		{
			char* mensagem_erro = "Problemas na alocação da instrução de arfagem";
			vPrintString(mensagem_erro);
		}
	}

	vTaskDelete(NULL);
}

void vRoll(void *pvParameters){
	for(;;)
	{
		// instruções para rolagem à direita
		if (orientacao != NULL && strcmp(orientacao, "direita") == 0)	
		{
			xSemaphoreTake(xSemaphore, portMAX_DELAY);

			motor0 += DEFAULT_CHANGE;
			motor3 += DEFAULT_CHANGE;
			motor1 -= DEFAULT_CHANGE;
			motor2 -= DEFAULT_CHANGE;

			char *pMensagem = "Rolagem para a direita\n";
			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);

			vTaskDelay(portTICK_RATE_MS * 20);
		}
		// instruções para rolagem à esquerda
		else if (orientacao != NULL && strcmp(orientacao, "esquerda") == 0)
		{
			xSemaphoreTake(xSemaphore, portMAX_DELAY);

			motor1 += DEFAULT_CHANGE;
			motor2 += DEFAULT_CHANGE;
			motor0 -= DEFAULT_CHANGE;
			motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Rolagem para a esquerda\n";

			// Printa o status dos motores
			print_status(pMensagem);
			xSemaphoreGive(xSemaphore);

			vTaskDelay(portTICK_RATE_MS * 20);
		}
		else
		{
			char* mensagem_erro = "Problemas na alocação da instrução de Rolagem";
			vPrintString(mensagem_erro);
		}
	}

	vTaskDelete(NULL);
}

void radioFrenquencia(void *pvParameters){
	for (;;)
	{
		// calculando valores aleatorios
		int dadoUm = rand() % 100;
		int dadoDois = rand() % 100;
		int dadoTres = rand() % 100;

		xSemaphoreTake(xSemaphore, portMAX_DELAY);

		// atribuindo instruções com base nos resultados aleatórios
		if (dadoUm % 2 == 0){
			sprintf(sentido, "horario");
		}else{
			sprintf(sentido, "anti-horario");
		}

		if (dadoDois % 2 == 0){
			sprintf(direcao, "frente");
		}else{
			sprintf(direcao, "tras");
		}

		if (dadoTres % 2 == 0){
			sprintf(orientacao, "direita");
		}else{
			sprintf(orientacao, "esquerda");
		}

		// concatenação e print das instruções novas
		char status[BUFFER_SIZE * 3 + 50];
		snprintf(status, sizeof(status), "INSTRUÇÔES: %s, %s, %s\n", direcao, orientacao, sentido);

		vPrintString(status);

		xSemaphoreGive(xSemaphore);
		vTaskDelay(portTICK_RATE_MS * 100);
	}
}

void main_(void)
{
	motor0, motor1, motor2, motor3 = DEFAULT_VALUE;

	srand(time(NULL));

	vSemaphoreCreateBinary(xSemaphore);

	strcpy(direcao, "frente");
	strcpy(orientacao, "direita");
	strcpy(sentido, "horario");

	xTaskCreate(vYaw, "Guinada", 1000, NULL, 2, NULL);
	xTaskCreate(vRoll, "Rolagem", 1000, NULL, 2, NULL);
	xTaskCreate(vPitch, "Arfagem", 1000, NULL, 2, NULL);
	xTaskCreate(radioFrenquencia, "Radio Frequencia", 1000, NULL, 1, NULL);

	vTaskStartScheduler();

	return;
}
