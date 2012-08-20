/* hdate.c
 * Example file for libhdate. 
 * 
 * compile:
 * gcc `pkg-config --libs --cflags libhdate` hdate.c -o hdate
 * 
 * Copyright:  2004 (c) Yaacov Zamir 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>				/* For printf */
#include <hdate.h>				/* For hebrew date */
#include <stdlib.h>				/* For atoi */
#include <locale.h>				/* For setlocale */
#include <unistd.h>				/* For getopt */

#define FALSE 0
#define TRUE -1

/* FIXME: global var - ugly ! */
int iCal_uid_counter = 0;

/* some useful time zones
 * Eilat 29, -34, 2
 * Haifa 32, -34, 2
 * Jerusalem 31, -35, 2
 * Tel Aviv 32, -34, 2
 */

/* print help */
int
print_help (char *program)
{
	printf ("hdate - example program for libhdate\n\n");

	printf ("USAGE: %s [-sShdr]\n", program);
	printf ("   [-L longitude -l latitude]\n");
	printf ("   [-z timezone]\n");
	printf ("   [[[day] month] year]/[julian_day]\n");

	printf ("OPTIONS:\n");
	printf ("   -s : Print sunrise/sunset times.\n");
	printf ("   -c : Print Shabat enter/leave times.\n");
	printf ("        Shabat starts 20 min before sunset,\n");
	printf ("        and exits when three stars are out.\n");
	printf ("   -t : Print day times -\n");
	printf ("        first light, talit, sunrise, midday, sunset,\n");
	printf ("        first stars, three stars.\n");
	printf ("   -S : Print using short format.\n");
	printf ("   -h : Print holidays.\n");
	printf ("   -H : Print just holidays.\n");
	printf ("   -r : Print weekly reading on saturday.\n");
	printf ("   -R : Print just weekly reading on saturday.\n");
	printf ("   -o : Print Sefirat Haomer.\n");
	printf ("   -j : Print Julian day number.\n");

	printf ("   -d : Use diaspora reading and holidays.\n");
	printf ("   -i : Use iCal formated output.\n");
	printf ("   -l xx : Set the latitude for solar calculations to\n");
	printf ("              xx degrees.  Negative values are south.\n");
	printf ("   -L xx : Set the longitude for solar calculations to\n");
	printf ("              xx degrees.  *Negative values are west*.\n");
	printf
		("        The -l and -L switches must both be used, or not at all.\n");
	printf ("   -z : Use specified timezone,\n");

	printf
		("   ( default location for sunrise/set is Tel Aviv winter time ).\n");
	printf ("   some useful locations and time zones:\n");
	printf ("      Eilat : 29, 34, 2\n");
	printf ("      Haifa : 32, 34, 2\n");
	printf ("      Jerusalem : 31, 35, 2\n");
	printf ("      Tel Aviv : 32, 34, 2\n");
	printf ("      Ashdod : 31, 34, 2\n");
	printf ("      Beer Sheva : 31, 34, 2\n");
	printf ("      Tiberias : 32, 35, 2\n");
	printf ("      London : 51, 0, 0\n");
	printf ("      Paris : 48, 2, 1\n");
	printf ("      New York : 40, -74, -5\n");
	printf ("      Moscow : 55, 37, 3\n");

	return 0;
}

/* print ical header */
int
print_ical_header ()
{
	/* Print start of iCal format */
	printf ("BEGIN:VCALENDAR\n");
	printf ("VERSION:2.0\n");
	printf ("CALSCALE:GREGORIAN\n");
	printf ("METHOD:PUBLISH\n");

	return 0;
}

/* print ical footer */
int
print_ical_footer ()
{
	/* Print end of iCal format */
	printf ("END:VCALENDAR\n");

	return 0;
}

