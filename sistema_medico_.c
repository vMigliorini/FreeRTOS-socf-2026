
#include "FreeRTOS.h"
#include "task.h"
#include "basic_io.h"
#include <time.h>

/*
 1 - O protÛtipo da funÁ„o de uma tarefa deve sempre retornar void, ou seja, a tarefa n„o possui retorno.
 2 - A funÁ„o de uma tarefa deve receber um par‚metro de ponteiro void. 
*/
void vBpm(void *pvParameters);
void vSaturacao(void *pvParameters);
void vTemperatura(void *pvParameters);

/*
 DeniÁ„o da estrutura da funÁ„o
*/
void vBpm(void *pvParameters)
{
	
	for(;;)
	{
		int randBpm = 25 + rand() % (130 - 25 + 1);
		const char* alertaStatus = "Batimentos: ";

		if (randBpm <= 50){
			alertaStatus = "Atenção! Bradicardia: ";

		}else if (randBpm > 110){
			alertaStatus = "Atenção! Taquicardia: ";

		}

		vPrintStringAndNumber(alertaStatus, randBpm);
		vTaskDelay(1000);
	}

	vTaskDelete(NULL);
}

void vSaturacao(void *pvParameters)
{
	
	for(;;)
	{
		int randSat = 85 + rand() % (105 - 85 + 1);
		const char* alertaStatus = "Saturação: ";

		if (95 > randSat >= 90){
			alertaStatus = "Atenção! Baixa saturação de oxigênio: ";

		}else if (randSat < 90){
			alertaStatus = "Crítico! Baixa grave na saturação de oxigênio: ";

		}

		vPrintStringAndNumber(alertaStatus, randSat);
		vTaskDelay(1000);
	}

	vTaskDelete(NULL);
}

void vTemperatura(void *pvParameters)
{
	
	for(;;)
	{
		float randTemp = 32 + rand() % (42 - 32 + 1);
		char* alertaStatus = "Temperatura: ";
		
		if (randTemp > 37){
			alertaStatus = "Atenção! Paciente febril: ";

		}else if (randTemp < 35.5){
			alertaStatus = "Atenção! Paciente Hiportermico: ";

		}

		vPrintStringAndNumber(alertaStatus, randTemp);
		vTaskDelay(1000);
	}

	vTaskDelete(NULL);
}

void main_(void)
{

	srand(time(NULL));


	xTaskCreate(vBpm, "Batimentos", 1000, NULL, 1, NULL);
	xTaskCreate(vSaturacao, "Saturacao", 1000, NULL, 1, NULL);
	xTaskCreate(vTemperatura, "Temperatura", 1000, NULL, 1, NULL);

	// Inicia o escalonador de tarefas
	vTaskStartScheduler();

	for (;; );
//	return 0;
}

