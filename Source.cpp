#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define TEST_HANDLE(object) typedef struct object##_T *object;
TEST_HANDLE(foo);

foo adrs;
foo* memaddress = &adrs;

void create(foo* pFoo) {
    *memaddress = *pFoo;
    std::cout << "Foo address is: " << pFoo << std::endl;
}
void destroy(foo Foo) {
    std::cout << "I am destroying Foo!" << std::endl;
    if (memaddress == &Foo) {
        delete[] memaddress;
        delete[] Foo;
    }
}

enum shaderStages {// deconstructed Vulkan enumeration of shader stages
    test1 = 1, test2 = 2, test3 = 3
};

struct rofTest {// Desconstructed version of my shader object
    foo test;
    rofTest(std::string const& filename, shaderStages stage) {
        std::cout << "Test's address: " << &test << std::endl;
        create(&test);
    }
    ~rofTest() {
        std::cout << "I am destructing!";
        destroy(test);
    }

    //rofTest(const rofTest& other) = delete;
    //rofTest& operator=(const rofTest& other) = delete;

    rofTest(rofTest&& other) noexcept // move constructor
        : test(std::exchange(other.test, nullptr))
    {}

    rofTest& operator=(rofTest&& other) noexcept // move assignment
    {
        std::swap(test, other.test);
        return *this;
    }

};

int main()
{
    std::cout << "memaddress is: " << memaddress << std::endl;
    try {
        std::vector<rofTest> testi
        {
            {"file1.shader", test1}, {"file2.shader", test2}, {"file3.shader", test3}
        };
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}