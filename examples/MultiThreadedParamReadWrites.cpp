#include <ps_table.hpp>
#include <ps_paramserver.hpp>
#include <thread>
#include <chrono>
#include <Eigen/Dense>
#include <signal.h>

bool runloop = true;
void sig_handler(int) {runloop = false;}
void worker(int, ps::ParameterTable&);

int main(int argc, char const *argv[])
{
    signal(SIGINT,sig_handler);
    signal(SIGTERM,sig_handler);

    ps::ParameterTable pTable;

    int v1 = 10;
    double v2 = 2.1;
    int a[10];
    // std::vector<double> v3(3);
    std::vector<double> v4(8);
    std::array<double, 3> v3;
    double v5 = 0;
    Eigen::Vector<double, 100> v6;
    // Eigen::VectorXd v7(100);
    Eigen::Vector<double, 100> v7, v8, v9, v10, v11, v12, v13;
    v6.setOnes();

    pTable.addTableEntry("key1",v1);
    pTable.addTableEntry("key2", v2,1);
    pTable.addTableEntry("key2", v5);

    pTable.addTableEntry("key3", a);
    pTable.addTableEntry<double,100>("key4",v6.data(),1);
    // pTable.addTableEntry<double,100>("key5",v7.data(),0);
    pTable.addTableEntry<double,100>("key5",v7.data(),1);
    pTable.addTableEntry<double,100>("key6",v8.data(),1);
    pTable.addTableEntry<double,100>("key7",v9.data(),1);
    pTable.addTableEntry<double,100>("key8",v10.data(),1);
    pTable.addTableEntry<double,100>("key9",v11.data(),1);
    pTable.addTableEntry<double,100>("key10",v12.data(),1);
    pTable.addTableEntry<double,100>("key11",v13.data(),1);

    auto keymap = pTable.getKeyIndexMap();

    for (const auto& [key, value] : keymap)
    {
        std::cout << "key: " << key << " index: " << value << std::endl;
    }
    
    std::thread thread1(worker, 1,std::ref(pTable));
    std::thread thread2(worker, 2,std::ref(pTable));
    std::thread thread3(worker, 3,std::ref(pTable));
    std::thread thread4(worker, 4,std::ref(pTable));
    std::thread thread5(worker, 5,std::ref(pTable));
    std::thread thread6(worker, 6,std::ref(pTable));
    std::thread thread7(worker, 7,std::ref(pTable));
    std::thread thread8(worker, 8,std::ref(pTable));
    std::thread thread9(worker, 9,std::ref(pTable));
    std::thread thread10(worker, 10,std::ref(pTable));

    std::cout << "**************************** Read and write test *************************" << std::endl;
    double count = 0.0;

    while (runloop)
    {
        v6 = count*Eigen::VectorXd::Ones(100);
        v7 = count*Eigen::VectorXd::Ones(100);
        v8 = count*Eigen::VectorXd::Ones(100);
        v9 = count*Eigen::VectorXd::Ones(100);
        v10 = count*Eigen::VectorXd::Ones(100);
        v11 = count*Eigen::VectorXd::Ones(100);
        v12 = count*Eigen::VectorXd::Ones(100);
        v13 = count*Eigen::VectorXd::Ones(100);

        pTable.writeToKey("key4", v6.data());
        pTable.writeToKey("key5",v7.data());
        pTable.writeToKey("key6",v8.data());
        pTable.writeToKey("key7",v9.data());
        pTable.writeToKey("key8",v10.data());
        pTable.writeToKey("key9",v11.data());
        pTable.writeToKey("key10",v12.data());
        pTable.writeToKey("key11", v13.data());
        // pTable.readFromKey("key4", &v7);
        // std::cout << "v5: " << v2 << std::endl;
        count = count + 0.01;
    }

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();
    thread10.join();

    return 0;
}


void worker(int n,ps::ParameterTable& pTable)
{
    Eigen::VectorXd v1(100), v2(100), v3(100), v4(100), v5(100), v6(100), v7(100),v8(100);
    // v = Eigen::VectorXd::Ones(100);
    int count = 0;
    while (runloop)
    {
        pTable.readFromKey("key4",v1.data());
        pTable.readFromKey("key5",v2.data());
        pTable.readFromKey("key6",v3.data());
        pTable.readFromKey("key7",v4.data());
        pTable.readFromKey("key8",v5.data());
        pTable.readFromKey("key9",v6.data());
        pTable.readFromKey("key10",v7.data());
        pTable.readFromKey("key11",v8.data());
        if (count % (100000) == 0)
            std::cout << "worker: "<< n << " v: " << v2.transpose() << std::endl;
        count++;
    }
    std::cout << "exiting worker thread: " << n << std::endl;
}