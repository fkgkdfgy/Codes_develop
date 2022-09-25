/*
 * @Author: Liu Weilong
 * @Date: 2021-05-30 20:59:53
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-30 21:09:22
 * @Description: 
 */

#include "sophus/so3.hpp"
#include "Eigen/Eigen"

template <typename Derived1, typename Derived2>
inline void rightJacobianSO3(const Eigen::MatrixBase<Derived1> &phi,
                             const Eigen::MatrixBase<Derived2> &J_phi) {
  EIGEN_STATIC_ASSERT_FIXED_SIZE(Derived1);
  EIGEN_STATIC_ASSERT_FIXED_SIZE(Derived2);
  EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived1, 3);
  EIGEN_STATIC_ASSERT_MATRIX_SPECIFIC_SIZE(Derived2, 3, 3);

  using Scalar = typename Derived1::Scalar;

  Eigen::MatrixBase<Derived2> &J =
      const_cast<Eigen::MatrixBase<Derived2> &>(J_phi);

  Scalar phi_norm2 = phi.squaredNorm();
  Scalar phi_norm = std::sqrt(phi_norm2);
  Scalar phi_norm3 = phi_norm2 * phi_norm;

  J.setIdentity();

  if (Sophus::Constants<Scalar>::epsilon() < phi_norm) {
    Eigen::Matrix<Scalar, 3, 3> phi_hat = Sophus::SO3<Scalar>::hat(phi);
    Eigen::Matrix<Scalar, 3, 3> phi_hat2 = phi_hat * phi_hat;

    J -= phi_hat * (1 - std::cos(phi_norm)) / phi_norm2;
    J += phi_hat2 * (phi_norm - std::sin(phi_norm)) / phi_norm3;
  }
}

template <typename Derived1, typename Derived2>
inline void rightJacobianInvSO3(const Eigen::MatrixBase<Derived1> &phi,
                                const Eigen::MatrixBase<Derived2> &J_phi) {
  EIGEN_STATIC_ASSERT_FIXED_SIZE(Derived1);
  EIGEN_STATIC_ASSERT_FIXED_SIZE(Derived2);
  EIGEN_STATIC_ASSERT_VECTOR_SPECIFIC_SIZE(Derived1, 3);
  EIGEN_STATIC_ASSERT_MATRIX_SPECIFIC_SIZE(Derived2, 3, 3);

  using Scalar = typename Derived1::Scalar;

  Eigen::MatrixBase<Derived2> &J =
      const_cast<Eigen::MatrixBase<Derived2> &>(J_phi);

  Scalar phi_norm2 = phi.squaredNorm();
  Scalar phi_norm = std::sqrt(phi_norm2);

  J.setIdentity();

  if (Sophus::Constants<Scalar>::epsilon() < phi_norm) {
    Eigen::Matrix<Scalar, 3, 3> phi_hat = Sophus::SO3<Scalar>::hat(phi);
    Eigen::Matrix<Scalar, 3, 3> phi_hat2 = phi_hat * phi_hat;

    J += phi_hat / 2;
    J += phi_hat2 * (1 / phi_norm2 - (1 + std::cos(phi_norm)) /
                                         (2 * phi_norm * std::sin(phi_norm)));
  }
}




