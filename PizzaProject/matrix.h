#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

#include "utils.h"

class Matrix_4
{
public:
    std::vector<float> matrix;

    /*
    class Row
    {
        public:
        Row(const int& in_row, std::vector<float>& in_matrix):
        row(in_row), matrix(in_matrix)
        {}
        
        float& operator [](const int& in_idx)
        {
            return matrix[4*row + in_idx];
        }
        
        private:
        int row;
        std::vector<float>& matrix;
    };
    
    Row operator[] (const int& in_idx)
    {
        return Row(in_idx, matrix);
    }
    */

    Matrix_4():
        matrix(16)
    { 
        for (int i = 0; i < 16; i++)
        {
            if (i % 5 == 0)
                matrix[i] = 1;
            else
                matrix[i] = 0;
        }
    }

    Matrix_4& operator = (const Matrix_4& other)
    {
        matrix = other.matrix;
        return *this;
    }

    Matrix_4 operator * (const Matrix_4& other)
    {
        Matrix_4 to_return;
        
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int pos = 4 * i + j;
                float total = 0;
                for (int k = 0; k < 4; k++)
                {
                    int row = (4 * i) + k;
                    int col = (4 * k) + j;
                    
                    total += matrix[row] * other.matrix[col];
                }
                to_return.matrix[pos] = total;
            }
        }

        return to_return;
    }

    void print()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int pos = 4 * i + j;

                std::cout << matrix[pos] << "\t";
            }
            std::cout << "\n";
        }
    }
    
    void set_matrix(std::vector<float> in_mat)
    {
        matrix = in_mat;
    }

    void scale(float s_x, float s_y, float s_z)
    {
        Matrix_4 mat;
        mat.set_matrix({ s_x,   0,   0,   0,
                          0 , s_y,   0,   0,
                          0 ,   0, s_z,   0,
                          0 ,   0,   0,   1   });

        *this = mat * (*this);
    }

    void translate(float m_x, float m_y, float m_z)
    {
        Matrix_4 mat;
        mat.set_matrix({ 1, 0, 0, m_x,
                         0, 1, 0, m_y,
                         0, 0, 1, m_z,
                         0, 0, 0,   1   });

        *this = mat * (*this);
    }

    void rotate_x(float in_angle)
    {
        float ang = utils::ang_to_rad(in_angle);
        float sin = std::sin(ang);
        float cos = std::cos(ang);

        Matrix_4 mat;
        mat.set_matrix({ 1,   0,    0,  0,
                         0, cos, -sin,  0,
                         0, sin,  cos,  0,
                         0,   0,    0,  1});

        *this = mat * (*this);
    }
    
    void rotate_y(float in_angle)
    {
        float ang = utils::ang_to_rad(in_angle);
        float sin = std::sin(ang);
        float cos = std::cos(ang);

        Matrix_4 mat;
        mat.set_matrix({  cos,  0,  sin,  0,
                            0,  1,    0,  0,
                         -sin,  0,  cos,  0,
                            0,  0,    0,  1});

        *this = mat * (*this);
    }

    void rotate_z(float in_angle)
    {
        float ang = utils::ang_to_rad(in_angle);
        float sin = std::sin(ang);
        float cos = std::cos(ang);

        Matrix_4 mat;
        mat.set_matrix({ cos, -sin,  0,  0,
                         sin,  cos,  0,  0,
                           0,    0,  1,  0,
                           0,    0,  0,  1});

        *this = mat * (*this);
    }
};

#endif