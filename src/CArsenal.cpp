#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace po = boost::program_options;
namespace logging = boost::log;

void log_example() {
    // 设置日志输出到文件
    logging::add_file_log("sample.log");
    
    // 不同级别的日志
    BOOST_LOG_TRIVIAL(trace) << "这是一个 trace 日志";
    BOOST_LOG_TRIVIAL(debug) << "这是一个 debug 日志";
    BOOST_LOG_TRIVIAL(info) << "这是一个 info 日志";
    BOOST_LOG_TRIVIAL(warning) << "这是一个 warning 日志";
    BOOST_LOG_TRIVIAL(error) << "这是一个 error 日志";
    BOOST_LOG_TRIVIAL(fatal) << "这是一个 fatal 日志";
}

void thread_example() {
    boost::thread t([](){
        BOOST_LOG_TRIVIAL(info) << "线程执行中...";
        boost::this_thread::sleep_for(boost::chrono::seconds(1));
        BOOST_LOG_TRIVIAL(info) << "线程结束";
    });
    t.join();
}

int main(int argc, char* argv[]) {
    // 1. 命令解析示例
    po::options_description desc("允许的选项");
    desc.add_options()
        ("help", "显示帮助信息")
        ("name", po::value<std::string>(), "你的名字")
        ("verbose,v", "详细输出");
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    
    if (vm.count("name")) {
        BOOST_LOG_TRIVIAL(info) << "你好, " << vm["name"].as<std::string>() << "!";
    } else {
        BOOST_LOG_TRIVIAL(warning) << "没有提供名字";
    }
    
    // 2. 日志示例
    log_example();
    
    // 3. 线程示例
    thread_example();
    
    return 0;
}