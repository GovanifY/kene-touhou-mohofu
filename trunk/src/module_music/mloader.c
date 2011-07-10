
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	MOD_MUSIC_MODULE of;

static	MOD_MUSIC_INTERNAL_LOADER *firstloader = NULL;

u16 mm_fine_tune[16] =
{
	8363, 8413, 8463, 8529, 8581, 8651, 8723, 8757,
	7895, 7941, 7985, 8046, 8107, 8169, 8232, 8280
};

MM_CHAR* MusicMod_InfoLoader(void)
{
	int len = 0;
	MOD_MUSIC_INTERNAL_LOADER *l;
	MM_CHAR *list = NULL;

	MUTEX_LOCK(lists);
	/* compute size of buffer */
	for (l=firstloader; l; l=l->next)
	{
		len += 1+(l->next?1:0)+strlen(l->version);
	}
	if (len)
	{
		list = mod_music_malloc(len*sizeof(MM_CHAR));
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
	#if 0//廃止 曲のコメント文字列:
	if (len)/* コメントがある場合 */
	{
		of.comment = (MM_CHAR*)mod_music_malloc(len+1);
		if (!(of.comment))
		{
			return (0);/*(失敗)*/
		}
		mod_music_file_read_u8S(of.comment, len, mm_midi.mod_FILE_reader);
		/* translate IT linefeeds.(IT形式の場合改行を修正する。) */
		{
			int i;
			for (i=0; i<len; i++)
			{
				if (of.comment[i]=='\r')
				{
					of.comment[i] = '\n';
				}
			}
		}
		of.comment[len] = (0);	/* just in case */	/* [EOS] 文字列終端 */
	}
	if (!of.comment[0])/* コメントが無い場合 */
	{
		free(of.comment);
		of.comment = NULL;
	}
	#endif
	return (1);/*(成功)*/
}
#endif // LINK_IT

#if 0
MM_BOOL ReadLinedComment(u16 lines, u16 linelen)
{
	MM_CHAR *tempcomment;
	MM_CHAR *line;
	MM_CHAR *storage;
	u16 t;
	u16 len = (lines * linelen);
	if (lines)
	{
		tempcomment 	= (MM_CHAR*)mod_music_malloc(len+1);
		if (!tempcomment)	{return (0);}
		storage 		= (MM_CHAR*)mod_music_malloc(linelen+1);
		if (!storage)
		{
			free(tempcomment);
			return (0);
		}
		mod_music_file_read_u8S(tempcomment, len, mm_midi.mod_FILE_reader);

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
		{	of.comment	= (MM_CHAR*)mod_music_malloc(total+1);
			if (!(of.comment))
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
					strcat(of.comment, storage);strcat(of.comment, "\r");
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
	if (!(of.positions=mod_music_calloc(total, sizeof(u16))))	{return (0);}
	return (1);
}

MM_BOOL AllocPatterns(void)
{
	if ((!of.numpat)||(!of.numchn))
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	/* Allocate track sequencing array */
	if (!(of.patterns=(u16*)mod_music_calloc((u32)(of.numpat+1)*of.numchn, sizeof(u16))))	{return (0);}
	if (!(of.pattrows=(u16*)mod_music_calloc(of.numpat+1, sizeof(u16))))	{return (0);}
//
	{
		int tracks = 0;
		int s;
		int t;
		for (t=0; t<=of.numpat; t++)
		{
			of.pattrows[t] = 64;
			for (s=0; s<of.numchn; s++)
			{	of.patterns[(t*of.numchn)+s]=tracks++;	}
		}
	}
	return (1);
}

MM_BOOL AllocTracks(void)
{
	if (!of.numtrk)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(of.tracks = (u8 **)mod_music_calloc(of.numtrk, sizeof(u8 *))))	{return (0);}
	return (1);
}

MM_BOOL AllocInstruments(void)
{
	if (!of.numins)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(of.instruments = (MM_INSTRUMENT*)mod_music_calloc(of.numins, sizeof(MM_INSTRUMENT))))
	{	return (0);}
//
	{
		int t;
		int n;
		for (t=0;t<of.numins;t++)
		{
			for (n=0;n<INSTNOTES;n++)
			{
				/* Init note / sample lookup table */
				of.instruments[t].samplenote[n]   = n;
				of.instruments[t].samplenumber[n] = t;
			}
			of.instruments[t].globvol = 64;
		}
	}
	return (1);
}

MM_BOOL AllocSamples(void)
{
	if (!of.numsmp)
	{
		mod_music_error_number=MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		return (0);
	}
	if (!(of.samples = (MM_SAMPLE*)mod_music_calloc(of.numsmp, sizeof(MM_SAMPLE)))) {return (0);}
//
	{
		u16 u;
		for (u=0;u<of.numsmp;u++)
		{
			of.samples[u].panning = 128;	/* center */
			of.samples[u].handle  = -1;
			of.samples[u].globvol = 64;
			of.samples[u].volume  = 64;
		}
	}
	return (1);
}

static MM_BOOL ML_LoadSamples(void)
{
	MM_SAMPLE *s;
	int u;
	for (u=of.numsmp, s=of.samples;u;u--, s++)
	{	if (s->length)	{SL_RegisterSample(mm_midi.mod_FILE_reader, s, MD_MUSIC);}}
	return (1);
}


/*
	mikmod helper function
*/
/* Creates a CSTR out of a character buffer of 'len' bytes, but strips any
   terminating non-printing characters like 0, spaces etc.					  */
MM_CHAR *DupStr(MM_CHAR* s, u16 len, MM_BOOL strict)
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
	MM_CHAR *d /*= NULL*/;
	d = (MM_CHAR*)mod_music_malloc(sizeof(MM_CHAR)*(len+1));
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
	MOD_VIRTUAL_MIXER_SampleUnload(handle);/*(たぶん)*/
}
#endif/*(1)*/

