/**
 * Image.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "Image.hpp"

Image::Image()
{
}

Image::Image(int width, int height)
{
   allocateImage(width, height);
}

Image::~Image()
{
   delete[] r;
   delete[] g;
   delete[] b;
}

unsigned int Image::index2D(unsigned int x, unsigned int y)
{
   return x + width * y;
}


void Image::allocateImage(int width, int height)
{  
   this->width = width;
   this->height = height;
   unsigned int size = width * height;
   
   r = new float[size]();
   g = new float[size]();
   b = new float[size]();
}

void Image::readTGA(const char *filename)
{
   FILE *fptr;
   TGAHeader header;
   int x, y;
   size_t result;   
   
   fptr = fopen(filename,"r");
   
   if (fptr == NULL)
   {
      cerr << "File error\n";
      exit(1);
   }
   
   //Read header data
   header.idlength = fgetc(fptr);
   header.colourmaptype = fgetc(fptr);
   header.datatypecode = fgetc(fptr);
   
   if (header.datatypecode != 2)
   {
      cerr << "Unable to read TGA file.\n" << "This reader can only read uncompressed, 24-bit RGB images.\n";
      exit(1);
   }
   
   result = fread(&header.colourmaporigin,2,1,fptr);
   result = fread(&header.colourmaplength,2,1,fptr);
   header.colourmapdepth = fgetc(fptr);
   
   result = fread(&header.x_origin,2,1,fptr);
   result = fread(&header.y_origin,2,1,fptr);
   
   result = fread(&header.width,2,1,fptr);
   result = fread(&header.height,2,1,fptr);
   width = header.width;
   height = header.height;
   
   header.bitsperpixel = fgetc(fptr);
   header.imagedescriptor = fgetc(fptr);
   if (header.bitsperpixel != 24)
   {
      cerr << "Unable to read TGA file.\n" << "This reader can only read uncompressed, 24-bit RGB images.\n";
      exit(1);
   }
   
   if (width > 0 && height > 0)
      allocateImage(width, height);
      
   //Read the RGB values
   for (y = 0; y < height; y++)
   {
      for (x = 0; x < width; x++)
      {
         unsigned int index = index2D(x,y);
         b[index] = ((float)fgetc(fptr)) / 255.0f;
         g[index] = ((float)fgetc(fptr)) / 255.0f;
         r[index] = ((float)fgetc(fptr)) / 255.0f;
      }
   }
}

/**
 * Writes out the image
 * 
 * Note: The bottom left corner is (0, 0)
 **/
void Image::writeTGA(const char *filename, unsigned int divisor)
{
   FILE *fp;
   int x, y;
   fp = fopen(filename, "w");
   if (!fp)
   {
     cerr << "File " << filename << " could not be written to." << endl;
     exit(EXIT_FAILURE);
   }
   
   //Write Header
   fputc(0,fp);
   fputc(0,fp);
   
   /* uncompressed RGB */
   fputc(2,fp);                         
   fputc(0,fp); 
   fputc(0,fp);
   fputc(0,fp); 
   fputc(0,fp);
   fputc(0,fp);
   
   /* X origin */
   fputc(0,fp); 
   fputc(0,fp);           
   /* y origin */
   fputc(0,fp); 
   fputc(0,fp);
   
   /* width and height */           
   fputc((width & 0x00FF),fp);
   fputc((width & 0xFF00) / 256,fp);
   fputc((height & 0x00FF),fp);
   fputc((height & 0xFF00) / 256,fp);
   
   /* 24 bit bitmap */
   fputc(24,fp);                        
   fputc(0,fp);
   
   capColors();

   //Write Data
   for (y = 0; y < height; y++)
   {
      for (x = 0; x < width; x++)
      {
         unsigned int index = index2D(x,y);
         fputc(((char)(b[index]*255/divisor)),fp);
         /* G (Green) value */
         fputc(((char)(g[index]*255/divisor)),fp);
         /* R (Red) value */
         fputc(((char)(r[index]*255/divisor)),fp);
      }
   }
   
   fclose(fp);
      
}

void Image::scaleColors()
{
   int i, j;
   float colorMax = 0.000001;
   
   for (i = 0; i < width; i++)
   {
      for (j = 0; j < height; j++)
      {
         unsigned int index = index2D(i,j);
         if(r[index] > colorMax)
            colorMax = r[index];
         if(g[index] > colorMax)
            colorMax = g[index];
         if(b[index] > colorMax)
            colorMax = b[index];
      }
   }
   
   for (i = 0; i < width; i++)
   {
      for (j = 0; j < height; j++)
      {
         unsigned int index = index2D(i,j);
         r[index] /= colorMax;
         g[index] /= colorMax;
         b[index] /= colorMax;
      }
   }
}

void Image::capColors()
{
   int i, j;
   for (i = 0; i < width; i++)
   {
      for (j = 0; j < height; j++)
      {
         unsigned int index = index2D(i,j);
         if (r[index] > 1.0f)
            r[index] = 1.0f;
         if (g[index] > 1.0f)
            g[index] = 1.0f;
         if (b[index] > 1.0f)
            b[index] = 1.0f;
      }
   }
}

vec3 Image::getColor(int x, int y)
{
   return vec3(r[index2D(x,y)], g[index2D(x,y)], b[index2D(x,y)]);
}

void Image::setColor(int x, int y, float newR, float newG, float newB)
{
   unsigned int index = index2D(x,y);
   r[index] = newR;
   g[index] = newG;
   b[index] = newB;
}

void Image::setColor(int x, int y, const vec3& color)
{
   unsigned int index = index2D(x,y);
   r[index] = color.r;
   g[index] = color.g;
   b[index] = color.b;
}

void Image::addColor(int x, int y, float newR, float newG, float newB)
{
   unsigned int index = index2D(x,y);
   r[index] += newR;
   g[index] += newG;
   b[index] += newB;
}

void Image::addColor(int x, int y, const vec3& color)
{
   unsigned int index = index2D(x,y);
   r[index] += color.r;
   g[index] += color.g;
   b[index] += color.b;
}

void Image::fill(vec3 color)
{
   int i,j;
   for (i = 0; i < width; i++)
   {
      for (j = 0; j < height; j++)
      {
         unsigned int index = index2D(i,j);
         r[index] = color.x;
         g[index] = color.y;
         b[index] = color.z;
      }
   }
}




