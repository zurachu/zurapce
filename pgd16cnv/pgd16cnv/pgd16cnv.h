#pragma once

/** @file
	Windows BMP ����16�K�� P/ECE BMP �ɕϊ�.
	@author zurachu
*/

#include <windows.h>

/**
	P/ECE BMP �w�b�_�\����.
	include/Piece_Bmp.h �����p�B
	�e�v�f�ɓ���l�́A16�K���p�Ɋg���������̂ƂȂ�B
*/
typedef struct {
	DWORD	head;		///< HEADER   'PBEX'
	DWORD	fsize;		///< �t�@�C���� �iBYTE�P�ʁj
	BYTE	bpp;		///< bit�[�x  �i4�j
	BYTE	mask;		///< �}�X�N��bit�[�x  �i0/1�j
	short	w;			///< X��		8�s�N�Z���P�ʌ���
	short	h;			///< Y����
	DWORD	buf_size;	///< BMP�T�C�Y	�iBYTE�P�ʁj
}PBMP_FILEHEADER;

/**
	�q�[�v���m�ۂ��āAWindows BMP ����16�K�� P/ECE BMP �ɕϊ������f�[�^���i�[.
	@param bi Windows BMP ���
	@param bmp Windows �r�b�g�}�b�v�����i�������ɂ̓g�b�v�_�E���Ŕz�u�j
	@return �i�[�����q�[�v�̈�i���s�� NULL�j
	@warning �s�v�ɂȂ����q�[�v�� free() ���邱�ƁB
*/
unsigned char* pgd16cnv( BITMAPINFOHEADER const* bi, unsigned char const* bmp );

