#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rk4.h"
#include "winbgi2.h"
#define l 0.5
#define g 9.81
#define omega0 0.5
#define alfa0 0.
#define m 70
void rhs_fun(double t,double *X, double *F);
void veuler(double t,double *X, double h, int n,double *F, void (*fun)(double,double*,double*), double *X1);
void energia(double *X,double *Ec, double *Ep, double *Ek);

void main()
{
	int n;
	double h;
	printf("podaj dlugosc kroku calkowania:");
	scanf("%lf",&h);
	double Ec,Ep,Ek;
	printf("Jaka jest ilosc rownan rozniczkowych w ukladzie?:\n");
	scanf("%d",&n);
	double* X;
	double* F;
	double* X1;
	X=(double *)malloc(n*sizeof(*X));
	F=(double *)malloc(n*sizeof(*F));
	X1=(double *)malloc(n*sizeof(*X1));
	int metoda;
	printf("Jaka metoda rozwiazac uklad rownan?\nEulera-0\nRK4-1\n");
	scanf("%d",&metoda);
	graphics(800, 600);
	scale(-8, -1, 8, 1);
	title("alfa[°]","omega[rad/s]","wykres fazowy");
	X[0]=omega0;
	X[1]=alfa0/180.*acos(-1.);
	for(double t=0.;t<=10.;t+=h)
	{
		setcolor(0.5);
		point(X[1]*180/acos(-1.),X[0]);
		if(metoda==0)
		{
			veuler(t,X,h,n,F,rhs_fun,X1);
		}else vrk4(t,X,h,n,rhs_fun,X1);
		X[0]=X1[0];
		X[1]=X1[1];
	}
	printf("Wpisz dowolny symbol aby wyswietlic wykres rozkladu Energii");
	scanf("%d",&Ec);
	setgray(1.);
	X[0]=omega0;
	X[1]=alfa0;
	scale(0, 0, 5,5);
	title("czas[s]","Energia[J]","Ek-niebieski,Ep-zielony,Ec-czerwony");
	for(double t=0.;t<=10.;t+=h)
	{
		if(metoda==0)
		{
			veuler(t,X,h,n,F,rhs_fun,X1);
		}else vrk4(t,X,h,n,rhs_fun,X1);
		energia(X,&Ec,&Ep,&Ek);
		setcolor(1.);
		point(t,Ec);
		setcolor(0.5);
		point(t,Ep);
		setcolor(0.);
		point(t,Ek);
		
		X[0]=X1[0];
		X[1]=X1[1];
	}

	free(X);
	free(F);
	free(X1);
	wait();
}
void rhs_fun(double t,double *X, double *F)
{
	F[0] = -g / l*sin(X[1]);
	F[1] = X[0];
}
void veuler(double t,double *X, double h, int n,double *F, void (*fun)(double,double*,double*), double *X1)
{
	fun(t,X,F);
	for(int i=0;i<n;i++)
	{
	X1[i]=X[i]+h*F[i];
	}
}
void energia(double *X,double *Ec, double *Ep, double *Ek)
{
	*Ep=m*g*l*(1-cos(X[1]));
	*Ek=m*pow(l,2)/2*pow(X[0],2);
	*Ec=*Ep+*Ek;
}