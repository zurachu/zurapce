#if !defined( ZURAPCE_LINEAR_PARAMETER_H )
#define ZURAPCE_LINEAR_PARAMETER_H

/** @file
	���`���ԕω��p�����[�^.
	�����l����ŏI�l�܂ŕω����̒l���A���`��Ԃŋ��߂܂��B
	@author zurachu
*/

/// ���`���ԕω��p�����[�^�\����.
/// �����o�𒼐ڐG��Ȃ����ƁB
struct LinearParameter
{
	int start; ///< �����l
	int diff; ///< �����l�ƍŏI�l�̍���
	int frame_count; ///< �t���[�����J�E���^
	int frame_max; ///< �ŏI�l�ɒB����t���[����
};
typedef struct LinearParameter LinearParameter;

/**
	�ݒ�.
	@param p �C���X�^���X
	@param start �����l
	@param goal �ŏI�l
	@param frame �ŏI�l�ɒB����t���[����
	@warning (goal-start)*frame ���I�[�o�[�t���[����ꍇ�A�듮����N�����܂��B
*/
void LinearParameter_Set( LinearParameter* p, int start, int goal, int frame );

/**
	���݂̒l���擾.
	@param p �C���X�^���X
	@return ���݂̒l
*/
int LinearParameter_Get( LinearParameter const* p );

/**
	��Ԃ̍X�V.
	�t���[�����J�E���^���P�i�߂܂��B
	@param p �C���X�^���X
*/
void LinearParameter_Update( LinearParameter* p );

#endif // !defined( ZURAPCE_LINEAR_PARAMETER_H )
