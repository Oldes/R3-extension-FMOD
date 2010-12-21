REBOL [title: "Basic FMOD extension test"]

with: func [obj body][do bind body obj]

fmod: import/no-lib/no-user %ext-fmod.dll

FMOD_CAPS: map [
	HARDWARE:               1
    HARDWARE_EMULATED:      2
    OUTPUT_MULTICHANNEL:    4
    OUTPUT_FORMAT_PCM8:     8
    OUTPUT_FORMAT_PCM16:    16
    OUTPUT_FORMAT_PCM24:    32
    OUTPUT_FORMAT_PCM32:    64
    OUTPUT_FORMAT_PCMFLOAT: 128
    REVERB_EAX2:            256
    REVERB_EAX3:            512
    REVERB_EAX4:            1024
    REVERB_EAX5:            2048
    REVERB_I3DL2:           4096
    REVERB_LIMITED:         8192
]

print-DriverCaps: funct[id][
	caps: first fmod/system/GetDriverCaps id - 1
	foreach w words-of FMOD_CAPS [
		printf [15 2 24 5] reduce ["" "*" w  not zero? caps and FMOD_CAPS/(w)]
	]
]

with fmod [
	;probe System_GetDriverCaps 0
	with System [
		repeat i GetNumDrivers [
			print ["SND_Driver[" i "]:" GetDriverInfo i - 1]
			print-DriverCaps i
		]
		print ["GetNumCDROMDrives:" GetNumCDROMDrives]
		
		
	]
	init

	probe test-obj probe o: object [Room: 3.3 Reverb: 2 ]
	;probe o
	
	
	
	

	print ["MemStats:" Memory/GetStats false]
	print "--^/playing stream from network:"
	snd1: createStream "http://box.lebeda.ws/~rebolek/mp3/plavil.mp3" ;"http://www.hollowsun.com/HS2/products/novachord/audio/novaberlin.mp3" ;
	probe m: System/GetMemoryInfo 4294967295 4294967295
	;halt
	if snd1 [
		with Sound [
			probe GetDefaults snd1
			;probe SetDefaults snd1 44100. 0.1 1.0 128
			repeat i first probe GetNumTags  snd1 [
				probe GetTag snd1 "" i - 1
			]
			probe GetNumTags  snd1
		]
		probe ch1: playSound snd1
		;print ["Channel1 speaker mix:" mold mixdata: Channel/GetSpeakerMix ch1]
		;mxdata means: [frontleft frontright center lfe backleft backright sideleft sideright]
		;print ["plaing only right speakers:" Channel/SetSpeakerMix ch1 [0.0 1.0 0.0 0.0 0.0 1.0 0.0 1.0]]
	]
	print "--^/creating local sound:"
	probe snd2: createSound  "fletna3.mp3"
;	ch2: playSound snd2

	print ["MemStats:" Memory/GetStats false]
	print "--^/some system info:"
	with System [
		print ["Channels playing:" GetChannelsPlaying]
		print ["Spectrum:" mold GetSpectrum 64 0 1]
		print ["CPU usage:" mold GetCPUUsage]
		;Retrieves in percent of CPU time [dsp stream geometry update total]
		  
		print ["SoundRAM: " mold GetSoundRAM]
		;Retrieves the amount of dedicated sound ram available if the platform supports it.
		;Most platforms use main ram to store audio data, so this function usually isn't necessary.
	]
	print "--^/use>> FMOD/channelStop ch1 <<to stop the stream playback"
	
	
	;probe Channel/GetSpeakerMix ch1
	;channelStop ch1
	;snd1: createSound "fletna3.mp3"
	;probe Sound_GetDefaults snd1
	;probe Sound_SetDefaults snd1 11025.0 0.1 1.0 128
	
	;ch1: playSound snd1
	;ch2: playSound snd_bum:    createSound "bum_16bit.wav"
	;ask "press enter to stop beat"
	;channelStop ch2
	;print "use [channelStop ch1] to stop the rest"
]


halt
