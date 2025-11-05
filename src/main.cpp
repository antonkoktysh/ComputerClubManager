#include <iostream>
#include "core/computer_club_manager_impl.h"
#include "utils/exceptions.h"
#include "utils/file_parser.h"
#include "utils/result_printer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Expected input file" << std::endl;
        return 1;
    }

    try {
        auto [config, input_events] = computer_club::utils::FileParser::Parse(argv[1]);
        auto computer_club_manager =
            std::make_unique<computer_club::core::ComputerClubManagerImpl>(config);

        computer_club_manager->ProcessEvents(input_events);
        computer_club_manager->Close();

        computer_club::utils::PrintResults(std::cout, *computer_club_manager);

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}