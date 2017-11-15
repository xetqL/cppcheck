/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   cpptests.hpp
 * Author: xetql
 *
 * Created on October 2, 2017, 2:36 PM
 */

#ifndef CPPTESTS_HPP
#define CPPTESTS_HPP

#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

class Test{
public:
    Test(){};
    virtual std::string test_summary() = 0;
    virtual bool is_test_passed() = 0;
    virtual void check() = 0;
};

std::string passed(std::string text){
    return "\033[42m"+text+" passed!\033[49m";
}

std::string failed(std::string text){
    return "\033[41m"+text+" failed!\033[49m";
}

template <typename A>
class UnitTest : public Test{
public:
    std::string text;
    bool is_passed = false;
    int g(int x(int)) { return x(1); }
    std::function< A() > func;
    std::function< bool(A)> check_func ;

    UnitTest(std::string text, A value, A expected):
        func([value](){return value;}),
        check_func([expected](A value){return expected==value;}),
        text(text){
    }

    UnitTest(std::string text, std::function< A() > lambda, A expected):
        check_func([expected](A value){return expected==value;}),
        text(text),
        func(lambda){
    }

    UnitTest(std::string text, std::function< A() > lambda, std::function< bool(A) > expected):
        func(lambda),
        check_func(expected),
        text(text){
    }

    std::string test_summary(){
        return this->text;
    }
    void check(){
        this->is_passed = check_func(func());
        if(this->is_passed){
            this->text=passed(text);
        }else{
            this->text=failed(text);
        }
    }
    bool is_test_passed(){
        return this->is_passed;
    }

};

class TestsRunner {
    std::vector<std::shared_ptr<Test>> tests;
    unsigned int nb_passed = 0;
    std::string name;
public:
    TestsRunner(std::string name):name(name){};

    virtual ~TestsRunner() {}

    virtual int are_tests_passed(){
        return nb_passed == tests.size()? 0:-2;
    }

    virtual void run() {
        unsigned int tid = 1;
        for(auto const& test: tests){ // I DONT WANT THE OWNERSHIP BRO
            test->check();
            nb_passed += test->is_test_passed()? 1:0;
            std::cout << tid << "/"<< tests.size() << " " << test->test_summary()<< std::endl;
            tid++;
        }
    }

    virtual void summarize(){
        std::string final_summary = "Number of tests passed: " + std::to_string(nb_passed) + " / " + std::to_string(tests.size()) + " ->";
        std::cout << "Final summary of: "<<name<<std::endl;
        std::cout << "---------------------------"<<std::endl;
        std::cout << ((nb_passed == tests.size()) ? passed(final_summary) : failed(final_summary)) <<std::endl;
    }

    void add_test(std::shared_ptr<Test> t) {
        tests.push_back(t);
    }
};

#endif /* CPPTESTS_HPP */
