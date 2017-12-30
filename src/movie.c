/*--------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 1991-2018 by P. Wessel, W. H. F. Smith, R. Scharroo, J. Luis and F. Wobbe
 *	See LICENSE.TXT file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation; version 3 or any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	Contact info: gmt.soest.hawaii.edu
 *--------------------------------------------------------------------*/
/*
 * Author:	Paul Wessel
 * Date:	1-Jan-2018
 * Version:	6 API
 *
 * Brief synopsis: gmt movie automates the generation of animations
 */

#include "gmt_dev.h"

#define THIS_MODULE_NAME	"movie"
#define THIS_MODULE_LIB		"core"
#define THIS_MODULE_PURPOSE	"Create animation sequences"
#define THIS_MODULE_KEYS	""
#define THIS_MODULE_NEEDS	""
#define THIS_MODULE_OPTIONS	"V"

/* Control structure for movie */

#define MOVIE_NONE		0
#define MOVIE_MP4		1
#define MOVIE_GIF		2
#define MOVIE_PREFLIGHT		0
#define MOVIE_POSTFLIGHT	1

enum enum_script {BASH_MODE = 0,	/* Write Bash script */
	CSH_MODE,			/* Write C-shell script */
	DOS_MODE};			/* Write DOS script */

struct MOVIE_CTRL {
	struct In {	/* mainscript (Bourne, Bourne Again, csh, or DOS (bat) script) */
		bool active;
		enum enum_script mode;
		char *file;
		FILE *fp;
	} In;
	struct A {	/* -A<rate>[+l[<n>]]  */
		bool active;
		bool loop;
		double rate;
		unsigned int n_loops;
	} A;
	struct E {	/* -E */
		bool active;
	} E;
	struct F {	/* -F<videoformat> */
		bool active;
		unsigned int mode;
		char *format;
	} F;
	struct G {	/* -G<canvasfill> */
		bool active;
		char *fill;
	} G;
	struct N {	/* -N<movieprefix> */
		bool active;
		char *prefix;
	} N;
	struct Q {	/* -Q[<frame>] */
		bool active;
		int frame;
	} Q;
	struct S {	/* -Sb|f<script> */
		bool active;
		char *file;
		FILE *fp;
	} S[2];
	struct T {	/* -T<nframes>|<timefile> */
		bool active;
		unsigned int n_frames;
		char *file;
	} T;
	struct W {	/* -W<knownformat>|<papersize_and_dpu> */
		bool active;
		double dim[3];
		char unit;
		char *string;
	} W;
};

struct MOVIE_STATUS {
	bool started;	/* true if frame job has started */
	bool completed;	/* true if PNG has been successfully produced */
};

EXTERN_MSC void gmtlib_str_tolower (char *string);

GMT_LOCAL void *New_Ctrl (struct GMT_CTRL *GMT) {	/* Allocate and initialize a new control structure */
	struct MOVIE_CTRL *C;

	C = gmt_M_memory (GMT, NULL, 1, struct MOVIE_CTRL);
	C->A.rate = 24.0;	/* 24 frames/sec */
	C->Q.frame = -1;	/* No frame during debug */
	return (C);
}

GMT_LOCAL void Free_Ctrl (struct GMT_CTRL *GMT, struct MOVIE_CTRL *C) {	/* Deallocate control structure */
	gmt_M_unused (GMT);
	if (!C) return;
	gmt_M_str_free (C->In.file);
	gmt_M_str_free (C->W.string);
	gmt_M_str_free (C->F.format);
	gmt_M_str_free (C->G.fill);
	gmt_M_str_free (C->N.prefix);
	gmt_M_str_free (C->S[MOVIE_PREFLIGHT].file);
	gmt_M_str_free (C->S[MOVIE_POSTFLIGHT].file);
	gmt_M_str_free (C->T.file);
	gmt_M_free (GMT, C);
}

static int gmt_sleep (unsigned int msec) {
	//fprintf (stderr, "Waiting %u msec\n", msec);
#ifdef WIN32
	Sleep ((DWORD)msec);
	return 0;
#else
	return (usleep ((useconds_t)msec));
#endif
}

static void set_dvalue (FILE *fp, int mode, char *name, double value, char unit)
{	/* Assigns a named double variable given the script mode */
	switch (mode) {
		case BASH_MODE: fprintf (fp, "%s=%g", name, value); break;
		case CSH_MODE:  fprintf (fp, "set %s = %g", name, value); break;
		case DOS_MODE:  fprintf (fp, "set %s=%g", name, value); break;
	}
	if (unit) fprintf (fp, "%c", unit);
	fprintf (fp, "\n");
}

static void set_ivalue (FILE *fp, int mode, char *name, int value)
{	/* Assigns the a named int variable given the script mode */
	switch (mode) {
		case BASH_MODE: fprintf (fp, "%s=%d\n", name, value); break;
		case CSH_MODE:  fprintf (fp, "set %s = %d\n", name, value); break;
		case DOS_MODE:  fprintf (fp, "set %s=%d\n", name, value); break;
	}
}

static void set_tvalue (FILE *fp, int mode, char *name, char *value)
{	/* Assigns a named text variable given the script mode */
	switch (mode) {
		case BASH_MODE: fprintf (fp, "%s=%s\n", name, value); break;
		case CSH_MODE:  fprintf (fp, "set %s = %s\n", name, value); break;
		case DOS_MODE:  fprintf (fp, "set %s=%s\n", name, value); break;
	}
}

static void set_script (FILE *fp, int mode)
{	/* Writes the script's incantation line (or comment for DOS) */
	switch (mode) {
		case BASH_MODE: fprintf (fp, "#!/bin/bash\n"); break;
		case CSH_MODE:  fprintf (fp, "#!/bin/csh\n"); break;
		case DOS_MODE:  fprintf (fp, "REM Start of script\n"); break;
	}
}