/* print one day - gregorian and hebrew date */
int
print_date (hdate_struct * h, int opt_S, int opt_i)
{
	char *locale;
	char *language;
	char *bet;

	/* Get the name of the current locale.  */
#ifdef ENABLE_NLS
	locale = setlocale (LC_MESSAGES, NULL);
	language = getenv ("LANGUAGE");
#else
	locale = NULL;
	language = NULL;
#endif
	/* if hebrew add bet to the month */
	if ((locale && (locale[0] == 'h') && (locale[1] == 'e')) ||
		  (language && (language[0] == 'h') && (language[1] == 'e')))
	{
		bet="ב";
	}
	else
	{
		bet="";
	}
	
	if (opt_i)
	{
		printf ("%s %s%s ",
				hdate_get_int_string (h->hd_day),
				bet,
				hdate_get_hebrew_month_string (h->hd_mon, opt_S));
		printf ("%s", hdate_get_int_string (h->hd_year));
	}
	else if (opt_S)
	{
		printf ("%d.%d.%d ", h->gd_day, h->gd_mon, h->gd_year);

		/* check for hebrew locale */
		if (!((locale && (locale[0] == 'h') && (locale[1] == 'e')) || 
			  (language && (language[0] == 'h') && (language[1] == 'e'))))
		{						/* non hebrew numbers */

			printf ("%d", h->hd_day);

			/* FIXME: this only warks for english :-( */

			/* check for english locale */
			if ((!locale && !language) ||
				(language && (language[0] == 'C')) ||
				(!language && locale && (locale[0] == 'C')) ||
				(language && (language[0] == 'e') && (language[1] == 'n'))
				|| (!language && locale && (locale[0] == 'e')
					&& (locale[1] == 'n')))
			{
				switch (h->hd_day)
				{
					case 1:
					case 21:
					case 31:
						printf ("st of");
						break;
					case 2:
					case 22:
						printf ("nd of");
						break;
					case 3:
					case 23:
						printf ("rd of");
						break;
					default:
						printf ("th of");
				}
			}

		}
		else
		{						/* hebrew numbers */
			printf ("%s", hdate_get_int_string (h->hd_day));
		}

		printf (" %s%s, ", bet, hdate_get_hebrew_month_string (h->hd_mon, opt_S));
		printf ("%s\n", hdate_get_int_string (h->hd_year));
	}
	else
	{
		printf ("%s, %d %s%s %d, ",
				hdate_get_day_string (h->hd_dw, opt_S),
				h->gd_day,
				bet,
				hdate_get_month_string (h->gd_mon, opt_S), h->gd_year);
		printf ("%s %s%s ",
				hdate_get_int_string (h->hd_day),
				bet,
				hdate_get_hebrew_month_string (h->hd_mon, opt_S));
		printf ("%s\n", hdate_get_int_string (h->hd_year));
	}

	return 0;
}

/* print one day - sunrise/set times */
int
print_sunrise (hdate_struct * h, double lat, double lon, int tz, int opt_i)
{
	int sunrise, sunset;

	/* get times */
	hdate_get_utc_sun_time (h->gd_day, h->gd_mon, h->gd_year, lat, lon,
							&sunrise, &sunset);

	sunset = sunset + tz * 60;
	sunrise = sunrise + tz * 60;

	/* print sunset/rise times */
	printf ("%02d:%02d - %02d:%02d ",
			sunrise / 60, sunrise % 60, sunset / 60, sunset % 60);

	return 0;
}

