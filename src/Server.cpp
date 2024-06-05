#include <filesystem>

#include <fstream>

#include <string>

#include "zstr.hpp"

int main(int argc, char *argv[])

{

    // You can use print statements as follows for debugging, they'll be visible when running tests.



    // std::cout << "Logs from your program will appear here!\n";

    // Uncomment this block to pass the first stage

    //

Expand 33 lines

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