static void set_comment (FILE *fp, int mode, char *comment)
{	/* Write a comment given the script mode */
	switch (mode) {
		case BASH_MODE: case CSH_MODE:  fprintf (fp, "# %s\n", comment); break;
		case DOS_MODE:  fprintf (fp, "REM %s\n", comment); break;
	}
}

static char *place_var (int mode, char *name)
{	/* Prints this variable to stdout where needed in the script via the string static variable.
	 * PS!  Only one call per printf statement since string cannot hold more than one item at the time */
	static char string[128] = {""};
	if (mode == DOS_MODE)
		sprintf (string, "%%%s%%", name);
	else
		sprintf (string, "${%s}", name);
	return (string);
}

int dry_run_only (const char *cmd) {
	/* Dummy to not actually run the loop script when -Q is used */
	gmt_M_unused (cmd);
	return 0;
}

GMT_LOCAL int usage (struct GMTAPI_CTRL *API, int level) {
	gmt_show_name_and_purpose (API, THIS_MODULE_LIB, THIS_MODULE_NAME, THIS_MODULE_PURPOSE);
	if (level == GMT_MODULE_PURPOSE) return (GMT_NOERROR);
	GMT_Message (API, GMT_TIME_NONE, "usage: movie <mainscript> -N<prefix> -T<n_frames>|<timefile> -W<papersize>\n");
	GMT_Message (API, GMT_TIME_NONE, "\t[-A<rate>[+l[<n>]]] [-E] [-F<format>] [-G<fill>] [-Q[<frame>]] [-Sb<script>] [-Sf<script>] [%s]\n\n", GMT_V_OPT);

	if (level == GMT_SYNOPSIS) return (GMT_MODULE_SYNOPSIS);

	GMT_Message (API, GMT_TIME_NONE, "\t<mainscript> is the main GMT modern script that builds a single frame image.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-E Erase directory with PNG plots after converting to movie [leave in subdirectory <prefix>].\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-N Set project prefix used for movie files and directories.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-T Set number of frames, or give filename with frame-specific information.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-W Specify layout of the custom paper size. Choose from known dimensions or set them manually:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   Recognized 16:9 ratio formats:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     name:	pixel size	page size (SI)	page size (US)\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     4k:	3840 x 2160	24 x 13.5 cm	 9.6 x 5.4 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     1080p:	1920 x 1080	24 x 13.5 cm	 9.6 x 5.4 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     720p:	1280 x 720	24 x 13.5 cm	 9.6 x 5.4 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     540p:	 960 x 540	24 x 13.5 cm	 9.6 x 5.4 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   Recognized 4:3 ratio formats:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     name:	pixel size	page size (SI)	page size (US)\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     480p:	 640 x 480	24 x 18 cm	 9.6 x 7.2 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     360p:	 480 x 360	24 x 18 cm	 9.6 x 7.2 inch\n");
	GMT_Message (API, GMT_TIME_NONE, "\t  Note: PROJ_LENGTH_UNIT determines if you set SI or US dimensions.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   Alternatively, set a custom format directly:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     Give <width>x<height>x<dpi> for a custom frame dimension.\n");
	GMT_Message (API, GMT_TIME_NONE, "\n\tOPTIONS:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-A Set frame rate in frames/second [24]. Append +l to enable looping [GIF only];\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   optionally append a specific number of loops [infinite loop].\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-F Set the final movie format. Choose from these selections:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     gif:  Convert PNG frames into an animated GIF.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     mp4:  Convert PNG frames into an MP4 movie.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t     Default is none: No animated product, just create the PNG frames.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-G Set the canvas background color [none].\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-Q Dry-run.  Only the work scripts will be produced but none will be executed.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   Alternatively, append a frame number and we just produce that single PNG image.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t-S Set the optional background and foreground GMT modern scripts:\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   -Sb Append the name of a background script [none].\n");
	GMT_Message (API, GMT_TIME_NONE, "\t      Pre-compute items needed by <mainscript> and build a static background plot layer.\n");
	GMT_Message (API, GMT_TIME_NONE, "\t   -Sf Append name of foreground script [none].\n");
	GMT_Message (API, GMT_TIME_NONE, "\t      Build a static foreground plot overlay to be added to all frames.\n");
	GMT_Option (API, "V");
	
	return (GMT_MODULE_USAGE);
}

