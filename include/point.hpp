#ifndef POINT_HPP
#define POINT_HPP

class Point {
private:
    double x;
    double y;
    double z;

public:

    Point();
    Point(double n_x, double n_y, double n_z);

    double getX() const;
    double getY() const;
    double getZ() const;
    double get(int i) const;
    void setX(double n_x);
    void setY(double n_y);
    void setZ(double n_h);
    void set(int i, double value);
    void set(double x, double y, double z);
    void normalize();
};

#endif