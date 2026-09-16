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
#include "basic_io.h"
#include "semphr.h"
#include <string.h>

// declaracao das funcoes que serao passadas nas tasks
void vYaw(void *pvParameters);
void vRoll(void *pvParameters);
void vPitch(void *pvParameters);

// declaracao da struct que guarda a velocidade dos motores
typedef struct {
	int motor0;
	int motor1;
	int motor2;
	int motor3;
} Motores;

// declaracao da struct que guarda um ponteiro para a struct das velocidades e uma string de instrucao
typedef struct {
	Motores *pMotores;
	char *instrucao;
} Parametros;

// valor de acresimo e decresimo e valor padrao das velocidades do motores respectivamente
const int DEFAULT_CHANGE = 1;
const int DEFAULT_VALUE = 100;

// Semaforo para lidar com condição de corrida
static SemaphoreHandle_t xMotorMutex;

// funcao para printar o status dos motores e a instrucao
void print_status(char *pMensagem, Motores *pMotores){
	vPrintString(pMensagem);
 
	vPrintStringAndNumber("motor0: ", pMotores->motor0);
	vPrintStringAndNumber("motor1: ", pMotores->motor1);
	vPrintStringAndNumber("motor2: ", pMotores->motor2);
	vPrintStringAndNumber("motor3: ", pMotores->motor3);
}

// funcao para guinada
void vYaw(void *pvParameters) {

	// pega os parametros passados
	Parametros *pParametros = (Parametros *) pvParameters;

	// "Desembrulha os parametros"
	Motores *pMotores = pParametros->pMotores;
	char* instrucao = pParametros->instrucao;

	// Condicao para execucao das instrucoes especificas
	if (instrucao != NULL && strcmp(instrucao, "horario") == 0){
		
		for (;;){
			// pega o semaforo para alterar os valores e printar
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor0 += DEFAULT_CHANGE;
			pMotores->motor2 += DEFAULT_CHANGE;
			pMotores->motor1 -= DEFAULT_CHANGE;
			pMotores->motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Guinada para o sentido horario\n";

			// Printa o status dos motores
			print_status(pMensagem, pMotores);
			
			// solta o semaforo
			xSemaphoreGive(xMotorMutex);
			vTaskDelay(10);
		}

	}else if(instrucao != NULL && strcmp(instrucao, "anti-horario") == 0){

		for (;;){
			// pega o semaforo para alterar os valores e printar
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor1 += DEFAULT_CHANGE;
			pMotores->motor3 += DEFAULT_CHANGE;
			pMotores->motor0 -= DEFAULT_CHANGE;
			pMotores->motor2 -= DEFAULT_CHANGE;

			char *pMensagem = "Guinada para o sentido anti-horario\n";

			// Printa o status dos motores
			print_status(pMensagem, pMotores);
			// solta o semaforo
			xSemaphoreGive(xMotorMutex);

			vTaskDelay(10);
		}

	}else{
		char* mensagem_erro = "Problemas na alocação da instrução de guinada";
		vPrintString(mensagem_erro);
	}

	vTaskDelete(NULL);
}

void vPitch(void *pvParameters){
	// pega os parametros
	Parametros *pParametros = (Parametros *) pvParameters;

	// "Desembrulha"
	Motores *pMotores = pParametros->pMotores;
	char* instrucao = pParametros->instrucao;


	if (instrucao != NULL && strcmp(instrucao, "frente") == 0){
		
		for (;;){
			// pega o semaforo para alterar os valores e printar
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor2 += DEFAULT_CHANGE;
			pMotores->motor3 += DEFAULT_CHANGE;
			pMotores->motor0 -= DEFAULT_CHANGE;
			pMotores->motor1 -= DEFAULT_CHANGE;

			char *pMensagem = "arfagem para frente\n";

			// Printa o status dos motores
			print_status(pMensagem, pMotores);

			// Solta o semaforo
			xSemaphoreGive(xMotorMutex);

			vTaskDelay(40);
		}

	}else if(instrucao != NULL && strcmp(instrucao, "tras") == 0){

		for (;;){

			// pega o semaforo para alterar os valores e printar
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor0 += DEFAULT_CHANGE;
			pMotores->motor1 += DEFAULT_CHANGE;
			pMotores->motor2 -= DEFAULT_CHANGE;
			pMotores->motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Arfagem para tras\n";

			// Printa o status dos motores
			print_status(pMensagem, pMotores);

			// Solta o semaforo
			xSemaphoreGive(xMotorMutex);

			vTaskDelay(40);
		}

	}else{
		char* mensagem_erro = "Problemas na alocação da instrução de arfagem";
		vPrintString(mensagem_erro);
	}

	vTaskDelete(NULL);
}

