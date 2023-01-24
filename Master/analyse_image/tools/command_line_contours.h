#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

void usage(int /*argc*/, char **argv)
{
    // Name
    std::cout << "Name" << std::endl;
    std::cout << "        " << argv[0] << " - Edge detection.";
    std::cout << std::endl
              << std::endl;
    // Synopsis
    std::cout << "SYNOPSIS" << std::endl;
    std::cout << "        " << argv[0] << " /data/file"
              << " [--help|-h|-?]"
              << " [-multi] [-global] [-mask M] [threshold H] [-refining]"
              << std::endl
              << std::endl;
    // Options
    std::cout << "        --help, -h, -?" << std::endl
              << "                display this help and exit."
              << std::endl
              << std::endl;
    std::cout << "        -mask M"
              << std::endl
              << "                choosen mask between 0 = Prewitt, 1 = Sobel, 2 = Kirsch (0 by default)."
              << std::endl
              << std::endl;
    std::cout << "        -threshold H"
              << std::endl
              << "                threshold of convolution between 0 and 255 (0 by default)."
              << std::endl
              << std::endl;
    std::cout << "        -multi"
              << std::endl
              << "                multidirectional used (bidirectional by default)."
              << std::endl
              << std::endl;
    std::cout << "        -global"
              << std::endl
              << "                global threshold used (false by default)."
              << std::endl
              << std::endl;
    std::cout << "        -refining"
              << std::endl
              << "                refining used (false by default)."
              << std::endl
              << std::endl;
    exit(EXIT_FAILURE);
}

void error_command_line(int argc, char **argv, const char *msg)
{
    std::cout << "ERROR: " << msg << std::endl;
    usage(argc, argv);
}

void process_command_line(int argc, char **argv, bool &multi, bool &global,
                          bool &hysteresis, bool &refining, unsigned int &mask,
                          unsigned int &threshold) {
  multi = false;
  global = false;
  hysteresis = false;
  refining = false;
  mask = 0;
  threshold = 0;

  bool help_required = false;

  std::string arg;
  for (int i = 1; i < argc; ++i) {
    arg = std::string(argv[i]);
    if (arg == std::string("-h") || arg == std::string("--help") ||
        arg == std::string("-?")) {
      help_required = true;
    } else if (arg == "-multi") {
      multi = true;
    } else if (arg == "-global") {
      global = true;
    } else if (arg == "-hysteresis") {
      hysteresis = true;
    } else if (arg == "-refining") {
      refining = true;
    } else if (arg == "-mask") {
      if (argc - 1 - i < 1) {
        error_command_line(argc, argv,
                           "Error: we need a number after -mask option.");
      }
      mask = std::stod(std::string(argv[++i]));
    } else if (arg == "-threshold") {
      if (argc - 1 - i < 1) {
        error_command_line(argc, argv,
                           "Error: we need a number after -threshold option.");
      }
      threshold = std::stod(std::string(argv[++i]));
      if (threshold > 255 || threshold < 0) {
        error_command_line(argc, argv,
                           "Error: we need a number between 0 and 255 after "
                           "-threshold option.");
        threshold = 0;
      }
    }
  }
  if (help_required) {
    usage(argc, argv);
  }
}
#endif