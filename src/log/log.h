#pragma once

#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>
#include <string>

namespace mini_lsm {

enum class LogLevel : std::uint8_t {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
};

inline const char *log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

class Logger {
   public:
    // default log rotation settings
    static constexpr size_t DEFAULT_MAX_FILE_SIZE = 30 * 1024 * 1024;  // 30MB
    static constexpr int DEFAULT_MAX_FILE_COUNT = 4;

    static Logger &get_instance() {
        static Logger instance;
        return instance;
    }

    void set_log_level(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        min_level_ = level;
    }

    // set log file with rotation settings
    void set_log_file(const char *filename, size_t max_file_size = DEFAULT_MAX_FILE_SIZE,
                      int max_file_count = DEFAULT_MAX_FILE_COUNT) {
        std::lock_guard<std::mutex> lock(mutex_);
        close_log_file_unlocked();

        base_filename_ = filename;
        max_file_size_ = max_file_size;
        max_file_count_ = max_file_count;
        current_file_size_ = 0;
        current_file_index_ = 0;

        // search for existing log files to determine the current index
        find_current_file_index();

        // open the current log file
        open_current_log_file();
    }

    void close_log_file() {
        std::lock_guard<std::mutex> lock(mutex_);
        close_log_file_unlocked();
    }

    void log(LogLevel level, const char *file, int line, const char *format, ...) {
        if (level < min_level_) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        // get current time
        char time_buffer[32];
        time_t now = time(nullptr);
        struct tm *tm_info = localtime(&now);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);

        const char *filename = strrchr(file, '/');
        if (!filename) {
            filename = file;
        }
        filename = filename ? filename + 1 : file;

        // format the log message
        char message_buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(message_buffer, sizeof(message_buffer), format, args);
        va_end(args);

        // construct the final log line
        char log_line[1200];
        int log_len = snprintf(log_line, sizeof(log_line), "[%s] [%s] [%s:%d] %s\n", time_buffer,
                               log_level_to_string(level), filename, line, message_buffer);

        if (log_file_) {
            // check if we need to rotate the log file
            if (current_file_size_ + log_len > max_file_size_) {
                rotate_log_file();
            }

            // output log to file
            fprintf(log_file_, "%s", log_line);
            fflush(log_file_);
            current_file_size_ += log_len;
        } else {
            // output log to stderr
            fprintf(stderr, "%s", log_line);
        }
    }

   private:
    Logger()
        : min_level_(LogLevel::DEBUG),
          log_file_(nullptr),
          max_file_size_(DEFAULT_MAX_FILE_SIZE),
          max_file_count_(DEFAULT_MAX_FILE_COUNT),
          current_file_size_(0),
          current_file_index_(0) {}

    ~Logger() {
        close_log_file_unlocked();
    }

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void close_log_file_unlocked() {
        if (log_file_) {
            fclose(log_file_);
            log_file_ = nullptr;
        }
        base_filename_.clear();
        current_file_size_ = 0;
    }

    // generate log filename based on index
    std::string get_log_filename(int index) const {
        if (index == 0) {
            return base_filename_;
        }
        // insert index before file extension
        size_t dot_pos = base_filename_.rfind('.');
        if (dot_pos != std::string::npos) {
            return base_filename_.substr(0, dot_pos) + "." + std::to_string(index) +
                   base_filename_.substr(dot_pos);
        }
        return base_filename_ + "." + std::to_string(index);
    }

    // find the current log file index and size
    void find_current_file_index() {
        current_file_index_ = 0;

        // check main log file
        FILE *f = fopen(base_filename_.c_str(), "r");
        if (f) {
            fseek(f, 0, SEEK_END);
            current_file_size_ = ftell(f);
            fclose(f);

            // if main log file is full, check rotated files
            if (current_file_size_ >= max_file_size_) {
                for (int i = 1; i < max_file_count_; ++i) {
                    std::string fname = get_log_filename(i);
                    f = fopen(fname.c_str(), "r");
                    if (!f) {
                        // file does not exist, use this index
                        current_file_index_ = i;
                        current_file_size_ = 0;
                        break;
                    }
                    fseek(f, 0, SEEK_END);
                    size_t size = ftell(f);
                    fclose(f);

                    if (size < max_file_size_) {
                        // file exists and is not full, use this index
                        current_file_index_ = i;
                        current_file_size_ = size;
                        break;
                    }
                }
            }
        }
    }

    // open the current log file based on index
    void open_current_log_file() {
        std::string fname = get_log_filename(current_file_index_);
        log_file_ = fopen(fname.c_str(), "a");
        if (log_file_) {
            fseek(log_file_, 0, SEEK_END);
            current_file_size_ = ftell(log_file_);
        }
    }

    // ratate the log file
    void rotate_log_file() {
        if (log_file_) {
            fclose(log_file_);
            log_file_ = nullptr;
        }

        // move to next file index
        current_file_index_ = (current_file_index_ + 1) % max_file_count_;
        current_file_size_ = 0;

        // delete the old file if exists
        std::string fname = get_log_filename(current_file_index_);
        remove(fname.c_str());

        // open new log file
        log_file_ = fopen(fname.c_str(), "w");
    }

    LogLevel min_level_;
    std::mutex mutex_;
    FILE *log_file_;

    // 日志轮转相关成员
    std::string base_filename_;
    size_t max_file_size_;
    int max_file_count_;
    size_t current_file_size_;
    int current_file_index_;
};

}  // namespace mini_lsm

// logging macros
#define LOG_DEBUG(fmt, ...)                                                                  \
    mini_lsm::Logger::get_instance().log(mini_lsm::LogLevel::DEBUG, __FILE__, __LINE__, fmt, \
                                         ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                                                  \
    mini_lsm::Logger::get_instance().log(mini_lsm::LogLevel::INFO, __FILE__, __LINE__, fmt, \
                                         ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)                                                                  \
    mini_lsm::Logger::get_instance().log(mini_lsm::LogLevel::WARN, __FILE__, __LINE__, fmt, \
                                         ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                                                  \
    mini_lsm::Logger::get_instance().log(mini_lsm::LogLevel::ERROR, __FILE__, __LINE__, fmt, \
                                         ##__VA_ARGS__)