/* print one day - day times */
int
print_times (hdate_struct * h, double lat, double lon, int tz, int opt_i)
{
	int sun_hour, first_light, talit, sunrise;
	int midday, sunset, first_stars, three_stars;

	/* get times */
	hdate_get_utc_sun_time_full (h->gd_day, h->gd_mon, h->gd_year, lat, lon,
								 &sun_hour, &first_light, &talit, &sunrise,
								 &midday, &sunset, &first_stars, &three_stars);

	first_light = first_light + tz * 60;
	talit = talit + tz * 60;
	sunrise = sunrise + tz * 60;
	midday = midday + tz * 60;
	sunset = sunset + tz * 60;
	first_stars = first_stars + tz * 60;
	three_stars = three_stars + tz * 60;

	/* print sunset/rise times */	
	printf ("first light: %02d:%02d\ntalit: %02d:%02d\nsun rise: %02d:%02d\nmid day: %02d:%02d\n",
			first_light / 60, first_light % 60, talit / 60, talit % 60,
			sunrise / 60, sunrise % 60, midday / 60, midday % 60);
	printf ("sun set: %02d:%02d\nfirst stars: %02d:%02d\nthree stars: %02d:%02d\n",
			sunset / 60, sunset % 60, first_stars / 60, first_stars % 60,
			three_stars / 60, three_stars % 60);

	return 0;
}

/* print one day - holiday */
int
print_holiday (hdate_struct * h, int opt_d, int opt_S, int opt_i)
{
	int holyday;

	holyday = hdate_get_holyday (h, opt_d);

	if (holyday)
	{
		/* print holyday */
		printf ("%s", hdate_get_holyday_string (holyday, opt_S));
	}

	return 0;
}

/* print day in the omer */
int
print_omer (hdate_struct * h)
{
	int omer_day;
	
	omer_day = hdate_get_omer_day(h);
	
	if (omer_day != 0)
		printf ("%s", hdate_get_omer_string (omer_day));
	
	return 0;
}

/* print one day - reading */
int
print_reading (hdate_struct * h, int opt_d, int opt_S, int opt_i)
{
	int reading;

	reading = hdate_get_parasha (h, opt_d);

	if (reading)
	{
		/* print parash */
		printf ("%s", hdate_get_parasha_string (reading, opt_S));
	}

	return 0;
}

/* print one day - reading */
int
print_candales (hdate_struct * h, double lat, double lon, int tz, int opt_i)
{
	int sun_hour, first_light, talit, sunrise;
	int midday, sunset, first_stars, three_stars;

	/* check for friday - print knisat shabat */
	if (h->hd_dw == 6)
	{
		/* get times */
		hdate_get_utc_sun_time (h->gd_day, h->gd_mon, h->gd_year, lat, lon,
								&sunrise, &sunset);

		/* FIXME - knisar shabat 20 minutes before shkiaa ? */
		sunset = sunset + tz * 60 - 20;

		/* print sunset/rise times */
		printf ("(%d:%d)", sunset / 60, sunset % 60);
	}

	/* check for saturday - print motzay shabat */
	else if (h->hd_dw == 7)
	{
		/* get times */
		hdate_get_utc_sun_time_full (h->gd_day, h->gd_mon, h->gd_year, lat,
									 lon, &sun_hour, &first_light, &talit,
									 &sunrise, &midday, &sunset,
									 &first_stars, &three_stars);

		three_stars = three_stars + tz * 60;

		/* print motzay shabat */
		printf ("(%d:%d)", three_stars / 60, three_stars % 60);
	}

	return 0;
}