GMT_LOCAL int parse (struct GMT_CTRL *GMT, struct MOVIE_CTRL *Ctrl, struct GMT_OPTION *options) {

	/* This parses the options provided to grdcut and sets parameters in CTRL.
	 * Any GMT common options will override values set previously by other commands.
	 * It also replaces any file names specified as input or output with the data ID
	 * returned when registering these sources/destinations with the API.
	 */

	unsigned int n_errors = 0, n_files = 0, k;
	int n;
	char txt_a[GMT_LEN32] = {""}, txt_b[GMT_LEN32] = {""}, *c = NULL;
	double width, height16x9, height4x3, dpu;
	struct GMT_FILL fill;	/* Only used to make sure any fill is given with correct syntax */
	struct GMT_OPTION *opt = NULL;
	
	if (GMT->current.setting.proj_length_unit == GMT_INCH) {	/* US dimensions in inch given format names */
		width = 9.6;	height16x9 = 5.4;	height4x3 = 7.2;	dpu = 400.0;
		Ctrl->W.unit = 'i';
	}
	else {	/* SI dimensions in cm given format names */
		width = 24.0;	height16x9 = 13.5;	height4x3 = 18.0;	dpu = 160.0;
		Ctrl->W.unit = 'c';
	}

	for (opt = options; opt; opt = opt->next) {
		switch (opt->option) {

			case '<':	/* Input file */
				if (n_files++ > 0) break;
				if ((Ctrl->In.active = gmt_check_filearg (GMT, '<', opt->arg, GMT_IN, GMT_IS_TEXT)))
					Ctrl->In.file = strdup (opt->arg);
				else
					n_errors++;
				break;
				
			case 'A':	/* Frame rate */
				Ctrl->A.active = true;
				Ctrl->A.rate = atof (opt->arg);
				if ((c = strstr (opt->arg, "+l"))) {	/* Want to add a loop (animated GIF only) */
					Ctrl->A.loop = true;
					if (c[2]) Ctrl->A.n_loops = atoi (&c[2]);
				}
				break;

			case 'E':	/* Erase frames after movie has been made */
				Ctrl->E.active = true;
				break;
			case 'F':	/* Set movie format */
				Ctrl->F.active = true;
				if (!strcmp (opt->arg, "gif") || !strcmp (opt->arg, "GIF"))
					Ctrl->F.mode = MOVIE_GIF;
				else if (!strcmp (opt->arg, "mp4") || !strcmp (opt->arg, "MP4"))
					Ctrl->F.mode = MOVIE_MP4;
				break;
			case 'G':	/* Canvas fill */
				Ctrl->G.active = true;
				if (gmt_getfill (GMT, opt->arg, &fill))
					n_errors++;
				else
					Ctrl->G.fill = strdup (opt->arg);
				break;
			case 'N':	/* Movie prefix and directory name */
				Ctrl->N.active = true;
				Ctrl->N.prefix = strdup (opt->arg);
				break;
			
			case 'Q':	/* Debug, optionally run a single frame */
				Ctrl->Q.active = true;
				if (opt->arg[0]) Ctrl->Q.frame = atoi (opt->arg);
				break;
			case 'S':	/* background and foreground scripts */
				switch (opt->arg[0]) {
					case 'b':	k = MOVIE_PREFLIGHT;	break;	/* background */
					case 'f':	k = MOVIE_POSTFLIGHT;	break;	/* foreground */
					default:	/* Bad option */
						n_errors++;
						k = 9;
						GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Syntax error option -S: Select -Sb or -Sf\n");
						break;
				}
				if (k != 9) {	/* Got a valid f or b */
					Ctrl->S[k].active = true;
					Ctrl->S[k].file = strdup (&opt->arg[1]);
					if ((Ctrl->S[k].fp = fopen (Ctrl->S[k].file, "r")) == NULL) {
						GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Syntax error option -S%c: Unable to open file %s\n", opt->arg[0], Ctrl->S[k].file);
						n_errors++;
					}
				}
				break;
	
			case 'T':	/* Number of frames or name of file with frame information */
				Ctrl->T.active = true;
				Ctrl->T.file = strdup (opt->arg);
				break;
			case 'W':	/* Frame known dimension or custom sizes */
				Ctrl->W.active = true;
				if (!strcmp (opt->arg, "4k") || !strcmp (opt->arg, "4K")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height16x9;	Ctrl->W.dim[GMT_Z] = dpu;
				}
				else if (!strcmp (opt->arg, "1080p") || !strcmp (opt->arg, "1080P")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height16x9;	Ctrl->W.dim[GMT_Z] = dpu / 2.0;
				}
				else if (!strcmp (opt->arg, "720p") || !strcmp (opt->arg, "720P")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height16x9;	Ctrl->W.dim[GMT_Z] = dpu / 3.0;
				}
				else if (!strcmp (opt->arg, "540p") || !strcmp (opt->arg, "540P")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height16x9;	Ctrl->W.dim[GMT_Z] = dpu / 4.0;
				}
				else if (!strcmp (opt->arg, "480p") || !strcmp (opt->arg, "480P")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height4x3;	Ctrl->W.dim[GMT_Z] = dpu / 6.0;
				}
				else if (!strcmp (opt->arg, "360p") || !strcmp (opt->arg, "360P")) {
					Ctrl->W.dim[GMT_X] = width;	Ctrl->W.dim[GMT_Y] = height4x3;	Ctrl->W.dim[GMT_Z] = dpu / 8.0;
				}
				else {	/* Custom paper dimensions */
					if ((n = sscanf (opt->arg, "%[^x]x%[^x]x%lg", txt_a, txt_b, &Ctrl->W.dim[GMT_Z])) != 3) {
						GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Syntax error option -W: Requires a known format or give width x height x dpi\n");
						n_errors++;
					}
					else {
						Ctrl->W.dim[GMT_X] = gmt_M_to_inch (GMT, txt_a);	
						Ctrl->W.dim[GMT_Y] = gmt_M_to_inch (GMT, txt_b);
						if (strchr ("cip", txt_a[strlen(txt_a)-1]))
							Ctrl->W.unit = txt_a[strlen(txt_a)-1];
						else if (strchr ("cip", txt_b[strlen(txt_b)-1]))
							Ctrl->W.unit = txt_b[strlen(txt_b)-1];
						else	/* Use GMT default setting as unit */
							Ctrl->W.unit = GMT->session.unit_name[GMT->current.setting.proj_length_unit][0];
					}
				}
				break;

			default:	/* Report bad options */
				n_errors += gmt_default_error (GMT, opt->option);
				break;
		}
	}
		
	n_errors += gmt_M_check_condition (GMT, !Ctrl->W.active, "Syntax error -W: Must specify frame dimension\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->W.dim[GMT_X] <= 0.0 || Ctrl->W.dim[GMT_Y] <= 0.0, "Syntax error -W: Zero or negative dimensions given\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->W.dim[GMT_Z] <= 0.0, "Syntax error -W: Zero or negative dots-per-unit given\n");
	n_errors += gmt_M_check_condition (GMT, !Ctrl->N.active, "Syntax error -N: Must specify a movie prefix\n");
	n_errors += gmt_M_check_condition (GMT, !Ctrl->T.active, "Syntax error -T: Must specify number of frames or time file\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->A.loop && Ctrl->F.mode != MOVIE_GIF, "Syntax error -A: The loop modifier +l only applies to GIF animations\n");
	n_errors += gmt_M_check_condition (GMT, Ctrl->F.mode == MOVIE_NONE && Ctrl->E.active, "Syntax error -E: Cannot be used without -F specifying a movie product\n");
	
	if (n_files == 1) {	/* Determine script language from extension and open main script */
		if (strstr (Ctrl->In.file, ".bash") || strstr (Ctrl->In.file, ".sh"))
			Ctrl->In.mode = BASH_MODE;
		else if (strstr (Ctrl->In.file, ".csh"))
			Ctrl->In.mode = CSH_MODE;
		else if (strstr (Ctrl->In.file, ".bat"))
			Ctrl->In.mode = DOS_MODE;
		else {
			GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Unable to determine script language from the extension of your script %s\n", Ctrl->In.file);
			n_errors++;
		}
		if (n_errors == 0 && ((Ctrl->In.fp = fopen (Ctrl->In.file, "r")) == NULL)) {
			GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Unable to open main script file %s\n", Ctrl->In.file);
			n_errors++;
		}
	}
	else {
		GMT_Report (GMT->parent, GMT_MSG_NORMAL, "Must specify a main script file!");
		n_errors++;
	}
	return (n_errors ? GMT_PARSE_ERROR : GMT_NOERROR);
}

#define bailout(code) {gmt_M_free_options (mode); return (code);}
#define Return(code) {Free_Ctrl (GMT, Ctrl); gmt_end_module (GMT, GMT_cpy); bailout (code);}

int GMT_movie (void *V_API, int mode, void *args) {
	int error = 0;
	int (*run_script) (const char *);	/* pointer to system function or dummy */
	
	unsigned int n_values = 0, n_frames = 0, n_begin = 0, frame, col, p_width, p_height, k;
	unsigned int n_frames_not_started = 0, n_frames_completed = 0, first_frame = 0, n_cores_unused;
	bool done = false;
	
	char *extension[3] = {"sh", "csh", "bat"}, *load[3] = {"source", "source", "call"}, *rmfile[3] = {"rm -f", "rm -f", "del"};
	char var[3] = "$$%", *rmdir[3] = {"rm -rf", "rm -rf", "deltree"}, *mvfile[3] = {"mv -f", "mv -rf", "move"}, *export[3] = {"export ", "export ", ""};
	char init_file[GMT_LEN64] = {""}, state_prefix[GMT_LEN64] = {""}, state_file[GMT_LEN64] = {""}, cwd[PATH_MAX] = {""};
	char pre_file[GMT_LEN64] = {""}, post_file[GMT_LEN64] = {""}, main_file[GMT_LEN64] = {""}, line[GMT_BUFSIZ] = {""};
	char string[GMT_LEN64] = {""}, extra[GMT_LEN64] = {""}, cmd[GMT_LEN256] = {""}, cleanup_file[GMT_LEN64] = {""};
	char loop_cmd[GMT_LEN32] = {""}, png_file[GMT_LEN64] = {""};
	
	FILE *fp = NULL;
	struct GMT_CTRL *GMT = NULL, *GMT_cpy = NULL;
	struct GMT_DATASET *D = NULL;
	struct GMT_OPTION *options = NULL;
	struct MOVIE_STATUS *status = NULL;
	struct MOVIE_CTRL *Ctrl = NULL;
	struct GMTAPI_CTRL *API = gmt_get_api_ptr (V_API);	/* Cast from void to GMTAPI_CTRL pointer */

	/*----------------------- Standard module initialization and parsing ----------------------*/

	if (API == NULL) return (GMT_NOT_A_SESSION);
	if (mode == GMT_MODULE_PURPOSE) return (usage (API, GMT_MODULE_PURPOSE));	/* Return the purpose of program */
	options = GMT_Create_Options (API, mode, args);	if (API->error) return (API->error);	/* Set or get option list */

	if (!options || options->option == GMT_OPT_USAGE) bailout (usage (API, GMT_USAGE));		/* Return the usage message */
	if (options->option == GMT_OPT_SYNOPSIS) bailout (usage (API, GMT_SYNOPSIS));	/* Return the synopsis */

	/* Parse the command-line arguments */

	if ((GMT = gmt_init_module (API, THIS_MODULE_LIB, THIS_MODULE_NAME, THIS_MODULE_KEYS, THIS_MODULE_NEEDS, &options, &GMT_cpy)) == NULL) bailout (API->error); /* Save current state */
	if (GMT_Parse_Common (API, THIS_MODULE_OPTIONS, options)) Return (API->error);
	Ctrl = New_Ctrl (GMT);	/* Allocate and initialize a new control structure */
	if ((error = parse (GMT, Ctrl, options)) != 0) Return (error);

	/*---------------------------- This is the subplot main code ----------------------------*/

	/* Determine pixel dimensions of images */
	p_width =  urint (ceil (Ctrl->W.dim[GMT_X] * Ctrl->W.dim[GMT_Z]));
	p_height = urint (ceil (Ctrl->W.dim[GMT_Y] * Ctrl->W.dim[GMT_Z]));
	
	if (Ctrl->Q.active && Ctrl->Q.frame == -1) {	/* Nothing but scripts will be produced */
		GMT_Report (API, GMT_MSG_NORMAL, "Dry-run enabled - only scripts will be created.\n");
		run_script = dry_run_only;
	}
	else {	/* Will run scripts and may even need to make a movie */
		run_script = system;
	
		if (Ctrl->F.mode == MOVIE_GIF && Ctrl->Q.frame == -1) {	/* Ensure we have GraphicsMagick installed */
			sprintf (cmd, "gm version");
			if ((fp = popen (cmd, "r")) == NULL) {
				GMT_Report (API, GMT_MSG_NORMAL, "GraphicsMagick is not installed - cannot build animated GIF.\n");
				Return (GMT_RUNTIME_ERROR);
			}
			else pclose (fp);
		}
		else if (Ctrl->F.mode == MOVIE_MP4 && Ctrl->Q.frame == -1) {	/* Ensure we have ffmpeg installed */
			sprintf (cmd, "ffmpeg -version");
			if ((fp = popen (cmd, "r")) == NULL) {
				GMT_Report (API, GMT_MSG_NORMAL, "ffmpeg is not installed - cannot build MP4 movie.\n");
				Return (GMT_RUNTIME_ERROR);
			}
			else {	/* OK, but check if width is odd or even */
				pclose (fp);
				if (p_width % 2)	/* Don't like odd pixel widths */
					GMT_Report (API, GMT_MSG_NORMAL, "Your frame width is an odd number of pixels (%u).  This may not work with ffmpeg...\n", p_width);
			}
		}
	}
	
	GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Paper dimensions: Width = %g%c Height = %g%c\n", Ctrl->W.dim[GMT_X], Ctrl->W.unit, Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
	GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Pixel dimensions: %u x %u\n", p_width, p_height);
	
	/* Create a working directory which will house every local file and subdirectories created */
	
	if (gmt_mkdir (Ctrl->N.prefix)) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to create working directory %s - exiting.\n", Ctrl->N.prefix);
		Return (GMT_RUNTIME_ERROR);
	}
	/* Make this directory the current directory */
	if (chdir (Ctrl->N.prefix)) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to change directory to %s - exiting.\n", Ctrl->N.prefix);
		perror (Ctrl->N.prefix);
		Return (GMT_RUNTIME_ERROR);
	}
	/* Get full path to this working directory */
	if (getcwd (cwd, GMT_BUFSIZ) == NULL) {
		GMT_Report (GMT->parent, GMT_MSG_VERBOSE, "Unable to determine current working directory.\n");
		Return (GMT_RUNTIME_ERROR);
	}
	
	/* Create the initialization file for all scripts */
	
	sprintf (init_file, "movie_init.%s", extension[Ctrl->In.mode]);
	if ((fp = fopen (init_file, "w")) == NULL) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to create file %s - exiting\n", init_file);
		Return (GMT_ERROR_ON_FOPEN);
	}
	
	sprintf (string, "Static parameters for movie %s", Ctrl->N.prefix);
	set_comment (fp, Ctrl->In.mode, string);
	set_dvalue (fp, Ctrl->In.mode, "GMT_MOVIE_WIDTH", Ctrl->W.dim[GMT_X], Ctrl->W.unit);
	set_dvalue (fp, Ctrl->In.mode, "GMT_MOVIE_HEIGHT", Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
	set_dvalue (fp, Ctrl->In.mode, "GMT_MOVIE_DPI", Ctrl->W.dim[GMT_Z], 0);
	set_tvalue (fp, Ctrl->In.mode, "GMT_MOVIE_DIR", cwd);
	fprintf (fp, "gmt set PS_MEDIA %g%cx%g%c\n", Ctrl->W.dim[GMT_X], Ctrl->W.unit, Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
	if (Ctrl->G.active)	/* Want to set background color - we do this in psconvert */
		sprintf (extra, "A+g%s,P", Ctrl->G.fill);
	else	/* In either case we set Portrait mode */
		sprintf (extra, "P");
	fclose (fp);
	
	if (Ctrl->S[MOVIE_PREFLIGHT].active) {	/* Create the preflight script from the users background script */
		sprintf (pre_file, "movie_preflight.%s", extension[Ctrl->In.mode]);
		if ((fp = fopen (pre_file, "w")) == NULL) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to create file %s - exiting\n", pre_file);
			Return (GMT_ERROR_ON_FOPEN);
		}
		set_script (fp, Ctrl->In.mode);					/* Write 1st line of a script */
		set_comment (fp, Ctrl->In.mode, "Preflight script");
		fprintf (fp, "%s", export[Ctrl->In.mode]);			/* Hardwire a PPID since subshells may mess things up */
		set_tvalue (fp, Ctrl->In.mode, "GMT_PPID", "$$");
		fprintf (fp, "%s %s\n", load[Ctrl->In.mode], init_file);	/* Include the initialization parameters */
		while (gmt_fgets (GMT, line, GMT_BUFSIZ, Ctrl->S[MOVIE_PREFLIGHT].fp)) {	/* Read the background and copy to loop script with some exceptions */
			if (strstr (line, "gmt begin")) {	/* Need to insert gmt figure after this line */
				fprintf (fp, "gmt begin\n");	/* Ensure there are no args here since we are using gmt figure instead */
				set_comment (fp, Ctrl->In.mode, "\tSet fixed background output ps name");
				fprintf (fp, "\tgmt figure gmt_movie_background ps\n");
				fprintf (fp, "\tgmt set PS_MEDIA %g%cx%g%c\n", Ctrl->W.dim[GMT_X], Ctrl->W.unit, Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
			}
			else if (!strstr (line, "#!/bin"))	/* Skip any leading shell incantation since already placed */
				fprintf (fp, "%s", line);	/* Just copy the line as is */
		}
		fclose (Ctrl->S[MOVIE_PREFLIGHT].fp);	/* Done reading the foreground script */
		fclose (fp);
#ifndef WIN32
		/* Set executable bit if not Windows */
		if (chmod (pre_file, S_IRWXU)) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to make preflight script executable: %s - exiting.\n", pre_file);
			Return (GMT_RUNTIME_ERROR);
		}
#endif
		/* Run the pre-flight which may create a file needed later via -T */
		if ((error = system (pre_file))) {
			GMT_Report (API, GMT_MSG_NORMAL, "Running preflight script %s returned error %d - exiting.\n", pre_file, error);
			Return (GMT_RUNTIME_ERROR);
		}
	}
	
	/* Now we can check -T since any needed file will have been created */
	
	if (!gmt_access (GMT, Ctrl->T.file, R_OK)) {	/* A file by that name exists and is readable */
		if ((D = GMT_Read_Data (API, GMT_IS_DATASET, GMT_IS_FILE, GMT_IS_NONE, GMT_READ_NORMAL, NULL, Ctrl->T.file, NULL)) == NULL) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to read time file: %s - exiting\n", Ctrl->T.file);
			Return (API->error);
		}
		if (D->n_segments > 1) {	/* We insist on a simple file structure with a single segment */
			GMT_Report (API, GMT_MSG_NORMAL, "Your time file %s has more than one segment - reformat first\n", Ctrl->T.file);
			Return (API->error);
		}
		n_frames = D->n_records;	/* Number of records means number of frames */
		n_values = D->n_columns;	/* The number of per-frame parameters we need to place into the per-frame parameter files */
	}
	else	/* Just gave the number of frames */
		n_frames = atoi (Ctrl->T.file);
	
	if (n_frames == 0) {	/* But need to check it */
		GMT_Report (API, GMT_MSG_NORMAL, "No frames specified! - exiting.\n");
		Return (GMT_RUNTIME_ERROR);
	}

	GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Number of frames: %d\n", n_frames);
	
	if (Ctrl->S[MOVIE_POSTFLIGHT].active) {	/* Prepare the postflight script */
		sprintf (post_file, "movie_postflight.%s", extension[Ctrl->In.mode]);
		if ((fp = fopen (post_file, "w")) == NULL) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to create postflight file %s - exiting\n", post_file);
			Return (GMT_ERROR_ON_FOPEN);
		}
		set_script (fp, Ctrl->In.mode);					/* Write 1st line of a script */
		set_comment (fp, Ctrl->In.mode, "Postflight script");
		fprintf (fp, "%s", export[Ctrl->In.mode]);			/* Hardwire a PPID since subshells may mess things up */
		set_tvalue (fp, Ctrl->In.mode, "GMT_PPID", "$$");
		fprintf (fp, "%s %s\n", load[Ctrl->In.mode], init_file);	/* Include the initialization parameters */
		while (gmt_fgets (GMT, line, GMT_BUFSIZ, Ctrl->S[MOVIE_POSTFLIGHT].fp)) {	/* Read the foreground and copy to loop script with some exceptions */
			if (strstr (line, "gmt begin")) {	/* Need to insert gmt figure after this line */
				fprintf (fp, "gmt begin\n");	/* Ensure there are no args here since we are using gmt figure instead */
				set_comment (fp, Ctrl->In.mode, "\tSet fixed foreground output ps name");
				fprintf (fp, "\tgmt figure gmt_movie_foreground ps\n");
				fprintf (fp, "\tgmt set PS_MEDIA %g%cx%g%c\n", Ctrl->W.dim[GMT_X], Ctrl->W.unit, Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
			}
			else if (!strstr (line, "#!/bin"))	/* Skip any leading shell incantation since already placed */
				fprintf (fp, "%s", line);	/* Just copy the line as is */
		}
		fclose (Ctrl->S[MOVIE_POSTFLIGHT].fp);	/* Done reading the foreground script */
		fclose (fp);
#ifndef WIN32
		/* Set executable bit if not Windows */
		if (chmod (post_file, S_IRWXU)) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to make postflight script executable: %s - exiting\n", post_file);
			Return (GMT_RUNTIME_ERROR);
		}
