/*
 * utility.c:
 *  This file contains some utility functions used in MOEA/D for initialization and memory control related.
 *
 * Authors:
 *  Renzhi Chen <rxc332@cs.bham.ac.uk>
 *  Ke Li <k.li@exeter.ac.uk>
 *
 * Institution:
 *  Computational Optimization and Data Analytics (CODA) Group @ University of Exeter
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

# include "../../header/selection.h"

int C;

/* Memory release for MOEA/D related pointers */
void moead_free ()
{
    int i;

    free (ideal_point);

    for (i = 0; i < reference_size; i++)
        free (lambda[i]);
    free (lambda);

    for (i = 0 ; i < popsize ; i++)
        free(neighborhood[i]);
    free(neighborhood);

    lambda       = NULL;
    ideal_point  = NULL;
    neighborhood = NULL;

    return;
}

/* Initialize the weight vectors according to the Das and Dennis' method */
void initialize_uniform_weight ()
{
    int i, j, l;
    int ptr;
    int layer;

    int* layer_size;
    int* gaps_table;

    double shrink;
    double* Vec;

    gaps_table = weight_gaps_table[number_objective];
    for (layer = 0; layer < number_objective; layer++)
        if (gaps_table[layer] <= 0)
            break;

    reference_size = 0;
    layer_size = (int *) malloc (sizeof(int) * layer);
    for (i = 0; i < layer; i++)
    {
        layer_size[i]  = combination (number_objective + gaps_table[i] - 1, gaps_table[i]);
        reference_size = reference_size + layer_size[i];
    }

    lambda = (double **) malloc (reference_size * sizeof(double *));
    for (i = 0; i < reference_size; i++)
        lambda[i] = (double *) malloc(number_objective * sizeof(double));

    C   = 0;
    ptr = 0;

    shrink = 1;
    for (l = 0; l < layer; l++) {
        Vec = (double *) malloc (number_objective * sizeof(double));
        for (i = 0; i < number_objective; i++)
            Vec[i] = 0;
        set_weight (Vec, gaps_table[l], 0, number_objective);
        for (i = ptr; i < ptr + layer_size[l]; i++)
            for (j = 0; j < number_objective; j++) {
                lambda[i][j] = lambda[i][j] / gaps_table[l];
                lambda[i][j] = (1 - shrink) / number_objective + shrink * lambda[i][j];
            }
        ptr    = ptr + layer_size[l];
        shrink = shrink * 0.8;
        free(Vec);
    }
    free(layer_size);

    return;
}

/* Sample uniformly distributed weight vectors according to Das and Denis' method */
void set_weight (double* v, double unit, double sum, int dim)
{
    int i;

    if (dim == number_objective)
    {
        for ( i = 0; i < number_objective; i++)
            v[i] = 0;
    }

    if (dim == 1)
    {
        v[0] = unit - sum;
        for ( i = 0; i < number_objective; i++)
            lambda[C][i] = v[i];
        C = C + 1;
        return;
    }
    for (i = 0; i <= unit - sum; i++)
    {
        v[dim - 1] = i;
        setweight(v, unit, sum + i, dim - 1);
    }

    return;
}

/* Initialize the neighborhood structure of weight vectors */
void initialize_neighborhood ()
{
    int i, j;
    struct double_s *dist;

    dist         = (struct double_s*) malloc (sizeof(struct double_s) * reference_size);
    neighborhood = (int **) malloc (popsize * sizeof(int *));   // free in moead_free

    for (i = 0; i < popsize; i++)
        neighborhood[i] = (int *) malloc (neighbor_size * sizeof(int));
    for (i = 0; i < popsize; i++)
    {
        int id = i % reference_size;
        // calculate the distances based on weight vectors
        for (j = 0; j < reference_size; j++)
        {
            dist[j].x   = euclidian_distance (lambda[id], lambda[j], number_objective);
            dist[j].idx = j;
        }
        qsort (dist, reference_size, sizeof(struct double_s), sort_double_cmp);  // ascending order
        for( j = 0 ; j < neighbor_size ; j ++)
            neighborhood[i][j] = dist[j].idx;
    }

    free(dist);

    return;
}

/* Initialize the ideal point */
void initialize_idealpoint (void * pop)
{
    int i;

    population_real * ptr = (population_real*) pop;
    ideal_point = (double *) malloc (sizeof(double) * number_objective);    // free in moead_free

    for (i = 0; i < number_objective; i++)
        ideal_point[i] = INF;

    for (i = 0 ;i < popsize ; i ++)
        update_ideal_point(&(ptr->ind[i]));

    return;
}

/* Update the current ideal point */
void update_ideal_point (individual_real * individual)
{
    int i;

    for (i = 0; i < number_objective; i++)
        if (individual->obj[i] < ideal_point[i])
            ideal_point[i] = individual->obj[i];

    return;
}

/* Initialize the nadir point */
void initialize_nadirpoint (void * pop)
{
    int i;

    population_real * ptr = (population_real*) pop;
    nadir_point = (double *) malloc (sizeof(double) * number_objective);    // free in moead_free

    for (i = 0; i < number_objective; i++)
        nadir_point[i] = -INF;

    for (i = 0 ;i < popsize ; i ++)
        update_nadir_point(&(ptr->ind[i]));

    return;
}

/* Update the current ideal point */
void update_nadir_point (individual_real * individual)
{
    int i;

    for (i = 0; i < number_objective; i++)
        if (individual->obj[i] < ideal_point[i])
            nadir_point[i] = individual->obj[i];

    return;
}
