#include <cppcheck.hpp>
#include <functional>
#include <random>
#include <memory>

int main(int argc, char const *argv[]) {
    auto test_1 = std::make_shared<UnitTest<int>>("Should be ok", 4*4, 16);
    auto test_lambda = std::make_shared<UnitTest<int>> ("Should be a wrong test", [](){
        int i = 0;
        while(i<9999999){
            i += rand() > 0.5 ? 1:0;
        }
        return 12;
    }, 16);
    auto test_2= std::make_shared<UnitTest<int>>("Should be wrong", 4-4, 16);

    auto test_lambda_lambda = std::make_shared<UnitTest<int>>("Should be a valid test", [](){
        int i = 0;
        while(i<9999999){
            i += rand() > 0.5 ? 1:0;
        }
        return 12;
    }, [](int v){ return v > 1; });

    TestsRunner runner("my test app");

    runner.add_test(test_1);
    runner.add_test(test_lambda);
    runner.add_test(test_lambda_lambda);
    runner.add_test(test_2);
    runner.run();
    runner.summarize();

    return runner.are_tests_passed();
}
