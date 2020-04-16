/* Original code by Ted Unangst:
 * https://flak.tedunangst.com/post/sct-set-color-temperature
 * Additions by Steven Biele
 * (location and time based setting) */
/* public domain, do as you wish */
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


/* cribbed from redshift, but truncated with 500K steps */
static const struct { float r; float g; float b; } whitepoints[] = {
	{ 1.00000000,  0.18172716,  0.00000000, }, /* 1000K */
	{ 1.00000000,  0.42322816,  0.00000000, },
	{ 1.00000000,  0.54360078,  0.08679949, },
	{ 1.00000000,  0.64373109,  0.28819679, },
	{ 1.00000000,  0.71976951,  0.42860152, },
	{ 1.00000000,  0.77987699,  0.54642268, },
	{ 1.00000000,  0.82854786,  0.64816570, },
	{ 1.00000000,  0.86860704,  0.73688797, },
	{ 1.00000000,  0.90198230,  0.81465502, },
	{ 1.00000000,  0.93853986,  0.88130458, },
	{ 1.00000000,  0.97107439,  0.94305985, },
	{ 1.00000000,  1.00000000,  1.00000000, }, /* 6500K */
	{ 0.95160805,  0.96983355,  1.00000000, },
	{ 0.91194747,  0.94470005,  1.00000000, },
	{ 0.87906581,  0.92357340,  1.00000000, },
	{ 0.85139976,  0.90559011,  1.00000000, },
	{ 0.82782969,  0.89011714,  1.00000000, },
	{ 0.80753191,  0.87667891,  1.00000000, },
	{ 0.78988728,  0.86491137,  1.00000000, }, /* 10000K */
	{ 0.77442176,  0.85453121,  1.00000000, },
};

int interval;

int set_gamma(int temp, Display *dpy)
{
	int screen = DefaultScreen(dpy);
	Window root = RootWindow(dpy, screen);

	XRRScreenResources *res = XRRGetScreenResourcesCurrent(dpy, root);

	if (temp < 1000 || temp > 10000)
		temp = 6500;

	temp -= 1000;
	double ratio = temp % 500 / 500.0;
#define AVG(c) whitepoints[temp / 500].c * (1 - ratio) + whitepoints[temp / 500 + 1].c * ratio
	double gammar = AVG(r);
	double gammag = AVG(g);
	double gammab = AVG(b);

	int num_crtcs = res->ncrtc;
	for (int c = 0; c < res->ncrtc; c++) {
		int crtcxid = res->crtcs[c];
		XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(dpy, res, crtcxid);

		int size = XRRGetCrtcGammaSize(dpy, crtcxid);

		XRRCrtcGamma *crtc_gamma = XRRAllocGamma(size);

		for (int i = 0; i < size; i++) {
			double g = 65535.0 * i / size;
			crtc_gamma->red[i] = g * gammar;
			crtc_gamma->green[i] = g * gammag;
			crtc_gamma->blue[i] = g * gammab;
		}
		XRRSetCrtcGamma(dpy, crtcxid, crtc_gamma);

		XFree(crtc_gamma);
	}
}

int calculate_temp()
{
    int ctemp;

    time_t now = time(NULL);
    struct tm *now2 = localtime(&now);
    char now3[5];
    strftime(now3, 5, "%H%M", now2);
    int minutes = now2->tm_hour * 60 + now2->tm_min;

    switch(minutes) {
        //sunrise
        case 300 ... 390:
            ctemp = 2540 + 44 * ( minutes - 300 );
            interval = 120;
            break;
        //day
        case 391 ... 1169:
            ctemp = 6500;
            interval = 600;
            break;
        //sunset
        case 1170 ... 1290:
            ctemp = 6500 - 33 * ( minutes - 1170 );
            interval = 120;
            break;
        //night
        default:
            ctemp = 2540;
            interval = 600;
            break;
    }

    //printf("Current time is %s.\n", now3);
    //printf("Current minutes are %d.\n", minutes);
    //printf("Calculated temperature is %d.\n", ctemp);
    //printf("Interval is %d.\n", interval);

    return ctemp;
}

int main(int argc, char **argv)
{
	Display *dpy = XOpenDisplay(NULL);

	int temp = 6500;
    interval = 120;
	if (argc == 2) {
		temp = atoi(argv[1]);
        set_gamma(temp, dpy);
    }
    else {
        while (True) {
            temp = calculate_temp();
            set_gamma(temp, dpy);
            sleep(interval);
        }
    }

    XCloseDisplay(dpy);
}

