#include <ps_databuffer.hpp>

int main(int argc, char const *argv[])
{
    int a[2], b[2];
    paramserver::SpmcBuffer<int> spmc;
    paramserver::Slot<int[2]> s;
    s.write(1,0);

    std::cout << "elem size: " << s.getElemSize() << std::endl;
    std::cout << "total size: " << s.getSize() << std::endl;
    s.read(a);
    std::cout << "value: " << a[0] << std::endl;

    return 0;
}

