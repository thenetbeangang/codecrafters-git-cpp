#include <iostream>

#include <filesystem>

#include <fstream>

#include <string>

#include "zstr.hpp"

int main(int argc, char *argv[])

{

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cout << "Logs from your program will appear here!\n";
    // Uncomment this block to pass the first stage

    if (argc < 2)

    {

        std::cerr << "No command provided.\n";

        return EXIT_FAILURE;

    }

    std::string command = argv[1];

    if (command == "init")

    {

        try

        {

            std::filesystem::create_directory(".git");

            std::filesystem::create_directory(".git/objects");

            std::filesystem::create_directory(".git/refs");

            std::ofstream headFile(".git/HEAD");

            if (headFile.is_open())

            {

                headFile << "ref: refs/heads/main\n";

                headFile.close();

            }

            else

            {

                std::cerr << "Failed to create .git/HEAD file.\n";

                return EXIT_FAILURE;

            }

            std::cout << "Initialized git directory\n";

        }

        catch (const std::filesystem::filesystem_error &e)

        {

            std::cerr << e.what() << '\n';

            return EXIT_FAILURE;

        }

    }

    else if (command == "cat-file")

    {

        if (argc <= 3)

        {

            std::cerr << "Invalid arguments, required `-p <blob_sha>`\n";

            return EXIT_FAILURE;

        }

        // this is bad.

        const std::string flag = argv[2];

        if (flag != "-p")

        {

            std::cerr << "Invalid flag for cat-file, expected `-p`\n";

            return EXIT_FAILURE;

        }

        const std::string value = argv[3];

        const std::string dir_name = value.substr(0, 2);

        const std::string blob_sha = value.substr(2);

        // not secure - technically. - can have LFI

        std::string path = ".git/objects/" + dir_name + "/" + blob_sha;

        zstr::ifstream input(path, std::ofstream::binary);

        if (!input.is_open())

        {

            std::cerr << "Failed to open object file?\n";

            return EXIT_FAILURE;

        }

        std::string object_str{std::istreambuf_iterator<char>(input),

                               std::istreambuf_iterator<char>()};

        input.close();

        const auto object_content = object_str.substr(object_str.find('\0') + 1);

        std::cout << object_content << std::flush;

    }

    else

    {

        std::cerr << "Unknown command " << command << '\n';

        return EXIT_FAILURE;

    }

    

        return EXIT_SUCCESS;

}