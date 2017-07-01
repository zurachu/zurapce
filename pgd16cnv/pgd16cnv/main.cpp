#include "pgd16cnv.h"
#include "../bmp/bmp.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>

static void usage( void )
{
	fprintf( stderr, "usage:\n"
					 "\tWindows BMP �� 16�K���g�� P/ECE BMP �R���o�[�^\n"
					 "\tpgd16cnv [option] bmpfile ...\n"
					 "\toption -t �e�L�X�g�o�� (.c) �f�t�H���g\n"
					 "\t       -b �o�C�i���o�� (.pgx)\n"
					 "\tbmpfile Windows BMP �t�@�C����\n"
					 "\tBMP �t�@�C���͕��� 8 �̔{���A4bit or 8bit�A�����k�ł��邱�ƁB\n"
		   );
}

static void file_write_by_text( char const* filename, char const* data_name, unsigned char const* p, int size )
{
	FILE* fp = fopen( filename, "wt" );
	if( fp )
	{
		fprintf( fp, "// size %d bytes\n", size );
		fprintf( fp, "unsigned char const %s[] = {\n", data_name );
		for( int i = 0; i < size; i++ )
		{
			fprintf( fp, "0x%02x, ", *p++ );
			if( i % 16 == 16 - 1 ) { fprintf( fp, "\n" ); }
		}
		fprintf( fp, "\n};\n");
		fclose( fp );
	}
}

static void file_write_by_binary( char const* filename, unsigned char const* p, int size )
{
	FILE* fp = fopen( filename, "wb" );
	if( fp )
	{
		fwrite( p, 1, size, fp );
		fclose( fp );
	}
}

int main(int argc, char *argv[])
{
	enum OUTPUT_TYPE
	{
		OUTPUT_TYPE_TEXT,
		OUTPUT_TYPE_BINARY,
	} output_type = OUTPUT_TYPE_TEXT;
	
	if( argc < 2 )
	{
		usage();
		return 1;
	}

	for( int i = 1; i < argc; i++ )
	{
		if( !strcmp( argv[ i ], "-t" ) )
		{	// �Ȍ�̃t�@�C���̓e�L�X�g�o��
			output_type = OUTPUT_TYPE_TEXT;
		}
		else if( !strcmp( argv[ i ], "-b" ) )
		{	// �Ȍ�̃t�@�C���̓o�C�i���o��
			output_type = OUTPUT_TYPE_BINARY;
		}
		else
		{
			unsigned char* const p = Bmp_FileReadAlloc( argv[ i ] );
			if( p )
			{
				BITMAPFILEHEADER* const bf = reinterpret_cast< BITMAPFILEHEADER* >( p );
				BITMAPINFOHEADER* const bi = reinterpret_cast< BITMAPINFOHEADER* >( p + sizeof(BITMAPFILEHEADER) );
				unsigned char* const bmp = p + bf->bfOffBits;
				if( bi->biHeight > 0 ) { Bmp_UpsideDown( bi, bmp ); } // �g�b�v�_�E���z�u
				unsigned char* const q = pgd16cnv( bi, bmp );
				if( q )
				{
					char out_path[_MAX_PATH];
					char drive[_MAX_DRIVE];
					char dir[_MAX_DIR];
					char fname[_MAX_FNAME];
					char ext[_MAX_EXT];
					int const size = reinterpret_cast< PBMP_FILEHEADER* >( q )->fsize;
					_splitpath( argv[ i ], drive, dir, fname, ext );
					if( output_type == OUTPUT_TYPE_TEXT )
					{
						_makepath( out_path, drive, dir, fname, ".c" );
						file_write_by_text( out_path, fname, q, size );
					}
					else
					{
						_makepath( out_path, drive, dir, fname, ".pgx" );
						file_write_by_binary( out_path, q, size );
					}
					printf( "Convert %s\n", out_path );
					free( q );
				}
				free( p );
			}
		}
	}
	return 0;
}