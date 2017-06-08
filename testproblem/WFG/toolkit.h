#ifndef SAMARITAN_TOOLKIT_H
#define SAMARITAN_TOOLKIT_H
#include <math.h>
#include "../../header/global.h"

int Degenerate;

double *wfg_temp;
double *temp;
double *wfg_w;

int wfg_K;
void WFG_ini ();
void WFG_free ();
int WFG1_t1 (double* y,int y_size, int k, double * result);
int WFG1_t2 (double* y, int y_size, int k, double * result);
int WFG1_t3 ( double* y, int y_size, double *result);
int WFG1_t4(double *y,int y_size,int k,int M,double *result);
int WFG2_t2 (double * y, int y_size, int k,double * result);
int WFG2_t3 (double *y, int y_size,int k,int M,double *result);
int WFG4_t1 (double *y, int y_size, double *result);
int WFG5_t1 (double * y , int y_size, double * result );
int WFG6_t2 (double* y,int y_size, int k, const int M, double * result);
int WFG7_t1 (double * y, int y_size,int k, double * result);
int WFG8_t1 (double* y, int y_size, int k,double * result);
int WFG9_t1(double* y, int y_size,double* result );
int WFG9_t2 ( double* y,int y_size, int k, double * result);

void WFG1_shape (double *y, int size, double * result);
void WFG2_shape (double *y, int size, double * result);
void WFG3_shape (double *y, int size, double * result);
void WFG4_shape (double *y, int size, double * result);
void WFG42_shape (double *y, int size, double * result);
void WFG43_shape (double *y, int size, double * result);
void WFG44_shape (double *y, int size, double * result);
void WFG45_shape (double *y, int size, double * result);
void WFG46_shape (double *y, int size, double * result);
void WFG47_shape (double *y, int size, double * result);
void WFG48_shape (double *y, int size, double * result);


#endif //SAMARITAN_TOOLKIT_H