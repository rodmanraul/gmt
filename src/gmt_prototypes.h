/*--------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 1991-2016 by P. Wessel, W. H. F. Smith, R. Scharroo, J. Luis and F. Wobbe
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
/* gmt_prototypes.h  --  All low-level GMT lib function prototypes.
 * These are all part of the gmt_dev.h distributed functions

   Authors:	P. Wessel, W. H. F. Smith, R. Scharroo, and J. Luis
   Date:	1-OCT-2009
   Version:	5 API
*/

/*!
 * \file gmt_prototypes.h
 * \brief All low-level GMT API function prototypes.
 */

#ifndef _GMT_PROTOTYPES_H
#define _GMT_PROTOTYPES_H

#ifdef DEBUG
EXTERN_MSC void gmt_grd_dump (struct GMT_GRID_HEADER *header, float *grid, bool is_complex, char *txt);
#endif

/* Temporary redef of strdup to allow check of memory leaks due to usage of strdup */
#ifdef FISH_STRDUP_LEAKS
EXTERN_MSC char *gmt_strdup (struct GMT_CTRL *GMT, const char *s);
#endif

/* gmt_init.c: */

EXTERN_MSC int gmt_get_V (char arg);
EXTERN_MSC bool gmt_check_filearg (struct GMT_CTRL *GMT, char option, char *file, unsigned int direction, unsigned int family);
EXTERN_MSC int gmt_parse_model (struct GMT_CTRL *GMT, char option, char *in_arg, unsigned int dim, struct GMT_MODEL *M);
EXTERN_MSC struct GMT_CTRL * gmt_begin (struct GMTAPI_CTRL *API, const char *session, unsigned int pad);
EXTERN_MSC void gmt_end (struct GMT_CTRL *GMT);
EXTERN_MSC struct GMT_CTRL * gmt_begin_module (struct GMTAPI_CTRL *API, const char *lib_name, const char *mod_name, struct GMT_CTRL **Ccopy);
EXTERN_MSC void gmt_end_module (struct GMT_CTRL *GMT, struct GMT_CTRL *Ccopy);
EXTERN_MSC int gmt_init_time_system_structure (struct GMT_CTRL *GMT, struct GMT_TIME_SYSTEM *time_system);
EXTERN_MSC int gmt_init_scales (struct GMT_CTRL *GMT, unsigned int unit, double *fwd_scale, double *inv_scale, double *inch_to_unit, double *unit_to_inch, char *unit_name);
EXTERN_MSC int gmt_set_measure_unit (struct GMT_CTRL *GMT, char unit);
EXTERN_MSC char * gmt_putfill (struct GMT_CTRL *GMT, struct GMT_FILL *F);
EXTERN_MSC char * gmt_putcolor (struct GMT_CTRL *GMT, double *rgb);
EXTERN_MSC char * gmt_putrgb (struct GMT_CTRL *GMT, double *rgb);
EXTERN_MSC char * gmt_putcmyk (struct GMT_CTRL *GMT, double *cmyk);
EXTERN_MSC char * gmt_puthsv (struct GMT_CTRL *GMT, double *hsv);
EXTERN_MSC double gmt_convert_units (struct GMT_CTRL *GMT, char *value, unsigned int from_default, unsigned int target_unit);
EXTERN_MSC enum gmt_enum_units gmt_get_unit_number (struct GMT_CTRL *GMT, char unit);
EXTERN_MSC unsigned int gmt_check_scalingopt (struct GMT_CTRL *GMT, char option, char unit, char *unit_name);
EXTERN_MSC int gmt_parse_common_options (struct GMT_CTRL *GMT, char *list, char option, char *item);
EXTERN_MSC int gmt_default_error (struct GMT_CTRL *GMT, char option);
EXTERN_MSC bool gmt_get_time_system (struct GMT_CTRL *GMT, char *name, struct GMT_TIME_SYSTEM *time_system);
EXTERN_MSC int gmt_hash_lookup (struct GMT_CTRL *GMT, const char *key, struct GMT_HASH *hashnode, unsigned int n, unsigned int n_hash);
EXTERN_MSC void gmt_syntax (struct GMT_CTRL *GMT, char option);
EXTERN_MSC void gmt_cont_syntax (struct GMT_CTRL *GMT, unsigned int indent, unsigned int kind);
EXTERN_MSC void gmt_refpoint_syntax (struct GMT_CTRL *GMT, char option, char *string, unsigned int kind, unsigned int part);
EXTERN_MSC void gmt_mapscale_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_maprose_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_mapinsert_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_mappanel_syntax (struct GMT_CTRL *GMT, char option, char *string, unsigned int kind);
EXTERN_MSC void gmt_fill_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_pen_syntax (struct GMT_CTRL *GMT, char option, char *string, unsigned int mode);
EXTERN_MSC void gmt_rgb_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_inc_syntax (struct GMT_CTRL *GMT, char option, bool error);
EXTERN_MSC void gmt_label_syntax (struct GMT_CTRL *GMT, unsigned int indent, unsigned int kind);
EXTERN_MSC void gmt_dist_syntax (struct GMT_CTRL *GMT, char option, char *string);
EXTERN_MSC void gmt_vector_syntax (struct GMT_CTRL *GMT, unsigned int mode);
EXTERN_MSC void gmt_segmentize_syntax (struct GMT_CTRL *GMT, char option, unsigned int mode);
EXTERN_MSC void gmt_img_syntax (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_GSHHG_syntax (struct GMT_CTRL *GMT, char option);
EXTERN_MSC void gmt_explain_options (struct GMT_CTRL *GMT, char *options);
EXTERN_MSC void gmt_getdefaults (struct GMT_CTRL *GMT, char *this_file);
EXTERN_MSC void gmt_putdefaults (struct GMT_CTRL *GMT, char *this_file);
EXTERN_MSC int gmt_hash_init (struct GMT_CTRL *GMT, struct GMT_HASH *hashnode , char **keys, unsigned int n_hash, unsigned int n_keys);
EXTERN_MSC void gmt_extract_label (struct GMT_CTRL *GMT, char *line, char *label, struct GMT_OGR_SEG *G);
EXTERN_MSC void gmt_check_lattice (struct GMT_CTRL *GMT, double *inc, unsigned int *registration, bool *active);
EXTERN_MSC int gmt_check_binary_io (struct GMT_CTRL *GMT, uint64_t n_req);
EXTERN_MSC char * gmt_putparameter (struct GMT_CTRL *GMT, const char *keyword);
EXTERN_MSC unsigned int gmt_setparameter (struct GMT_CTRL *GMT, const char *keyword, char *value);
EXTERN_MSC void gmt_set_pad (struct GMT_CTRL *GMT, unsigned int npad);
EXTERN_MSC int gmt_get_ellipsoid (struct GMT_CTRL *GMT, char *name);
EXTERN_MSC int gmt_init_vector_param (struct GMT_CTRL *GMT, struct GMT_SYMBOL *S, bool set, bool outline, struct GMT_PEN *pen, bool do_fill, struct GMT_FILL *fill);
EXTERN_MSC int gmt_parse_vector (struct GMT_CTRL *GMT, char symbol, char *text, struct GMT_SYMBOL *S);
EXTERN_MSC bool gmt_check_region (struct GMT_CTRL *GMT, double wesn[]);
EXTERN_MSC int gmt_pickdefaults (struct GMT_CTRL *GMT, bool lines, struct GMT_OPTION *options);
EXTERN_MSC unsigned int gmt_setdefaults (struct GMT_CTRL *GMT, struct GMT_OPTION *options);
EXTERN_MSC int gmt_loaddefaults (struct GMT_CTRL *GMT, char *file);
EXTERN_MSC int gmt_parse_symbol_option (struct GMT_CTRL *GMT, char *text, struct GMT_SYMBOL *p, unsigned int mode, bool cmd);
EXTERN_MSC int gmt_message (struct GMT_CTRL *GMT, char *format, ...);
EXTERN_MSC int gmt_report_func (struct GMT_CTRL *GMT, unsigned int level, const char *source_line, const char *format, ...);
EXTERN_MSC int gmt_get_num_processors();
EXTERN_MSC void gmt_disable_i_opt (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_reenable_i_opt (struct GMT_CTRL *GMT);
#ifdef WIN32
EXTERN_MSC void gmt_setmode (struct GMT_CTRL *GMT, int direction);
#endif

/* gmt_bcr.c: */
EXTERN_MSC double gmt_bcr_get_z (struct GMT_CTRL *GMT, struct GMT_GRID *G, double xx, double yy);		/* Compute z(x,y) from bcr structure and grid */
EXTERN_MSC double gmt_bcr_get_z_fast (struct GMT_CTRL *GMT, struct GMT_GRID *G, double xx, double yy);		/* Same but without region and nan checks */
EXTERN_MSC int gmt_bcr_get_img (struct GMT_CTRL *GMT, struct GMT_IMAGE *G, double xx, double yy, unsigned char *z);		/* Compute z(x,y) from bcr structure and image */

/* gmt_customio.c: */

#ifdef HAVE_GDAL
/* Format # 22 */
EXTERN_MSC int gmt_gdalread (struct GMT_CTRL *GMT, char *gdal_filename, struct GMT_GDALREAD_IN_CTRL *prhs, struct GMT_GDALREAD_OUT_CTRL *Ctrl);
EXTERN_MSC int gmt_gdalwrite (struct GMT_CTRL *GMT, char *filename, struct GMT_GDALWRITE_CTRL *prhs);
#endif

/* gmt_fft.c: */

EXTERN_MSC int gmt_fft_set_wave (struct GMT_CTRL *GMT, unsigned int mode, struct GMT_FFT_WAVENUMBER *K);
EXTERN_MSC double gmt_fft_get_wave (uint64_t k, struct GMT_FFT_WAVENUMBER *K);
EXTERN_MSC double gmt_fft_any_wave (uint64_t k, unsigned int mode, struct GMT_FFT_WAVENUMBER *K);
EXTERN_MSC void gmt_suggest_fft_dim (struct GMT_CTRL *GMT, unsigned int nx, unsigned int ny, struct GMT_FFT_SUGGESTION *fft_sug, bool do_print);

/* gmt_grdio.c: */

EXTERN_MSC void gmt_grd_detrend (struct GMT_CTRL *GMT, struct GMT_GRID *Grid, unsigned int mode, double *a);
EXTERN_MSC void gmt_grd_minmax (struct GMT_CTRL *GMT, struct GMT_GRID *Grid, double xyz[2][3]);
EXTERN_MSC struct GMT_GRID * gmt_create_grid (struct GMT_CTRL *GMT);
EXTERN_MSC struct GMT_GRID * gmt_duplicate_grid (struct GMT_CTRL *GMT, struct GMT_GRID *G, unsigned int mode);
EXTERN_MSC void gmt_grd_init (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *header, struct GMT_OPTION *options, bool update);
EXTERN_MSC void gmt_decode_grd_h_info (struct GMT_CTRL *GMT, char *input, struct GMT_GRID_HEADER *h);
EXTERN_MSC void gmt_free_grid (struct GMT_CTRL *GMT, struct GMT_GRID **G, bool free_grid);
EXTERN_MSC void gmt_set_grdinc (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h);
EXTERN_MSC void gmt_set_grddim (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h);
EXTERN_MSC void gmt_grd_pad_on (struct GMT_CTRL *GMT, struct GMT_GRID *G, unsigned int *pad);
EXTERN_MSC void gmt_grd_pad_off (struct GMT_CTRL *GMT, struct GMT_GRID *G);
EXTERN_MSC void gmt_grd_pad_zero (struct GMT_CTRL *GMT, struct GMT_GRID *G);
EXTERN_MSC void gmt_grd_zminmax (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h, float *z);
EXTERN_MSC int gmt_read_grd_info (struct GMT_CTRL *GMT, char *file, struct GMT_GRID_HEADER *header);
EXTERN_MSC int gmt_write_grd_info (struct GMT_CTRL *GMT, char *file, struct GMT_GRID_HEADER *header);
EXTERN_MSC int gmt_read_grd (struct GMT_CTRL *GMT, char *file, struct GMT_GRID_HEADER *header, float *grid, double *wesn, unsigned int *pad, int complex_mode);
EXTERN_MSC int gmt_write_grd (struct GMT_CTRL *GMT, char *file, struct GMT_GRID_HEADER *header, float *grid, double *wesn, unsigned int *pad, int complex_mode);
EXTERN_MSC int gmt_adjust_loose_wesn (struct GMT_CTRL *GMT, double wesn[], struct GMT_GRID_HEADER *header);
EXTERN_MSC int gmt_grd_setregion (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h, double *wesn, unsigned int interpolant);
EXTERN_MSC int gmt_grd_RI_verify (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h, unsigned int mode);
EXTERN_MSC int gmt_read_img (struct GMT_CTRL *GMT, char *imgfile, struct GMT_GRID *G, double *wesn, double scale, unsigned int mode, double lat, bool init);
EXTERN_MSC int gmt_conv_intext2dbl (struct GMT_CTRL *GMT, char *record, unsigned int ncols);
EXTERN_MSC bool gmt_grd_pad_status (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *header, unsigned int *pad);
EXTERN_MSC int gmt_set_outgrid (struct GMT_CTRL *GMT, char *file, bool separate, struct GMT_GRID *G, struct GMT_GRID **Out);
EXTERN_MSC int gmt_change_grdreg (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h, unsigned int registration);
EXTERN_MSC void gmt_grd_shift (struct GMT_CTRL *GMT, struct GMT_GRID *Grid, double shift);
EXTERN_MSC void gmt_grd_set_ij_inc (struct GMT_CTRL *GMT, unsigned int nx, int *ij_inc);
EXTERN_MSC double * gmt_grd_coord (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h, int dir);
#ifdef HAVE_GDAL
EXTERN_MSC int gmt_read_image (struct GMT_CTRL *GMT, char *file, struct GMT_IMAGE *I, double *wesn,
			unsigned int *pad, unsigned int complex_mode);		/* Function to read true images via GDAL */
int gmt_read_image_info (struct GMT_CTRL *GMT, char *file, struct GMT_IMAGE *I);
#endif

#ifdef _POSTSCRIPTLIGHT_H
/* gmt_plot.c prototypes only included if postscriptlight has been included */

EXTERN_MSC char * gmt_export2proj4 (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_textpath_init (struct GMT_CTRL *GMT, struct GMT_PEN *BP, double Brgb[]);
EXTERN_MSC void gmt_draw_map_rose (struct GMT_CTRL *GMT, struct GMT_MAP_ROSE *mr);
EXTERN_MSC int gmt_draw_map_scale (struct GMT_CTRL *GMT, struct GMT_MAP_SCALE *ms);
EXTERN_MSC void gmt_draw_map_insert (struct GMT_CTRL *GMT, struct GMT_MAP_INSERT *B);
EXTERN_MSC void gmt_draw_map_panel (struct GMT_CTRL *GMT, double x, double y, unsigned int mode, struct GMT_MAP_PANEL *P);
EXTERN_MSC void gmt_geo_line (struct GMT_CTRL *GMT, double *lon, double *lat, uint64_t n);
EXTERN_MSC void gmt_geo_polygons (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S);
EXTERN_MSC void gmt_geo_ellipse (struct GMT_CTRL *GMT, double lon, double lat, double major, double minor, double azimuth);
EXTERN_MSC void gmt_geo_wedge (struct GMT_CTRL *GMT, double xlon, double xlat, double radius, char unit, double az_start, double az_stop, unsigned int mode);
EXTERN_MSC void gmt_geo_rectangle (struct GMT_CTRL *GMT, double lon, double lat, double width, double height, double azimuth);
EXTERN_MSC unsigned int gmt_geo_vector (struct GMT_CTRL *GMT, double lon0, double lat0, double azimuth, double length, struct GMT_PEN *pen, struct GMT_SYMBOL *S);
EXTERN_MSC void gmt_draw_front (struct GMT_CTRL *GMT, double x[], double y[], uint64_t n, struct GMT_FRONTLINE *f);
EXTERN_MSC void gmt_map_basemap (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_map_clip_off (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_map_clip_on (struct GMT_CTRL *GMT, double rgb[], unsigned int flag);
EXTERN_MSC void gmt_plot_line (struct GMT_CTRL *GMT, double *x, double *y, unsigned int *pen, uint64_t n, unsigned int mode);
EXTERN_MSC void gmt_setpen (struct GMT_CTRL *GMT, struct GMT_PEN *pen);
EXTERN_MSC void gmt_setfill (struct GMT_CTRL *GMT, struct GMT_FILL *fill, int outline);
EXTERN_MSC void gmt_vertical_axis (struct GMT_CTRL *GMT, unsigned int mode);
EXTERN_MSC void gmt_xy_axis (struct GMT_CTRL *GMT, double x0, double y0, double length, double val0, double val1, struct GMT_PLOT_AXIS *A, bool below, bool annotate);
EXTERN_MSC int gmt_draw_custom_symbol (struct GMT_CTRL *GMT, double x0, double y0, double size[], struct GMT_CUSTOM_SYMBOL *symbol, struct GMT_PEN *pen, struct GMT_FILL *fill, unsigned int outline);
EXTERN_MSC void gmt_contlabel_plot (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G);
EXTERN_MSC void gmt_plane_perspective (struct GMT_CTRL *GMT, int plane, double level);
EXTERN_MSC void gmt_plotcanvas (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_write_label_record (struct GMT_CTRL *GMT, FILE *fp, double x, double y, double angle, char *label, bool save_angle);
EXTERN_MSC int gmt_contlabel_save_begin (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G);
EXTERN_MSC int gmt_contlabel_save_end (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G);
EXTERN_MSC unsigned int gmt_setfont (struct GMT_CTRL *GMT, struct GMT_FONT *F);
EXTERN_MSC void gmt_plotend (struct GMT_CTRL *GMT);
EXTERN_MSC struct PSL_CTRL * gmt_plotinit (struct GMT_CTRL *GMT, struct GMT_OPTION *options);
EXTERN_MSC struct GMT_PS * gmt_duplicate_ps (struct GMT_CTRL *GMT, struct GMT_PS *P_from, unsigned int mode);
EXTERN_MSC uint64_t gmt_geo_polarcap_segment (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S, double **lon, double **lat);

#endif /* _POSTSCRIPTLIGHT_H */

/* gmt_io.c: */

EXTERN_MSC void gmt_format_abstime_output (struct GMT_CTRL *GMT, double dt, char *text);
EXTERN_MSC int gmt_ascii_output_col (struct GMT_CTRL *GMT, FILE *fp, double x, uint64_t col);
EXTERN_MSC bool gmt_z_input_is_nan_proxy (struct GMT_CTRL *GMT, unsigned int col, double value);
EXTERN_MSC bool gmt_is_a_blank_line (char *line);
EXTERN_MSC void gmt_eliminate_lon_jumps (struct GMT_CTRL *GMT, double *lon, uint64_t n_rows);
EXTERN_MSC void gmt_set_geographic (struct GMT_CTRL *GMT, unsigned int dir);
EXTERN_MSC void gmt_set_cartesian (struct GMT_CTRL *GMT, unsigned int dir);
EXTERN_MSC void gmt_set_xycolnames (struct GMT_CTRL *GMT, char *string);
EXTERN_MSC p_to_io_func GMT_get_io_ptr (struct GMT_CTRL *GMT, int direction, enum GMT_swap_direction swap, char type);
EXTERN_MSC bool gmt_is_ascii_record (struct GMT_CTRL *GMT, struct GMT_OPTION *head);
EXTERN_MSC void gmt_set_segmentheader (struct GMT_CTRL *GMT, int direction, bool true_false);
EXTERN_MSC void gmt_set_tableheader (struct GMT_CTRL *GMT, int direction, bool true_false);
EXTERN_MSC void gmt_io_binary_header (struct GMT_CTRL *GMT, FILE *fp, unsigned int dir);
EXTERN_MSC void * gmt_z_input (struct GMT_CTRL *GMT, FILE *fp, uint64_t *n, int *status);
EXTERN_MSC int gmt_z_output (struct GMT_CTRL *GMT, FILE *fp, uint64_t n, double *data);
EXTERN_MSC int gmt_get_io_type (struct GMT_CTRL *GMT, char type);
EXTERN_MSC struct GMT_QUAD * gmt_quad_init (struct GMT_CTRL *GMT, uint64_t n_items);
EXTERN_MSC void gmt_quad_reset (struct GMT_CTRL *GMT, struct GMT_QUAD *Q, uint64_t n_items);
EXTERN_MSC void gmt_quad_add (struct GMT_CTRL *GMT, struct GMT_QUAD *Q, double x);
EXTERN_MSC unsigned int gmt_quad_finalize (struct GMT_CTRL *GMT, struct GMT_QUAD *Q);
EXTERN_MSC char * gmt_fgets (struct GMT_CTRL *GMT, char *str, int size, FILE *stream);
EXTERN_MSC int gmt_fclose (struct GMT_CTRL *GMT, FILE *stream);
EXTERN_MSC int gmt_access (struct GMT_CTRL *GMT, const char *filename, int mode);		/* access wrapper */
EXTERN_MSC FILE * gmt_fopen (struct GMT_CTRL *GMT, const char *filename, const char *mode);
EXTERN_MSC char * gmt_getdatapath (struct GMT_CTRL *GMT, const char *stem, char *path, int mode);	/* Look for data file */
EXTERN_MSC char * gmt_getsharepath (struct GMT_CTRL *GMT, const char *subdir, const char *stem, const char *suffix, char *path, int mode);	/* Look for shared file */
EXTERN_MSC void gmt_write_tableheader (struct GMT_CTRL *GMT, FILE *fp, char *txt);
EXTERN_MSC void gmt_write_segmentheader (struct GMT_CTRL *GMT, FILE *fp, uint64_t n_cols);		/* Write segment header back out */
EXTERN_MSC void gmt_write_textrecord (struct GMT_CTRL *GMT, FILE *fp, char *txt);
EXTERN_MSC void gmt_ascii_format_col (struct GMT_CTRL *GMT, char *text, double x, unsigned int direction, uint64_t col);
EXTERN_MSC void gmt_lon_range_adjust (unsigned int range, double *lon);		/* Adjust the longitude given the desired range */
EXTERN_MSC void gmt_add_to_record (struct GMT_CTRL *GMT, char *record, double val, uint64_t col, unsigned int way, unsigned int sep);
EXTERN_MSC int gmt_scanf (struct GMT_CTRL *GMT, char *p, unsigned int expectation, double *val);	/* Convert strings to double, handling special formats [Data records only ] */
EXTERN_MSC int gmt_scanf_arg (struct GMT_CTRL *GMT, char *p, unsigned int expectation, double *val);	/* Convert strings to double, handling special formats [ command line only ] */
EXTERN_MSC bool gmt_not_numeric (struct GMT_CTRL *GMT, char *text);				/* Rules out _some_ text as possible numerics */
EXTERN_MSC bool gmt_parse_segment_item (struct GMT_CTRL *GMT, char *in_string, char *pattern, char *out_string);
EXTERN_MSC int gmt_set_cols (struct GMT_CTRL *GMT, unsigned int direction, uint64_t expected);
EXTERN_MSC uint64_t gmt_get_cols (struct GMT_CTRL *GMT, unsigned int direction);
EXTERN_MSC struct GMT_DATASET * gmt_create_dataset (struct GMT_CTRL *GMT, uint64_t n_tables, uint64_t n_segments, uint64_t n_rows, uint64_t n_columns, unsigned int geometry, bool alloc_only);
EXTERN_MSC struct GMT_DATATABLE * gmt_create_table (struct GMT_CTRL *GMT, uint64_t n_segments, uint64_t n_rows, uint64_t n_columns, bool alloc_only);
EXTERN_MSC void gmt_free_textset (struct GMT_CTRL *GMT, struct GMT_TEXTSET **data);
EXTERN_MSC struct GMT_TEXTSET * gmt_duplicate_textset (struct GMT_CTRL *GMT, struct GMT_TEXTSET *Din, unsigned int mode);
EXTERN_MSC void gmt_adjust_dataset (struct GMT_CTRL *GMT, struct GMT_DATASET *D, uint64_t n_columns);
EXTERN_MSC struct GMT_DATASET * gmt_alloc_dataset (struct GMT_CTRL *GMT, struct GMT_DATASET *Din, uint64_t n_rows, uint64_t n_columns, unsigned int mode);
EXTERN_MSC struct GMT_DATASET * gmt_duplicate_dataset (struct GMT_CTRL *GMT, struct GMT_DATASET *Din, unsigned int mode, unsigned int *geometry);
EXTERN_MSC struct GMT_DATATABLE * gmt_read_table (struct GMT_CTRL *GMT, void *source, unsigned int source_type, bool greenwich, unsigned int *geometry, bool use_GMT_io);
EXTERN_MSC int gmt_write_dataset (struct GMT_CTRL *GMT, void *dest, unsigned int dest_type, struct GMT_DATASET *D, bool use_GMT_io, int table);
EXTERN_MSC int gmt_alloc_segment (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S, uint64_t n_rows, uint64_t n_columns, bool first);
EXTERN_MSC void gmt_free_segment (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT **segment, enum GMT_enum_alloc alloc_mode);
EXTERN_MSC void gmt_free_table (struct GMT_CTRL *GMT, struct GMT_DATATABLE *table, enum GMT_enum_alloc alloc_mode);
EXTERN_MSC void gmt_free_dataset (struct GMT_CTRL *GMT, struct GMT_DATASET **data);
EXTERN_MSC void gmt_free_palette (struct GMT_CTRL *GMT, struct GMT_PALETTE **P);
EXTERN_MSC void gmt_free_ps (struct GMT_CTRL *GMT, struct GMT_PS **P);
EXTERN_MSC struct GMT_PS * gmt_read_ps (struct GMT_CTRL *GMT, void *source, unsigned int source_type, unsigned int mode);
EXTERN_MSC int gmt_write_ps (struct GMT_CTRL *GMT, void *dest, unsigned int dest_type, unsigned int mode, struct GMT_PS *P);
EXTERN_MSC void gmt_copy_ps (struct GMT_CTRL *GMT, struct GMT_PS *P_copy, struct GMT_PS *P_obj);
EXTERN_MSC int gmt_append_ogr_item (struct GMT_CTRL *GMT, char *name, unsigned int type, struct GMT_OGR *S);
EXTERN_MSC void gmt_write_ogr_header (FILE *fp, struct GMT_OGR *G);

EXTERN_MSC struct GMT_IMAGE * gmt_create_image (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_free_image (struct GMT_CTRL *GMT, struct GMT_IMAGE **I, bool free_image);
EXTERN_MSC struct GMT_MATRIX * gmt_create_matrix (struct GMT_CTRL *GMT, uint64_t n_layers, unsigned int direction);
EXTERN_MSC void gmt_free_matrix (struct GMT_CTRL *GMT, struct GMT_MATRIX **M, bool free_matrix);
EXTERN_MSC struct GMT_VECTOR * gmt_create_vector (struct GMT_CTRL *GMT, uint64_t n_columns, unsigned int direction);
EXTERN_MSC void gmt_free_vector (struct GMT_CTRL *GMT, struct GMT_VECTOR **V, bool free_vector);
EXTERN_MSC int gmt_load_aspatial_string (struct GMT_CTRL *GMT, struct GMT_OGR *G, uint64_t col, char out[GMT_BUFSIZ]);
EXTERN_MSC double gmt_get_aspatial_value (struct GMT_CTRL *GMT, int col, struct GMT_DATASEGMENT *S);
EXTERN_MSC void gmt_set_seg_minmax (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S);
EXTERN_MSC void gmt_set_seg_polar (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S);
EXTERN_MSC int gmt_determine_pole (struct GMT_CTRL *GMT, double *lon, double *lat, uint64_t n);
EXTERN_MSC void gmt_skip_xy_duplicates (struct GMT_CTRL *GMT, bool mode);
EXTERN_MSC void gmt_duplicate_ogr_seg (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S_to, struct GMT_DATASEGMENT *S_from);
EXTERN_MSC struct GMT_DATASEGMENT * gmt_duplicate_segment (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *Sin);
EXTERN_MSC char * gmt_trim_segheader (struct GMT_CTRL *GMT, char *line);
EXTERN_MSC int gmt_alloc_univector (struct GMT_CTRL *GMT, union GMT_UNIVECTOR *u, unsigned int type, uint64_t n_rows);
EXTERN_MSC int gmt_parse_segment_header (struct GMT_CTRL *GMT, char *header, struct GMT_PALETTE *P, bool *use_fill, struct GMT_FILL *fill, struct GMT_FILL *def_fill, bool *use_pen, struct GMT_PEN *pen, struct GMT_PEN *def_pen, unsigned int def_outline, struct GMT_OGR_SEG *G);
EXTERN_MSC int gmt_parse_z_io (struct GMT_CTRL *GMT, char *txt, struct GMT_PARSE_Z_IO *z);
EXTERN_MSC int gmt_init_z_io (struct GMT_CTRL *GMT, char format[], bool repeat[], enum GMT_swap_direction swab, off_t skip, char type, struct GMT_Z_IO *r);
EXTERN_MSC int gmt_set_z_io (struct GMT_CTRL *GMT, struct GMT_Z_IO *r, struct GMT_GRID *G);
EXTERN_MSC void gmt_check_z_io (struct GMT_CTRL *GMT, struct GMT_Z_IO *r, struct GMT_GRID *G);
EXTERN_MSC void gmt_init_io_columns (struct GMT_CTRL *GMT, unsigned int dir);
EXTERN_MSC void gmt_write_newheaders (struct GMT_CTRL *GMT, FILE *fp, uint64_t n_columns);
EXTERN_MSC char **gmt_get_dir_list (struct GMT_CTRL *GMT, char *path, char *ext);
EXTERN_MSC void gmt_free_dir_list (struct GMT_CTRL *GMT, char ***list);
EXTERN_MSC void gmt_assign_segment (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S, uint64_t n_rows, uint64_t n_columns);
EXTERN_MSC double *gmt_assign_vector (struct GMT_CTRL *GMT, uint64_t n_rows, uint64_t col);
EXTERN_MSC bool gmt_input_is_bin (struct GMT_CTRL *GMT, const char *filename);
EXTERN_MSC bool gmt_skip_output (struct GMT_CTRL *GMT, double *cols, uint64_t n_cols);
EXTERN_MSC void gmt_set_dataset_minmax (struct GMT_CTRL *GMT, struct GMT_DATASET *D);
EXTERN_MSC void gmt_set_textset_minmax (struct GMT_CTRL *GMT, struct GMT_TEXTSET *D);

/* gmt_M_memory.c: */

EXTERN_MSC void gmt_prep_tmp_arrays (struct GMT_CTRL *GMT, size_t row, size_t n_cols);
EXTERN_MSC void gmt_free_tmp_arrays (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_set_meminc (struct GMT_CTRL *GMT, size_t increment);
EXTERN_MSC void gmt_reset_meminc (struct GMT_CTRL *GMT);
EXTERN_MSC void * gmt_M_memory_func (struct GMT_CTRL *GMT, void *prev_addr, size_t nelem, size_t size, bool align, const char *where);
EXTERN_MSC void gmt_free_func (struct GMT_CTRL *GMT, void *addr, bool align, const char *where);

/* gmt_support.c: */

EXTERN_MSC void gmt_init_pen (struct GMT_CTRL *GMT, struct GMT_PEN *pen, double width);
EXTERN_MSC void gmt_init_fill (struct GMT_CTRL *GMT, struct GMT_FILL *fill, double r, double g, double b);
EXTERN_MSC int gmt_intpol (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, uint64_t m, double *u, double *v, int mode);
EXTERN_MSC unsigned int gmt_inonout (struct GMT_CTRL *GMT, double x, double y, const struct GMT_DATASEGMENT *S);
EXTERN_MSC void gmt_just_to_lonlat (struct GMT_CTRL *GMT, int justify, bool geo, double *x, double *y);
EXTERN_MSC struct GMT_REFPOINT * gmt_get_refpoint (struct GMT_CTRL *GMT, char *arg);
EXTERN_MSC void gmt_set_refpoint (struct GMT_CTRL *GMT, struct GMT_REFPOINT *A);
EXTERN_MSC void gmt_free_refpoint (struct GMT_CTRL *GMT, struct GMT_REFPOINT **Ap);
EXTERN_MSC void gmt_flip_azim_d (struct GMT_CTRL *GMT, double *azim);
EXTERN_MSC void gmt_flip_angle_d (struct GMT_CTRL *GMT, double *angle);
EXTERN_MSC struct GMT_DATATABLE *gmt_make_profile (struct GMT_CTRL *GMT, char option, char *args, bool resample, bool project, bool get_distances, double step, enum GMT_enum_track mode, double xyz[2][3]);
EXTERN_MSC unsigned int gmtlib_split_line_at_dateline (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S, struct GMT_DATASEGMENT ***Lout);
EXTERN_MSC unsigned int gmt_split_poly_at_dateline (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S, struct GMT_DATASEGMENT ***Lout);
EXTERN_MSC bool gmt_x_is_outside (struct GMT_CTRL *GMT, double *x, double left, double right);
EXTERN_MSC void gmt_set_xy_domain (struct GMT_CTRL *GMT, double wesn_extended[], struct GMT_GRID_HEADER *h);
EXTERN_MSC int gmt_BC_init (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h);
EXTERN_MSC int gmt_grd_BC_set (struct GMT_CTRL *GMT, struct GMT_GRID *G, unsigned int direction);
EXTERN_MSC void gmtlib_inplace_transpose (float *A, unsigned int n_rows, unsigned int n_cols);
EXTERN_MSC struct GMT_PALETTE * gmt_truncate_cpt (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, double z_low, double z_high);
EXTERN_MSC void gmt_free_int_selection (struct GMT_CTRL *GMT, struct GMT_INT_SELECTION **S);
EXTERN_MSC struct GMT_INT_SELECTION * gmt_set_int_selection (struct GMT_CTRL *GMT, char *item);
EXTERN_MSC bool gmt_get_int_selection (struct GMT_CTRL *GMT, struct GMT_INT_SELECTION *S, uint64_t this);
EXTERN_MSC void gmt_free_text_selection (struct GMT_CTRL *GMT, struct GMT_TEXT_SELECTION **S);
EXTERN_MSC bool gmt_get_text_selection (struct GMT_CTRL *GMT, struct GMT_TEXT_SELECTION *S, struct GMT_DATASEGMENT *T, bool last_match);
EXTERN_MSC struct GMT_TEXT_SELECTION * gmt_set_text_selection (struct GMT_CTRL *GMT, char *arg);
EXTERN_MSC int gmt_flip_justify (struct GMT_CTRL *GMT, unsigned int justify);
EXTERN_MSC int gmt_get_pair (struct GMT_CTRL *GMT, char *string, unsigned int mode, double par[]);
EXTERN_MSC void gmt_centroid (struct GMT_CTRL *GMT, double x[], double y[], uint64_t n, double *pos, int geo);
EXTERN_MSC void gmt_decorated_line (struct GMT_CTRL *GMT, double **xxx, double **yyy, uint64_t nn, struct GMT_DECORATE *G, struct GMT_TEXTSET *D, uint64_t seg);
EXTERN_MSC unsigned int gmt_trim_line (struct GMT_CTRL *GMT, double **x, double **yy, uint64_t *nn, struct GMT_PEN *P);
EXTERN_MSC int gmt_detrend (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, double increment, double *intercept, double *slope, int mode);
EXTERN_MSC void gmt_str_toupper (char *string);
EXTERN_MSC char * gmt_memory_use (size_t bytes);
EXTERN_MSC double gmt_conv_distance (struct GMT_CTRL *GMT, double value, char in_unit, char out_unit);

EXTERN_MSC int gmt_image_BC_set (struct GMT_CTRL *GMT, struct GMT_IMAGE *I);
EXTERN_MSC bool gmt_y_out_of_bounds (struct GMT_CTRL *GMT, int *j, struct GMT_GRID_HEADER *h, bool *wrap_180);
EXTERN_MSC bool gmt_x_out_of_bounds (struct GMT_CTRL *GMT, int *i, struct GMT_GRID_HEADER *h, bool wrap_180);
EXTERN_MSC bool gmt_row_col_out_of_bounds (struct GMT_CTRL *GMT, double *in, struct GMT_GRID_HEADER *h, unsigned int *row, unsigned int *col);
EXTERN_MSC int gmt_list_cpt (struct GMT_CTRL *GMT, char option);
EXTERN_MSC struct GMT_PALETTE * gmt_sample_cpt (struct GMT_CTRL *GMT, struct GMT_PALETTE *Pin, double z[], int nz, bool continuous, bool reverse, bool log_mode, bool no_inter);
EXTERN_MSC int gmt_write_cpt (struct GMT_CTRL *GMT, void *dest, unsigned int dest_type, unsigned int cpt_flags, struct GMT_PALETTE *P);
EXTERN_MSC void gmt_cpt_transparency (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, double transparency, unsigned int mode);
EXTERN_MSC void gmt_copy_palette (struct GMT_CTRL *GMT, struct GMT_PALETTE *P_to, struct GMT_PALETTE *P_from);
EXTERN_MSC int gmt_contlabel_info (struct GMT_CTRL *GMT, char flag, char *txt, struct GMT_CONTOUR *G);
EXTERN_MSC void gmt_contlabel_init (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G, unsigned int mode);
EXTERN_MSC int gmt_contlabel_specs (struct GMT_CTRL *GMT, char *txt, struct GMT_CONTOUR *G);
EXTERN_MSC void gmt_decorate_init (struct GMT_CTRL *GMT, struct GMT_DECORATE *G, unsigned int mode);
EXTERN_MSC int gmt_decorate_specs (struct GMT_CTRL *GMT, char *txt, struct GMT_DECORATE *D);
EXTERN_MSC int gmt_decorate_info (struct GMT_CTRL *GMT, char flag, char *txt, struct GMT_DECORATE *D);
EXTERN_MSC int gmt_contlabel_prep (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G, double xyz[2][3]);
EXTERN_MSC int gmt_decorate_prep (struct GMT_CTRL *GMT, struct GMT_DECORATE *G, double xyz[2][3]);
EXTERN_MSC void gmt_contlabel_free (struct GMT_CTRL *GMT, struct GMT_CONTOUR *G);
EXTERN_MSC void gmt_hold_contour (struct GMT_CTRL *GMT, double **xx, double **yy, uint64_t nn, double zval, char *label, char ctype, double cangle, bool closed, bool contour, struct GMT_CONTOUR *G);
EXTERN_MSC void gmt_x_free (struct GMT_CTRL *GMT, struct GMT_XOVER *X);
EXTERN_MSC int gmt_init_track (struct GMT_CTRL *GMT, double y[], uint64_t n, struct GMT_XSEGMENT **S);
EXTERN_MSC uint64_t gmt_crossover (struct GMT_CTRL *GMT, double xa[], double ya[], uint64_t sa[], struct GMT_XSEGMENT A[], uint64_t na, double xb[], double yb[], uint64_t sb[], struct GMT_XSEGMENT B[], uint64_t nb, bool internal, bool geo, struct GMT_XOVER *X);
EXTERN_MSC void * gmt_M_malloc_func (struct GMT_CTRL *GMT, void *ptr, size_t n, size_t *n_alloc, size_t element_size, const char *where);
EXTERN_MSC char * gmt_make_filename (struct GMT_CTRL *GMT, char *template, unsigned int fmt[], double z, bool closed, unsigned int count[]);
EXTERN_MSC void gmt_str_setcase (struct GMT_CTRL *GMT, char *value, int mode);
EXTERN_MSC char * gmt_putusername (struct GMT_CTRL *GMT);
EXTERN_MSC unsigned int * gmt_prep_nodesearch (struct GMT_CTRL *GMT, struct GMT_GRID *G, double radius, unsigned int mode, unsigned int *d_row, unsigned int *actual_max_d_col);
EXTERN_MSC struct GMT_PALETTE * gmt_get_cpt (struct GMT_CTRL *GMT, char *file, enum GMT_enum_cpt mode, double zmin, double zmax);
EXTERN_MSC unsigned int gmt_gcd_euclid (unsigned int a, unsigned int b);
EXTERN_MSC unsigned int gmt_optimal_dim_for_surface (struct GMT_CTRL *GMT, unsigned int factors[], unsigned int nx, unsigned int ny, struct GMT_SURFACE_SUGGESTION **S);
EXTERN_MSC int gmt_best_dim_choice (struct GMT_CTRL *GMT, unsigned int mode, unsigned int in_dim[], unsigned int out_dim[]);
EXTERN_MSC void gmt_sprintf_float (char *string, char *format, double x);
EXTERN_MSC void gmt_enable_threads (struct GMT_CTRL *GMT);
EXTERN_MSC unsigned int gmt_validate_modifiers (struct GMT_CTRL *GMT, const char *string, const char option, const char *valid_modifiers);
EXTERN_MSC double gmt_pol_area (double x[], double y[], uint64_t n);
EXTERN_MSC void gmt_adjust_refpoint (struct GMT_CTRL *GMT, struct GMT_REFPOINT *ref, double dim[], double off[], int justify, int anchor);
EXTERN_MSC unsigned int gmt_parse_segmentize (struct GMT_CTRL *GMT, char option, char *in_arg, unsigned int mode, struct GMT_SEGMENTIZE *S);
EXTERN_MSC void gmt_symbol_free (struct GMT_CTRL *GMT, struct GMT_SYMBOL *S);

/* gmt_calclock.c: */

EXTERN_MSC double gmt_rdc2dt (struct GMT_CTRL *GMT, int64_t rd, double secs);
EXTERN_MSC void gmt_dt2rdc (struct GMT_CTRL *GMT, double t, int64_t *rd, double *s);
EXTERN_MSC int64_t gmt_rd_from_gymd (struct GMT_CTRL *GMT, int gy, int gm, int gd);
EXTERN_MSC void gmt_format_calendar (struct GMT_CTRL *GMT, char *date, char *clock, struct GMT_DATE_IO *D, struct GMT_CLOCK_IO *W, bool upper, unsigned int kind, double dt);
EXTERN_MSC void gmt_gcal_from_rd (struct GMT_CTRL *GMT, int64_t rd, struct GMT_GCAL *gcal);

/* gmt_map.c: */

EXTERN_MSC bool gmt_cart_outside (struct GMT_CTRL *GMT, double x, double y);
EXTERN_MSC void gmt_auto_frame_interval (struct GMT_CTRL *GMT, unsigned int axis, unsigned int item);
EXTERN_MSC double gmt_az_backaz (struct GMT_CTRL *GMT, double lonE, double latE, double lonS, double latS, bool baz);
EXTERN_MSC double gmt_distance (struct GMT_CTRL *GMT, double lonS, double latS, double lonE, double latE);
EXTERN_MSC double gmt_azim_to_angle (struct GMT_CTRL *GMT, double lon, double lat, double c, double azim);
EXTERN_MSC uint64_t gmt_clip_to_map (struct GMT_CTRL *GMT, double *lon, double *lat, uint64_t np, double **x, double **y);
EXTERN_MSC uint64_t gmt_compact_line (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, int pen_flag, int *pen);
EXTERN_MSC uint64_t gmt_geo_to_xy_line (struct GMT_CTRL *GMT, double *lon, double *lat, uint64_t n);
EXTERN_MSC uint64_t gmt_graticule_path (struct GMT_CTRL *GMT, double **x, double **y, int dir, bool check, double w, double e, double s, double n);
EXTERN_MSC int gmt_grd_project (struct GMT_CTRL *GMT, struct GMT_GRID *I, struct GMT_GRID *O, bool inverse);
EXTERN_MSC int gmt_img_project (struct GMT_CTRL *GMT, struct GMT_IMAGE *I, struct GMT_IMAGE *O, bool inverse);
EXTERN_MSC uint64_t gmt_map_clip_path (struct GMT_CTRL *GMT, double **x, double **y, bool *donut);
EXTERN_MSC bool gmt_map_outside (struct GMT_CTRL *GMT, double lon, double lat);
EXTERN_MSC bool gmt_geo_to_xy (struct GMT_CTRL *GMT, double lon, double lat, double *x, double *y);
EXTERN_MSC bool gmt_geo_to_xy_noshift (struct GMT_CTRL *GMT, double lon, double lat, double *x, double *y);
EXTERN_MSC void gmt_geoz_to_xy (struct GMT_CTRL *GMT, double x, double y, double z, double *x_out, double *y_out);
EXTERN_MSC int gmt_project_init (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *header, double *inc, unsigned int nx, unsigned int ny, unsigned int dpi, unsigned int offset);
EXTERN_MSC int gmt_map_setup (struct GMT_CTRL *GMT, double wesn[]);
EXTERN_MSC double gmt_x_to_xx (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_y_to_yy (struct GMT_CTRL *GMT, double y);
EXTERN_MSC double gmt_z_to_zz (struct GMT_CTRL *GMT, double z);
EXTERN_MSC void gmt_xy_to_geo (struct GMT_CTRL *GMT, double *lon, double *lat, double x, double y);
EXTERN_MSC void gmt_xy_to_geo_noshift (struct GMT_CTRL *GMT, double *lon, double *lat, double x, double y);
EXTERN_MSC void gmt_xyz_to_xy (struct GMT_CTRL *GMT, double x, double y, double z, double *x_out, double *y_out);
EXTERN_MSC double * gmt_dist_array (struct GMT_CTRL *GMT, double x[], double y[], uint64_t n, bool cumulative);
EXTERN_MSC double * gmt_dist_array_2 (struct GMT_CTRL *GMT, double x[], double y[], uint64_t n, double scale, int dist_flag);
EXTERN_MSC uint64_t gmt_map_truncate (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, uint64_t start, int side);
EXTERN_MSC unsigned int gmt_init_distaz (struct GMT_CTRL *GMT, char c, unsigned int mode, unsigned int type);
EXTERN_MSC bool gmt_near_lines (struct GMT_CTRL *GMT, double lon, double lat, struct GMT_DATATABLE *T, unsigned int return_mindist, double *dist_min, double *x_near, double *y_near);
EXTERN_MSC bool gmt_near_a_line (struct GMT_CTRL *GMT, double lon, double lat, uint64_t seg, struct GMT_DATASEGMENT *S, unsigned int return_mindist, double *dist_min, double *x_near, double *y_near);
EXTERN_MSC bool gmt_near_a_point (struct GMT_CTRL *GMT, double x, double y, struct GMT_DATATABLE *T, double dist);
EXTERN_MSC double gmt_great_circle_dist_meter (struct GMT_CTRL *GMT, double x0, double y0, double x1, double y1);
EXTERN_MSC double gmt_lat_swap (struct GMT_CTRL *GMT, double lat, unsigned int itype);
EXTERN_MSC double gmt_mindist_to_point (struct GMT_CTRL *GMT, double lon, double lat, struct GMT_DATATABLE *T, uint64_t *id);
EXTERN_MSC bool gmt_UTMzone_to_wesn (struct GMT_CTRL *GMT, unsigned int zone_x, char zone_y, int hemi, double wesn[]);
EXTERN_MSC void gmt_ECEF_forward (struct GMT_CTRL *GMT, double in[], double out[]);
EXTERN_MSC void gmt_ECEF_inverse (struct GMT_CTRL *GMT, double in[], double out[]);
EXTERN_MSC void gmt_ECEF_init (struct GMT_CTRL *GMT, struct GMT_DATUM *D);
EXTERN_MSC void gmt_datum_init (struct GMT_CTRL *GMT, struct GMT_DATUM *from, struct GMT_DATUM *to, bool heights);
EXTERN_MSC int gmt_set_datum (struct GMT_CTRL *GMT, char *text, struct GMT_DATUM *D);
EXTERN_MSC void gmt_conv_datum (struct GMT_CTRL *GMT, double in[], double out[]);
EXTERN_MSC struct GMT_DATASEGMENT * gmt_get_smallcircle (struct GMT_CTRL *GMT, double plon, double plat, double colat, uint64_t m);

/* gmt_shore.c: */

EXTERN_MSC int gmt_set_levels (struct GMT_CTRL *GMT, char *info, struct GMT_SHORE_SELECT *I);
EXTERN_MSC int gmt_get_shore_bin (struct GMT_CTRL *GMT, unsigned int b, struct GMT_SHORE *c);
EXTERN_MSC int gmt_get_br_bin (struct GMT_CTRL *GMT, unsigned int b, struct GMT_BR *c, unsigned int *level, unsigned int n_levels);
EXTERN_MSC void gmt_free_shore_polygons (struct GMT_CTRL *GMT, struct GMT_GSHHS_POL *p, unsigned int n);
EXTERN_MSC void gmt_free_shore (struct GMT_CTRL *GMT, struct GMT_SHORE *c);
EXTERN_MSC void gmt_free_br (struct GMT_CTRL *GMT, struct GMT_BR *c);
EXTERN_MSC void gmt_shore_cleanup (struct GMT_CTRL *GMT, struct GMT_SHORE *c);
EXTERN_MSC void gmt_br_cleanup (struct GMT_CTRL *GMT, struct GMT_BR *c);
EXTERN_MSC int gmt_init_shore (struct GMT_CTRL *GMT, char res, struct GMT_SHORE *c, double wesn[], struct GMT_SHORE_SELECT *I);
EXTERN_MSC int gmt_init_br (struct GMT_CTRL *GMT, char which, char res, struct GMT_BR *c, double wesn[]);
EXTERN_MSC int gmt_assemble_shore (struct GMT_CTRL *GMT, struct GMT_SHORE *c, int dir, bool assemble, double west, double east, struct GMT_GSHHS_POL **pol);
EXTERN_MSC int gmt_assemble_br (struct GMT_CTRL *GMT, struct GMT_BR *c, bool shift, double edge, struct GMT_GSHHS_POL **pol);
EXTERN_MSC int gmt_prep_shore_polygons (struct GMT_CTRL *GMT, struct GMT_GSHHS_POL **p, unsigned int np, bool sample, double step, int anti_bin);
EXTERN_MSC int gmt_set_resolution (struct GMT_CTRL *GMT, char *res, char opt);
EXTERN_MSC char gmt_shore_adjust_res (struct GMT_CTRL *GMT, char res);
EXTERN_MSC struct GMT_DATASET * gmt_get_gshhg_lines (struct GMT_CTRL *GMT, double wesn[], char res, struct GMT_SHORE_SELECT *A);
EXTERN_MSC int gmt_shore_level_at_point (struct GMT_CTRL *GMT, struct GMT_SHORE *c, int inside, double lon, double lat);

/* gmt_vector.c: */

EXTERN_MSC int gmt_chol_dcmp (struct GMT_CTRL *GMT, double *a, double *d, double *cond, int nr, int n);
EXTERN_MSC void gmt_chol_recover (struct GMT_CTRL *GMT, double *a, double *d, int nr, int n, int nerr, bool donly);
EXTERN_MSC void gmt_chol_solv (struct GMT_CTRL *GMT, double *a, double *x, double *y, int nr, int n);
EXTERN_MSC void gmt_set_tbl_minmax (struct GMT_CTRL *GMT, struct GMT_DATATABLE *T);
EXTERN_MSC void gmt_matrix_vect_mult (struct GMT_CTRL *GMT, unsigned int dim, double a[3][3], double b[3], double c[3]);
EXTERN_MSC void gmt_make_rot_matrix (struct GMT_CTRL *GMT, double lonp, double latp, double w, double R[3][3]);
EXTERN_MSC void gmt_make_rot_matrix2 (struct GMT_CTRL *GMT, double E[3], double w, double R[3][3]);

/* gmt_support.c: */

EXTERN_MSC void gmt_sort_array (struct GMT_CTRL *GMT, void *base, uint64_t n, unsigned int type);
EXTERN_MSC bool gmt_polygon_is_open (struct GMT_CTRL *GMT, double x[], double y[], uint64_t n);
EXTERN_MSC int gmt_polygon_centroid (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, double *Cx, double *Cy);
EXTERN_MSC int gmt_get_distance (struct GMT_CTRL *GMT, char *line, double *dist, char *unit);
EXTERN_MSC uint64_t gmt_contours (struct GMT_CTRL *GMT, struct GMT_GRID *Grid, unsigned int smooth_factor, unsigned int int_scheme, int orient, unsigned int *edge, bool *first, double **x, double **y);
EXTERN_MSC int gmt_get_format (struct GMT_CTRL *GMT, double interval, char *unit, char *prefix, char *format);
EXTERN_MSC int gmt_get_index (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, double value);
EXTERN_MSC int gmt_get_rgb_from_z (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, double value, double *rgb);
EXTERN_MSC int gmt_get_fill_from_z (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, double value, struct GMT_FILL *fill);
EXTERN_MSC int gmt_get_rgbtxt_from_z (struct GMT_CTRL *GMT, struct GMT_PALETTE *P, char *text);
EXTERN_MSC bool gmt_getfill (struct GMT_CTRL *GMT, char *line, struct GMT_FILL *fill);
EXTERN_MSC bool gmt_getinc (struct GMT_CTRL *GMT, char *line, double inc[]);
EXTERN_MSC int gmt_getincn (struct GMT_CTRL *GMT, char *line, double inc[], unsigned int n);
EXTERN_MSC int gmt_getfont (struct GMT_CTRL *GMT, char *line, struct GMT_FONT *F);
EXTERN_MSC bool gmt_getpen (struct GMT_CTRL *GMT, char *line, struct GMT_PEN *pen);
EXTERN_MSC void gmt_freepen (struct GMT_CTRL *GMT, struct GMT_PEN *pen);
EXTERN_MSC bool gmt_getrgb (struct GMT_CTRL *GMT, char *line, double *rgb);
EXTERN_MSC int gmt_getrose (struct GMT_CTRL *GMT, char option, char *text, struct GMT_MAP_ROSE *mr);
EXTERN_MSC int gmt_getscale (struct GMT_CTRL *GMT, char option, char *text, struct GMT_MAP_SCALE *ms);
EXTERN_MSC int gmt_getinsert (struct GMT_CTRL *GMT, char option, char *text, struct GMT_MAP_INSERT *B);
EXTERN_MSC int gmt_getpanel (struct GMT_CTRL *GMT, char option, char *text, struct GMT_MAP_PANEL **P);
EXTERN_MSC char * gmt_putfont (struct GMT_CTRL *GMT, struct GMT_FONT *F);
EXTERN_MSC char * gmt_putpen (struct GMT_CTRL *GMT, struct GMT_PEN *pen);
EXTERN_MSC int gmt_intpol (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, uint64_t m, double *u, double *v, int mode);
EXTERN_MSC int gmt_just_decode (struct GMT_CTRL *GMT, char *key, int def);
EXTERN_MSC unsigned int gmt_minmaxinc_verify (struct GMT_CTRL *GMT, double min, double max, double inc, double slop);
EXTERN_MSC unsigned int gmt_get_arc (struct GMT_CTRL *GMT, double x0, double y0, double r, double dir1, double dir2, double **x, double **y);
EXTERN_MSC unsigned int gmt_non_zero_winding (struct GMT_CTRL *GMT, double xp, double yp, double *x, double *y, uint64_t n_path);
EXTERN_MSC unsigned int gmt_getmodopt (struct GMT_CTRL *GMT, const char *string, const char *sep, unsigned int *pos, char *token);
EXTERN_MSC unsigned int gmt_verify_expectations (struct GMT_CTRL *GMT, unsigned int wanted, unsigned int got, char *item);
EXTERN_MSC void gmt_RI_prepare (struct GMT_CTRL *GMT, struct GMT_GRID_HEADER *h);
EXTERN_MSC struct GMT_DATASEGMENT * gmt_prepare_contour (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, double z);
EXTERN_MSC void gmt_get_plot_array (struct GMT_CTRL *GMT);
EXTERN_MSC void gmt_illuminate (struct GMT_CTRL *GMT, double intensity, double *rgb);
EXTERN_MSC int gmt_colorname2index (struct GMT_CTRL *GMT, char *name);
EXTERN_MSC void gmt_list_custom_symbols (struct GMT_CTRL *GMT);
EXTERN_MSC struct GMT_PALETTE * gmt_read_cpt (struct GMT_CTRL *GMT, void *source, unsigned int source_type, unsigned int cpt_flags);
EXTERN_MSC void gmt_smart_justify (struct GMT_CTRL *GMT, int just, double angle, double dx, double dy, double *x_shift, double *y_shift, unsigned int mode);
EXTERN_MSC struct GMT_DATASET * gmt_resample_data (struct GMT_CTRL *GMT, struct GMT_DATASET *Din, double along_ds, unsigned int mode, unsigned int ex_cols, enum GMT_enum_track smode);
EXTERN_MSC struct GMT_DATASET * gmt_crosstracks (struct GMT_CTRL *GMT, struct GMT_DATASET *Din, double cross_length, double across_ds, uint64_t n_cols, unsigned int mode);
EXTERN_MSC uint64_t gmt_resample_path (struct GMT_CTRL *GMT, double **x, double **y, uint64_t n_in, double step_out, enum GMT_enum_track mode);
EXTERN_MSC bool gmt_crossing_dateline (struct GMT_CTRL *GMT, struct GMT_DATASEGMENT *S);
EXTERN_MSC struct GMT_DATASET * gmt_segmentize_data (struct GMT_CTRL *GMT, struct GMT_DATASET *Din, struct GMT_SEGMENTIZE *S);
EXTERN_MSC int gmt_err_func (struct GMT_CTRL *GMT, int err, bool fail, char *file, const char *where);
EXTERN_MSC int64_t gmt_delaunay (struct GMT_CTRL *GMT, double *x_in, double *y_in, uint64_t n, int **link);
EXTERN_MSC void gmt_delaunay_free (struct GMT_CTRL *GMT, int **link);
EXTERN_MSC unsigned int gmt_get_prime_factors (struct GMT_CTRL *GMT, uint64_t n, unsigned int *f);
EXTERN_MSC int64_t gmt_voronoi (struct GMT_CTRL *GMT, double *x_in, double *y_in, uint64_t n, double *we, double **x_out, double **y_out);

/* gmt_vector.c: */

EXTERN_MSC void gmt_cart_to_geo (struct GMT_CTRL *GMT, double *lat, double *lon, double *a, bool degrees);
EXTERN_MSC void gmt_n_cart_to_geo (struct GMT_CTRL *GMT, uint64_t n, double *x, double *y, double *z, double *lon, double *lat);
EXTERN_MSC void gmt_geo_to_cart (struct GMT_CTRL *GMT, double lat, double lon, double *a, bool degrees);
EXTERN_MSC void gmt_add3v (struct GMT_CTRL *GMT, double *a, double *b, double *c);
EXTERN_MSC void gmt_sub3v (struct GMT_CTRL *GMT, double *a, double *b, double *c);
EXTERN_MSC double gmt_dot3v (struct GMT_CTRL *GMT, double *a, double *b);
EXTERN_MSC double gmt_dot2v (struct GMT_CTRL *GMT, double *a, double *b);
EXTERN_MSC double gmt_mag3v (struct GMT_CTRL *GMT, double *a);
EXTERN_MSC void gmt_cross3v (struct GMT_CTRL *GMT, double *a, double *b, double *c);
EXTERN_MSC void gmt_normalize3v (struct GMT_CTRL *GMT, double *a);
EXTERN_MSC void gmt_normalize2v (struct GMT_CTRL *GMT, double *a);
EXTERN_MSC void gmt_set_line_resampling (struct GMT_CTRL *GMT, bool active, unsigned int mode);
EXTERN_MSC uint64_t gmt_fix_up_path (struct GMT_CTRL *GMT, double **a_lon, double **a_lat, uint64_t n, double step, unsigned int mode);
EXTERN_MSC int gmt_jacobi (struct GMT_CTRL *GMT, double *a, unsigned int n, unsigned int m, double *d, double *v, double *b, double *z, unsigned int *nrots);
EXTERN_MSC int gmt_gauss (struct GMT_CTRL *GMT, double *a, double *vec, unsigned int n, unsigned int nstore, bool itriag);
EXTERN_MSC int gmt_gaussjordan (struct GMT_CTRL *GMT, double *a, unsigned int n, double *b);
EXTERN_MSC int gmt_svdcmp (struct GMT_CTRL *GMT, double *a, unsigned int m, unsigned int n, double *w, double *v);
EXTERN_MSC int gmt_solve_svd (struct GMT_CTRL *GMT, double *u, unsigned int m, unsigned int n, double *v, double *w, double *b, unsigned int k, double *x, double *cutoff, unsigned int mode);
EXTERN_MSC void gmt_polar_to_cart (struct GMT_CTRL *GMT, double r, double theta, double *a, bool degrees);
EXTERN_MSC void gmt_cart_to_polar (struct GMT_CTRL *GMT, double *r, double *theta, double *a, bool degrees);

/* From gmt_parse.c */
/* This macro is called via each modules Return macro so API and options are set */
#define gmt_M_free_options(mode) {if (mode >= 0 && GMT_Destroy_Options (API, &options) != GMT_OK) exit (EXIT_FAILURE);}

/* From gmt_api.c */
EXTERN_MSC struct GMTAPI_CTRL *gmt_get_api_ptr (struct GMTAPI_CTRL *ptr);
EXTERN_MSC void gmt_show_name_and_purpose (void *API, const char *name, const char *component, const char *purpose);

/* From gmt_stat.c */
EXTERN_MSC double gmt_bei (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_ber (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_kei (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_ker (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_plm (struct GMT_CTRL *GMT, int l, int m, double x);
EXTERN_MSC double gmt_plm_bar (struct GMT_CTRL *GMT, int l, int m, double x, bool ortho);
EXTERN_MSC void gmt_plm_bar_all (struct GMT_CTRL *GMT, int lmax, double x, bool ortho, double *plm);
EXTERN_MSC double gmt_factorial (struct GMT_CTRL *GMT, int n);
EXTERN_MSC double gmt_i0 (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_i1 (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_in (struct GMT_CTRL *GMT, unsigned int n, double x);
EXTERN_MSC double gmt_k0 (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_k1 (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_kn (struct GMT_CTRL *GMT, unsigned int n, double x);
EXTERN_MSC double gmt_dilog (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_permutation (struct GMT_CTRL *GMT, int n, int r);
EXTERN_MSC double gmt_combination (struct GMT_CTRL *GMT, int n, int r);
EXTERN_MSC double gmt_sinc (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_erfinv (struct GMT_CTRL *GMT, double x);
EXTERN_MSC double gmt_rand (struct GMT_CTRL *GMT);
EXTERN_MSC double gmt_nrand (struct GMT_CTRL *GMT);
EXTERN_MSC double gmt_lrand (struct GMT_CTRL *GMT);
EXTERN_MSC int gmt_chebyshev (struct GMT_CTRL *GMT, double x, int n, double *t);
EXTERN_MSC double gmt_corrcoeff (struct GMT_CTRL *GMT, double *x, double *y, uint64_t n, unsigned int mode);
EXTERN_MSC double gmt_corrcoeff_f (struct GMT_CTRL *GMT, float *x, float *y, uint64_t n, unsigned int mode);
EXTERN_MSC double gmt_Fcrit (struct GMT_CTRL *GMT, double alpha, double nu1, double nu2);
EXTERN_MSC double gmt_chi2crit (struct GMT_CTRL *GMT, double alpha, double nu);
EXTERN_MSC double gmt_extreme (struct GMT_CTRL *GMT, double *x, uint64_t n, double x_default, int kind, int way);
EXTERN_MSC double gmt_tcrit (struct GMT_CTRL *GMT, double alpha, double nu);
EXTERN_MSC double gmt_zcrit (struct GMT_CTRL *GMT, double alpha);
EXTERN_MSC double gmt_zdist (struct GMT_CTRL *GMT, double x);
EXTERN_MSC int gmt_median (struct GMT_CTRL *GMT, double *x, uint64_t n, double xmin, double xmax, double m_initial, double *med);
EXTERN_MSC int gmt_mode (struct GMT_CTRL *GMT, double *x, uint64_t n, uint64_t j, bool sort, int mode_selection, unsigned int *n_multiples, double *mode_est);
EXTERN_MSC int gmt_mode_f (struct GMT_CTRL *GMT, float *x, uint64_t n, uint64_t j, bool sort, int mode_selection, unsigned int *n_multiples, double *mode_est);
EXTERN_MSC double gmt_mean_and_std (struct GMT_CTRL *GMT, double *x, uint64_t n, double *std);
EXTERN_MSC double gmt_mean_weighted (struct GMT_CTRL *GMT, double *x, double *w, uint64_t n);
EXTERN_MSC double gmt_median_weighted (struct GMT_CTRL *GMT, struct GMT_OBSERVATION *data, uint64_t n, double quantile);
EXTERN_MSC double gmt_mode_weighted (struct GMT_CTRL *GMT, struct GMT_OBSERVATION *data, uint64_t n);
EXTERN_MSC int gmt_sig_f (struct GMT_CTRL *GMT, double chi1, uint64_t n1, double chi2, uint64_t n2, double level, double *prob);
EXTERN_MSC double gmt_t_pdf (struct GMT_CTRL *GMT, double t, uint64_t nu);
EXTERN_MSC double gmt_t_cdf (struct GMT_CTRL *GMT, double t, uint64_t nu);
EXTERN_MSC double gmt_f_pdf (struct GMT_CTRL *GMT, double F, uint64_t nu1, uint64_t nu2);
EXTERN_MSC double gmt_f_cdf (struct GMT_CTRL *GMT, double F, uint64_t nu1, uint64_t nu2);
EXTERN_MSC double gmt_chi2_pdf (struct GMT_CTRL *GMT, double c, uint64_t nu);
EXTERN_MSC double gmt_poissonpdf (struct GMT_CTRL *GMT, double k, double lambda);
EXTERN_MSC void gmt_chi2 (struct GMT_CTRL *GMT, double chi2, double nu, double *prob);
EXTERN_MSC void gmt_poisson_cdf (struct GMT_CTRL *GMT, double k, double mu, double *prob);
EXTERN_MSC double gmt_binom_pdf (struct GMT_CTRL *GMT, uint64_t x, uint64_t n, double p);
EXTERN_MSC double gmt_binom_cdf (struct GMT_CTRL *GMT, uint64_t x, uint64_t n, double p);
EXTERN_MSC double gmt_weibull_pdf (struct GMT_CTRL *GMT, double x, double scale, double shape);
EXTERN_MSC double gmt_weibull_cdf (struct GMT_CTRL *GMT, double x, double scale, double shape);
EXTERN_MSC double gmt_weibull_crit (struct GMT_CTRL *GMT, double p, double scale, double shape);
EXTERN_MSC void gmt_getmad (struct GMT_CTRL *GMT, double *x, uint64_t n, double location, double *scale);
EXTERN_MSC void gmt_getmad_f (struct GMT_CTRL *GMT, float *x, uint64_t n, double location, double *scale);
EXTERN_MSC double gmt_psi (struct GMT_CTRL *GMT, double z[], double p[]);
EXTERN_MSC void gmt_PvQv (struct GMT_CTRL *GMT, double x, double v_ri[], double pq[], unsigned int *iter);
EXTERN_MSC double gmt_quantile (struct GMT_CTRL *GMT, double *x, double q, uint64_t n);
EXTERN_MSC double gmt_quantile_f (struct GMT_CTRL *GMT, float *x, double q, uint64_t n);

#endif /* _GMT_PROTOTYPES_H */
