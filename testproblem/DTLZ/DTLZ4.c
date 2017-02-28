/*
 * DTLZ1.c
 *
 * Authors:
 *  Renzhi Chen <rxc332@cs.bham.ac.uk>
 *  Ke Li <k.li@exeter.ac.uk>
 *
 * Copyright (c) 2017 Renzhi Chen, Ke Li
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../header/global.h"

void dtlz4 (double *xreal, double *obj)
{
    int i, j;
    int aux;
    double gx = 0;
    double sum = 0;
    double alpha = 100;
    int k = number_variable - number_objective + 1;

    for(i = number_variable - k; i < number_variable; i++)
        gx += (xreal[i] - 0.5) * (xreal[i] - 0.5);

    for (i = 0; i < number_objective; i++)
    {
        obj[i] = (1.0 + gx);
    }

    for (i = 0; i < number_objective; i++)
    {
        for (j = 0; j < number_objective - (i + 1); j++)
        {

            obj[i] *= cos(PI * 0.5 * pow(xreal[j],alpha));
        }
        if (i != 0)
        {
            aux = number_objective - (i + 1);
            obj[i] *= sin(PI * 0.5 * pow(xreal[aux],alpha));
        }
    }
}