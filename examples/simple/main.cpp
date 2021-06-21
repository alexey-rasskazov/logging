#include <logging/logger.h>
#include <logging/sink/file.h>
#include <logging/sink/cout.h>

struct Point
{
    int x, y;

    Point(int x, int y) : x{x}, y{y} {}

    operator std::string()
    {
        return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
    }
};

int main(int argc, char *argv[])
{
    using namespace logging;

    // Setup logging
    Logger log;
    log.set_level(LogLevel::INFO);
    FileSink file_sink(Formatter{}, "logs/test.log");
    CoutSink cout_sink("${time:%H:%M:%S.%f} ${level_name}: ${message}");
    log.add_sink(&file_sink);
    log.add_sink(&cout_sink);

    // Write logs
    log.write(LogLevel::INFO) << "Start test";
    log.write(LogLevel::INFO) << u8"Привет мир" << '!' 
                              << " coord: " << Point{3, 5} 
                              << " " << sizeof(std::string);

    log.write(LogLevel::WARNING) << "Has args: " << (argc > 1) << ", N: " << argc - 1;
    for (int i = 1; i < argc; ++i) {
        log.write(LogLevel::INFO) << "Arg " << i << " = " << argv[i];
    }
    log.write(LogLevel::INFO) << "End test";

    return 0;
}