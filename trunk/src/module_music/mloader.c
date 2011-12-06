
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	 $Id: mloader.c, v 1.6 2000/11/17 21:38:05 hercules Exp $
	These routines are used to access the available module loaders
---------------------------------------------------------*/

#include <string.h>
//#include <memory.h>


//	MF_READER *mod_FILE_reader;
	MOD_MUSIC_MODULE mmoo;

static	MOD_MUSIC_INTERNAL_LOADER *firstloader = NULL;

u16 mm_fine_tune[16] =
{
	8363, 8413, 8463, 8529, 8581, 8651, 8723, 8757,
	7895, 7941, 7985, 8046, 8107, 8169, 8232, 8280
};

MM_s8* MusicMod_InfoLoader(void)
{
	int len = 0;
	MOD_MUSIC_INTERNAL_LOADER *l;
	MM_s8 *list = NULL;

	MUTEX_LOCK(lists);
	/* compute size of buffer */
	for (l=firstloader; l; l=l->next)
	{
		len += 1+(l->next?1:0)+strlen(l->version);
	}
	if (len)
	{
		list = mod_music_malloc(len*sizeof(MM_s8));
		if (list)
		{
			list[0] = 0;
			/* list all registered module loders */
			for (l=firstloader; l; l=l->next)
			{
				sprintf(list, (l->next)?"%s%s\n":"%s%s", list, l->version);
			}
		}
	}
	MUTEX_UNLOCK(lists);
	return (list);
}

void mod_music_registerloader(MOD_MUSIC_INTERNAL_LOADER* ldr)
{
	MOD_MUSIC_INTERNAL_LOADER *cruise = firstloader;
	if (cruise)
	{
		while (cruise->next)
		{
			cruise = cruise->next;
		}
		cruise->next = ldr;
	}
	else
	{
		firstloader = ldr;
	}
}

void mod_music_unregisterloaders(void)
{
	MOD_MUSIC_INTERNAL_LOADER *cruise=firstloader, *ldr;
	while ( cruise )
	{
		ldr 		= cruise;
		cruise		= cruise->next;
		ldr->next	= NULL;
	}
	firstloader 	= NULL;
}

void MusicMod_RegisterLoader(/*struct*/ MOD_MUSIC_INTERNAL_LOADER* ldr)
{
	/* if we try to register an invalid loader, or an already registered loader,
	   ignore this attempt */
	if ((!ldr)||(ldr->next))
	{	return;}
	MUTEX_LOCK(lists);
	mod_music_registerloader(ldr);
	MUTEX_UNLOCK(lists);
}

#if 0//#ifdef LINK_IT
MM_BOOL ReadComment(u16 len)
{
	#if 0//�p�~ �Ȃ̃R�����g������:
	if (len)/* �R�����g������ꍇ */
	{
		mmoo.comment = (MM_s8*)mod_music_malloc(len+1);
		if (!(mmoo.comment))
		{
			return (0);/*(���s)*/
		}
		mod_music_file_read_u8S(mmoo.comment, len, mmff.mod_FILE_reader);
		/* translate IT linefeeds.(IT�`���̏ꍇ���s���C������B) */
		{
			int i;
			for (i=0; i<len; i++)
			{
				if (mmoo.comment[i]=='\r')
				{
					mmoo.comment[i] = '\n';
				}
			}
		}
		mmoo.comment[len] = (0);	/* just in case */	/* [EOS] ������I�[ */
	}
	if (!mmoo.comment[0])/* �R�����g�������ꍇ */
	{
		free(mmoo.comment);
		mmoo.comment = NULL;
	}
	#endif
	return (1);/*(����)*/
}
#endif // LINK_IT

#if 0
MM_BOOL ReadLinedComment(u16 lines, u16 linelen)
{
	MM_s8 *tempcomment;
	MM_s8 *line;
	MM_s8 *storage;
	u16 t;
	u16 len = (lines * linelen);
	if (lines)
	{
		tempcomment 	= (MM_s8*)mod_music_malloc(len+1);
		if (!tempcomment)	{return (0);}
		storage 		= (MM_s8*)mod_music_malloc(linelen+1);
		if (!storage)
		{
			free(tempcomment);
			return (0);
		}
		mod_music_file_read_u8S(tempcomment, len, mmff.mod_FILE_reader);

		/* compute message length */
		{
			line	= tempcomment;
			u16 total;
			total	= 0;
			for (t=0; t<lines; t++)
			{
				int i;
				for (i=linelen; (i>=0)&&(line[i]==' '); i--)
				{
					line[i] = 0;
				}
				for (i=0; i<linelen; i++)
				{	if (!line[i])
					{	break;
					}
				}
				total	+= (1+i);
				line	+= linelen;
			}
		}
		if (total>lines)
		{	mmoo.comment	= (MM_s8*)mod_music_malloc(total+1);
			if (!(mmoo.comment))
			{
				free(storage);
				free(tempcomment);
				return (0);
			}
			/* convert message */
			{
				line = tempcomment;
				for (t=0; t<lines; t++)
				{
					int i;
					for (i=0; i<linelen; i++)
					{
						if (!(storage[i]=line[i]))
						{
							break;
						}
					}
					storage[i] = 0; /* if (i==linelen) */
					strcat(mmoo.comment, storage);strcat(mmoo.comment, "\r");
					line += linelen;
				}
			}
			free(storage);
			free(tempcomment);
		}
	}
	return (1);
}
#endif // 0

