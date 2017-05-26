#include "toolkit.h"

int next_int (char * st, int st_len, int pos)
{
    int i;
    int re = 0;
    int flag = 0;
    for(i = pos; i < st_len; i++ )
    {
        if(st[i]>'0'&&st[i]<'9')
        {
            flag = 1;
            re = re * 10;
            re += st[i] - '0';
        }
        else if(flag)
        {
            return re;
        }
        else if(st[i] == 0)
            break;
    }
    print_error(flag == 0, 1, "can't get next int in WFG ini");
    return re;
}
void WFG_ini ()
{
    wfg_K = next_int(problem_param_stream,200,0);

    wfg_w = malloc(sizeof(double)*number_variable+number_objective);
    temp = malloc(sizeof(double)*number_objective);
    wfg_temp = malloc(sizeof(double)*(number_variable+number_objective));
}
void WFG_free ()
{
    free(wfg_temp);
    free(temp);
    free(wfg_w);
}
void WFG_normalise (double* z, int z_size ,double* result)
{
    int i;
    for( i = 0; i < z_size; i++ )
    {
        double bound = 2.0*( i+1 );
        result[i] =  z[i] / bound ;
    }


}

void calculate_x(double * x, double * result, int size)
{
    int i;
    double val = x[size-1];
    if ( !Degenerate && val < 1 )
        val = 1;

    result[0] = x[0];
    result[size-1] = result[size -1];

    for (i = 1; i< size-1; i++)
    {
        result[i] = (x[i]-0.5)*val+0.5;
    }
}
void calculate_f (double D, double x, double* h, int size, double *result)
{

    int i;
    int S = 1;
    for( i = 0; i < size; i++ )
    {
        //S = S *2;
        result[i] = ( D*x + S*h[i] );
    }
}
/* shape function */

double linear(double* x, int M, int m )
{

    int i;
    double result = 1.0;

    for( i=1; i <= M-m; i++ )
    {
        result *= x[i-1];
    }

    if( m != 1 )
    {
        result *= 1 - x[M-m];
    }
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result ;
}

double convex(double *x, int x_size, int m )
{
    int i;
    double result = 1.0;

    for( i=1; i <= x_size-m; i++ )
    {
        result *= 1.0 - cos( x[i-1]*PI/2.0 );
    }

    if( m != 1 )
    {
        result *= 1.0 - sin( x[x_size-m]*PI/2.0 );
    }
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result ;
}

double concave (double *x, int x_size, int m )
{
    int i;
    double result = 1.0;

    for( i=1; i <= x_size-m; i++ )
    {
        result *= sin( x[i-1]*PI/2.0 );
    }

    if( m != 1 )
    {
        result *= cos( x[x_size-m]*PI/2.0 );
    }
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result ;

}

double mixed (double *x, int A, double alpha)
{

    double tmp = 2.0*A*PI;
    double result = pow( 1.0-x[0]-cos( tmp*x[0] + PI/2.0 )/tmp, alpha );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;

}

