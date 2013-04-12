#include "pngio.h"

/* Read a PNG file.  You may want to return an error code if the read
 * fails (depending upon the failure).  There are two "prototypes" given
 * here - one where we are given the filename, and we need to open the
 * file, and the other where we are given an open file (possibly with
 * some or all of the magic bytes read - see comments above).
 */
int read_png(char *file_name, int *w, int *h, unsigned char *pixout[], 
	     int *numchannel) {
  return read_png_(file_name,w,h,NULL,NULL,pixout,false,numchannel);
}
int read_png_gl(char *file_name, int *w, int *h, int *rw, int *rh, unsigned char *pixout[]) {
  int chan;
  return read_png_(file_name,w,h,rw,rh,pixout,true,&chan);
}

int read_png_(char *file_name, int *w, int *h,  int *rw, int *rh, unsigned char *pixout[], bool forgl, int *numchannel)
{
   png_structp png_ptr;
   png_infop info_ptr;
   unsigned int sig_read = 0;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type, compression_type, filter_type;
   FILE *fp;

   if ((fp = fopen(file_name, "rb")) == NULL)
      return (ERROR);

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return (ERROR);
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
      return (ERROR);
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
      fclose(fp);
      /* If we get here, we had a problem reading the file */
      return (ERROR);
   }

   /* One of the following I/O initialization methods is REQUIRED */
   /* Set up the input control if you are using standard C streams */
   png_init_io(png_ptr, fp);

   /* If we have already read some of the signature */
   png_set_sig_bytes(png_ptr, sig_read);

   png_read_info(png_ptr, info_ptr);
   png_get_IHDR(png_ptr, info_ptr, &width, &height,
       &bit_depth, &color_type, &interlace_type,
       &compression_type, &filter_type);

   // The following code transforms grayscale images of less than 8 to 8 bits,
   // changes paletted images to RGB, and adds a full alpha channel if there
   // is transparency information in a tRNS chunk. 
   if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8) 
     png_set_expand(png_ptr);
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
     png_set_expand(png_ptr);
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
     png_set_expand(png_ptr);

   if (bit_depth == 16) //ensure 8-bit
     png_set_strip_16(png_ptr);
   if (bit_depth < 8) //ensure 8-bit
     png_set_packing(png_ptr);

   if (forgl) { //width and height must be powers of 2.
     *rh = height; //set the real return vals first
     *rw = width;
     //these are probably the only likely ranges
     if (width > 512)
       width = 1024;
     else if (width > 256)
       width = 512;
     else 
       width = 256;
     if (height > 512)
       height = 1024;
     else if (height > 256)
       height = 512;
     else 
       height = 256;
   }
   
   //printf("colortype %d %d\n",color_type,PNG_COLOR_TYPE_RGBA); 
   *numchannel=3;
   if(color_type==PNG_COLOR_TYPE_RGBA||color_type==PNG_COLOR_TYPE_RGB_ALPHA)
     *numchannel=4;

   unsigned char *pixels = new unsigned char[width*height*sizeof(unsigned char)*(*numchannel)]; //for rgba
   png_byte **row_pointers = new png_byte*[height];
   for (unsigned int k = 0; k < height; k++)
     row_pointers[k] = pixels + (k)* 
       width*sizeof(unsigned char)*(*numchannel); //for r,g,b,a

   png_read_image(png_ptr, row_pointers);
   png_read_end(png_ptr, NULL);

   /* At this point you have read the entire image */

   //set return values
   *h = height;
   *w = width;
   *pixout = pixels;

   /* clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

   /* close the file */
   fclose(fp);

   /* that's it */
   return (OK);
}

/* write a png file */
int write_png(char *file_name, int width, int height, unsigned char pixels[],int numchannel)
{
   FILE *fp;
   png_structp png_ptr;
   png_infop info_ptr;
//   png_colorp palette;

   /* open the file */
   fp = fopen(file_name, "wb");
   if (fp == NULL)
      return (ERROR);

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
				     NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return (ERROR);
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
      return (ERROR);
   }

   /* Set error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(fp);
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return (ERROR);
   }

   int color_type=(numchannel==3)?PNG_COLOR_TYPE_RGB:PNG_COLOR_TYPE_RGBA;
   //printf("colortype %d\n",color_type);

   /* set up the output control if you are using standard C streams */
   png_init_io(png_ptr, fp);

   png_set_IHDR(png_ptr, info_ptr, width, height,
		8, //bit_depth, 
		color_type, //color_type, 
		PNG_INTERLACE_NONE, //interlace_type,
		PNG_COMPRESSION_TYPE_DEFAULT, //compression_type, 
		PNG_FILTER_TYPE_DEFAULT); //filter_type
   png_write_info(png_ptr, info_ptr);

   png_byte **row_pointers = new png_byte*[height];
   for (int k = 0; k < height; k++)
     row_pointers[k] = pixels + (k)* 
       width*sizeof(unsigned char)*(numchannel); //for r,g,b,a
   png_write_image(png_ptr, row_pointers);
   png_write_end(png_ptr, info_ptr);

   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   /* close the file */
   fclose(fp);

   /* that's it */
   return (OK);
}