#endif
		/* Run post-flight now before dealing with the loop so the overlay exists */
		if ((error = run_script (post_file))) {
			GMT_Report (API, GMT_MSG_NORMAL, "Running postflight script %s returned error %d - exiting.\n", post_file, error);
			Return (GMT_RUNTIME_ERROR);
		}
	}
	
	/* Create parameter include files for each frame */
	
	for (frame = 0; frame < n_frames; frame++) {
		if (Ctrl->Q.active && Ctrl->Q.frame != -1 && (int)frame != Ctrl->Q.frame) continue;	/* Just doing a single frame for debugging */
		sprintf (state_prefix, "movie_params_%6.6d", frame);
		sprintf (state_file, "%s.%s", state_prefix, extension[Ctrl->In.mode]);
		if ((fp = fopen (state_file, "w")) == NULL) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to create frame parameter file %s - exiting\n", state_file);
			Return (GMT_ERROR_ON_FOPEN);
		}
		sprintf (state_prefix, "Parameter file for frame %6.6d", frame);
		set_comment (fp, Ctrl->In.mode, state_prefix);
		sprintf (state_prefix, "%s_%6.6d", Ctrl->N.prefix, frame);
		set_ivalue (fp, Ctrl->In.mode, "GMT_MOVIE_FRAME", frame);	/* Current frame number */
		set_tvalue (fp, Ctrl->In.mode, "GMT_MOVIE_NAME", state_prefix);	/* Current frame name prefix */
		for (col = 0; col < n_values; col++) {	/* Place frame variables from file into parameter file */
			sprintf (string, "GMT_MOVIE_VAL%u", col+1);
			set_dvalue (fp, Ctrl->In.mode, string, D->table[0]->segment[0]->data[col][frame], 0);
		}
		if (D && D->table[0]->segment[0]->text)	/* Also place any string parameter */
			set_tvalue (fp, Ctrl->In.mode, "GMT_MOVIE_STRING", D->table[0]->segment[0]->text[frame]);
		fclose (fp);
	}
	
	/* Now build loop script from the mainscript */
	
	sprintf (main_file, "movie_frame.%s", extension[Ctrl->In.mode]);
	if ((fp = fopen (main_file, "w")) == NULL) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to create loop frame script file %s - exiting\n", main_file);
		Return (GMT_ERROR_ON_FOPEN);
	}
	set_script (fp, Ctrl->In.mode);					/* Write 1st line of a script */
	set_comment (fp, Ctrl->In.mode, "Main frame loop script");
	fprintf (fp, "%s", export[Ctrl->In.mode]);			/* Hardwire a PPID since subshells may mess things up */
	if (Ctrl->In.mode == DOS_MODE)	/* Set PPID under Windows to be the frame number */
		fprintf (fp, "GMT_PPID %c1\n", var[Ctrl->In.mode]);
	else	/* On UNIX we use the script's PID as PPID */
		set_tvalue (fp, Ctrl->In.mode, "GMT_PPID", "$$");
	set_comment (fp, Ctrl->In.mode, "Set static and frame-specific parameters");
	fprintf (fp, "%s %s\n", load[Ctrl->In.mode], init_file);	/* Include the initialization parameters */
	fprintf (fp, "%s movie_params_%c1.%s\n", load[Ctrl->In.mode], var[Ctrl->In.mode], extension[Ctrl->In.mode]);	/* Include the frame parameters */
	fprintf (fp, "mkdir %s\n", place_var (Ctrl->In.mode, "GMT_MOVIE_NAME"));	/* Make a temp directory for this frame */
	fprintf (fp, "cd %s\n", place_var (Ctrl->In.mode, "GMT_MOVIE_NAME"));		/* cd to the temp directory */
	while (gmt_fgets (GMT, line, GMT_BUFSIZ, Ctrl->In.fp)) {	/* Read the mainscript and copy to loop script with some exceptions */
		if (strstr (line, "gmt begin")) {	/* Need to insert gmt figure after this line */
			fprintf (fp, "gmt begin\n");	/* Ensure there are no args here since we are using gmt figure instead */
			set_comment (fp, Ctrl->In.mode, "\tSet output PNG name and conversion parameters");
			fprintf (fp, "\tgmt figure %s png", place_var (Ctrl->In.mode, "GMT_MOVIE_NAME"));
			fprintf (fp, " E%s,%s\n", place_var (Ctrl->In.mode, "GMT_MOVIE_DPI"), extra);
			fprintf (fp, "\tgmt set PS_MEDIA %g%cx%g%c\n", Ctrl->W.dim[GMT_X], Ctrl->W.unit, Ctrl->W.dim[GMT_Y], Ctrl->W.unit);
			n_begin++;	/* Processed the gmt begin line */
		}
		else if (!strstr (line, "#!/bin"))	/* Skip any leading shell incantation since already placed */
			fprintf (fp, "%s", line);	/* Just copy the line as is */
	}
	fclose (Ctrl->In.fp);	/* Done reading the main script */
	set_comment (fp, Ctrl->In.mode, "Move PNG file up to parent directory and cd up one level");
	fprintf (fp, "%s %s.png ..\n", mvfile[Ctrl->In.mode], place_var (Ctrl->In.mode, "GMT_MOVIE_NAME"));	/* Move PNG plot up to parent dir */
	fprintf (fp, "cd ..\n");	/* cd up to parent dir */
	set_comment (fp, Ctrl->In.mode, "Remove frame directory and parameter file");
	fprintf (fp, "%s %s\n", rmdir[Ctrl->In.mode], place_var (Ctrl->In.mode, "GMT_MOVIE_NAME"));	/* Remove the work dir and any files in it*/
	fprintf (fp, "%s movie_params_%c1.%s\n", rmfile[Ctrl->In.mode], var[Ctrl->In.mode], extension[Ctrl->In.mode]);	/* Remove the parameter file */
	fclose (fp);
	
	if (n_begin != 1) {
		GMT_Report (API, GMT_MSG_NORMAL, "Main script %s is not in modern mode - exiting\n", main_file);
		Return (GMT_RUNTIME_ERROR);
	}
	
