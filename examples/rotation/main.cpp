#include <logging/logger.h>
#include <logging/sink/file.h>

int main(int argc, char *argv[])
{
    logging::Logger log;
    log.set_level(LogLevel::INFO);
    logging::FileSink file_sink_h(
        logging::Formatter{},
        "logs/test_%Y-%m-%d_%H.log" // rotation every hour, 
        , 10                        // 10 files maximum
    ); 
    logging::FileSink file_sink_d(
        logging::Formatter{},
        "logs/test_%Y-%m-%d.log" // rotation every day, 
        , 5                      // 5 files maximum
    ); 
    log.add_sink(&file_sink_h);
    log.add_sink(&file_sink_d);

    log.write(LogLevel::WARNING) << "Start test";

    log.write(LogLevel::INFO) << "Has args: " << (argc > 1) << ", N: " << argc - 1;
    for (int i = 1; i < argc; ++i) {
        log.write(LogLevel::INFO) << "Arg " << i << " = " << argv[i];
    }

    log.write(LogLevel::WARNING) << "End test";

    return 0;
}