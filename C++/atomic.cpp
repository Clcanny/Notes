#include <atomic>

void atomicFunction()
{
    std::atomic<int> a;
    a = 0;
    a++;
}

int main()
{
}
