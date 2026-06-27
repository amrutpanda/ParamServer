#define EIGEN_RUNTIME_NO_MALLOC
#include <Eigen/Dense>
#include <Eigen/Core>

int main(int argc, char const *argv[])
{
    Eigen::Matrix3d a = 3*Eigen::Matrix3d::Ones();
    Eigen::MatrixXd b = 4*Eigen::MatrixXd::Ones(3,4);

    Eigen::MatrixXd c = 0*Eigen::Matrix3d::Ones(3,4);

    Eigen::internal::set_is_malloc_allowed(true);
    c = (a*b) + b;
    Eigen::internal::set_is_malloc_allowed(true);
    return 0;
}
