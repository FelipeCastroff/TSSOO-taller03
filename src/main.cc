#include <global.h>
#include <checkArgs.hpp>

uint64_t *Array = nullptr;
uint32_t *vRandom = nullptr;
uint64_t *sumasParciales = nullptr;
uint32_t *LlenadoSerial = nullptr;
uint32_t sumSerial;

bool verbosidad;

void Llenado(size_t start, size_t end, size_t flag)
{
	for (auto i = start; i < end; ++i)
	{
		switch(flag){
			case 0:
				LlenadoSerial[i]=vRandom[i];
				break;
			case 1:
			 	Array[i] = vRandom[i];
				break;
		}
	}
}

void sumaParcial(size_t start, size_t end, size_t j, size_t flag)
{
	for (size_t i = start; i < end; ++i)
	{
		switch(flag){
			case 0:
				sumSerial += LlenadoSerial[i];
				break;
			case 1:
				sumasParciales[j] += Array[i];
				break;
		}
	}
}

int main(int argc, char **argv)
{
	uint64_t totalElementos;
	uint32_t numThreads;
	uint64_t l_inferior;
	uint64_t l_superior;
	uint32_t sumConsolidada;
	uint32_t acumulableOPenMP = 0;

	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	l_inferior = argumentos->getArgs().l_inf;
	l_superior = argumentos->getArgs().l_sup;
	verbosidad = argumentos->getArgs().verbosidad;

	if (verbosidad)
	{
		std::cout << "Elementos: " << totalElementos << std::endl;
		std::cout << "Threads  : " << numThreads << std::endl;
		std::cout << "Límite inferior rango aleatorio : " << l_inferior << std::endl;
		std::cout << "Límite superior rango aleatorio : " << l_superior << std::endl;
	}
	std::vector<std::thread *> llenado_thread;
	std::vector<std::thread *> sumaP_thread;

	Array = new uint64_t[totalElementos];
	LlenadoSerial = new uint32_t[totalElementos];
	vRandom = new uint32_t[totalElementos];
	sumasParciales = new uint64_t[numThreads];
	auto vectorOpm = new uint32_t[totalElementos];
	

	// ----- Llenar arreglo con random ---- //

	std::random_device deviceV;
	std::mt19937 rngV(deviceV());
	std::uniform_int_distribution<uint32_t> nArregloRandom(l_inferior, l_superior);

	for (size_t i = 0; i < totalElementos; ++i)
	{
		uint32_t number = nArregloRandom(rngV);
		vRandom[i] = number;
	}

	//-----------------------------------------Llenado-----------------------------------------//
	// -- Llenado paralelo -- //

	auto start = std::chrono::system_clock::now();

	for (uint32_t i = 0; i < numThreads; ++i)
	{
		llenado_thread.push_back(new std::thread(Llenado,
												 i * (totalElementos) / numThreads,
												 (i + 1) * (totalElementos) / numThreads,1));
	}
	for (auto &th : llenado_thread)
	{
		th->join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoLlenadoThread = elapsed.count();

	// -- Llenado serial -- //

	start = std::chrono::system_clock::now();

	Llenado(0, totalElementos, 0);

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoLlenadoSerial = elapsed.count();

	//-----------------------------------------Suma-----------------------------------------//

	// -- Suma paralelo -- //
	
	for (size_t j = 0; j < numThreads; ++j)
	{
		sumaP_thread.push_back(new std::thread(sumaParcial,
											   j * (totalElementos) / numThreads,
											   (j + 1) * (totalElementos) / numThreads,
											   j,1));
	}
	start = std::chrono::system_clock::now();

	for (auto &th : sumaP_thread)
	{
		th->join();
	}
	//Consolidación//
	for (size_t k = 0; k < numThreads; ++k)
	{
		sumConsolidada += sumasParciales[k];
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoSumaThread = elapsed.count();

	// -- Suma Serial -- //

	start = std::chrono::system_clock::now();

	/*for (size_t i = 0; i < totalElementos; ++i)
	{
		sumSerial += LlenadoSerial[i];
	}*/
	
	sumaParcial(0, totalElementos, 0, 0);

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoSumaSerial = elapsed.count();

	//-----------------------------------------Módulo OpenMP-----------------------------------------//

	// ------------- Módulo Llenado OpenMp ------------- //

	//--- Llenado Paralelo ---//

	start = std::chrono::system_clock::now();

	#pragma omp parallel for num_threads(numThreads)
	for (size_t i = 0; i < totalElementos; ++i)
	{
		vectorOpm[i] = vRandom[i];
	}

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoLlenadoOpenMP = elapsed.count();

	// ------------- Módulo Suma OpenMP ------------- //

	// --- Suma Paralelo ---//
	
	start = std::chrono::system_clock::now();

	#pragma omp parallel for reduction(+:acumulableOPenMP) num_threads(numThreads)
	for (size_t i = 0; i < totalElementos; ++i)
	{
		acumulableOPenMP += vectorOpm[i];
	}

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto TiempoSumaOpenMP = elapsed.count();

	// -- Resultados -- //
	if (verbosidad)
	{
		std::cout << "======Resultados======" << std::endl;
		std::cout << "La suma Total secuencial			: " << sumSerial << std::endl;
		std::cout << "La suma Total thread				: " << sumConsolidada << std::endl;
		std::cout << "La suma Total OpenMP Parallel 			: " << acumulableOPenMP << std::endl;

		if (totalElementos > 10000000)
		{
			float speedUpF = (TiempoLlenadoSerial / TiempoLlenadoThread);
			float eficienciadF = 1 / (1 + (TiempoLlenadoThread / TiempoLlenadoSerial));

			float speedUp = (TiempoSumaSerial / TiempoSumaThread);
			float eficiencia = 1 / (1 + (TiempoSumaThread / TiempoSumaSerial));
			std::cout << "====Desempeño====" << std::endl;
			std::cout << "SpeedUp Llenado 				: " << speedUpF << "[ms/ms]" << std::endl;
			std::cout << "Eficiencia Llenado 				: " << eficienciadF << "[1/ms]" << std::endl;
			std::cout << "SpeedUp Suma 					: " << speedUp << "[ms/ms]" << std::endl;
			std::cout << "Eficiencia Suma 				: " << eficiencia << "[1/ms]" << std::endl;
		}
		else
		{
			float speedUpF = 0;
			float eficienciadF = 0;
			float speedUp = 0;
			float eficiencia = 0;
		}
	}
	std::cout << "=======Módulo OpenMP=======" << std::endl;
	std::cout << "====Llenado arreglo====" << std::endl;
	std::cout << "Tiempo OpenMp 					: " << TiempoLlenadoOpenMP << "[ms]" << std::endl;
	std::cout << "Tiempo Secuencial 				: " << TiempoLlenadoSerial << "[ms]" << std::endl;
	if(verbosidad){
	std::cout << "Tiempo Threads					: " << TiempoLlenadoThread << "[ms]" << std::endl;
	}
	std::cout << "====Suma valores====" << std::endl;
	std::cout << "Tiempo OpenMP 		 			: " << TiempoSumaOpenMP << "[ms]" << std::endl;
	std::cout << "Tiempo Secuencial 				: " << TiempoSumaSerial << "[ms]" << std::endl;
	if(verbosidad){
	std::cout << "Tiempo threads					: " << TiempoSumaThread << "[ms]" << std::endl;
	}
	return (EXIT_SUCCESS);
}