#ifndef WIN32
	/* Set executable bit if not Windows */
	if (chmod (main_file, S_IRWXU)) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to make script executable: %s - exiting\n", main_file);
		Return (GMT_RUNTIME_ERROR);
	}
#endif
	
	if (Ctrl->Q.active) {	/* Possibly do one frame and then exit (no movie generated) */
		if (Ctrl->Q.frame != -1) {	/* Just do one frame and then exit (no movie generated) */
			char frame_cmd[GMT_LEN32];
	        	sprintf (frame_cmd, "%s %6.6d", main_file, Ctrl->Q.frame);
			if ((error = run_script (frame_cmd))) {
				GMT_Report (API, GMT_MSG_NORMAL, "Running script %s returned error %d - exiting.\n", frame_cmd, error);
				Return (GMT_RUNTIME_ERROR);
			}
			GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Single PNG frame built: %s_%6.6d.png\n", Ctrl->N.prefix, Ctrl->Q.frame);
		}
		Return (GMT_NOERROR);
	}

	/* Finally, we can run all the frames in a controlled loop, launching new jobs as cores become available */

	frame = 0; n_frames_not_started = n_frames;
	n_cores_unused = MAX (1, API->n_cores - 1);	/* Remove one for movie module thread */
	status = gmt_M_memory (GMT, NULL, n_frames, struct MOVIE_STATUS);
	
	while (!done) {	/* Keep trying until all frames have completed */
		/* Launch new jobs if posible */
		while (n_frames_not_started && n_cores_unused) {
#if WIN32
	        	sprintf (loop_cmd, "start %s %6.6d", main_file, frame);
#else
	        	sprintf (loop_cmd, "%s %6.6d &", main_file, frame);
#endif
			GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Launch script for frame %6.6d\n", frame);
			if ((error = system (loop_cmd))) {
				GMT_Report (API, GMT_MSG_NORMAL, "Running script %s returned error %d - aborting.\n", loop_cmd, error);
				Return (GMT_RUNTIME_ERROR);
			}
			status[frame].started = true;	/* We have launched this frame job */
			frame++;			/* Go to next frame for next launch */
			n_frames_not_started--;		/* One less of these */
			n_cores_unused--;		/* This core is now busy */
			//fprintf (stderr, "L: frame = %d\tn_frames_not_started = %d\tn_cores_unused = %d\tn_frames_completed = %d\n",
			//	frame, n_frames_not_started, n_cores_unused, n_frames_completed);
		}
		gmt_sleep (10000);	/* Wait 0.01 seconds - then check for completion */
		/* Check for completion of PNG plots */
		for (k = first_frame; k < frame; k++) {	/* Only loop over range of frames that are currently in play */
			if (status[k].completed) continue;	/* Already finished with this one */
			if (!status[k].started) continue;	/* Nothing to do here yet */
			/* Here we can check if this job has completed */
	        	sprintf (png_file, "%s_%6.6d.png", Ctrl->N.prefix, k);
			if (access (png_file, F_OK)) continue;	/* Not found */
			GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Frame %6.6d completed\n", k);
			status[k].completed = true;
			n_cores_unused++;	/* Freed up one core */
			n_frames_completed++;	/* One more done */
			//fprintf (stderr, "C: frame = %d\tn_frames_not_started = %d\tn_cores_unused = %d\tn_frames_completed = %d\n",
			//	frame, n_frames_not_started, n_cores_unused, n_frames_completed);
		}
		/* Adjust first_frame if needed */
		while (first_frame < n_frames && status[first_frame].completed) first_frame++;
		if (n_frames_completed == n_frames) done = true;	/* All frames completed */
	}
	
	gmt_M_free (GMT, status);	/* Done with this structure array */
	
	/* Cd back up to parent directory */
	if (chdir ("..")) {
		GMT_Report (API, GMT_MSG_NORMAL, "Unable to change directory to parent directory - exiting.\n");
		perror (Ctrl->N.prefix);
		Return (GMT_RUNTIME_ERROR);
	}

	if (Ctrl->F.mode == MOVIE_GIF) {
		/* Set up system call to gm */
		unsigned int delay = urint (100.0 / Ctrl->A.rate);	/* Delay to nearest 1/100 s */
		if (Ctrl->A.loop)	/* Add the -loop argument */
			sprintf (extra, "-loop %u +dither", Ctrl->A.n_loops);
		else
			sprintf (extra, "+dither");
		sprintf (cmd, "gm convert -delay %u %s %s/%s_*.png %s.gif", delay, extra, Ctrl->N.prefix, Ctrl->N.prefix, Ctrl->N.prefix);
		GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Run: %s\n", cmd);
		if ((error = system (cmd))) {
			GMT_Report (API, GMT_MSG_NORMAL, "Running GIF conversion returned error %d - exiting.\n", error);
			Return (GMT_RUNTIME_ERROR);
		}
		GMT_Report (API, GMT_MSG_LONG_VERBOSE, "GIF animation built: %s.gif\n", Ctrl->N.prefix);
	}
	else if (Ctrl->F.mode == MOVIE_MP4) {
		/* Set up system call to ffmpeg */
		if (gmt_M_is_verbose (GMT, GMT_MSG_VERBOSE))
			sprintf (extra, "verbose");
		else
			sprintf (extra, "quiet");
		sprintf (cmd, "ffmpeg -loglevel %s -f image2 -pattern_type glob -framerate %g -y -i \"%s/%s_*.png\" -pix_fmt yuv420p %s.m4v",
			extra, Ctrl->A.rate, Ctrl->N.prefix, Ctrl->N.prefix, Ctrl->N.prefix);
		GMT_Report (API, GMT_MSG_LONG_VERBOSE, "Run: %s\n", cmd);
		if ((error = system (cmd))) {
			GMT_Report (API, GMT_MSG_NORMAL, "Running MP4 conversion returned error %d - exiting.\n", error);
			Return (GMT_RUNTIME_ERROR);
		}
		GMT_Report (API, GMT_MSG_LONG_VERBOSE, "MP4 movie built: %s.mv4\n", Ctrl->N.prefix);
	}

	if (Ctrl->E.active) {	/* Prepare the cleanup script */
		sprintf (cleanup_file, "movie_cleanup.%s", extension[Ctrl->In.mode]);
		if ((fp = fopen (cleanup_file, "w")) == NULL) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to create cleanup file %s - exiting\n", cleanup_file);
			Return (GMT_ERROR_ON_FOPEN);
		}
		set_script (fp, Ctrl->In.mode);					/* Write 1st line of a script */
		set_comment (fp, Ctrl->In.mode, "Cleanup script removes directory with frame files");
		fprintf (fp, "%s %s\n", rmdir[Ctrl->In.mode], Ctrl->N.prefix);	/* Delete the entire directory with PNG frames and tmp files */
		fprintf (fp, "%s %s\n", rmfile[Ctrl->In.mode], cleanup_file);	/* Delete the cleanup script when it finishes */
		fclose (fp);
#ifndef WIN32
		/* Set executable bit if not Windows */
		if (chmod (cleanup_file, S_IRWXU)) {
			GMT_Report (API, GMT_MSG_NORMAL, "Unable to make cleanup script executable: %s - exiting\n", cleanup_file);
			Return (GMT_RUNTIME_ERROR);
		}
#endif
		/* Run cleanup at the end */
		if ((error = system (cleanup_file))) {
			GMT_Report (API, GMT_MSG_NORMAL, "Running cleanup script %s returned error %d - exiting.\n", cleanup_file, error);
			Return (GMT_RUNTIME_ERROR);
		}
	}
	else
		GMT_Report (API, GMT_MSG_LONG_VERBOSE, "%u frame PNG files placed in directory: %s\n", n_frames, Ctrl->N.prefix);

	Return (GMT_NOERROR);
}