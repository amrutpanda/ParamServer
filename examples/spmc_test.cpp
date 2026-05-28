#include <ps_databuffer.hpp>
#include <ps_table.hpp>

int main(int argc, char const *argv[])
{
    int a[2], b[2];
    ps::SpmcBuffer<int> spmc;
    ps::Slot<int,2> s;
    spmc.write(1);
    a[0] = 10;
    s.write(a);
    ps::ParameterTable ptable;
    int ret = ptable.addTableEntry("1",a,1);


    // std::cout << "ind: " << ret << std::endl;
    std::cout << "elem size: " << s.getElemSize() << std::endl;
    std::cout << "total size: " << s.getSize() << std::endl;
    s.read(a);
    std::cout << "value: " << a[0] << std::endl;

    return 0;
}

