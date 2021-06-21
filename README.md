# C++ Logger

Simple C++ logging library. It can be used to write logs to files or std::cout.
There are few terms of logging:

- `Logger` - an object is used to register log messages
- `Sink` - the target object that writes messages
- `LogLevel` - the severity level of messages that can be used to filter the messages output
- `Formatter` - determines the output format

In order to use the logger, you should create a `Logger` object and one or more sink objects.
Then you should add sinks to the `Logger` object using the `Logger::add_sink` method.

There are the following sink types:

- `FileSink` - writes messages to a file
- `CoutSink` - writes to the stdout.

## Files rotation

When you create a `FileSink`, you must provide a filename or a filename template.

```cpp
FileSink(const std::string &file_template, unsigned int max_num_files = 0);
```

The template can contain following variables:

 -  %Y - year YYYY
 -  %m - month number (01 - 12)
 -  %d - day of month (01 - 31)
 -  %j - day of year (001 - 366)
 -  %W - week of the year as a decimal number (Monday is the first day of the week) (00 - 53)
 -  %H - hour as a decimal number (00 - 23)

Current local time defines the filename according to the template and a new file is created each time when the filename changes. 

You can also specify the maximum number of files (0 - unlimited), so when a new file is created and the file number exceeds the limit - the oldest one is removed.

## Example

```cpp
#include <logging/logger.h>
#include <logging/sink/file.h>

int main(int argc, char *argv[])
{
    // setup logger
    logging::Logger log;
    logging::FileSink file_sink(logging::Formatter{}, "log.txt");
    log.add_sink(&filel_sink);

    // write line
    log.write(LogLevel::INFO) << "Command: " << argv[0];

    return 0;
}
```