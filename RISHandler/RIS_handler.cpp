//Factum Arte Desarrollos Digitales
//Description: Application to visualize and manage RIS files

#include "stdafx.h"
#include <math.h>
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "FreeImage.h"

// Basic app methods
void setup( int argc, char *argv[] );
void loop();
void exit();
void keyPressed();
void mouseHandler(int event, int x, int y, int flags, void* param);

// Other methods
void	RISTOTIFF( char* inputFileName, char* outputFileName );
float	map		( float value, float istart, float istop, float ostart, float ostop );

// App definitions
bool		appAlive = true;

float		zArray[5201][5201];
int			imageWidth	= 5201;
int			imageHeight = 5201;

float       NULL_DATA		= -3.37e+38;

// User input args
char* inputRis;
char* outputTif;
float inputStart	= 0.0f;
float inputStop		= 0.0f;
float outputStart	= 0.0f;
float outputStop	= 0.0f;

// main() 
int main(int argc, char *argv[])
{
	setup( argc, argv );

	return 0;
}

// setup()
//INPUT LINE COMMAND PARA ris de 0 a 33mm y TIF de 16 bits: ..\\data\\input.ris ..\\data\\output.tif 0.0f 33.0f 0.0f 65535.0f
void setup( int argc, char *argv[] )
{
	// Create GUI
	//cvNamedWindow( "TIFF2RIS-EXPORTER", CV_WINDOW_AUTOSIZE );

	// Print help
	printf( "RIS2TIFF (16bits) Converter - v.01 (4-1-2013)\n\n");

	printf( "Help:\n\nparameter 1: input ris file\n");
	printf( "parameter 2: output ris file\n");
	printf( "parameter 3: input start value (float)\n");
	printf( "parameter 4: input stop value (float)\n");
	printf( "parameter 5: output start value (float)\n");
	printf( "parameter 6: output stop value (float)\n\n");

	printf( "Example usage:\n\nRIS2TIFF-ExporterApp.exe ..\\data\\input.ris ..\\data\\output.tif -0.5f 0.5f 0.0f 1.0f \n\n");
	
	
	// Display each command-line argument.
	printf("Command-line arguments:\n");
	for( int count = 0; count < argc; count++ )
	{
		char* arg = argv[count];
		if (count > 0 )
			printf( "Arg[%i]: %s\n", count, arg );
	}
	

	// Get input args
	inputRis = argv[1];
	outputTif = argv[2];
	inputStart = atof(argv[3]);
	inputStop = atof(argv[4]);
	outputStart = atof(argv[5]);
	outputStop = atof(argv[6]);

	// Generate output file
	RISTOTIFF( inputRis, outputTif );
}

// Map function (from Processing source)
float map( float value, float istart, float istop, float ostart, float ostop )
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

// Generates a 32 bit TIFF from input RIS file
void RISTOTIFF( char* inputFileName, char* outputFileName )
{
	// Load input RIS file
	FILE* lRis = fopen ( inputFileName, "rb" );

	// Jump to data position
	for (int i = 0; i < 88; i++){	    
		uchar a = getc (lRis);   
	}   

	// Read z array
	fread ( zArray , 1 , sizeof(zArray) , lRis );

	//Get max value of RIS
	float RISmax = zArray [0][0];
	float RISmin = zArray [0][0];
	for (int i=0; i<5200; i++) 
	{
		for (int j=0; j<5200; j++)
			{
				if (zArray[i][j] > RISmax)
				RISmax = zArray [i][j];
				if (zArray[i][j] < RISmin)
				RISmin = zArray [i][j];
			}
	}
	std::cout<<"The max value of the matrix is: "<<RISmax<<"\n";
	std::cout<<"The min value of the matrix is: "<<RISmin<<"\n";
	//Beep(0,5000);

	// Close input file
	fclose (lRis);

	// Create 32 bit float image
	FIBITMAP *image = FreeImage_AllocateT(FIT_UINT16, imageWidth, imageHeight);

	//float* pixels;
	unsigned short* pixels;
	float height = 0.0f;
	float value = 0.0f;
	for(int y = 0; y < imageHeight; y++)
	{
		pixels = (unsigned short *)FreeImage_GetScanLine(image, y);
		for(int x = 0; x < imageWidth; x++)
		{
			// Map input/output values
			height = zArray[x][y];
			if ( height != NULL_DATA)
			{
				value = map ( height, inputStart, inputStop, outputStart, outputStop );
				//value = map ( height, RISmin, RISmax, outputStart, outputStop );
				pixels[x] = value;
			}else{
				pixels[x] = 0.0f;
			}
		}
	}

	// Save TIFF file
	FreeImage_Save( FIF_TIFF, image, outputFileName, TIFF_NONE );

	// Release memory
	FreeImage_Unload( image );

	printf("\nTIF file created: %s", outputFileName );

	//Beep(0, 1200);
}

// loop()
void loop()
{
	keyPressed();
}

// exit()
void exit()
{
	//cvDestroyWindow( "TIFF2RIS-EXPORTER" );
	appAlive = false;
}

// keyPressed()
void keyPressed()
{
	int key = cvWaitKey(33);

	if( key == 27 ) 
		exit();

	switch(key){
		case '1':
			break;
	}
};

// mouseHandler()
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
	  case CV_EVENT_LBUTTONDOWN:
		  break;
	  case CV_EVENT_LBUTTONUP:
		  break;
	}
}

// FreeImage error handler
void freeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) 
{
	printf("freeImageErrorHandler()");

	if( fif != FIF_UNKNOWN ) {
		printf("freeImageErrorHandler() - %s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
}