static void ML_XFreeSample(MM_SAMPLE *s)
{
	if (s->handle>=0)
	{
		MOD_VIRTUAL_MIXER_SampleUnload( (s16)(s->handle) );/*(たぶん)*/
	}
	if (s->samplename)	{free(s->samplename);}
}

static void ML_XFreeInstrument(MM_INSTRUMENT *i)
{
	if (i->insname) 	{free(i->insname);}
}

static void ML_FreeEx(MOD_MUSIC_MODULE *mf)
{
//廃止		if (mf->songname)	{free(mf->songname);}
//廃止		if (mf->comment)	{free(mf->comment);}
//廃止		if (mf->modtype)	{free(mf->modtype);}
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
	if ((mf) != (&of))
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
static MM_CHAR* Player_LoadTitle_internal(MF_READER *reader)
{
現在未使用
	MOD_MUSIC_INTERNAL_LOADER *l;
	mm_midi.mod_FILE_reader 			= reader;
	mod_music_error_number	= 0;
	mod_music_critical		= 0;
	mod_music_iobase_setcur(mm_midi.mod_FILE_reader);

	/* Try to find a loader that recognizes the module */
	for (l=firstloader; l; l=l->next)
	{
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);
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

MM_CHAR* Player_LoadTitle(MM_CHAR* filename)
{
	MM_CHAR* result = NULL;
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

	mm_midi.mod_FILE_reader 	= reader;
	mod_music_error_number		= 0;
	mod_music_critical			= 0;
	mod_music_iobase_setcur(mm_midi.mod_FILE_reader);

	/* Try to find a loader that recognizes the module */
	for (l=firstloader; l; l=l->next)
	{
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);
		if (l->Test()) break;
	}

	if (!l)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_NOT_A_MOD_MUSIC_MODULE;
		if (mod_music_error_handler) mod_music_error_handler();
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	/* init unitrk routines */
	if (!UniInit())
	{
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	/* load the song using the song's loader variable */
	memset(&of, 0, sizeof(MOD_MUSIC_MODULE));
	of.initvolume = 128;

	/* init panning array */
	for (t=0; t<64; t++)	{	of.panning[t] = ((t+1)&2) ? 255 : 0;	}
	for (t=0; t<64; t++)	{	of.chanvol[t] = 64; 	}

	/* init module loader and load the header / patterns */
	if (l->Init())
	{
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);
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
		ML_FreeEx(&of);
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}

	if (!ML_LoadSamples())
	{
		ML_FreeEx(&of);
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);mod_music_iobase_revert();
		return (NULL);
	}
	mf = ML_AllocUniMod();
	if (!mf)
	{
		ML_FreeEx(&of);
		mod_music_file_seek_rewind(mm_midi.mod_FILE_reader);mod_music_iobase_revert();
		if (mod_music_error_handler)	{	mod_music_error_handler();}
		return (NULL);
	}

	/* Copy the static MOD_MUSIC_MODULE contents into the dynamic MOD_MUSIC_MODULE struct. */
	memcpy(mf, &of, sizeof(MOD_MUSIC_MODULE));
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
MOD_MUSIC_MODULE* Player_Load(MM_CHAR* filename, int maxchan, MM_BOOL curious)
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
