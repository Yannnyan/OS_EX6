#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
using namespace doctest;

const int MIN_TESTS = 20;


struct ReporterGrader: public ConsoleReporter {
    ReporterGrader(const ContextOptions& input_options)
            : ConsoleReporter(input_options) {}

    void test_run_end(const TestRunStats& run_stats) override {
        ConsoleReporter::test_run_end(run_stats);
        // std::cout << "Grade: " << grade << std::endl;
    }
};

REGISTER_REPORTER("grader", /*priority=*/1, ReporterGrader);

int main(int argc, char** argv) {
    Context context;
    context.addFilter("reporters", "grader");
    return context.run();
}