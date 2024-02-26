#include <iomanip>
#include "Matrix4.h"
#include "Vec4.h"

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->values[i][j] = 0;
        }
    }
}

Matrix4::Matrix4(float values[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->values[i][j] = values[i][j];
        }
    }
}

Matrix4::Matrix4(const Matrix4 &other)
{
    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            this->values[i][j] = other.values[i][j];
        }
    }
}
Matrix4 Matrix4::operator=(const Matrix4 &other)
{
    if (this != &other)
    {
        for (int i = 0; i < 4; i ++)
        {
            for (int j = 0; j < 4; j ++)
            {
                this->values[i][j] = other.values[i][j];
            }
        }
    }

    return *this;
}

Vec4 Matrix4::operator*(Vec4 &v) const
{
    float res[4];
    float total;

    for (int i = 0; i < 4; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += this->values[i][j] * v.getNthComponent(j);
        }

        res[i] = total;
    }

    return Vec4(res[0], res[1], res[2], res[3]);
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const
{
    Matrix4 result;
    float total;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            total = 0;
            for (int k = 0; k < 4; k++)
            {
                total += this->values[i][k] * other.values[k][j];
            }

            result.values[i][j] = total;
        }
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const Matrix4 &m)
{

    os << std::fixed << std::setprecision(6) << "|" << m.values[0][0] << "|" << m.values[0][1] << "|" << m.values[0][2] << "|" << m.values[0][3] << "|"
       << std::endl
       << "|" << m.values[1][0] << "|" << m.values[1][1] << "|" << m.values[1][2] << "|" << m.values[1][3] << "|"
       << std::endl
       << "|" << m.values[2][0] << "|" << m.values[2][1] << "|" << m.values[2][2] << "|" << m.values[2][3] << "|"
       << std::endl
       << "|" << m.values[3][0] << "|" << m.values[3][1] << "|" << m.values[3][2] << "|" << m.values[3][3] << "|";

    return os;
}