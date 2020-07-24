#include <global.h>
#include <checkArgs.hpp>
#include <thread>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <omp.h>

uint32_t *vRandom = nullptr;
bool verbosidad;

int main(int argc, char **argv)
{
	uint64_t totalElementos;
	uint32_t numThreads;
	uint64_t l_inferior;
	uint64_t l_superior;
	uint32_t suma_Serial = 0;

	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	l_inferior = argumentos->getArgs().l_inf;
	l_superior = argumentos->getArgs().l_sup;
	verbosidad = argumentos->getArgs().verbosidad;

	if(verbosidad){
	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads << std::endl;
	std::cout << "Límite inferior rango aleatorio : " << l_inferior << std::endl;
	std::cout << "Límite superior rango aleatorio : " << l_superior << std::endl;
	}
	
	vRandom = new uint32_t[totalElementos];
	// ----- Llenar arreglo con random ---- //

	std::random_device deviceV;
	std::mt19937 rngV(deviceV());
	std::uniform_int_distribution<uint32_t> nArregloRandom(l_inferior, l_superior);

	for(size_t i=0; i < totalElementos; ++i){
		uint32_t number = nArregloRandom(rngV);
		vRandom[i]= number;
		//std::cout << "Numero random:  " << number << std::endl;
	}
	auto vectorSerial = new uint32_t[totalElementos];
	// -- Llenado serial -- //

	auto start = std::chrono::system_clock::now();
	for (size_t i = 0; i < totalElementos; i++)
	{
		vectorSerial[i]=(vRandom[i]);
	}
	auto end     = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TotalTimeSerialFilling = elapsed.count();

	// -- Suma Serial -- //

	start = std::chrono::system_clock::now();
	for (size_t i = 0; i < totalElementos; ++i)
	{
		suma_Serial += vectorSerial[i];
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TotalTimeSerialSum = elapsed.count();
	
	//-----------------------------------------Módulo OpenMP-----------------------------------------//
	// ------------- Módulo Llenado OpenMp ------------- //
	//--- Llenado Paralelo ---//
	auto vectorOmp = new uint32_t[totalElementos];
	start = std::chrono::system_clock::now();
	
	#pragma omp parallel for  num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){	
		vectorOmp[i] = vRandom[i];
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TotalTimeOpenMPParallel = elapsed.count();

	// ------------- Módulo Suma OpenMp ------------- //
	// --- Suma Paralelo ---//
	uint32_t acumulableP = 0;
	start = std::chrono::system_clock::now();
	#pragma omp parallel for reduction(+:acumulableP) num_threads(numThreads)
	for(size_t i = 0; i < totalElementos; ++i){
		acumulableP += vectorOmp[i];
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TotalTimeOpenMPsumP = elapsed.count();

	// -- Resultados -- //
	if(verbosidad){
	std::cout << "======Resultados======" << std::endl;
	std::cout << "La suma Total secuencial			: " << suma_Serial << std::endl;
	std::cout << "La suma Total OpenMP Parallel 			: " << acumulableP << std::endl;

	/*if(totalElementos > 10000000){
		float speedUpF = (TotalTimeSerialFilling/TotalTimeParallelFilling);
  		float eficienciadF = 1/(1+(TotalTimeParallelFilling/TotalTimeSerialFilling));

		float speedUp = (TotalTimeSerialSum/TotalTimeParallelSum);
  		float eficiencia = 1/(1+(TotalTimeParallelSum/TotalTimeSerialSum));
		std::cout << "====Desempeño====" << std::endl;
		std::cout << "SpeedUp Llenado : " << speedUpF << "[ms/ms]" <<std::endl;
		std::cout << "Eficiencia Llenado : " << eficienciadF << "[1/ms]" <<std::endl;
		std::cout << "SpeedUp Suma : " << speedUp << "[ms/ms]" <<std::endl;
		std::cout << "Eficiencia Suma : " << eficiencia << "[1/ms]" <<std::endl;
	}else
		{
		float speedUpF=0;
		float eficienciadF=0;
		float speedUp=0;
		float eficiencia=0;
		}*/
	}
	std::cout << "=======Módulo OpenMP=======" << std::endl;
	std::cout << "====Llenado arreglo====" << std::endl;
	std::cout << "Tiempo OpenMp Parallel				: " << TotalTimeOpenMPParallel << "[ms]" << std::endl;
	std::cout << "Tiempo Secuencial 				: " << TotalTimeSerialFilling << "[ms]" <<std::endl;
	std::cout << "====Suma valores====" << std::endl;
	std::cout << "Tiempo OpenMP Parallel 				: " << TotalTimeOpenMPsumP << "[ms]" <<std::endl;
	std::cout << "Tiempo Secuencial 				: " <<  TotalTimeSerialSum << "[ms]" << std::endl;
	
	return (EXIT_SUCCESS);
}