/* print one day - all */
int
print_day (hdate_struct * h,
		   int opt_d, int opt_S,
		   double lat, double lon, int tz, int opt_s, int opt_h, int opt_o, int opt_r,
		   int opt_R, int opt_j, int opt_H, int opt_i, int opt_c, int opt_t)
{
	char seperator[5];
	
	/* iCal format require \ before comma */
	if (opt_i) 
		snprintf(seperator, 5, "\\, ");
	else
		snprintf(seperator, 5, ", ");
	
	/* check for just parasha or holiday flag */
	if (opt_R && opt_H &&
		!hdate_get_parasha (h, opt_d) && !hdate_get_holyday (h, opt_d))
		return 0;
	if (opt_R && !opt_H && !hdate_get_parasha (h, opt_d))
		return 0;
	if (opt_H && !opt_R && !hdate_get_holyday (h, opt_d))
		return 0;

	/* check for iCal format */
	if (opt_i)
	{
		/* FIXME: older versions of mozilla calendar and evulotion 
		 * needed this hack */
		/* print_ical_header (); */

		printf ("BEGIN:VEVENT\n");
		printf ("UID:%d\n", ++iCal_uid_counter);
		printf ("DTSTART;VALUE=DATE:%04d%02d%02d\n", h->gd_year,
				h->gd_mon, h->gd_day);
		printf ("SUMMARY:");
	}

	/* print Julian day number */
	if (opt_j)
	{
		printf ("JDN-%d ", h->hd_jd);
	}

	/* print the day */
	print_date (h, opt_S, opt_i);
	
	if (opt_s)
	{
		printf (seperator);
		
		print_sunrise (h, lat, lon, tz, opt_i);
	}
	if (opt_t)
	{
		printf (seperator);
		
		print_times (h, lat, lon, tz, opt_i);
	}
	if (opt_h && hdate_get_holyday (h, opt_d) != 0)
	{
		printf (seperator);
		
		print_holiday (h, opt_d, opt_S, opt_i);
	}
	if (opt_o && hdate_get_omer_day(h) != 0)
	{
		printf (seperator);
		
		print_omer (h);
	}
	if (opt_r && hdate_get_parasha (h, opt_d) != 0)
	{
		printf (seperator);
		
		print_reading (h, opt_d, opt_S, opt_i);
	}
	if (opt_c)
	{
		printf (seperator);
		
		print_candales (h, lat, lon, tz, opt_i);
	}

	if (opt_r || opt_h || opt_s || opt_t || opt_i || opt_c || opt_o)
	{
		printf ("\n");
	}

	/* check for iCal format */
	if (opt_i)
	{
		printf ("CLASS:PUBLIC\n");
		printf ("DTEND;VALUE=DATE:%04d%02d%02d\n", h->gd_year,
				h->gd_mon, h->gd_day);
		printf ("CATEGORIES:Holidays\n");
		printf ("END:VEVENT\n");
		/* FIXME: older versions of mozilla calendar and evulotion 
		 * needed this hack */
		/* print_ical_footer (); */
	}

	return 0;
}

/* print one month - all */
int
print_month (int opt_d, int opt_S,
			 double lat, double lon, int tz,
			 int opt_s, int opt_h, int opt_o, int opt_r, int opt_R, int opt_j,
			 int opt_H, int opt_i, int opt_c, int opt_t, int month, int year)
{
	hdate_struct h;
	int jd;

	/* get date of month start */
	hdate_set_gdate (&h, 1, month, year);
	jd = h.hd_jd;

	/* print month header */
	if (!opt_i && !opt_S)
		printf ("\n%s:\n", hdate_get_month_string (h.gd_mon, opt_S));

	/* print month days */
	while (h.gd_mon == month)
	{
		print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
				   opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

		jd++;
		hdate_set_jd (&h, jd);
	}

	return 0;
}

