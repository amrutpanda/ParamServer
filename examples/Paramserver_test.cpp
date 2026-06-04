#include <ps_paramserver.hpp>

template <unsigned int N = 1>
class Controller
{
private:
     static constexpr unsigned int n = N;
     ps::ParamServer ps;
     Eigen::Vector<double,n> state1;
     Eigen::Vector<double,n> state2;
public:
    Controller(ps::ParamServer& ps_ ): ps(ps_) {};
    ~Controller() = default;
    void create_params()
    {
        ps.AddParameterVec("key_1", state1,0);
        ps.AddParameterVec("key_2", state2,0);
    };
};

int main(int argc, char const *argv[])
{
    ps::ParamServer pserver;

    Eigen::Vector<int, 100> vi;
    Eigen::Vector<double, 100> vd;

    std::string prefix = "key";
    int gid = pserver.createGroup("ethercat");
    gid = pserver.createGroup("control");
    std::cout << "group id: " << gid << std::endl;

    Controller<7> controller(pserver);

    // for (size_t i = 0; i < vi.size(); i++)
    // {
    //     pserver.AddParameter(prefix+std::to_string(i), vi[i],1);
    // }

    // for (size_t i = 0; i < vd.size(); i++)
    // {
    //     int idx = vi.size() + i;
    //     pserver.AddParameter(prefix+std::to_string(idx), vd[i],1);
    // }
    
    return 0;
}
