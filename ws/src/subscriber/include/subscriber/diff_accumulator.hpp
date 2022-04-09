#include <math.h>
namespace subscriber {
struct Val3D {
    Val3D(const double _x = 0.0, const double _y = 0.0, const double _z = 0.0):
        x(_x), y(_y), z(_z){}
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    double abs() const {
        return sqrt( pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
    }

    Val3D operator-() const {
       return {-x, -y, -z};
    }

    Val3D& operator+=(const Val3D& other){
        x+= other.x;
        y+= other.y;
        z+= other.z;
        return *this;
    }

    Val3D& operator-=(const Val3D& other){
        *this+= -other;
        return *this;
    }

};

const Val3D operator+(const Val3D& lhs, const Val3D& rhs){
    auto res = lhs;
    res+=rhs;
    return res;
}
const Val3D operator-(const Val3D& lhs, const Val3D& rhs){
    auto res = lhs;
    res-=rhs;
    return res;
}

class DistanceAccumulator {
public:
    double value = 0.0;

    double accumulate(const Val3D& new_val){
        auto delta = new_val - prev_val;
        value += delta.abs();
        prev_val = new_val;
        return value;
    }

private:
    Val3D prev_val;
};

class Derivative{
public:
    Derivative(double _alpha = 0.0): alpha_(_alpha){}

    double derivate (Val3D val, double time_pt){
        const double delta_t = time_pt - prev_time_pt_;
        prev_time_pt_ = time_pt;
        const double div_new = (val - prev_val).abs() / delta_t;
        prev_val = val;
        div_ = alpha_*div_ + (1 - alpha_) * div_new; //simple lo-pass filter
        return div_;
    }
private:
    double alpha_ = 0.0; //low-pass filter coef. 0 - No filter; 1 - no change;
    Val3D prev_val = {};
    double div_ = 0.0;
    double prev_time_pt_ = 0.0;
};
} //subscriber