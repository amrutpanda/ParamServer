#include <ps_table.hpp>
#include <Eigen/Dense>

int main(int argc, char const *argv[])
{
    ps::ParameterTable pTable;

    int v1 = 1;
    double v2 = 3.4;
    Eigen::Vector3d v3;
    v3.setOnes();

    std::cout << "adding table entry" << std::endl;
    pTable.addTableEntry<int>("key1",v1);
    pTable.addTableEntry<double>("key2",v2,1);
    pTable.addTableEntry<double,3>("key3",v3.data());

    std::cout << "finished" << std::endl;
    return 0;
}