/* print one hebrew month - all */
int
print_hebrew_month (int opt_d, int opt_S,
					double lat, double lon, int tz,
					int opt_s, int opt_h, int opt_o, int opt_r, int opt_R, int opt_j,
					int opt_H, int opt_i, int opt_c, int opt_t, int month,
					int year)
{
	hdate_struct h;
	int jd;

	/* get date of month start */
	hdate_set_hdate (&h, 1, month, year);
	jd = h.hd_jd;

	if (h.hd_size_of_year > 365 && month == 6)
	{							/* adar of meoberet */
		/* print Adar I and Adar II month days */
		hdate_set_hdate (&h, 1, 13, year);
		jd = h.hd_jd;

		/* print month header */
		if (!opt_i && !opt_S)
			printf ("\n%s:\n", hdate_get_hebrew_month_string (h.hd_mon, opt_S));

		/* print adar I days */
		while (h.hd_mon == 13)
		{
			print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s,
					   opt_h, opt_o, opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

			jd++;
			hdate_set_jd (&h, jd);
		}

		hdate_set_hdate (&h, 1, 14, year);
		jd = h.hd_jd;

		/* print month header */
		if (!opt_i && !opt_S)
			printf ("\n%s:\n", hdate_get_hebrew_month_string (h.hd_mon, opt_S));

		/* print adar II days */
		while (h.hd_mon == 14)
		{
			print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s,
					   opt_h, opt_o, opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

			jd++;
			hdate_set_jd (&h, jd);
		}
	}
	else
	{
		/* print month header */
		if (!opt_i && !opt_S)
			printf ("\n%s:\n", hdate_get_hebrew_month_string (h.hd_mon, opt_S));

		/* print month days */
		while (h.hd_mon == month)
		{
			print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s,
					   opt_h, opt_o, opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

			jd++;
			hdate_set_jd (&h, jd);
		}
	}

	return 0;
}

/* print one gregorian year - all */
int
print_year (int opt_d, int opt_S,
			double lat, double lon, int tz, int opt_s, int opt_h, int opt_o, int opt_r,
			int opt_R, int opt_j, int opt_H, int opt_i, int opt_c,
			int opt_t, int year)
{
	int month = 1;

	/* print year header */
	if (!opt_i && !opt_S)
		printf ("%d:\n", year);

	/* print year months */
	while (month < 13)
	{
		print_month (opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o, opt_r,
					 opt_R, opt_j, opt_H, opt_i, opt_c, opt_t, month, year);
		month++;
	}

	return 0;
}

/* print one hebrew year - all */
int
print_hebrew_year (int opt_d, int opt_S,
				   double lat, double lon, int tz,
				   int opt_s, int opt_h, int opt_o, int opt_r, int opt_R, int opt_j,
				   int opt_H, int opt_i, int opt_c, int opt_t, int year)
{
	int month = 1;

	/* print year header */
	if (!opt_i && !opt_S)
		printf ("%s:\n", hdate_get_int_string (year));

	/* print year months */
	while (month < 13)
	{
		print_hebrew_month (opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
							opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t,
							month, year);
		month++;
	}

	return 0;
}

