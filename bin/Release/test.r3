REBOL [title: "Basic FMOD extension test"]

with: func [obj body][do bind body obj]

fmod: import/no-lib/no-user %ext-fmod.dll
with fmod [
	;probe System_GetDriverCaps 0
	init
	with System [
		repeat i GetNumDrivers [
			print ["SND_Driver[" i "]:" GetDriverInfo i - 1]
		]
		print ["GetNumCDROMDrives:" GetNumCDROMDrives]
	]
	print ["MemStats:" Memory/GetStats false]
	print "--^/playing stream from network:"
	snd1: createStream "http://box.lebeda.ws/~rebolek/mp3/plavil.mp3" ;"http://www.hollowsun.com/HS2/products/novachord/audio/novaberlin.mp3" ;
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
	]
	print "--^/creating local sound:"
	probe snd2: createSound  "fletna3.mp3"
	
	print ["MemStats:" Memory/GetStats false]
	print "--^/some system info:"
	with System [
		print ["Channels playing:" GetChannelsPlaying]
		
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
