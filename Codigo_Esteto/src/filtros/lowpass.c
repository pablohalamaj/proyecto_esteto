/*
===============================================================================
 Name        : lowpass.c
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description : coeficientes del filtro pasa bajos digital
===============================================================================
*/

#include "lowpass.h"

const int lowpass_taps[LOWPASS_TAP_NUM] = {
		 -19,
		  644,
		  402,
		  443,
		  443,
		  384,
		  270,
		  116,
		  -54,
		  -209,
		  -319,
		  -361,
		  -326,
		  -222,
		  -76,
		  72,
		  174,
		  186,
		  72,
		  -175,
		  -537,
		  -967,
		  -1393,
		  -1730,
		  -1894,
		  -1820,
		  -1476,
		  -874,
		  -71,
		  837,
		  1724,
		  2462,
		  2939,
		  3081,
		  2869,
		  2345,
		  1608,
		  795,
		  60,
		  -458,
		  -665,
		  -532,
		  -107,
		  492,
		  1088,
		  1484,
		  1498,
		  1002,
		  -45,
		  -1580,
		  -3431,
		  -5341,
		  -6999,
		  -8094,
		  -8362,
		  -7640,
		  -5900,
		  -3260,
		  18,
		  3562,
		  6942,
		  9731,
		  11566,
		  12206,
		  11566,
		  9731,
		  6942,
		  3562,
		  18,
		  -3260,
		  -5900,
		  -7640,
		  -8362,
		  -8094,
		  -6999,
		  -5341,
		  -3431,
		  -1580,
		  -45,
		  1002,
		  1498,
		  1484,
		  1088,
		  492,
		  -107,
		  -532,
		  -665,
		  -458,
		  60,
		  795,
		  1608,
		  2345,
		  2869,
		  3081,
		  2939,
		  2462,
		  1724,
		  837,
		  -71,
		  -874,
		  -1476,
		  -1820,
		  -1894,
		  -1730,
		  -1393,
		  -967,
		  -537,
		  -175,
		  72,
		  186,
		  174,
		  72,
		  -76,
		  -222,
		  -326,
		  -361,
		  -319,
		  -209,
		  -54,
		  116,
		  270,
		  384,
		  443,
		  443,
		  402,
		  644,
		  -19
		/*
		-8858824,
		  6003323,
		  12563243,
		  22554988,
		  34445997,
		  46183080,
		  55268898,
		  59164851,
		  55788691,
		  44095689,
		  24536209,
		  -703795,
		  -27799499,
		  -51730180,
		  -67033220,
		  -68753208,
		  -53419173,
		  -19871629,
		  30257820,
		  92516531,
		  160137980,
		  224962013,
		  278706724,
		  314171785,
		  326566858,
		  314171785,
		  278706724,
		  224962013,
		  160137980,
		  92516531,
		  30257820,
		  -19871629,
		  -53419173,
		  -68753208,
		  -67033220,
		  -51730180,
		  -27799499,
		  -703795,
		  24536209,
		  44095689,
		  55788691,
		  59164851,
		  55268898,
		  46183080,
		  34445997,
		  22554988,
		  12563243,
		  6003323,
		  -8858824*/
};