int
main (int argc, char *argv[])
{
	hdate_struct h;				/* The Hebrew date */
	int c;

	int day;					/* The Gregorian date */
	int month;
	int year;

	/* hebcal style command line options */
	char *progname = argv[0];
	int opt_s = 0;				/* -s option sunrise/set times */
	int opt_c = 0;				/* -c option shabat enter/leave times */
	int opt_t = 0;				/* -t option print times of day */
	int opt_S = 0;				/* -S Short format flag */
	int opt_h = 0;				/* -h option holidays */
	int opt_H = 0;				/* -H option just holidays */
	int opt_r = 0;				/* -r option reading */
	int opt_R = 0;				/* -R option just reading */
	int opt_j = 0;				/* -j option Julian day number */
	int opt_d = 0;				/* -d option diaspora */
	int opt_i = 0;				/* -i option iCal */
	int opt_o = 0;				/* -o option Sfirat Haomer */

	double lat = 32.0;			/* -l option default to Tel aviv latitude */
	double lon = 34.0;			/* -L option default to Tel aviv longitude */
	int tz = 2;					/* -z option default to Tel aviv time zone */

	/* init locale */
	setlocale (LC_ALL, "");

	/* command line parsing */
	while ((c = getopt (argc, argv, "sctShHorRjdil:L:z:")) != -1)
	{
		switch (c)
		{
		case 's':
			opt_s = 1;
			break;
		case 'c':
			opt_c = 1;
			break;
		case 't':
			opt_t = 1;
			break;
		case 'S':
			opt_S = 1;
			break;
		case 'H':
			opt_H = 1;
		case 'h':
			opt_h = 1;
			break;
		case 'o':
			opt_o = 1;
			break;	
		case 'R':
			opt_R = 1;
		case 'j':
			opt_j = 1;
		case 'r':
			opt_r = 1;
			break;
		case 'd':
			opt_d = 1;
			break;
		case 'i':
			opt_i = 1;
			break;
		case 'l':
			if (optarg)
				lat = (double) atof (optarg);
			break;
		case 'L':
			if (optarg)
				lon = (double) atof (optarg);
			break;
		case 'z':
			if (optarg)
				tz = atoi (optarg);
			break;
		default:
			print_help (argv[0]);
			exit (0);
			break;
		}
	}

	/* Get calendar */
	if (argc == optind)
	{							/* no date entered */
		/* set date for today */
		hdate_set_gdate (&h, 0, 0, 0);

		if (opt_i)
			print_ical_header ();

		print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
				   opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

		if (opt_i)
			print_ical_footer ();

		exit (0);
	}
	else if (argc == (optind + 1))
	{							/* only year or julian day number */
		/* get year */
		year = atoi (argv[optind]);

		if (year <= 0)
		{						/* error */
			print_help (argv[0]);
			exit (0);
		}
		if (year > 100000)
		{						/* julian day number */
			/* year is julian day number */
			hdate_set_jd (&h, year);

			if (opt_i)
				print_ical_header ();

			print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
					   opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

			if (opt_i)
				print_ical_footer ();

			exit (0);
		}
		else if (year > 3000)
		{						/* hebrew year */
			if (opt_i)
				print_ical_header ();

			print_hebrew_year (opt_d, opt_S, lat, lon, tz, opt_s,
							   opt_h, opt_o, opt_r, opt_R, opt_j, opt_H, opt_i,
							   opt_c, opt_t, year);

			if (opt_i)
				print_ical_footer ();

			exit (0);
		}
		else
		{
			if (opt_i)
				print_ical_header ();

			print_year (opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
						opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t, year);

			if (opt_i)
				print_ical_footer ();

			exit (0);
		}
	}
	else if (argc == (optind + 2))
	{							/*only month and year */
		/* get year */
		year = atoi (argv[optind + 1]);
		month = atoi (argv[optind]);

		if (year <= 0)
		{						/* error */
			print_help (argv[0]);
			exit (0);
		}
		if (year > 3000)
		{						/* hebrew year */
			if (opt_i)
				print_ical_header ();

			print_hebrew_month (opt_d, opt_S, lat, lon, tz, opt_s,
								opt_h, opt_o, opt_r, opt_R, opt_j, opt_H, opt_i,
								opt_c, opt_t, month, year);

			if (opt_i)
				print_ical_footer ();

			exit (0);
		}
		else
		{
			if (opt_i)
				print_ical_header ();

			print_month (opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
						 opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t,
						 month, year);

			if (opt_i)
				print_ical_footer ();

			exit (0);
		}
	}
	else if (argc == (optind + 3))
	{							/*day month and year */
		/* get year */
		year = atoi (argv[optind + 2]);
		month = atoi (argv[optind + 1]);
		day = atoi (argv[optind]);

		if (year <= 0)
		{						/* error */
			print_help (argv[0]);
			exit (0);
		}

		/* set date */
		if (year > 3000)
		{						/* hebrew year */
			hdate_set_hdate (&h, day, month, year);
		}
		else
		{
			hdate_set_gdate (&h, day, month, year);
		}

		if (opt_i)
			print_ical_header ();

		print_day (&h, opt_d, opt_S, lat, lon, tz, opt_s, opt_h, opt_o,
				   opt_r, opt_R, opt_j, opt_H, opt_i, opt_c, opt_t);

		if (opt_i)
			print_ical_footer ();

		exit (0);
	}
	else
	{
		print_help (argv[0]);
		exit (0);
	}

	return 0;
}
