#include "software/new_geom/util/closest_point.h"

#include <gtest/gtest.h>

TEST(ClosestPointTest, three_points_general)
{
    Point p = closestPointOnLine(Point(0, 0), Point(2, 0), Point(0, 2));
    EXPECT_EQ(p, Point(1, 1));
}

TEST(ClosestPointTest, point_line_general)
{
    Point p = closestPointOnLine(Point(0, 0), Line(Point(2, 0), Point(0, 2)));
    EXPECT_EQ(p, Point(1, 1));
}

TEST(ClosestPointTest, point_on_line)
{
    Point p = closestPointOnLine(Point(4, 4), Line(Point(0, 0), Point(1, 1)));
    EXPECT_EQ(p, Point(4, 4));
}