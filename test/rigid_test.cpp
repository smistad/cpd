// cpd - Coherent Point Drift
// Copyright (C) 2016 Pete Gadomski <pete.gadomski@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <cpd/rigid.hpp>

#include "test/support.hpp"

namespace cpd {

class RigidTest : public FishTest {};

TEST_F(RigidTest, StandaloneFunction) {
    auto result = rigid(m_fish1, m_fish2);
    ASSERT_EQ(m_fish2.rows(), result.points.rows());
    EXPECT_TRUE(m_fish1.isApprox(result.points, 1e-4));
    EXPECT_TRUE(m_rotation.matrix().isApprox(result.rotation, 1e-4))
        << result.rotation;
    EXPECT_TRUE(
        m_translation.transpose().isApprox(-1 * result.translation, 1e-4))
        << result.translation;
    EXPECT_DOUBLE_EQ(1.0, result.scaling);
}

TEST_F(RigidTest, ClassBased) {
    Rigid rigid;
    auto result = rigid.compute(m_fish1, m_fish2);
    EXPECT_TRUE(m_fish1.isApprox(result.points, 1e-4));
}

TEST_F(RigidTest, GettersAndSetters) {
    Rigid rigid;
    rigid.set_max_iterations(1).set_tolerance(2.0).set_outlier_weight(3.0);
    rigid.no_reflections(false).allow_scaling(true);
    EXPECT_EQ(1, rigid.max_iterations());
    EXPECT_DOUBLE_EQ(2.0, rigid.tolerance());
    EXPECT_DOUBLE_EQ(3.0, rigid.outlier_weight());
    EXPECT_FALSE(rigid.no_reflections());
    EXPECT_TRUE(rigid.allow_scaling());
}

TEST_F(RigidTest, WithSigma2) {
    auto result = rigid(m_fish1, m_fish2, 1.0);
    EXPECT_TRUE(m_fish1.isApprox(result.points, 1e-4));
}

TEST(Rigid, UTM) {
    Matrix utm = load_test_data_matrix("utm.txt");
    Matrix utm2 = utm.array() + 2;
    auto result = rigid(utm, utm2, 3.0);
    EXPECT_TRUE(utm.isApprox(result.points, 1e-4));
}

TEST_F(RigidTest, NoFGT) {
    Rigid rigid;
    rigid.use_fgt(false);
    auto result = rigid.compute(m_fish1, m_fish2);
    EXPECT_TRUE(m_fish1.isApprox(result.points, 1e-4));
}
}
