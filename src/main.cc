#include <array>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "../lib/log.hpp"
#include "./lib/semaphore.hpp"

// Количество элементов в фиксированном буфере
static constexpr size_t amount_message_buff = 4;

// Семафоры для реализации паттерна (Producer/Consumer)
static semaphore full(0);
static semaphore empty(amount_message_buff);

// Указатель на обьект логгирования (для программы он один)
std::shared_ptr<logging::Log> global_log;

// шаблон функции в зависимости от количества элементов в буфере
// увеличивает compile-time, но в данной реализации некритично
// функция поток протребитель
template <size_t N>
void message_handler(std::array<logging::UnitLog, N> &buff_message) {
    // итерация по буферу
    for (size_t i = 0;; i = (i + 1) % amount_message_buff) {
	full.acquire();
	global_log->note_to_logfile(buff_message[i]);
	empty.release();
    }
}

int main(int args, char **argv) {

    // Обработка при лишних аргуметах командой строки
    if (args >= 4 || args <= 2) {
	std::cerr << "Inappropriate number of command line arguments";
	return 1;
    }

    auto status_default = std::string(argv[1]);
    std::filesystem::path path_file(argv[2]);

	// Ошибка которую я не обрабатываю, так как хочу завершения программы при неправильном статусе 
	auto stat_status_default = logging::table_status.at(status_default);

    try {
	// Создание обьекта для логгирования в динамической памяти для дальнешей
	// передачи
	std::shared_ptr<logging::Log> my_log = std::make_shared<logging::Log>(
	    std::move(path_file), logging::table_status[status_default]);

	// передача владения глобальному указателю
	global_log = std::move(my_log);

    } catch (std::exception &ex) {
	std::cerr << ex.what() << std::endl;
	throw;
    }

    // Наш буфер (общий ресурс для двух потоков )
    std::array<logging::UnitLog, amount_message_buff> buff_message;

    // Запускаем новый поток обработки сообщений
    std::thread proccess_message(message_handler<amount_message_buff>,
				 std::ref(buff_message));

    // Цикл который проходится по буфферу (бесконечное количество раз)
    for (size_t i = 0;; i = (i + 1) % amount_message_buff) {

	std::string status, message;

	std::cout << "Write Status Message (for default remain empty string):"
		  << std::endl;
	std::getline(std::cin, status);
	    
	status = status.empty() ? "NOTICE" : status;
	     	
	std::cout << "Write Message:" << std::endl;

	std::getline(std::cin, message);
	logging::UnitLog unit_log;
	    
	// не кидаем std::terminate при ошибки создания, а продолжаем 
	try {
	    // создания единицы логирования
	    unit_log = logging::UnitLog(
		std::move(message), std::chrono::system_clock::now(), status);

		// пустое место "приобретается"
		empty.acquire();

		buff_message[i] = std::move(unit_log);

		// полное место освобождается
		full.release();
		} 
		catch (std::exception &ex) {
	    	logging::DEBUG_LOG_INFO(ex.what());
		}
	}

    proccess_message.join();

    return 0;
}
