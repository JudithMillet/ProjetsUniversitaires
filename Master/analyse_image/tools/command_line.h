#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

void usage(int /*argc*/, char **argv)
{
    // Name
    std::cout << "Name" << std::endl;
    std::cout << "        " << argv[0] << " - Test region growing.";
    std::cout << std::endl
              << std::endl;
    // Synopsis
    std::cout << "SYNOPSIS" << std::endl;
    std::cout << "        " << argv[0] << " /data/file"
              << " [--help|-h|-?]"
              << " [-nbSeeds N] [-tolerance T] [threshold H] [-smoothing] [-threads] [-outline]"
              << std::endl
              << std::endl;
    // Options
    std::cout << "        --help, -h, -?" << std::endl
              << "                display this help and exit."
              << std::endl
              << std::endl;
    std::cout << "        -nbSeeds N"
              << std::endl
              << "                numbers of seeds (100 by default)."
              << std::endl
              << std::endl;
    std::cout << "        -tolerance T"
              << std::endl
              << "                tolerance rate (80% by default)."
              << std::endl
              << std::endl;
    std::cout << "        -threshold H"
              << std::endl
              << "                threshold of similarity (5 by default)."
              << std::endl
              << std::endl;
    std::cout << "        -smoothing"
              << std::endl
              << "                smooths the output image."
              << std::endl
              << std::endl;
    std::cout << "        -threads"
              << std::endl
              << "                use threads."
              << std::endl
              << std::endl;
    std::cout << "        -outline"
              << std::endl
              << "                show outline."
              << std::endl
              << std::endl;
    exit(EXIT_FAILURE);
}

void error_command_line(int argc, char **argv, const char *msg)
{
    std::cout << "ERROR: " << msg << std::endl;
    usage(argc, argv);
}

void process_command_line(int argc, char **argv, unsigned int &nbSeeds, unsigned int &tolerance, unsigned int &threshold, bool &smoothing, bool &threads, bool &outline)
{
    nbSeeds = 100;
    tolerance = 80;
    threshold = 5;
    smoothing = false;
    threads = false;
    outline = false;

    bool help_required = false;

    std::string arg;
    for (int i = 1; i < argc; ++i)
    {
        arg = std::string(argv[i]);
        if (arg == std::string("-h") || arg == std::string("--help") || arg == std::string("-?"))
        {
            help_required = true;
        }
        else if (arg == "-nbSeeds")
        {
            if (argc - 1 - i < 1)
            {
                error_command_line(argc, argv, "Error: we need a number after -nbSeeds option.");
            }
            nbSeeds = std::stod(std::string(argv[++i]));
        }
        else if (arg == "-tolerance")
        {
            if (argc - 1 - i < 1)
            {
                error_command_line(argc, argv, "Error: we need a number after -tolerance option.");
            }
            tolerance = std::stod(std::string(argv[++i]));
        }
        else if (arg == "-threshold")
        {
            if (argc - 1 - i < 1)
            {
                error_command_line(argc, argv, "Error: we need a number after -threshold option.");
            }
            threshold = std::stod(std::string(argv[++i]));
        }
        else if (arg == "-smoothing")
        {
            smoothing = true;
        }
        else if (arg == "-threads")
        {
            threads = true;
        }
        else if (arg == "-outline")
        {
            outline = true;
        }
    }
    if (help_required)
    {
        usage(argc, argv);
    }
}
#endif