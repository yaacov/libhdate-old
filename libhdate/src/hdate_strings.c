/*  libhdate - Hebrew calendar library
 *
 *  Copyright (C) 1984-2003 Amos Shapir, 2004-2007  Yaacov Zamir <kzamir@walla.co.il>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_NLS
#include <locale.h>
#endif

#include "hdate.h"
#include "support.h"

/**
 @brief helper function to find hebrew locale
 
 @return 0 - latin locale, -1 - hebrew locale
*/
int
hdate_is_hebrew_locale()
{
	char *locale;
	char *language;

	/* Get the name of the current locale.  */
#ifdef ENABLE_NLS
	locale = setlocale (LC_MESSAGES, NULL);
	language = getenv ("LANGUAGE");
#else
	locale = NULL;
	language = NULL;
#endif

	if (!((locale && (locale[0] == 'h') && (locale[1] == 'e')) ||
		  (language && (language[0] == 'h') && (language[1] == 'e'))))
	{
		/* not hebrew locale return false */
		return 0;
	}
	
	return -1;
}
/**
 @brief convert an integer to hebrew string UTF-8 (logical)
 
 @param n The int to convert

 @attention ( 0 < n < 10000)
 @warning uses a static string, so output should be copied away.
*/
char *
hdate_get_int_string (int n)
{
	int length;
	static char h_number[100];
	static char *digits[3][10] = {
		{" ", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט"},
		{"ט", "י", "כ", "ל", "מ", "נ", "ס", "ע", "פ", "צ"},
		{" ", "ק", "ר", "ש", "ת"}
	};

	if (!hdate_is_hebrew_locale())
	{
		/* not hebrew locale return the number in decimal form */
		snprintf (h_number, 100, "%d", n);
		return h_number;
	}

	/* sanity checks */
	if (n < 1 || n > 10000)
	{
		return NULL;
	}

	strncpy (h_number, "", 100);

	if (n >= 1000)
	{
		strncat (h_number, digits[0][n / 1000], 100);
		n %= 1000;
	}
	while (n >= 400)
	{
		strncat (h_number, digits[2][4], 100);
		n -= 400;
	}
	if (n >= 100)
	{
		strncat (h_number, digits[2][n / 100], 100);
		n %= 100;
	}
	if (n >= 10)
	{
		if (n == 15 || n == 16)
			n -= 9;
		strncat (h_number, digits[1][n / 10], 100);
		n %= 10;
	}
	if (n > 0)
		strncat (h_number, digits[0][n], 16);

	length = strlen (h_number);

	/* add the ' and " to hebrew numbers */
	if (length <= 2)
	{
		strncat (h_number, "'", 100);
	}
	else
	{
		h_number[length + 1] = h_number[length];
		h_number[length] = h_number[length - 1];
		h_number[length - 1] = h_number[length - 2];
		h_number[length - 2] = '\"';
	}

	return h_number;
}

/**
 @brief Return a static string, with name of wek day.

 @param day The number of the day 0..6 (0 - sun).
 @param s A short flag (true - returns a short string: sun, .., false returns: sunday, .. ).
*/
char *
hdate_get_day_string (int day, int s)
{
	/* create a static days array */
	static char *days[2][7] = {
		{N_("Sun"), N_("Mon"), N_("Tue"), N_("Wed"), N_("Thu"),
		 N_("Fri"), N_("Sat")},
		{N_("Sunday"), N_("Monday"), N_("Tuesday"), N_("Wednesday"),
		 N_("Thursday"), N_("Friday"), N_("Saturday")}
	};

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	/* make sure s is 0 or 1 */
	s = s ? 0 : 1;

	if (day >= 1 && day <= 7)
	{
		return _(days[s][day - 1]);
	}

	/* if not a valid day return NULL */
	return NULL;
}

/**
 @brief Return a static string, with name of month.

 @param month The number of the month 1..12 (1 - jan).
 @param s A short flag.
*/
char *
hdate_get_month_string (int month, int s)
{

	static char *months[2][12] = {
		{N_("Jan"), N_("Feb"), N_("Mar"), N_("Apr"), N_("May"),
		 N_("Jun"), N_("Jul"), N_("Aug"), N_("Sep"), N_("Oct"),
		 N_("Nov"), N_("Dec")},
		{N_("January"), N_("February"), N_("March"),
		 N_("April"), N_("May"), N_("June"),
		 N_("July"), N_("August"), N_("September"),
		 N_("October"), N_("November"), N_("December")}
	};

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	/* make sure s is 0 or 1 */
	s = s ? 0 : 1;

	if (month >= 1 && month <= 12)
	{
		return _(months[s][month - 1]);
	}

	/* if not a valid month return NULL */
	return NULL;
}

/**
 @brief Return a static string, with name of hebrew month.

 @param month The number of the month 1..14 (1 - tishre, 13 - adar 1, 14 - adar 2).
 @param s A short flag.
*/
char *
hdate_get_hebrew_month_string (int month, int s)
{
	/* FIXME: abbrev of Hebrew months */
	static char *months[2][14] = {
		{N_("Tishrei"), N_("Cheshvan"), N_("Kislev"), N_("Tevet"),
		 N_("Sh'vat"), N_("Adar"), N_("Nisan"), N_("Iyyar"),
		 N_("Sivan"), N_("Tamuz"), N_("Av"), N_("Elul"), N_("Adar I"),
		 N_("Adar II")},
		{N_("Tishrei"), N_("Cheshvan"), N_("Kislev"), N_("Tevet"),
		 N_("Sh'vat"), N_("Adar"), N_("Nisan"), N_("Iyyar"),
		 N_("Sivan"), N_("Tamuz"), N_("Av"), N_("Elul"), N_("Adar I"),
		 N_("Adar II")}
	};

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	/* make sure s is 0 or 1 */
	s = s ? 0 : 1;

	if (month >= 1 && month <= 14)
	{
		return _(months[s][month - 1]);
	}

	/* if not a valid month return NULL */
	return NULL;
}

/**
 @brief Name of hebrew holyday.

 @param holyday The holyday number.
 @param s A short flag.
*/
char *
hdate_get_holyday_string (int holyday, int s)
{
	/* holyday strings */
	static char *holydays[2][37] = {
		{
		 N_("Rosh Hashana I"),	/* 1 */
		 N_("Rosh Hashana II"),
		 N_("Tzom Gedaliah"),
		 N_("Yom Kippur"),
		 N_("Sukkot"),
		 N_("Hol hamoed Sukkot"),
		 N_("Hoshana raba"),
		 N_("Simchat Torah"),
		 N_("Chanukah"),
		 N_("Asara B'Tevet"),	/* 10 */
		 N_("Tu B'Shvat"),
		 N_("Ta'anit Esther"),
		 N_("Purim"),
		 N_("Shushan Purim"),
		 N_("Pesach"),
		 N_("Hol hamoed Pesach"),
		 N_("Yom HaAtzma'ut"),
		 N_("Lag B'Omer"),
		 N_("Erev Shavuot"),
		 N_("Shavuot"),			/* 20 */
		 N_("Tzom Tammuz"),
		 N_("Tish'a B'Av"),
		 N_("Tu B'Av"),
		 N_("Yom HaShoah"),
		 N_("Yom HaZikaron"),	/* 25 */
		 N_("Yom Yerushalayim"),
		 N_("Shmini Atzeret"),
		 N_("Pesach VII"),
		 N_("Pesach VIII"),
		 N_("Shavuot II"),   /* 30 */
		 N_("Sukkot II"),
		 N_("Pesach II"),	 
		 N_("Family Day"),
		 N_("Memorial day for fallen whose place of burial is unknown"), 
		 N_("Rabin memorial day"), /* 35 */
		 N_("Zhabotinsky day"),
		 N_("Erev Yom Kippur")},
		{
		 N_("Rosh Hashana I"),
		 N_("Rosh Hashana II"),
		 N_("Tzom Gedaliah"),
		 N_("Yom Kippur"),
		 N_("Sukkot"),
		 N_("Hol hamoed Sukkot"),
		 N_("Hoshana raba"),
		 N_("Simchat Torah"),
		 N_("Chanukah"),
		 N_("Asara B'Tevet"),
		 N_("Tu B'Shvat"),
		 N_("Ta'anit Esther"),
		 N_("Purim"),
		 N_("Shushan Purim"),
		 N_("Pesach"),
		 N_("Hol hamoed Pesach"),
		 N_("Yom HaAtzma'ut"),
		 N_("Lag B'Omer"),
		 N_("Erev Shavuot"),
		 N_("Shavuot"),
		 N_("Tzom Tammuz"),
		 N_("Tish'a B'Av"),
		 N_("Tu B'Av"),
		 N_("Yom HaShoah"),
		 N_("Yom HaZikaron"),
		 N_("Yom Yerushalayim"),
		 N_("Shmini Atzeret"),
		 N_("Pesach VII"),
		 N_("Pesach VIII"),
		 N_("Shavuot II"),
		 N_("Sukkot II"),
		 N_("Pesach II"),
		 N_("Family Day"),
		 N_("Memorial day for fallen whose place of burial is unknown"), 
		 N_("Yitzhak Rabin memorial day"), /* 35 */
		 N_("Zeev Zhabotinsky day"),
		 N_("Erev Yom Kippur")}
	};

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	/* make sure s is 0 or 1 */
	s = s ? 0 : 1;

	if (holyday >= 1 && holyday <= 37)
	{
		return _(holydays[s][holyday - 1]);
	}

	/* if not a valid holyday return NULL */
	return NULL;
}

/**
 @brief Return a static string, with the day in the omer

 @param omer day The day in the omer.
 @return a static string, with the day in the omer
*/
char *
hdate_get_omer_string (int omer_day)
{
	static char omer_string[500];
	
	/* this is not a day in the omer */
	if (omer_day < 1 || omer_day > 49)
		return NULL;
	
	/* create a nice string */
	snprintf(omer_string, 500, "%s %s",
					hdate_get_int_string(omer_day),
					_("in the Omer"));
	
	/* return the string */
	return omer_string;
}

/**
 @brief Name of Parasha

 @param parasha The Number of Parasha 1-Bereshit
	(55 trow 61 are joined strings e.g. Vayakhel Pekudei)
 @param s A short flag.
*/
char *
hdate_get_parasha_string (int parasha, int s)
{
	/* FIXME: abbrev of parasha */
	static char *parashaot[2][62] = {
		{
		 N_("none"),
		 N_("Bereshit"),		/* 1 */
		 N_("Noach"),
		 N_("Lech-Lecha"),
		 N_("Vayera"),
		 N_("Chayei Sara"),
		 N_("Toldot"),
		 N_("Vayetzei"),
		 N_("Vayishlach"),
		 N_("Vayeshev"),
		 N_("Miketz"),			/* 10 */
		 N_("Vayigash"),
		 N_("Vayechi"),
		 N_("Shemot"),
		 N_("Vaera"),
		 N_("Bo"),
		 N_("Beshalach"),
		 N_("Yitro"),
		 N_("Mishpatim"),
		 N_("Terumah"),
		 N_("Tetzaveh"),		/* 20 */
		 N_("Ki Tisa"),
		 N_("Vayakhel"),
		 N_("Pekudei"),
		 N_("Vayikra"),
		 N_("Tzav"),
		 N_("Shmini"),
		 N_("Tazria"),
		 N_("Metzora"),
		 N_("Achrei Mot"),
		 N_("Kedoshim"),		/* 30 */
		 N_("Emor"),
		 N_("Behar"),
		 N_("Bechukotai"),
		 N_("Bamidbar"),
		 N_("Nasso"),
		 N_("Beha'alotcha"),
		 N_("Sh'lach"),
		 N_("Korach"),
		 N_("Chukat"),
		 N_("Balak"),			/* 40 */
		 N_("Pinchas"),
		 N_("Matot"),
		 N_("Masei"),
		 N_("Devarim"),
		 N_("Vaetchanan"),
		 N_("Eikev"),
		 N_("Re'eh"),
		 N_("Shoftim"),
		 N_("Ki Teitzei"),
		 N_("Ki Tavo"),			/* 50 */
		 N_("Nitzavim"),
		 N_("Vayeilech"),
		 N_("Ha'Azinu"),
		 N_("Vezot Habracha"),	/* 54 */
		 N_("Vayakhel-Pekudei"),
		 N_("Tazria-Metzora"),
		 N_("Achrei Mot-Kedoshim"),
		 N_("Behar-Bechukotai"),
		 N_("Chukat-Balak"),
		 N_("Matot-Masei"),
		 N_("Nitzavim-Vayeilech")},
		{
		 N_("none"),
		 N_("Bereshit"),		/* 1 */
		 N_("Noach"),
		 N_("Lech-Lecha"),
		 N_("Vayera"),
		 N_("Chayei Sara"),
		 N_("Toldot"),
		 N_("Vayetzei"),
		 N_("Vayishlach"),
		 N_("Vayeshev"),
		 N_("Miketz"),			/* 10 */
		 N_("Vayigash"),
		 N_("Vayechi"),
		 N_("Shemot"),
		 N_("Vaera"),
		 N_("Bo"),
		 N_("Beshalach"),
		 N_("Yitro"),
		 N_("Mishpatim"),
		 N_("Terumah"),
		 N_("Tetzaveh"),		/* 20 */
		 N_("Ki Tisa"),
		 N_("Vayakhel"),
		 N_("Pekudei"),
		 N_("Vayikra"),
		 N_("Tzav"),
		 N_("Shmini"),
		 N_("Tazria"),
		 N_("Metzora"),
		 N_("Achrei Mot"),
		 N_("Kedoshim"),		/* 30 */
		 N_("Emor"),
		 N_("Behar"),
		 N_("Bechukotai"),
		 N_("Bamidbar"),
		 N_("Nasso"),
		 N_("Beha'alotcha"),
		 N_("Sh'lach"),
		 N_("Korach"),
		 N_("Chukat"),
		 N_("Balak"),			/* 40 */
		 N_("Pinchas"),
		 N_("Matot"),
		 N_("Masei"),
		 N_("Devarim"),
		 N_("Vaetchanan"),
		 N_("Eikev"),
		 N_("Re'eh"),
		 N_("Shoftim"),
		 N_("Ki Teitzei"),
		 N_("Ki Tavo"),			/* 50 */
		 N_("Nitzavim"),
		 N_("Vayeilech"),
		 N_("Ha'Azinu"),
		 N_("Vezot Habracha"),	/* 54 */
		 N_("Vayakhel-Pekudei"),
		 N_("Tazria-Metzora"),
		 N_("Achrei Mot-Kedoshim"),
		 N_("Behar-Bechukotai"),
		 N_("Chukat-Balak"),
		 N_("Matot-Masei"),
		 N_("Nitzavim-Vayeilech")}
	};

#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	/* make sure s is 0 or 1 */
	s = s ? 0 : 1;

	if (parasha >= 1 && parasha <= 61)
	{
		return _(parashaot[s][parasha]);
	}

	/* if not a valid parasha return NULL */
	return NULL;
}

/**
 @brief Return a static string, with the hebrew date.

 return the short ( e.g. "1 Tishrey" ) or 
 long (e.g. "Tuesday 18 Tishrey 5763 Hol hamoed Sukot" ) formated date.

 @param h The hdate_struct of the date to print.
 @param diaspora if true give diaspora holydays
 @param s A short flag (true - returns a short string, false returns a long string. ).
*/
char *
hdate_get_format_date (hdate_struct const *h, int diaspora, int s)
{
	static char format_date[500];
	static char temp[500];
	int holyday;
	int omer_day;
	
	if (h)
	{
		if (s)
		{						
			/* short format */
			/* if hebrew add bet to the month */
			if (hdate_is_hebrew_locale())
				snprintf (format_date, 500, "%s ב%s",
					hdate_get_int_string (h->hd_day),
					hdate_get_hebrew_month_string (h->hd_mon, s));
			else
				snprintf (format_date, 500, "%s %s",
					hdate_get_int_string (h->hd_day),
					hdate_get_hebrew_month_string (h->hd_mon, s));
			
			return (format_date);
		}
		else
		{
			/* if hebrew add bet to the month */
			if (hdate_is_hebrew_locale())
				snprintf (temp, 500, "%s, %s ב%s",
					hdate_get_day_string (h->hd_dw, s),
					hdate_get_int_string (h->hd_day),
					hdate_get_hebrew_month_string (h->hd_mon, s));
			else
				snprintf (temp, 500, "%s, %s %s",
					hdate_get_day_string (h->hd_dw, s),
					hdate_get_int_string (h->hd_day),
					hdate_get_hebrew_month_string (h->hd_mon, s));
					
			snprintf (format_date, 500, "%s %s",
					temp, hdate_get_int_string (h->hd_year));

			/* if a day in the omer print it */
			omer_day = hdate_get_omer_day(h);
			
			if (omer_day != 0)
				snprintf (temp, 500, "%s, %s",
						format_date, hdate_get_omer_string (omer_day));
			else
				snprintf (temp, 500, "%s", format_date);
			
			/* if holyday print it */
			holyday = hdate_get_holyday (h, diaspora);
			
			if (holyday != 0)
				snprintf (temp, 500, "%s, %s",
						  format_date, hdate_get_holyday_string (holyday, 0));
			else
				snprintf (temp, 500, "%s", format_date);
			
			return (temp);
		}
	}

	return NULL;
}

/**
 @brief Return a static string, with the package name and version

 @return a a static string, with the package name and version
*/
char *
hdate_get_version_string ()
{
	static char version[500];

	/* make a "packge version" string */
	snprintf (version, 500, "%s %s", PACKAGE, VERSION);

	return (version);
}

/**
 @brief Return a static string, with the name of translator

 @return a a static string, with the name of translator
*/
char *
hdate_get_translator_string ()
{
	/* if untranslated return null */
	if (strcmp (_("translator"), "translator") == 0)
		return NULL;

	/* return the translator name */
	return _("translator");
}