void vRoll(void *pvParameters){
	// Pega os parametros
	Parametros *pParametros = (Parametros *) pvParameters;

	// "Desembrulha"
	Motores *pMotores = pParametros->pMotores;
	char* instrucao = pParametros->instrucao;


	if (instrucao != NULL && strcmp(instrucao, "direita") == 0){
		
		for (;;){

			// Pega o semaforo para alterar e printar os valores
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor0 += DEFAULT_CHANGE;
			pMotores->motor3 += DEFAULT_CHANGE;
			pMotores->motor1 -= DEFAULT_CHANGE;
			pMotores->motor2 -= DEFAULT_CHANGE;

			char *pMensagem = "Rolagem para a direita\n";
			// Printa o status dos motores
			print_status(pMensagem, pMotores);

			// "Desembrulha"
			xSemaphoreGive(xMotorMutex);
			vTaskDelay(20);
		}

	}else if(instrucao != NULL && strcmp(instrucao, "esquerda") == 0){

		for (;;){

			// Pega o semaforo para alterar e printar os valores
			xSemaphoreTake(xMotorMutex, portMAX_DELAY);

			pMotores->motor1 += DEFAULT_CHANGE;
			pMotores->motor2 += DEFAULT_CHANGE;
			pMotores->motor0 -= DEFAULT_CHANGE;
			pMotores->motor3 -= DEFAULT_CHANGE;

			char *pMensagem = "Rolagem para a esquerda\n";

			// Printa o status dos motores
			print_status(pMensagem, pMotores);

			// Solta o semaforo
			xSemaphoreGive(xMotorMutex);
			vTaskDelay(20);
		}

	}else{
		char* mensagem_erro = "Problemas na alocação da instrução de Rolagem";
		vPrintString(mensagem_erro);
	}

	vTaskDelete(NULL);
}

void main_(void)
{
	// Declara a struct dos motores
	static Motores motores;

	// Definindo os valores dos motores
	motores.motor0 = DEFAULT_VALUE;
	motores.motor1 = DEFAULT_VALUE;
	motores.motor2 = DEFAULT_VALUE;
	motores.motor3 = DEFAULT_VALUE;

	// define uma struct para cada orientacao
	static Parametros parametrosYaw, parametrosRoll, parametrosPitch;

	// aloca os motores das structs parametros com base nos motores que já declaramos
	parametrosYaw.pMotores = &motores;
	parametrosRoll.pMotores = &motores;
	parametrosPitch.pMotores = &motores;

	// Declaracao do semaforo
	xMotorMutex = xSemaphoreCreateMutex();

	//Define cada instrucao e cria as task
	// caso queira testar cada instrucao, altere os valores de instrucao
	parametrosYaw.instrucao = "horario";
	xTaskCreate(vYaw, "Guinada", 1000, &parametrosYaw, 1, NULL);

	parametrosRoll.instrucao = "direita";
	xTaskCreate(vRoll, "Rolagem", 1000, &parametrosRoll, 1, NULL);

	parametrosPitch.instrucao = "frente";
	xTaskCreate(vPitch, "Arfagem", 1000, &parametrosPitch, 1, NULL);

	// Inicia o escalonador de tarefas
	vTaskStartScheduler();

	return;
}