MM_BOOL AllocPositions(int total)
{
	if (!total)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(mmoo.positions=mod_music_calloc(total, sizeof(u16))))	{return (0);}
	return (1);
}

MM_BOOL AllocPatterns(void)
{
	if ((!mmoo.numpat)||(!mmoo.numchn))
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	/* Allocate track sequencing array */
	if (!(mmoo.patterns=(u16*)mod_music_calloc((u32)(mmoo.numpat+1)*mmoo.numchn, sizeof(u16))))	{return (0);}
	if (!(mmoo.pattrows=(u16*)mod_music_calloc(mmoo.numpat+1, sizeof(u16))))	{return (0);}
//
	{
		int tracks = 0;
		int s;
		int t;
		for (t=0; t<=mmoo.numpat; t++)
		{
			mmoo.pattrows[t] = 64;
			for (s=0; s<mmoo.numchn; s++)
			{	mmoo.patterns[(t*mmoo.numchn)+s]=tracks++;	}
		}
	}
	return (1);
}

MM_BOOL AllocTracks(void)
{
	if (!mmoo.numtrk)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(mmoo.tracks = (u8 **)mod_music_calloc(mmoo.numtrk, sizeof(u8 *))))	{return (0);}
	return (1);
}

MM_BOOL AllocInstruments(void)
{
	if (!mmoo.numins)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(mmoo.instruments = (MM_INSTRUMENT*)mod_music_calloc(mmoo.numins, sizeof(MM_INSTRUMENT))))
	{	return (0);}
//
	{
		int t;
		int n;
		for (t=0;t<mmoo.numins;t++)
		{
			for (n=0;n<INSTNOTES;n++)
			{
				/* Init note / sample lookup table */
				mmoo.instruments[t].samplenote[n]   = n;
				mmoo.instruments[t].samplenumber[n] = t;
			}
			mmoo.instruments[t].globvol = 64;
		}
	}
	return (1);
}

MM_BOOL AllocSamples(void)
{
	if (!mmoo.numsmp)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(mmoo.samples = (MM_SAMPLE*)mod_music_calloc(mmoo.numsmp, sizeof(MM_SAMPLE)))) {return (0);}
//
	{
		u16 u;
		for (u=0;u<mmoo.numsmp;u++)
		{
			mmoo.samples[u].panning = 128;	/* center */
			mmoo.samples[u].handle  = -1;
			mmoo.samples[u].globvol = 64;
			mmoo.samples[u].volume  = 64;
		}
	}
	return (1);
}

static MM_BOOL ML_LoadSamples(void)
{
	MM_SAMPLE *s;
	int u;
	for (u=mmoo.numsmp, s=mmoo.samples;u;u--, s++)
	{	if (s->length)	{SL_RegisterSample(mmff.mod_FILE_reader, s, MD_MUSIC);}}
	return (1);
}


/*
	mikmod helper function
*/
/* Creates a CSTR out of a character buffer of 'len' bytes, but strips any
   terminating non-printing characters like 0, spaces etc.					  */
MM_s8 *DupStr(MM_s8* s, u16 len, MM_BOOL strict)
{
	/* Scan for last printing char in buffer [includes high ascii up to 254] */
	while (len)
	{
		if (s[len-1]>0x20)	{break;}
		len--;
	}

	u16 t;
	/* Scan forward for possible NULL character */
	if (strict)
	{
		for (t=0; t<len; t++)	{if (!s[t]) 	{break;}}
		if (t<len)	{len=t;}
	}

	/* When the buffer wasn't completely empty, allocate a cstring and copy the
	   buffer into that string, except for any control-chars */
	MM_s8 *d /*= NULL*/;
	d = (MM_s8*)mod_music_malloc(sizeof(MM_s8)*(len+1));
	if (d)
	{
		for (t=0; t<len; t++)	{	d[t] = ((s[t]<32)?('.'):(s[t]));	}
		d[len]=0;
	}
	return (d);
}


