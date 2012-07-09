
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���C�Y���ۂ��J�[�h���`���܂��B
	�Ǝv�������ǁA�S�R������ۂ��Ȃ��B
	-------------------------------------------------------
	�Ƃ肠�����J�[�h��̎����B
	-------------------------------------------------------
	�J�E���^��p���W�X�^�͔p�~���āA�ėp���W�X�^�Ɠ��������B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ŏI�`�Ԃ��ۂ��z
	-------------------------------------------------------
	���@�_��32���������e�B
	���E�Ɍ��݂Ɍ��B32������1/4���B����͔����e�B
	-------------------------------------------------------
	�V�V�X�e���ŃJ�[�h�ꂪ�g����\��(?)�Ȃ̂ŁA
	�J�[�h����ӎ����ď����Ď���B
	-------------------------------------------------------
	�g�p���W�X�^(�Ă���):
	REG_08_REG0 	�J�E���^�B��{�I��4��Ɉ�񏈗��B
	REG_09_REG1 	32�����e�ɂ���`���ƁB
	REG_0a_REG2 	���E�ŐF��ς��悤�B
	REG_0b_REG3 	�p�x��ێ�����ˁB
	REG_0c_REG4
	REG_0d_REG5
	REG_0e_REG6
	REG_0f_REG7 	(BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA)


	�J�E���^������Z���ɂ͕ϐ��ȊO�ɁA�u0�ȊO�̒萔�v���g����B
	�J�E���^������Z���Ɂu0�v�͎g���Ȃ��B(�������s�ւ͖�����)
	(0�Ȃ�Εϐ��A0�ȊO�͒萔�Ƃ����d�l�B)

	�萔	t256(1)
	�萔	t256(1.2)
	�萔	((65536/4)/32)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_44_ruiz3)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 4);// ��{�I��4��Ɉ�񏈗��B
	if (0==(REG_08_REG0))
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 32);//	32�����e�ɂ���`���ƁB
		if (1==(REG_09_REG1))	/* �n�߂Ă̏ꍇ */
		{
			count_up_limit_NUM(REG_NUM_0a_REG2, 2);//	���E�ŐF��ς��悤�B
			calculate_jikinerai();/* ���@�_���p�쐬 */
			REG_0b_REG3 = HATSUDAN_03_angle65536;/* ���@�_���p */
		}
		if (0==REG_0a_REG2)
				{	(REG_0b_REG3) += ((65536/4)/32);	}
		else	/* else �̓J�[�h��ŏ����̂߂�ǂ������̂Ŏg��Ȃ��B */
	//	if (1==REG_0a_REG2)
				{	(REG_0b_REG3) -= ((65536/4)/32);	}
		HATSUDAN_01_speed256				= (t256(1.2)+(REG_09_REG1<<3)); 	/* �e�� */
		HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
		HATSUDAN_03_angle65536				= (REG_0b_REG3);
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA) + REG_0a_REG2;	/* [ �e] */
		hatudan_system_regist_single(); /*(1�e����)*/
	}
}
#if 0
[ID:supeka3500] 			; //���ʕ�����
����u�J�[�h��̂Ă��Ɓv	; //����

[init]	; //�������Z�N�V����
r0		44		; //�o�^�ԍ�(r0�͏�ɒ萔0�Ȃ̂ŏ������ł��Ȃ��B�����ŁA�������Z�N�V�����ɏ������ꍇ�A�o�^�ԍ��ɂ���B)
r1		5
r8		125
;	�g�p���W�X�^(�Ă���):
;	r1	��芸����4��Ɉ��ł�����B
;	r2	32�����e�ɂ���`���ƁB
;	r3	���E�ŐF��ς��悤�B
;	r4	�p�x��ێ�����ˁB

[spell] 	; //���e�Z�N�V����
{
	; // �J�[�h��͖��߂ƍő�ň���3�܂ŁB
	count_up_limit r1, 4, 0 			; ��芸����4��Ɉ��ł�����B
	if_equal 0, r1						; // if�͈���2�܂ň���3�̓V�X�e�����g��(����3==�s�����̏ꍇ�̃W�����v��̈ʒu)
	{
		count_up_limit r2, 32, 0		; 32�����e�ɂ���`���ƁB
		if_equal 1, r2					; �n�߂Ă̏ꍇ
		{
			count_up_limit r3, 2, 0 	; ���E�ŐF��ς��悤�B
			jiki_nerai r4				; ���@�_���p�쐬  tmp_angleCCW65536_jiki_nerai();/*  */
			set r4, HATSUDAN_03_angle65536		; // /* ���@�_���p */
		}
		if_equal 0, r3
		{
			add r4, 512 				; 512 == ((65536/4)/32)
		}
	;	else �̓J�[�h��ł͑��݂��Ȃ��B
		if_equal 1, r3
		{
			add r4, -512				; 512 == ((65536/4)/32)
		}
		set HATSUDAN_01_speed256,			(t256(1.2)+(r2<<3)) 			; /* �e�� */
		set HATSUDAN_02_speed_offset,		t256(1) 						; /*(�e�X�g)*/
		set HATSUDAN_03_angle65536, 		(r4)							;
		set HATSUDAN_04_tama_spec,			(DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT)			; /* (r33-)�W���e */
		set HATSUDAN_05_bullet_obj_type,	(BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA) + r3		; /* [ �e] */
		hatudan_system_regist_single												; /*(1�e����)*/
	}
	label 123	;
	jump 123	;
}
#endif
