#include "allegro5\allegro.h"
#include "allegro5\allegro_primitives.h"
#include <stdio.h>
#include <math.h>

typedef struct
{
	double lStart;
	double lEnd;
	double lConstant;
	double Xo;
	double Yo;
}info_t;

#define PI	3.14159265358979323846
#define ANCHO	900
#define ALTO	900
#define ERROR	-1
#define NO_ERROR	1
#define WARNING	0
#define REPETICIONES ceil(log(info.lEnd/info.lStart)/log(info.lConstant)) //numero de veces que se debe multiplicar a lStart por lConstant para que alcanze a lEnd
#define REP_MAX	8 //maximo de repeticiones permitidas para un tiempo razonable de espera


void draw_octogonal_fractal(double lStart, double lEnd, double lConstant, double Xo, double Yo, ALLEGRO_COLOR color);
void draw_octogon(double Xo, double Yo, double lado, ALLEGRO_COLOR color);
int validacion_fractal_octogonal(info_t info);

int main(int argc, char** argv)
{
	ALLEGRO_EVENT_QUEUE * ev_manager = NULL;
	ALLEGRO_DISPLAY * display = NULL;

	info_t info = { 300,100,0.5,450,450 };


	if (!al_init())
	{
		printf("Fallo al inicializar allegro\n");
		exit(-1);
	}

	if (!al_init_primitives_addon())
	{
		printf("Fallo al inicializar primitivas\n");
		exit(-1);
	}

	ev_manager = al_create_event_queue();
	al_set_app_name("Fractal");
	display = al_create_display(ANCHO, ALTO);

	if (!ev_manager)
	{
		al_destroy_display(display);
		al_destroy_event_queue(ev_manager);
	}

	if (!display)
	{
		al_destroy_display(display);
		al_destroy_event_queue(ev_manager);
	}

	ALLEGRO_EVENT ev;
	al_register_event_source(ev_manager, al_get_display_event_source(display));

	ALLEGRO_COLOR rojo = al_map_rgb(255, 0, 0);

	printf("%d", validacion_fractal_octogonal(info));
	draw_octogonal_fractal(300, 10, 0.5, 450, 450, rojo);

	al_flip_display();
	al_rest(5);

	//destuyo todo
	al_destroy_display(display);
	al_destroy_event_queue(ev_manager);

	return 0;
}


void draw_octogonal_fractal(double lStart, double lEnd, double lConstant, double Xo, double Yo, ALLEGRO_COLOR color)
{
	double radius = (lStart) / (2 * sin(PI / 8));
	double offset = radius*(1 / sqrt(2));
	if(lStart<lEnd)
	{
		return;
	}
	else
	{
		draw_octogon(Xo,Yo,radius,color); //dibujo un octagono de centro (Xo,Yo)
		draw_octogonal_fractal(lStart*lConstant,lEnd,lConstant, Xo + radius, Yo, color); //llamo a la funcion  recursiva en los 8 vertices del octagono
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo + offset, Yo + offset, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo, Yo + radius, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo - offset, Yo + offset, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo - radius, Yo, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo - offset, Yo - offset, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo, Yo - radius, color);
		draw_octogonal_fractal(lStart*lConstant, lEnd, lConstant, Xo + offset, Yo - offset, color);
	}

}

void draw_octogon(double Xo, double Yo, double radius, ALLEGRO_COLOR color)
{
	double offset = radius*(1 / sqrt(2));
	al_draw_line(Xo + radius, Yo, Xo + offset, Yo + offset, color, 2);
	al_draw_line(Xo + offset, Yo + offset, Xo, Yo + radius, color, 2);
	al_draw_line(Xo, Yo + radius, Xo - offset, Yo + offset, color, 2);
	al_draw_line(Xo - radius, Yo, Xo - offset, Yo + offset, color, 2);
	al_draw_line(Xo - radius, Yo, Xo - offset, Yo - offset, color, 2);
	al_draw_line(Xo, Yo - radius, Xo - offset, Yo - offset, color, 2);
	al_draw_line(Xo, Yo - radius, Xo + offset, Yo - offset, color, 2);
	al_draw_line(Xo + radius, Yo, Xo + offset, Yo - offset, color, 2);
}

int validacion_fractal_octogonal(info_t info)
{
	int status = NO_ERROR;
	if ((info.lConstant < 0) || (info.lEnd < 0) || (info.lStart < 0) || (info.Xo < 0) || (info.Yo < 0))
	{
		status = ERROR; //no se aceptan valores negativos de ningun tipo
	}
	else if ((info.Xo > ANCHO) || (info.Yo > ALTO))
	{
		status = ERROR; //centro se encuentra fuera del display
	}
	else if (info.lConstant > 1)
	{
		status = ERROR; //lConstant esta entre cero y uno
	}
	else if (info.lStart < info.lEnd)
	{
		status = ERROR; //lStart debe ser mayor a lEnd
	}
	else if (REPETICIONES > REP_MAX)
	{
		status = WARNING; 
	/*	
	    No hay error, pero con estos parametros el programa puede colgar en la recursion.
	    Numero de repeticiones mayores a 8 hacen que el arbol de recursion sea muy grande 
	    y tarde mucho en procesarse el fractal
	*/
	}
	return status;
}