#if (0)
//extern void 	MD_SampleUnload(s16);
static void MD_SampleUnload(s16 handle)/* mloader.c */
{
	MOD_VIRTUAL_MIXER_SampleUnload(handle);/*(���Ԃ�)*/
}
#endif/*(1)*/

static void ML_XFreeSample(MM_SAMPLE *s)
{
	if (s->handle>=0)
	{
		MOD_VIRTUAL_MIXER_SampleUnload( (s16)(s->handle) );/*(���Ԃ�)*/
	}
	if (s->samplename)	{free(s->samplename);}
}

static void ML_XFreeInstrument(MM_INSTRUMENT *i)
{
	if (i->insname) 	{free(i->insname);}
}

static void ML_FreeEx(MOD_MUSIC_MODULE *mf)
{
//�p�~		if (mf->songname)	{free(mf->songname);}
//�p�~		if (mf->comment)	{free(mf->comment);}
//�p�~		if (mf->modtype)	{free(mf->modtype);}
	if (mf->positions)	{free(mf->positions);}
	if (mf->patterns)	{free(mf->patterns);}
	if (mf->pattrows)	{free(mf->pattrows);}
	//
	u16 t;
	if (mf->tracks)
	{
		for (t=0; t<(mf->numtrk); t++)
		{
			if (mf->tracks[t])
			{
				free(mf->tracks[t]);
			}
		}
		free(mf->tracks);
	}
	if (mf->instruments)
	{
		for (t=0; t<(mf->numins); t++)
		{
			ML_XFreeInstrument(&mf->instruments[t]);
		}
		free(mf->instruments);
	}
	if (mf->samples)
	{
		for (t=0; t<(mf->numsmp); t++)
		{
			if (mf->samples[t].length)
			{
				ML_XFreeSample(&mf->samples[t]);
			}
		}
		free(mf->samples);
	}
	memset(mf, 0, sizeof(MOD_MUSIC_MODULE));
	if ((mf) != (&mmoo))
	{
		free(mf);
	}
}

static MOD_MUSIC_MODULE *ML_AllocUniMod(void)
{
	MOD_MUSIC_MODULE *mf;
	mf = mod_music_malloc(sizeof(MOD_MUSIC_MODULE));
	return (mf);
}

static void Player_Free_internal(MOD_MUSIC_MODULE *mf)
{
	if (mf)
	{
		Player_Exit_internal(mf);
		ML_FreeEx(mf);
	}
}

void Player_Free(MOD_MUSIC_MODULE *mf)
{
	MUTEX_LOCK(vars);
	Player_Free_internal(mf);
	MUTEX_UNLOCK(vars);
}

#if (0)//def USE_MIKMOD_ALONE
static MM_s8* Player_LoadTitle_internal(MF_READER *reader)
{
���ݖ��g�p
	MOD_MUSIC_INTERNAL_LOADER *l;
	mmff.mod_FILE_reader 			= reader;
	mod_music_error_number	= 0;
	mod_music_critical		= 0;
	mod_music_iobase_setcur(mmff.mod_FILE_reader);

	/* Try to find a loader that recognizes the module */
	for (l=firstloader; l; l=l->next)
	{
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);
		if (l->Test())
		{	break;	}
	}
	if (!l)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		if (mod_music_error_handler)
		{	mod_music_error_handler();	}
		return (NULL);
	}
	return (l->LoadTitle());
}

MM_s8* Player_LoadTitle(MM_s8* filename)
{
	MM_s8* result = NULL;
//	FILE* fp;
	MYMOD_FILE_ptr fp;
	MF_READER* reader;
	fp = mod_music_fopen_FILE(filename/*, "rb"*/);
	if (fp)
	{
		reader = mod_music_new_file_reader(fp);
		if (reader)
		{
			MUTEX_LOCK(lists);
			result = Player_LoadTitle_internal(reader);
			MUTEX_UNLOCK(lists);
			mod_music_delete_file_reader(reader);
		}
		MYMOD_fclose(fp);
	}
	return (result);
}
#endif /* USE_MIKMOD_ALONE */

