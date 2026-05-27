#include <ps_table.hpp>
#include <thread>
#include <Eigen/Dense>

int main(int argc, char const *argv[])
{
    ps::ParameterTable pTable;

    int v1 = 10;
    double v2 = 2.1;
    int a[10];
    std::vector<double> v3(3);
    std::vector<double> v4(8);
    double v5 = 0;
    Eigen::Vector3d v6;
    v6.setOnes();

    pTable.addTableEntry("key1",v1);
    pTable.addTableEntry("key2", v2,1);
    pTable.addTableEntry("key2", v5);

    pTable.addTableEntry("key3", a);
    pTable.addTableEntry("key4",v6.data(),1);
    pTable.addTableEntry("key4",v3.data(),0);

    auto keymap = pTable.getKeyIndexMap();

    for (const auto& [key, value] : keymap)
    {
        std::cout << "key: " << key << " index: " << value << std::endl;
    }
    
    return 0;
}
