REBOL [comment: "autogenerated"]

"FMOD_RESULT" [
    "FMOD_OK" 0 
    "FMOD_ERR_ALREADYLOCKED" 1 
    "FMOD_ERR_BADCOMMAND" 2 
    "FMOD_ERR_CDDA_DRIVERS" 3 
    "FMOD_ERR_CDDA_INIT" 4 
    "FMOD_ERR_CDDA_INVALID_DEVICE" 5 
    "FMOD_ERR_CDDA_NOAUDIO" 6 
    "FMOD_ERR_CDDA_NODEVICES" 7 
    "FMOD_ERR_CDDA_NODISC" 8 
    "FMOD_ERR_CDDA_READ" 9 
    "FMOD_ERR_CHANNEL_ALLOC" 10 
    "FMOD_ERR_CHANNEL_STOLEN" 11 
    "FMOD_ERR_COM" 12 
    "FMOD_ERR_DMA" 13 
    "FMOD_ERR_DSP_CONNECTION" 14 
    "FMOD_ERR_DSP_FORMAT" 15 
    "FMOD_ERR_DSP_NOTFOUND" 16 
    "FMOD_ERR_DSP_RUNNING" 17 
    "FMOD_ERR_DSP_TOOMANYCONNECTIONS" 18 
    "FMOD_ERR_FILE_BAD" 19 
    "FMOD_ERR_FILE_COULDNOTSEEK" 20 
    "FMOD_ERR_FILE_DISKEJECTED" 21 
    "FMOD_ERR_FILE_EOF" 22 
    "FMOD_ERR_FILE_NOTFOUND" 23 
    "FMOD_ERR_FILE_UNWANTED" 24 
    "FMOD_ERR_FORMAT" 25 
    "FMOD_ERR_HTTP" 26 
    "FMOD_ERR_HTTP_ACCESS" 27 
    "FMOD_ERR_HTTP_PROXY_AUTH" 28 
    "FMOD_ERR_HTTP_SERVER_ERROR" 29 
    "FMOD_ERR_HTTP_TIMEOUT" 30 
    "FMOD_ERR_INITIALIZATION" 31 
    "FMOD_ERR_INITIALIZED" 32 
    "FMOD_ERR_INTERNAL" 33 
    "FMOD_ERR_INVALID_ADDRESS" 34 
    "FMOD_ERR_INVALID_FLOAT" 35 
    "FMOD_ERR_INVALID_HANDLE" 36 
    "FMOD_ERR_INVALID_PARAM" 37 
    "FMOD_ERR_INVALID_POSITION" 38 
    "FMOD_ERR_INVALID_SPEAKER" 39 
    "FMOD_ERR_INVALID_SYNCPOINT" 40 
    "FMOD_ERR_INVALID_VECTOR" 41 
    "FMOD_ERR_IRX" 42 
    "FMOD_ERR_MAXAUDIBLE" 43 
    "FMOD_ERR_MEMORY" 44 
    "FMOD_ERR_MEMORY_CANTPOINT" 45 
    "FMOD_ERR_MEMORY_IOP" 46 
    "FMOD_ERR_MEMORY_SRAM" 47 
    "FMOD_ERR_NEEDS2D" 48 
    "FMOD_ERR_NEEDS3D" 49 
    "FMOD_ERR_NEEDSHARDWARE" 50 
    "FMOD_ERR_NEEDSSOFTWARE" 51 
    "FMOD_ERR_NET_CONNECT" 52 
    "FMOD_ERR_NET_SOCKET_ERROR" 53 
    "FMOD_ERR_NET_URL" 54 
    "FMOD_ERR_NET_WOULD_BLOCK" 55 
    "FMOD_ERR_NOTREADY" 56 
    "FMOD_ERR_OUTPUT_ALLOCATED" 57 
    "FMOD_ERR_OUTPUT_CREATEBUFFER" 58 
    "FMOD_ERR_OUTPUT_DRIVERCALL" 59 
    "FMOD_ERR_OUTPUT_ENUMERATION" 60 
    "FMOD_ERR_OUTPUT_FORMAT" 61 
    "FMOD_ERR_OUTPUT_INIT" 62 
    "FMOD_ERR_OUTPUT_NOHARDWARE" 63 
    "FMOD_ERR_OUTPUT_NOSOFTWARE" 64 
    "FMOD_ERR_PAN" 65 
    "FMOD_ERR_PLUGIN" 66 
    "FMOD_ERR_PLUGIN_INSTANCES" 67 
    "FMOD_ERR_PLUGIN_MISSING" 68 
    "FMOD_ERR_PLUGIN_RESOURCE" 69 
    "FMOD_ERR_PRELOADED" 70 
    "FMOD_ERR_PROGRAMMERSOUND" 71 
    "FMOD_ERR_RECORD" 72 
    "FMOD_ERR_REVERB_INSTANCE" 73 
    "FMOD_ERR_SUBSOUND_ALLOCATED" 74 
    "FMOD_ERR_SUBSOUND_CANTMOVE" 75 
    "FMOD_ERR_SUBSOUND_MODE" 76 
    "FMOD_ERR_SUBSOUNDS" 77 
    "FMOD_ERR_TAGNOTFOUND" 78 
    "FMOD_ERR_TOOMANYCHANNELS" 79 
    "FMOD_ERR_UNIMPLEMENTED" 80 
    "FMOD_ERR_UNINITIALIZED" 81 
    "FMOD_ERR_UNSUPPORTED" 82 
    "FMOD_ERR_UPDATE" 83 
    "FMOD_ERR_VERSION" 84 
    "FMOD_ERR_EVENT_FAILED" 85 
    "FMOD_ERR_EVENT_INFOONLY" 86 
    "FMOD_ERR_EVENT_INTERNAL" 87 
    "FMOD_ERR_EVENT_MAXSTREAMS" 88 
    "FMOD_ERR_EVENT_MISMATCH" 89 
    "FMOD_ERR_EVENT_NAMECONFLICT" 90 
    "FMOD_ERR_EVENT_NOTFOUND" 91 
    "FMOD_ERR_EVENT_NEEDSSIMPLE" 92 
    "FMOD_ERR_EVENT_GUIDCONFLICT" 93 
    "FMOD_ERR_EVENT_ALREADY_LOADED" 94 
    "FMOD_ERR_MUSIC_UNINITIALIZED" 95 
    "FMOD_ERR_MUSIC_NOTFOUND" 96 
    "FMOD_ERR_MUSIC_NOCALLBACK" 97 
    "FMOD_RESULT_FORCEINT" 65536
] 
"FMOD_OUTPUTTYPE" [
    "FMOD_OUTPUTTYPE_AUTODETECT" 0 
    "FMOD_OUTPUTTYPE_UNKNOWN" 1 
    "FMOD_OUTPUTTYPE_NOSOUND" 2 
    "FMOD_OUTPUTTYPE_WAVWRITER" 3 
    "FMOD_OUTPUTTYPE_NOSOUND_NRT" 4 
    "FMOD_OUTPUTTYPE_WAVWRITER_NRT" 5 
    "FMOD_OUTPUTTYPE_DSOUND" 6 
    "FMOD_OUTPUTTYPE_WINMM" 7 
    "FMOD_OUTPUTTYPE_WASAPI" 8 
    "FMOD_OUTPUTTYPE_ASIO" 9 
    "FMOD_OUTPUTTYPE_OSS" 10 
    "FMOD_OUTPUTTYPE_ALSA" 11 
    "FMOD_OUTPUTTYPE_ESD" 12 
    "FMOD_OUTPUTTYPE_PULSEAUDIO" 13 
    "FMOD_OUTPUTTYPE_COREAUDIO" 14 
    "FMOD_OUTPUTTYPE_PS2" 15 
    "FMOD_OUTPUTTYPE_PS3" 16 
    "FMOD_OUTPUTTYPE_XBOX360" 17 
    "FMOD_OUTPUTTYPE_PSP" 18 
    "FMOD_OUTPUTTYPE_WII" 19 
    "FMOD_OUTPUTTYPE_ANDROID" 20 
    "FMOD_OUTPUTTYPE_MAX" 21 
    "FMOD_OUTPUTTYPE_FORCEINT" 65536
] 
"FMOD_SPEAKERMODE" [
    "FMOD_SPEAKERMODE_RAW" 0 
    "FMOD_SPEAKERMODE_MONO" 1 
    "FMOD_SPEAKERMODE_STEREO" 2 
    "FMOD_SPEAKERMODE_QUAD" 3 
    "FMOD_SPEAKERMODE_SURROUND" 4 
    "FMOD_SPEAKERMODE_5POINT1" 5 
    "FMOD_SPEAKERMODE_7POINT1" 6 
    "FMOD_SPEAKERMODE_PROLOGIC" 7 
    "FMOD_SPEAKERMODE_MAX" 8 
    "FMOD_SPEAKERMODE_FORCEINT" 65536
] 
"FMOD_SPEAKER" [
    "FMOD_SPEAKER_FRONT_LEFT" 0 
    "FMOD_SPEAKER_FRONT_RIGHT" 1 
    "FMOD_SPEAKER_FRONT_CENTER" 2 
    "FMOD_SPEAKER_LOW_FREQUENCY" 3 
    "FMOD_SPEAKER_BACK_LEFT" 4 
    "FMOD_SPEAKER_BACK_RIGHT" 5 
    "FMOD_SPEAKER_SIDE_LEFT" 6 
    "FMOD_SPEAKER_SIDE_RIGHT" 7 
    "FMOD_SPEAKER_MAX" 8 
    "FMOD_SPEAKER_MONO" 0 
    "FMOD_SPEAKER_NULL" 8 
    "FMOD_SPEAKER_SBL" 6 
    "FMOD_SPEAKER_SBR" 7 
    "FMOD_SPEAKER_FORCEINT" 65536
] 
"FMOD_PLUGINTYPE" [
    "FMOD_PLUGINTYPE_OUTPUT" 0 
    "FMOD_PLUGINTYPE_CODEC" 1 
    "FMOD_PLUGINTYPE_DSP" 2 
    "FMOD_PLUGINTYPE_MAX" 3 
    "FMOD_PLUGINTYPE_FORCEINT" 65536
] 
"FMOD_SOUND_TYPE" [
    "FMOD_SOUND_TYPE_UNKNOWN" 0 
    "FMOD_SOUND_TYPE_AAC" 1 
    "FMOD_SOUND_TYPE_AIFF" 2 
    "FMOD_SOUND_TYPE_ASF" 3 
    "FMOD_SOUND_TYPE_AT3" 4 
    "FMOD_SOUND_TYPE_CDDA" 5 
    "FMOD_SOUND_TYPE_DLS" 6 
    "FMOD_SOUND_TYPE_FLAC" 7 
    "FMOD_SOUND_TYPE_FSB" 8 
    "FMOD_SOUND_TYPE_GCADPCM" 9 
    "FMOD_SOUND_TYPE_IT" 10 
    "FMOD_SOUND_TYPE_MIDI" 11 
    "FMOD_SOUND_TYPE_MOD" 12 
    "FMOD_SOUND_TYPE_MPEG" 13 
    "FMOD_SOUND_TYPE_OGGVORBIS" 14 
    "FMOD_SOUND_TYPE_PLAYLIST" 15 
    "FMOD_SOUND_TYPE_RAW" 16 
    "FMOD_SOUND_TYPE_S3M" 17 
    "FMOD_SOUND_TYPE_SF2" 18 
    "FMOD_SOUND_TYPE_USER" 19 
    "FMOD_SOUND_TYPE_WAV" 20 
    "FMOD_SOUND_TYPE_XM" 21 
    "FMOD_SOUND_TYPE_XMA" 22 
    "FMOD_SOUND_TYPE_VAG" 23 
    "FMOD_SOUND_TYPE_AUDIOQUEUE" 24 
    "FMOD_SOUND_TYPE_XWMA" 25 
    "FMOD_SOUND_TYPE_MAX" 26 
    "FMOD_SOUND_TYPE_FORCEINT" 65536
] 
"FMOD_SOUND_FORMAT" [
    "FMOD_SOUND_FORMAT_NONE" 0 
    "FMOD_SOUND_FORMAT_PCM8" 1 
    "FMOD_SOUND_FORMAT_PCM16" 2 
    "FMOD_SOUND_FORMAT_PCM24" 3 
    "FMOD_SOUND_FORMAT_PCM32" 4 
    "FMOD_SOUND_FORMAT_PCMFLOAT" 5 
    "FMOD_SOUND_FORMAT_GCADPCM" 6 
    "FMOD_SOUND_FORMAT_IMAADPCM" 7 
    "FMOD_SOUND_FORMAT_VAG" 8 
    "FMOD_SOUND_FORMAT_XMA" 9 
    "FMOD_SOUND_FORMAT_MPEG" 10 
    "FMOD_SOUND_FORMAT_CELT" 11 
    "FMOD_SOUND_FORMAT_MAX" 12 
    "FMOD_SOUND_FORMAT_FORCEINT" 65536
] 
"FMOD_OPENSTATE" [
    "FMOD_OPENSTATE_READY" 0 
    "FMOD_OPENSTATE_LOADING" 1 
    "FMOD_OPENSTATE_ERROR" 2 
    "FMOD_OPENSTATE_CONNECTING" 3 
    "FMOD_OPENSTATE_BUFFERING" 4 
    "FMOD_OPENSTATE_SEEKING" 5 
    "FMOD_OPENSTATE_PLAYING" 6 
    "FMOD_OPENSTATE_SETPOSITION" 7 
    "FMOD_OPENSTATE_MAX" 8 
    "FMOD_OPENSTATE_FORCEINT" 65536
] 
"FMOD_SOUNDGROUP_BEHAVIOR" [
    "FMOD_SOUNDGROUP_BEHAVIOR_FAIL" 0 
    "FMOD_SOUNDGROUP_BEHAVIOR_MUTE" 1 
    "FMOD_SOUNDGROUP_BEHAVIOR_STEALLOWEST" 2 
    "FMOD_SOUNDGROUP_BEHAVIOR_MAX" 3 
    "FMOD_SOUNDGROUP_BEHAVIOR_FORCEINT" 65536
] 
"FMOD_CHANNEL_CALLBACKTYPE" [
    "FMOD_CHANNEL_CALLBACKTYPE_END" 0 
    "FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE" 1 
    "FMOD_CHANNEL_CALLBACKTYPE_SYNCPOINT" 2 
    "FMOD_CHANNEL_CALLBACKTYPE_OCCLUSION" 3 
    "FMOD_CHANNEL_CALLBACKTYPE_MAX" 4 
    "FMOD_CHANNEL_CALLBACKTYPE_FORCEINT" 65536
] 
"FMOD_SYSTEM_CALLBACKTYPE" [
    "FMOD_SYSTEM_CALLBACKTYPE_DEVICELISTCHANGED" 0 
    "FMOD_SYSTEM_CALLBACKTYPE_DEVICELOST" 1 
    "FMOD_SYSTEM_CALLBACKTYPE_MEMORYALLOCATIONFAILED" 2 
    "FMOD_SYSTEM_CALLBACKTYPE_THREADCREATED" 3 
    "FMOD_SYSTEM_CALLBACKTYPE_BADDSPCONNECTION" 4 
    "FMOD_SYSTEM_CALLBACKTYPE_BADDSPLEVEL" 5 
    "FMOD_SYSTEM_CALLBACKTYPE_MAX" 6 
    "FMOD_SYSTEM_CALLBACKTYPE_FORCEINT" 65536
] 
"FMOD_DSP_FFT_WINDOW" [
    "FMOD_DSP_FFT_WINDOW_RECT" 0 
    "FMOD_DSP_FFT_WINDOW_TRIANGLE" 1 
    "FMOD_DSP_FFT_WINDOW_HAMMING" 2 
    "FMOD_DSP_FFT_WINDOW_HANNING" 3 
    "FMOD_DSP_FFT_WINDOW_BLACKMAN" 4 
    "FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS" 5 
    "FMOD_DSP_FFT_WINDOW_MAX" 6 
    "FMOD_DSP_FFT_WINDOW_FORCEINT" 65536
] 
"FMOD_DSP_RESAMPLER" [
    "FMOD_DSP_RESAMPLER_NOINTERP" 0 
    "FMOD_DSP_RESAMPLER_LINEAR" 1 
    "FMOD_DSP_RESAMPLER_CUBIC" 2 
    "FMOD_DSP_RESAMPLER_SPLINE" 3 
    "FMOD_DSP_RESAMPLER_MAX" 4 
    "FMOD_DSP_RESAMPLER_FORCEINT" 65536
] 
"FMOD_TAGTYPE" [
    "FMOD_TAGTYPE_UNKNOWN" 0 
    "FMOD_TAGTYPE_ID3V1" 1 
    "FMOD_TAGTYPE_ID3V2" 2 
    "FMOD_TAGTYPE_VORBISCOMMENT" 3 
    "FMOD_TAGTYPE_SHOUTCAST" 4 
    "FMOD_TAGTYPE_ICECAST" 5 
    "FMOD_TAGTYPE_ASF" 6 
    "FMOD_TAGTYPE_MIDI" 7 
    "FMOD_TAGTYPE_PLAYLIST" 8 
    "FMOD_TAGTYPE_FMOD" 9 
    "FMOD_TAGTYPE_USER" 10 
    "FMOD_TAGTYPE_MAX" 11 
    "FMOD_TAGTYPE_FORCEINT" 65536
] 
"FMOD_TAGDATATYPE" [
    "FMOD_TAGDATATYPE_BINARY" 0 
    "FMOD_TAGDATATYPE_INT" 1 
    "FMOD_TAGDATATYPE_FLOAT" 2 
    "FMOD_TAGDATATYPE_STRING" 3 
    "FMOD_TAGDATATYPE_STRING_UTF16" 4 
    "FMOD_TAGDATATYPE_STRING_UTF16BE" 5 
    "FMOD_TAGDATATYPE_STRING_UTF8" 6 
    "FMOD_TAGDATATYPE_CDTOC" 7 
    "FMOD_TAGDATATYPE_MAX" 8 
    "FMOD_TAGDATATYPE_FORCEINT" 65536
] 
"FMOD_DELAYTYPE" [
    "FMOD_DELAYTYPE_END_MS" 0 
    "FMOD_DELAYTYPE_DSPCLOCK_START" 1 
    "FMOD_DELAYTYPE_DSPCLOCK_END" 2 
    "FMOD_DELAYTYPE_DSPCLOCK_PAUSE" 3 
    "FMOD_DELAYTYPE_MAX" 4 
    "FMOD_DELAYTYPE_FORCEINT" 65536
] 
"FMOD_SPEAKERMAPTYPE" [
    "FMOD_SPEAKERMAPTYPE_DEFAULT" 0 
    "FMOD_SPEAKERMAPTYPE_ALLMONO" 1 
    "FMOD_SPEAKERMAPTYPE_ALLSTEREO" 2 
    "FMOD_SPEAKERMAPTYPE_51_PROTOOLS" 3
] 
"FMOD_CHANNELINDEX" [
    "FMOD_CHANNEL_FREE" -1 
    "FMOD_CHANNEL_REUSE" -2
] 
"FMOD_CAPS" [
    "FMOD_CAPS_NONE" 0 
    "FMOD_CAPS_HARDWARE" 1 
    "FMOD_CAPS_HARDWARE_EMULATED" 2 
    "FMOD_CAPS_OUTPUT_MULTICHANNEL" 4 
    "FMOD_CAPS_OUTPUT_FORMAT_PCM8" 8 
    "FMOD_CAPS_OUTPUT_FORMAT_PCM16" 16 
    "FMOD_CAPS_OUTPUT_FORMAT_PCM24" 32 
    "FMOD_CAPS_OUTPUT_FORMAT_PCM32" 64 
    "FMOD_CAPS_OUTPUT_FORMAT_PCMFLOAT" 128 
    "FMOD_CAPS_REVERB_EAX2" 256 
    "FMOD_CAPS_REVERB_EAX3" 512 
    "FMOD_CAPS_REVERB_EAX4" 1024 
    "FMOD_CAPS_REVERB_EAX5" 2048 
    "FMOD_CAPS_REVERB_I3DL2" 4096 
    "FMOD_CAPS_REVERB_LIMITED" 8192
] 
"FMOD_DEBUGLEVEL" [
    "FMOD_DEBUG_LEVEL_NONE" 0 
    "FMOD_DEBUG_LEVEL_LOG" 1 
    "FMOD_DEBUG_LEVEL_ERROR" 2 
    "FMOD_DEBUG_LEVEL_WARNING" 4 
    "FMOD_DEBUG_LEVEL_HINT" 8 
    "FMOD_DEBUG_LEVEL_ALL" 0 
    "FMOD_DEBUG_TYPE_MEMORY" 256 
    "FMOD_DEBUG_TYPE_THREAD" 512 
    "FMOD_DEBUG_TYPE_FILE" 1024 
    "FMOD_DEBUG_TYPE_NET" 2048 
    "FMOD_DEBUG_TYPE_EVENT" 4096 
    "FMOD_DEBUG_TYPE_ALL" 0 
    "FMOD_DEBUG_DISPLAY_TIMESTAMPS" 16777216 
    "FMOD_DEBUG_DISPLAY_LINENUMBERS" 33554432 
    "FMOD_DEBUG_DISPLAY_COMPRESS" 67108864 
    "FMOD_DEBUG_DISPLAY_THREAD" 134217728 
    "FMOD_DEBUG_DISPLAY_ALL" 0 
    "FMOD_DEBUG_ALL" 0
] 
"FMOD_MEMORY_TYPE" [
    "FMOD_MEMORY_NORMAL" 0 
    "FMOD_MEMORY_STREAM_FILE" 1 
    "FMOD_MEMORY_STREAM_DECODE" 2 
    "FMOD_MEMORY_SAMPLEDATA" 4 
    "FMOD_MEMORY_XBOX360_PHYSICAL" 1048576 
    "FMOD_MEMORY_PERSISTENT" 2097152 
    "FMOD_MEMORY_SECONDARY" 4194304 
    "FMOD_MEMORY_ALL" 0
] 
"FMOD_INITFLAGS" [
    "FMOD_INIT_NORMAL" 0 
    "FMOD_INIT_STREAM_FROM_UPDATE" 1 
    "FMOD_INIT_3D_RIGHTHANDED" 2 
    "FMOD_INIT_SOFTWARE_DISABLE" 4 
    "FMOD_INIT_SOFTWARE_OCCLUSION" 8 
    "FMOD_INIT_SOFTWARE_HRTF" 16 
    "FMOD_INIT_SOFTWARE_REVERB_LOWMEM" 64 
    "FMOD_INIT_ENABLE_PROFILE" 32 
    "FMOD_INIT_VOL0_BECOMES_VIRTUAL" 128 
    "FMOD_INIT_WASAPI_EXCLUSIVE" 256 
    "FMOD_INIT_PS2_DISABLECORE0REVERB" 65536 
    "FMOD_INIT_PS2_DISABLECORE1REVERB" 131072 
    "FMOD_INIT_PS2_DONTUSESCRATCHPAD" 262144 
    "FMOD_INIT_PS2_SWAPDMACHANNELS" 524288 
    "FMOD_INIT_PS3_PREFERDTS" 8388608 
    "FMOD_INIT_PS3_FORCE2CHLPCM" 16777216 
    "FMOD_INIT_WII_DISABLEDOLBY" 1048576 
    "FMOD_INIT_SYSTEM_MUSICMUTENOTPAUSE" 2097152 
    "FMOD_INIT_SYNCMIXERWITHUPDATE" 4194304 
    "FMOD_INIT_DTS_NEURALSURROUND" 33554432 
    "FMOD_INIT_GEOMETRY_USECLOSEST" 67108864 
    "FMOD_INIT_DISABLE_MYEARS" 134217728
] 
"FMOD_MODE" [
    "FMOD_DEFAULT" 0 
    "FMOD_LOOP_OFF" 1 
    "FMOD_LOOP_NORMAL" 2 
    "FMOD_LOOP_BIDI" 4 
    "FMOD_2D" 8 
    "FMOD_3D" 16 
    "FMOD_HARDWARE" 32 
    "FMOD_SOFTWARE" 64 
    "FMOD_CREATESTREAM" 128 
    "FMOD_CREATESAMPLE" 256 
    "FMOD_CREATECOMPRESSEDSAMPLE" 512 
    "FMOD_OPENUSER" 1024 
    "FMOD_OPENMEMORY" 2048 
    "FMOD_OPENMEMORY_POINT" 268435456 
    "FMOD_OPENRAW" 4096 
    "FMOD_OPENONLY" 8192 
    "FMOD_ACCURATETIME" 16384 
    "FMOD_MPEGSEARCH" 32768 
    "FMOD_NONBLOCKING" 65536 
    "FMOD_UNIQUE" 131072 
    "FMOD_3D_HEADRELATIVE" 262144 
    "FMOD_3D_WORLDRELATIVE" 524288 
    "FMOD_3D_LOGROLLOFF" 1048576 
    "FMOD_3D_LINEARROLLOFF" 2097152 
    "FMOD_3D_CUSTOMROLLOFF" 67108864 
    "FMOD_3D_IGNOREGEOMETRY" 1073741824 
    "FMOD_CDDA_FORCEASPI" 4194304 
    "FMOD_CDDA_JITTERCORRECT" 8388608 
    "FMOD_UNICODE" 16777216 
    "FMOD_IGNORETAGS" 33554432 
    "FMOD_LOWMEM" 134217728 
    "FMOD_LOADSECONDARYRAM" 536870912 
    "FMOD_VIRTUAL_PLAYFROMSTART" -2147483648
] 
"FMOD_TIMEUNIT" [
    "FMOD_TIMEUNIT_MS" 1 
    "FMOD_TIMEUNIT_PCM" 2 
    "FMOD_TIMEUNIT_PCMBYTES" 4 
    "FMOD_TIMEUNIT_RAWBYTES" 8 
    "FMOD_TIMEUNIT_PCMFRACTION" 16 
    "FMOD_TIMEUNIT_MODORDER" 256 
    "FMOD_TIMEUNIT_MODROW" 512 
    "FMOD_TIMEUNIT_MODPATTERN" 1024 
    "FMOD_TIMEUNIT_SENTENCE_MS" 65536 
    "FMOD_TIMEUNIT_SENTENCE_PCM" 131072 
    "FMOD_TIMEUNIT_SENTENCE_PCMBYTES" 262144 
    "FMOD_TIMEUNIT_SENTENCE" 524288 
    "FMOD_TIMEUNIT_SENTENCE_SUBSOUND" 1048576 
    "FMOD_TIMEUNIT_BUFFERED" 268435456
] 
"FMOD_REVERB_FLAGS" [
    "FMOD_REVERB_FLAGS_DECAYTIMESCALE" 1 
    "FMOD_REVERB_FLAGS_REFLECTIONSSCALE" 2 
    "FMOD_REVERB_FLAGS_REFLECTIONSDELAYSCALE" 4 
    "FMOD_REVERB_FLAGS_REVERBSCALE" 8 
    "FMOD_REVERB_FLAGS_REVERBDELAYSCALE" 16 
    "FMOD_REVERB_FLAGS_DECAYHFLIMIT" 32 
    "FMOD_REVERB_FLAGS_ECHOTIMESCALE" 64 
    "FMOD_REVERB_FLAGS_MODULATIONTIMESCALE" 128 
    "FMOD_REVERB_FLAGS_CORE0" 256 
    "FMOD_REVERB_FLAGS_CORE1" 512 
    "FMOD_REVERB_FLAGS_HIGHQUALITYREVERB" 1024 
    "FMOD_REVERB_FLAGS_HIGHQUALITYDPL2REVERB" 2048 
    "FMOD_REVERB_FLAGS_DEFAULT" none
] 
"FMOD_REVERB_PRESETS" [
    "FMOD_PRESET_OFF" none 
    "FMOD_PRESET_GENERIC" none 
    "FMOD_PRESET_PADDEDCELL" none 
    "FMOD_PRESET_ROOM" none 
    "FMOD_PRESET_BATHROOM" none 
    "FMOD_PRESET_LIVINGROOM" none 
    "FMOD_PRESET_STONEROOM" none 
    "FMOD_PRESET_AUDITORIUM" none 
    "FMOD_PRESET_CONCERTHALL" none 
    "FMOD_PRESET_CAVE" none 
    "FMOD_PRESET_ARENA" none 
    "FMOD_PRESET_HANGAR" none 
    "FMOD_PRESET_CARPETTEDHALLWAY" none 
    "FMOD_PRESET_HALLWAY" none 
    "FMOD_PRESET_STONECORRIDOR" none 
    "FMOD_PRESET_ALLEY" none 
    "FMOD_PRESET_FOREST" none 
    "FMOD_PRESET_CITY" none 
    "FMOD_PRESET_MOUNTAINS" none 
    "FMOD_PRESET_QUARRY" none 
    "FMOD_PRESET_PLAIN" none 
    "FMOD_PRESET_PARKINGLOT" none 
    "FMOD_PRESET_SEWERPIPE" none 
    "FMOD_PRESET_UNDERWATER" none 
    "FMOD_PRESET_DRUGGED" none 
    "FMOD_PRESET_DIZZY" none 
    "FMOD_PRESET_PSYCHOTIC" none 
    "FMOD_PRESET_PS2_ROOM" none 
    "FMOD_PRESET_PS2_STUDIO_A" none 
    "FMOD_PRESET_PS2_STUDIO_B" none 
    "FMOD_PRESET_PS2_STUDIO_C" none 
    "FMOD_PRESET_PS2_HALL" none 
    "FMOD_PRESET_PS2_SPACE" none 
    "FMOD_PRESET_PS2_ECHO" none 
    "FMOD_PRESET_PS2_DELAY" none 
    "FMOD_PRESET_PS2_PIPE" none
] 
"FMOD_REVERB_CHANNELFLAGS" [
    "FMOD_REVERB_CHANNELFLAGS_DIRECTHFAUTO" 1 
    "FMOD_REVERB_CHANNELFLAGS_ROOMAUTO" 2 
    "FMOD_REVERB_CHANNELFLAGS_ROOMHFAUTO" 4 
    "FMOD_REVERB_CHANNELFLAGS_INSTANCE0" 16 
    "FMOD_REVERB_CHANNELFLAGS_INSTANCE1" 32 
    "FMOD_REVERB_CHANNELFLAGS_INSTANCE2" 64 
    "FMOD_REVERB_CHANNELFLAGS_INSTANCE3" 128 
    "FMOD_REVERB_CHANNELFLAGS_DEFAULT" none
]