/* Loads a module given an reader */
static MOD_MUSIC_MODULE* Player_LoadGeneric_internal(MF_READER *reader, int maxchan, MM_BOOL curious)
{
	int t;
	MOD_MUSIC_INTERNAL_LOADER *l;
	MM_BOOL ok;
	MOD_MUSIC_MODULE *mf;

	mmff.mod_FILE_reader 		= reader;
	mod_music_error_number		= 0;
	mod_music_critical			= 0;
	mod_music_iobase_setcur(mmff.mod_FILE_reader);

	/* Try to find a loader that recognizes the module */
	for (l=firstloader; l; l=l->next)
	{
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);
		if (l->Test()) break;
	}

	if (!l)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		if (mod_music_error_handler) mod_music_error_handler();
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	/* init unitrk routines */
	if (!UniInit())
	{
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	/* load the song using the song's loader variable */
	memset(&mmoo, 0, sizeof(MOD_MUSIC_MODULE));
	mmoo.initvolume = 128;

	/* init panning array */
	for (t=0; t<64; t++)	{	mmoo.panning[t] = ((t+1)&2) ? 255 : 0;	}
	for (t=0; t<64; t++)	{	mmoo.chanvol[t] = 64; 	}

	/* init module loader and load the header / patterns */
	if (l->Init())
	{
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);
		ok = l->Load(curious);
	}
	else
	{
		ok = 0;
	}
	/* free loader and unitrk allocations */
	l->Cleanup();
	UniCleanup();

	if (!ok)
	{
		ML_FreeEx(&mmoo);
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	if (!ML_LoadSamples())
	{
		ML_FreeEx(&mmoo);
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}
	mf = ML_AllocUniMod();
	if (!mf)
	{
		ML_FreeEx(&mmoo);
		mod_music_file_seek_rewind(mmff.mod_FILE_reader);mod_music_iobase_revert();
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		return (NULL);
	}

	/* Copy the static MOD_MUSIC_MODULE contents into the dynamic MOD_MUSIC_MODULE struct. */
	memcpy(mf, &mmoo, sizeof(MOD_MUSIC_MODULE));
	mod_music_iobase_revert();

	if (0 < maxchan)
	{
		if (!((mf->flags)&UF_NNA) && ((mf->numchn)<maxchan))
		{	maxchan = mf->numchn;}
		else
		if ((mf->numvoices) && ((mf->numvoices)<maxchan))
		{	maxchan = mf->numvoices;}

		if (maxchan<(mf->numchn))	{	mf->flags |= UF_NNA;	}

		if (MusicMod_SetNumVoices_internal(maxchan, -1))
		{
			Player_Free(mf);
			return (NULL);
		}
	}
	if (SL_LoadSamples())
	{
		Player_Free_internal(mf);
		return (NULL);
	}
	if (Player_Init(mf))
	{
		Player_Free_internal(mf);
		mf = NULL;
	}
	return (mf);
}

static MOD_MUSIC_MODULE* s_Player_LoadGeneric(MF_READER *reader, int maxchan, MM_BOOL curious)
{
	MOD_MUSIC_MODULE* result;
	MUTEX_LOCK(vars);
	MUTEX_LOCK(lists);
	result = Player_LoadGeneric_internal(reader, maxchan, curious);
	MUTEX_UNLOCK(lists);
	MUTEX_UNLOCK(vars);
	return (result);
}


/* SDL_RWops compatability */
#if 0//def US E_RWOPS
/* Open a module via an SDL_rwop.
	The loader will initialize the specified song-player 'player'. */
MOD_MUSIC_MODULE* Player_LoadRW(MYMOD_FILE *fp_rw, int maxchan, MM_BOOL curious)
{
	MOD_MUSIC_MODULE* result = NULL;
	/*struct*/ MF_READER* reader = mod_music_new_rwops_reader(fp_rw);
	if (reader)
	{
		result = s_Player_LoadGeneric(reader, maxchan, curious);
		mod_music_delete_rwops_reader(reader);
	}
	return (result);
}
#endif /* US E_RWOPS */
/* End SDL_RWops compatability */

/* Loads a module given a file pointer.
   File is loaded from the current file seek position. */
//static MOD_MUSIC_MODULE* s_Player_LoadFP(MYMOD_FILE *fp_rw, int maxchan, MM_BOOL curious)
static MOD_MUSIC_MODULE* s_Player_LoadFP(MYMOD_FILE_ptr fp_rw, int maxchan, MM_BOOL curious)
{
	MOD_MUSIC_MODULE* result = NULL;
	/*struct*/ MF_READER* reader = mod_music_new_file_reader(fp_rw);
	if (reader)
	{
		result = s_Player_LoadGeneric(reader, maxchan, curious);
		mod_music_delete_file_reader(reader);
	}
	return (result);
}

#if 1 //def USE_MIKMOD_ALONE
/* Open a module via its filename.
	The loader will initialize the specified song-player 'player'. */
MOD_MUSIC_MODULE* Player_Load(MM_s8* filename, int maxchan, MM_BOOL curious)
{
//	FILE *fp;
	MYMOD_FILE_ptr fp;
	MOD_MUSIC_MODULE *mf = NULL;
	fp = mod_music_fopen_FILE(filename/*, "rb"*/);
	if (fp)
	{
		mf = s_Player_LoadFP(fp, maxchan, curious);
		MYMOD_fclose(fp);
	}
	return (mf);
}
#endif /* USE_MIKMOD_ALONE */

/* ex:set ts=4: */