double disc (double * x, int A, double alpha, double beta)
{

    double tmp1 = A*pow( x[0], beta )*PI;
    double result = 1.0 - pow( x[0], alpha )*pow( cos( tmp1 ), 2.0 );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

/* transform fuction */


double b_poly (double y, double alpha )
{
    double result = pow( y, alpha );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;

}
double min (double a, double b)
{
    if(a>b) return b;
    return a;
}
double b_flat (double y, double A,double B, double C)
{
    double tmp1 = min( 0.0, floor( y-B ) ) * A*( B-y )/B;
    double tmp2 = min( 0.0, floor( C-y ) ) * ( 1.0-A )*( y-C )/( 1.0-C );
    double result = A+tmp1-tmp2;
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;

}

double b_param (double y, double u, double A, double B, double C)
{

    double v = A - ( 1.0-2.0*u )*fabs( floor( 0.5-u )+A );
    double result =  pow( y, B + ( C-B )*v );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

double s_linear ( double y,  double A )
{
    double result = fabs( y-A )/fabs( floor( A-y )+A );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

double s_decept (double y, double A, double B, double C)
{

    double tmp1 = floor( y-A+B )*( 1.0-C+( A-B )/B )/( A-B );
    double tmp2 = floor( A+B-y )*( 1.0-C+( 1.0-A-B )/B )/( 1.0-A-B );
    double  result = 1.0 + ( fabs( y-A )-B )*( tmp1 + tmp2 + 1.0/B );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

double s_multi (double y, int A, double B, double C)
{


    double tmp1 = fabs( y-C )/( 2.0*( floor( C-y )+C ) );
    double tmp2 = ( 4.0*A+2.0 )*PI*( 0.5-tmp1 );
    double result = ( 1.0 + cos( tmp2 ) + 4.0*B*pow( tmp1, 2.0 ) )/( B+2.0 );
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

double r_sum (double * y, int y_size, double *w, int w_size)
{
    int i;
    double numerator   = 0.0;
    double denominator = 0.0;

    for( i = 0; i <  y_size; i++ )
    {
        numerator   += w[i]*y[i];
        denominator += w[i];
    }
    double result = numerator / denominator;
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}

double r_nonsep ( double *y,int y_size, const int A )
{

    double numerator = 0.0;
    int j,k;
    for( j = 0; j < y_size; j++ )
    {
        numerator += y[j];

        for( k = 0; k <= A-2; k++ )
        {
            numerator += fabs( y[j] - y[( j+k+1 ) % y_size] );
        }
    }

    const double tmp = ceil( A/2.0 );
    const double denominator = y_size*tmp*( 1.0 + 2.0*A - 2.0*tmp )/A;
    double result = numerator / denominator;
    if( result >1 ) result = 1;
    if( result < 0) result = 0;
    return  result;
}
/* Transform functions set */

int WFG1_t1 (double* y,int y_size, int k, double * result)
{
    int i;
    for( i = 0; i < k; i++ )
    {
        result[i] = y [i];
    }

    for( i = k; i < y_size; i++ )
    {
        result[i]= s_linear( y[i], 0.35 );
    }
    return  y_size;

}

int WFG1_t2 (double* y, int y_size, int k, double * result)
{

    int i;

    for( i = 0; i < k; i++ )
    {
        result[i] = y[i];
    }

    for( i = k; i < y_size; i++ )
    {
        result[i] = b_flat( y[i], 0.8, 0.75, 0.85  );
    }
    return y_size;
}

int WFG1_t3 ( double* y, int y_size, double *result)
{

    int i;

    for( i = 0; i < y_size; i++ )
    {
        result[i]=b_poly( y[i], 0.02 );
    }
    return y_size;
}

int WFG1_t4(double *y,int y_size,int k,int M,double *result)
{


    int i;

    for( i = 1; i <=y_size; i++ )
    {
        wfg_w[i-1] = 2.0*i;
    }

    for( i = 1; i <= M-1; i++ )
    {
        int head = ( i-1 )*k/( M-1 );
        int tail = i*k/( M-1 );
        temp[i-1]=r_sum( y+head,tail -head, wfg_w+head,tail -head);
    }

    temp[M-1] = r_sum( y+k,y_size-k, wfg_w+k,y_size-k);
    for( i = 0; i < M ; i++ )
        result[i] = temp[i];
    return M;
}

int WFG2_t2 (double * y, int y_size, int k,double * result)
{
    const int l = y_size-k;
    int i;

    for( i = 0; i < k; i++ )
    {
        result[i] = y[i];
    }

    for( i = k+1; i <= k+l/2; i++ )
    {
        const int head = k+2*( i-k )-2;
        const int tail = k+2*( i-k );

        result[i] = r_nonsep(y+head, tail-head , 2 );
    }

return k+l/2+1;
}

int WFG2_t3 (double *y, int y_size,int k,int M,double *result)
{

    int i;
    for( i = 1; i <=y_size; i++ )
    {
        wfg_w[i-1] = 1.0;
    }

    for( i = 1; i <= M-1; i++ )
    {
        const int head = ( i-1 )*k/( M-1 );
        const int tail = i*k/( M-1 );
        result[i-1] = r_sum( y+head,tail-head, wfg_w+head,tail-head) ;
    }



    result[M-1] = r_sum( y+k,y_size-k,wfg_w+k,y_size-k );

    return M;
}

int WFG4_t1 (double * y, int y_size,double * result)
{
    int i;

    for( i = 0; i < y_size; i++ )
    {
        result[i] = s_multi( y[i], 30, 10, 0.35 );
    }


}

void WFG1_shape (double *y, int size, double * result)
{
    int i;
    calculate_x(y,temp,size);

    for(i = 1;i <= size-1; i++)
        result[i-1] = convex( temp ,size, i );

    result[size-1] = mixed( temp, 5, 1.0 );

    calculate_f(1.0,temp[size-1],result,size,result);
}

void WFG2_shape (double *y, int size, double * result)
{
    int i;
    calculate_x(y,temp,size);

    for(i = 1;i <= size-1; i++)
        result[i-1] = convex( temp ,size, i );

    result[size-1] = disc( temp, 5, 1.0, 1.0 ) ;

    calculate_f(1.0,temp[size-1],result,size,result);
}


void WFG3_shape( double *y, int y_size ,double * result)
{

    int m;

    calculate_x( y,temp, y_size);

    for( m = 1; m <= y_size; m++ )
    {
        result[m-1] =  linear( temp, y_size,m );
    }

    calculate_f(1.0,temp[y_size-1],result,y_size,result);
}

void WFG4_shape( double *y, int y_size ,double * result )
{
    int m;
    calculate_x( y,temp, y_size);
    for( m = 1; m <= y_size; m++ )
    {
        result[m-1] =  concave( temp, y_size,m );
    }

    calculate_f(1.0,temp[y_size-1],result,y_size,result);
}