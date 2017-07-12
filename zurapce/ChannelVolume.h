#if !defined( ZURAPCE_CHANNEL_VOLUME_H )
#define ZURAPCE_CHANNEL_VOLUME_H

/** @file
	�e ch �̉��ʃt�F�[�h�����֘A.
	@author zurachu
*/

/**
	������.
	���ʃt�F�[�h�𗘗p����ꍇ�A�ŏ��ɌĂяo���ĉ������B
	���O�Ɏ��s���Ă����A�v���� ch ���̉��ʐݒ�������p���Ȃ��悤�A
	���������� ch ���̉��ʂ��ő�ɖ߂��܂��B
*/
void ChannelVolume_Init( void );

/**
	�I��.
	�t�F�[�h�� ch �̉��ʂ����������܂܂ŁA���Ɏ��s����A�v����
	ch ���̉��ʂ�ݒ肵�Ă��Ȃ��ꍇ�A�ݒ肪�����p����ĉ����������Ȃ�
	�Ȃ��Ă��܂��ꍇ������̂ŁA�I������ ch ���̉��ʂ��ő�ɖ߂��܂��B
*/
void ChannelVolume_Exit( void );

/**
	��Ԃ̍X�V.
	�S ch �̃t�F�[�h��Ԃ�1�t���[���i�߂܂��B
	pceAppProc() ����1�񂩁A����I�ɌĂяo���Ă��������B
*/
void ChannelVolume_Update( void );

/**
	���݂̉��ʂ���t�F�[�h���J�n���܂�.
	@param ch �`�����l��
	@param vol �ŏI����
	@param fade_time �t�F�[�h���ԁi�t���[�����j
*/
void ChannelVolume_Fade( int ch, int vol, int fade_time );

/**
	�t�F�[�h�C�����܂�.
	�ŏ����ʂ���ő剹�ʂɌ������ăt�F�[�h���J�n���܂��B
	@param ch �`�����l��
	@param fade_time �t�F�[�h���ԁi�t���[�����j
*/
void ChannelVolume_FadeIn( int ch, int fade_time );

/**
	�t�F�[�h�A�E�g���܂�.
	���݂̉��ʂ���ŏ����ʂɌ������ăt�F�[�h���J�n���܂��B
	@param ch �`�����l��
	@param fade_time �t�F�[�h���ԁi�t���[�����j
*/
void ChannelVolume_FadeOut( int ch, int fade_time );

#endif // !defined( ZURAPCE_CHANNEL_VOLUME_H )
