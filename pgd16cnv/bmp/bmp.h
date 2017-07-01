#pragma once

/** @file
	Windows BMP ����̕⏕.
	@author zurachu
*/

#include <windows.h>

/**
	�q�[�v���m�ۂ��� Windows BMP �t�@�C����ǂݍ���.
	@param filename BMP �t�@�C����
	@return �ǂݍ��񂾃q�[�v�̈�i���s�� NULL�j
	@warning �s�v�ɂȂ����q�[�v�� free() ���邱�ƁB
*/
unsigned char* Bmp_FileReadAlloc( char const* filename );

/**
	Windows BMP �̃g�b�v�_�E���A�{�g���A�b�v�����ւ���.
	@param bi Windows BMP ���
	@param bmp Windows �r�b�g�}�b�v����
*/
bool Bmp_UpsideDown( BITMAPINFOHEADER* bi, unsigned char* bmp );

