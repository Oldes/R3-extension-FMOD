#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define TO_WIN32

#include "reb-host.h"
#include "reb-ext-local.h"

#include "inc/fmod.h"
#include "inc/fmod_dsp.h"
#include "inc/fmod_errors.h"

#include "rebstr-fce.c"

RL_LIB *RL;


/* error handling */
FMOD_RESULT lastError;
void StoreError(FMOD_RESULT errType) {
	lastError = errType;
	RL->print((REBYTE*)"!!! FMOD ERROR: %d\n", errType);
}
#define ERRCHECK(result)\
{\
	if (result != FMOD_OK)\
	{\
		StoreError(result);\
		return RXR_FALSE;\
	}\
}
/* -------------- */
static u32* fmod_ext_words;

enum fmod_words {
	W_0=0,
	W_Instance,
	W_Environment,
	W_EnvSize,
	W_EnvDiffusion,
	W_Room,
	W_RoomHF,
	W_RoomLF,
	W_DecayTime,
	W_DecayHFRatio,
	W_DecayLFRatio,
	W_Reflections,
	W_ReflectionsDelay,
	W_ReflectionsPan,
	W_Reverb,
	W_ReverbDelay,
	W_ReverbPan,
	W_EchoTime,
	W_EchoDepth,
	W_ModulationTime,
	W_ModulationDepth,
	W_AirAbsorptionHF,
	W_HFReference,
	W_LFReference,
	W_RoomRolloffFactor,
	W_Diffusion,
	W_Density,
	W_Flags,

};

enum fmod_commands {
	CMD_FMOD_init,
	CMD_FMOD_init_words,
	CMD_FMOD_GetLastError,
	CMD_FMOD_createSound,
	CMD_FMOD_createStream,
	CMD_FMOD_channelStop,
	CMD_FMOD_playSound,
	CMD_test_obj,
/* generated */
	CMD_FMOD_Memory_Initialize,
	CMD_FMOD_Memory_GetStats,
	CMD_FMOD_Debug_SetLevel,
	CMD_FMOD_Debug_GetLevel,
	CMD_FMOD_File_SetDiskBusy,
	CMD_FMOD_File_GetDiskBusy,
	CMD_FMOD_System_SetOutput,
	CMD_FMOD_System_GetOutput,
	CMD_FMOD_System_GetNumDrivers,
	CMD_FMOD_System_GetDriverInfo,
	CMD_FMOD_System_GetDriverInfoW,
	CMD_FMOD_System_GetDriverCaps,
	CMD_FMOD_System_SetDriver,
	CMD_FMOD_System_GetDriver,
	CMD_FMOD_System_SetHardwareChannels,
	CMD_FMOD_System_SetSoftwareChannels,
	CMD_FMOD_System_GetSoftwareChannels,
	CMD_FMOD_System_SetSoftwareFormat,
	CMD_FMOD_System_GetSoftwareFormat,
	CMD_FMOD_System_SetDSPBufferSize,
	CMD_FMOD_System_GetDSPBufferSize,
	CMD_FMOD_System_SetFileSystem,
	CMD_FMOD_System_AttachFileSystem,
	CMD_FMOD_System_SetAdvancedSettings,
	CMD_FMOD_System_GetAdvancedSettings,
	CMD_FMOD_System_SetSpeakerMode,
	CMD_FMOD_System_GetSpeakerMode,
	CMD_FMOD_System_SetCallback,
	CMD_FMOD_System_SetPluginPath,
	CMD_FMOD_System_LoadPlugin,
	CMD_FMOD_System_UnloadPlugin,
	CMD_FMOD_System_GetNumPlugins,
	CMD_FMOD_System_GetPluginHandle,
	CMD_FMOD_System_GetPluginInfo,
	CMD_FMOD_System_SetOutputByPlugin,
	CMD_FMOD_System_GetOutputByPlugin,
	CMD_FMOD_System_CreateDSPByPlugin,
	CMD_FMOD_System_CreateCodec,
	CMD_FMOD_System_Close,
	CMD_FMOD_System_Update,
	CMD_FMOD_System_Set3DSettings,
	CMD_FMOD_System_Get3DSettings,
	CMD_FMOD_System_Set3DNumListeners,
	CMD_FMOD_System_Get3DNumListeners,
	CMD_FMOD_System_Set3DListenerAttributes,
	CMD_FMOD_System_Get3DListenerAttributes,
	CMD_FMOD_System_Set3DRolloffCallback,
	CMD_FMOD_System_Set3DSpeakerPosition,
	CMD_FMOD_System_Get3DSpeakerPosition,
	CMD_FMOD_System_SetStreamBufferSize,
	CMD_FMOD_System_GetStreamBufferSize,
	CMD_FMOD_System_GetVersion,
	CMD_FMOD_System_GetOutputHandle,
	CMD_FMOD_System_GetChannelsPlaying,
	CMD_FMOD_System_GetHardwareChannels,
	CMD_FMOD_System_GetCPUUsage,
	CMD_FMOD_System_GetSoundRAM,
	CMD_FMOD_System_GetNumCDROMDrives,
	CMD_FMOD_System_GetCDROMDriveName,
	CMD_FMOD_System_GetSpectrum,
	CMD_FMOD_System_GetWaveData,
	CMD_FMOD_System_CreateSound,
	CMD_FMOD_System_CreateStream,
	CMD_FMOD_System_CreateDSP,
	CMD_FMOD_System_CreateDSPByType,
	CMD_FMOD_System_CreateChannelGroup,
	CMD_FMOD_System_CreateSoundGroup,
	CMD_FMOD_System_CreateReverb,
	CMD_FMOD_System_PlaySound,
	CMD_FMOD_System_PlayDSP,
	CMD_FMOD_System_GetChannel,
	CMD_FMOD_System_GetMasterChannelGroup,
	CMD_FMOD_System_GetMasterSoundGroup,
	CMD_FMOD_System_SetReverbProperties,
	CMD_FMOD_System_GetReverbProperties,
	CMD_FMOD_System_SetReverbAmbientProperties,
	CMD_FMOD_System_GetReverbAmbientProperties,
	CMD_FMOD_System_GetDSPHead,
	CMD_FMOD_System_AddDSP,
	CMD_FMOD_System_LockDSP,
	CMD_FMOD_System_UnlockDSP,
	CMD_FMOD_System_GetDSPClock,
	CMD_FMOD_System_GetRecordNumDrivers,
	CMD_FMOD_System_GetRecordDriverInfo,
	CMD_FMOD_System_GetRecordDriverInfoW,
	CMD_FMOD_System_GetRecordDriverCaps,
	CMD_FMOD_System_GetRecordPosition,
	CMD_FMOD_System_RecordStart,
	CMD_FMOD_System_RecordStop,
	CMD_FMOD_System_IsRecording,
	CMD_FMOD_System_CreateGeometry,
	CMD_FMOD_System_SetGeometrySettings,
	CMD_FMOD_System_GetGeometrySettings,
	CMD_FMOD_System_LoadGeometry,
	CMD_FMOD_System_GetGeometryOcclusion,
	CMD_FMOD_System_SetNetworkProxy,
	CMD_FMOD_System_GetNetworkProxy,
	CMD_FMOD_System_SetNetworkTimeout,
	CMD_FMOD_System_GetNetworkTimeout,
	CMD_FMOD_System_SetUserData,
	CMD_FMOD_System_GetUserData,
	CMD_FMOD_System_GetMemoryInfo,
	CMD_FMOD_Sound_Release,
	CMD_FMOD_Sound_GetSystemObject,
	CMD_FMOD_Sound_Lock,
	CMD_FMOD_Sound_Unlock,
	CMD_FMOD_Sound_SetDefaults,
	CMD_FMOD_Sound_GetDefaults,
	CMD_FMOD_Sound_SetVariations,
	CMD_FMOD_Sound_GetVariations,
	CMD_FMOD_Sound_Set3DMinMaxDistance,
	CMD_FMOD_Sound_Get3DMinMaxDistance,
	CMD_FMOD_Sound_Set3DConeSettings,
	CMD_FMOD_Sound_Get3DConeSettings,
	CMD_FMOD_Sound_Set3DCustomRolloff,
	CMD_FMOD_Sound_Get3DCustomRolloff,
	CMD_FMOD_Sound_SetSubSound,
	CMD_FMOD_Sound_GetSubSound,
	CMD_FMOD_Sound_SetSubSoundSentence,
	CMD_FMOD_Sound_GetName,
	CMD_FMOD_Sound_GetLength,
	CMD_FMOD_Sound_GetFormat,
	CMD_FMOD_Sound_GetNumSubSounds,
	CMD_FMOD_Sound_GetNumTags,
	CMD_FMOD_Sound_GetTag,
	CMD_FMOD_Sound_GetOpenState,
	CMD_FMOD_Sound_ReadData,
	CMD_FMOD_Sound_SeekData,
	CMD_FMOD_Sound_SetSoundGroup,
	CMD_FMOD_Sound_GetSoundGroup,
	CMD_FMOD_Sound_GetNumSyncPoints,
	CMD_FMOD_Sound_GetSyncPoint,
	CMD_FMOD_Sound_GetSyncPointInfo,
	CMD_FMOD_Sound_AddSyncPoint,
	CMD_FMOD_Sound_DeleteSyncPoint,
	CMD_FMOD_Sound_SetMode,
	CMD_FMOD_Sound_GetMode,
	CMD_FMOD_Sound_SetLoopCount,
	CMD_FMOD_Sound_GetLoopCount,
	CMD_FMOD_Sound_SetLoopPoints,
	CMD_FMOD_Sound_GetLoopPoints,
	CMD_FMOD_Sound_GetMusicNumChannels,
	CMD_FMOD_Sound_SetMusicChannelVolume,
	CMD_FMOD_Sound_GetMusicChannelVolume,
	CMD_FMOD_Sound_SetMusicSpeed,
	CMD_FMOD_Sound_GetMusicSpeed,
	CMD_FMOD_Sound_SetUserData,
	CMD_FMOD_Sound_GetUserData,
	CMD_FMOD_Sound_GetMemoryInfo,
	CMD_FMOD_Channel_GetSystemObject,
	CMD_FMOD_Channel_Stop,
	CMD_FMOD_Channel_SetPaused,
	CMD_FMOD_Channel_GetPaused,
	CMD_FMOD_Channel_SetVolume,
	CMD_FMOD_Channel_GetVolume,
	CMD_FMOD_Channel_SetFrequency,
	CMD_FMOD_Channel_GetFrequency,
	CMD_FMOD_Channel_SetPan,
	CMD_FMOD_Channel_GetPan,
	CMD_FMOD_Channel_SetDelay,
	CMD_FMOD_Channel_GetDelay,
	CMD_FMOD_Channel_SetSpeakerMix,
	CMD_FMOD_Channel_GetSpeakerMix,
	CMD_FMOD_Channel_SetSpeakerLevels,
	CMD_FMOD_Channel_GetSpeakerLevels,
	CMD_FMOD_Channel_SetInputChannelMix,
	CMD_FMOD_Channel_GetInputChannelMix,
	CMD_FMOD_Channel_SetMute,
	CMD_FMOD_Channel_GetMute,
	CMD_FMOD_Channel_SetPriority,
	CMD_FMOD_Channel_GetPriority,
	CMD_FMOD_Channel_SetPosition,
	CMD_FMOD_Channel_GetPosition,
	CMD_FMOD_Channel_SetReverbProperties,
	CMD_FMOD_Channel_GetReverbProperties,
	CMD_FMOD_Channel_SetLowPassGain,
	CMD_FMOD_Channel_GetLowPassGain,
	CMD_FMOD_Channel_SetChannelGroup,
	CMD_FMOD_Channel_GetChannelGroup,
	CMD_FMOD_Channel_SetCallback,
	CMD_FMOD_Channel_Set3DAttributes,
	CMD_FMOD_Channel_Get3DAttributes,
	CMD_FMOD_Channel_Set3DMinMaxDistance,
	CMD_FMOD_Channel_Get3DMinMaxDistance,
	CMD_FMOD_Channel_Set3DConeSettings,
	CMD_FMOD_Channel_Get3DConeSettings,
	CMD_FMOD_Channel_Set3DConeOrientation,
	CMD_FMOD_Channel_Get3DConeOrientation,
	CMD_FMOD_Channel_Set3DCustomRolloff,
	CMD_FMOD_Channel_Get3DCustomRolloff,
	CMD_FMOD_Channel_Set3DOcclusion,
	CMD_FMOD_Channel_Get3DOcclusion,
	CMD_FMOD_Channel_Set3DSpread,
	CMD_FMOD_Channel_Get3DSpread,
	CMD_FMOD_Channel_Set3DPanLevel,
	CMD_FMOD_Channel_Get3DPanLevel,
	CMD_FMOD_Channel_Set3DDopplerLevel,
	CMD_FMOD_Channel_Get3DDopplerLevel,
	CMD_FMOD_Channel_GetDSPHead,
	CMD_FMOD_Channel_AddDSP,
	CMD_FMOD_Channel_IsPlaying,
	CMD_FMOD_Channel_IsVirtual,
	CMD_FMOD_Channel_GetAudibility,
	CMD_FMOD_Channel_GetCurrentSound,
	CMD_FMOD_Channel_GetSpectrum,
	CMD_FMOD_Channel_GetWaveData,
	CMD_FMOD_Channel_GetIndex,
	CMD_FMOD_Channel_SetMode,
	CMD_FMOD_Channel_GetMode,
	CMD_FMOD_Channel_SetLoopCount,
	CMD_FMOD_Channel_GetLoopCount,
	CMD_FMOD_Channel_SetLoopPoints,
	CMD_FMOD_Channel_GetLoopPoints,
	CMD_FMOD_Channel_SetUserData,
	CMD_FMOD_Channel_GetUserData,
	CMD_FMOD_Channel_GetMemoryInfo,
	CMD_FMOD_ChannelGroup_Release,
	CMD_FMOD_ChannelGroup_GetSystemObject,
	CMD_FMOD_ChannelGroup_SetVolume,
	CMD_FMOD_ChannelGroup_GetVolume,
	CMD_FMOD_ChannelGroup_SetPitch,
	CMD_FMOD_ChannelGroup_GetPitch,
	CMD_FMOD_ChannelGroup_Set3DOcclusion,
	CMD_FMOD_ChannelGroup_Get3DOcclusion,
	CMD_FMOD_ChannelGroup_SetPaused,
	CMD_FMOD_ChannelGroup_GetPaused,
	CMD_FMOD_ChannelGroup_SetMute,
	CMD_FMOD_ChannelGroup_GetMute,
	CMD_FMOD_ChannelGroup_Stop,
	CMD_FMOD_ChannelGroup_OverrideVolume,
	CMD_FMOD_ChannelGroup_OverrideFrequency,
	CMD_FMOD_ChannelGroup_OverridePan,
	CMD_FMOD_ChannelGroup_OverrideReverbProperties,
	CMD_FMOD_ChannelGroup_Override3DAttributes,
	CMD_FMOD_ChannelGroup_OverrideSpeakerMix,
	CMD_FMOD_ChannelGroup_AddGroup,
	CMD_FMOD_ChannelGroup_GetNumGroups,
	CMD_FMOD_ChannelGroup_GetGroup,
	CMD_FMOD_ChannelGroup_GetParentGroup,
	CMD_FMOD_ChannelGroup_GetDSPHead,
	CMD_FMOD_ChannelGroup_AddDSP,
	CMD_FMOD_ChannelGroup_GetName,
	CMD_FMOD_ChannelGroup_GetNumChannels,
	CMD_FMOD_ChannelGroup_GetChannel,
	CMD_FMOD_ChannelGroup_GetSpectrum,
	CMD_FMOD_ChannelGroup_GetWaveData,
	CMD_FMOD_ChannelGroup_SetUserData,
	CMD_FMOD_ChannelGroup_GetUserData,
	CMD_FMOD_ChannelGroup_GetMemoryInfo,
	CMD_FMOD_SoundGroup_Release,
	CMD_FMOD_SoundGroup_GetSystemObject,
	CMD_FMOD_SoundGroup_SetMaxAudible,
	CMD_FMOD_SoundGroup_GetMaxAudible,
	CMD_FMOD_SoundGroup_SetMaxAudibleBehavior,
	CMD_FMOD_SoundGroup_GetMaxAudibleBehavior,
	CMD_FMOD_SoundGroup_SetMuteFadeSpeed,
	CMD_FMOD_SoundGroup_GetMuteFadeSpeed,
	CMD_FMOD_SoundGroup_SetVolume,
	CMD_FMOD_SoundGroup_GetVolume,
	CMD_FMOD_SoundGroup_Stop,
	CMD_FMOD_SoundGroup_GetName,
	CMD_FMOD_SoundGroup_GetNumSounds,
	CMD_FMOD_SoundGroup_GetSound,
	CMD_FMOD_SoundGroup_GetNumPlaying,
	CMD_FMOD_SoundGroup_SetUserData,
	CMD_FMOD_SoundGroup_GetUserData,
	CMD_FMOD_SoundGroup_GetMemoryInfo,
	CMD_FMOD_DSP_Release,
	CMD_FMOD_DSP_GetSystemObject,
	CMD_FMOD_DSP_AddInput,
	CMD_FMOD_DSP_DisconnectFrom,
	CMD_FMOD_DSP_DisconnectAll,
	CMD_FMOD_DSP_Remove,
	CMD_FMOD_DSP_GetNumInputs,
	CMD_FMOD_DSP_GetNumOutputs,
	CMD_FMOD_DSP_GetInput,
	CMD_FMOD_DSP_GetOutput,
	CMD_FMOD_DSP_SetActive,
	CMD_FMOD_DSP_GetActive,
	CMD_FMOD_DSP_SetBypass,
	CMD_FMOD_DSP_GetBypass,
	CMD_FMOD_DSP_SetSpeakerActive,
	CMD_FMOD_DSP_GetSpeakerActive,
	CMD_FMOD_DSP_Reset,
	CMD_FMOD_DSP_SetParameter,
	CMD_FMOD_DSP_GetParameter,
	CMD_FMOD_DSP_GetNumParameters,
	CMD_FMOD_DSP_GetParameterInfo,
	CMD_FMOD_DSP_ShowConfigDialog,
	CMD_FMOD_DSP_GetInfo,
	CMD_FMOD_DSP_GetType,
	CMD_FMOD_DSP_SetDefaults,
	CMD_FMOD_DSP_GetDefaults,
	CMD_FMOD_DSP_SetUserData,
	CMD_FMOD_DSP_GetUserData,
	CMD_FMOD_DSP_GetMemoryInfo,
	CMD_FMOD_DSPConnection_GetInput,
	CMD_FMOD_DSPConnection_GetOutput,
	CMD_FMOD_DSPConnection_SetMix,
	CMD_FMOD_DSPConnection_GetMix,
	CMD_FMOD_DSPConnection_SetLevels,
	CMD_FMOD_DSPConnection_GetLevels,
	CMD_FMOD_DSPConnection_SetUserData,
	CMD_FMOD_DSPConnection_GetUserData,
	CMD_FMOD_DSPConnection_GetMemoryInfo,
	CMD_FMOD_Geometry_Release,
	CMD_FMOD_Geometry_AddPolygon,
	CMD_FMOD_Geometry_GetNumPolygons,
	CMD_FMOD_Geometry_GetMaxPolygons,
	CMD_FMOD_Geometry_GetPolygonNumVertices,
	CMD_FMOD_Geometry_SetPolygonVertex,
	CMD_FMOD_Geometry_GetPolygonVertex,
	CMD_FMOD_Geometry_SetPolygonAttributes,
	CMD_FMOD_Geometry_GetPolygonAttributes,
	CMD_FMOD_Geometry_SetActive,
	CMD_FMOD_Geometry_GetActive,
	CMD_FMOD_Geometry_SetRotation,
	CMD_FMOD_Geometry_GetRotation,
	CMD_FMOD_Geometry_SetPosition,
	CMD_FMOD_Geometry_GetPosition,
	CMD_FMOD_Geometry_SetScale,
	CMD_FMOD_Geometry_GetScale,
	CMD_FMOD_Geometry_Save,
	CMD_FMOD_Geometry_SetUserData,
	CMD_FMOD_Geometry_GetUserData,
	CMD_FMOD_Geometry_GetMemoryInfo,
	CMD_FMOD_Reverb_Release,
	CMD_FMOD_Reverb_Set3DAttributes,
	CMD_FMOD_Reverb_Get3DAttributes,
	CMD_FMOD_Reverb_SetProperties,
	CMD_FMOD_Reverb_GetProperties,
	CMD_FMOD_Reverb_SetActive,
	CMD_FMOD_Reverb_GetActive,
	CMD_FMOD_Reverb_SetUserData,
	CMD_FMOD_Reverb_GetUserData,
	CMD_FMOD_Reverb_GetMemoryInfo,

};

const char *init_block =
	"REBOL [\n"
		"Title: {Testing FMOD extension}\n"
		"Name: FMOD\n"
		"Type: extension\n"
	"]\n"
	"export init: command []\n"
	"export init-words: command [words [block!]]\n"
	"init-words [Instance Environment EnvSize EnvDiffusion Room RoomHF RoomLF DecayTime DecayHFRatio DecayLFRatio Reflections ReflectionsDelay ReflectionsPan Reverb ReverbDelay ReverbPan EchoTime EchoDepth ModulationTime ModulationDepth AirAbsorptionHF HFReference LFReference RoomRolloffFactor Diffusion Density Flags ]\n"
	"export GetLastError: command []\n"
	"export createSound: command [file [string!]]\n"
	"export createStream: command [file [string!]]\n"
	"export channelStop: command [channel [handle! none!]]\n"
	"export playSound: command [snd [handle!]]\n"
	"export test-obj: command [obj [object!]]\n"
/* generated */
	"export Memory: context[\n"
		"Initialize: command[{TODO}]\n"
		"GetStats: command[blocking [logic!] ]\n"
	"]\n"
	"export Debug: context[\n"
		"SetLevel: command[{TODO}]\n"
		"GetLevel: command[{TODO}]\n"
	"]\n"
	"export File: context[\n"
		"SetDiskBusy: command[busy [integer!] ]\n"
		"GetDiskBusy: command[]\n"
	"]\n"
	"export System: context[\n"
		"SetOutput: command[output [integer!] ]\n"
		"GetOutput: command[]\n"
		"GetNumDrivers: command[]\n"
		"GetDriverInfo: command[id [integer!] ]\n"
		"GetDriverInfoW: command[id [integer!] ]\n"
		"GetDriverCaps: command[id [integer!] ]\n"
		"SetDriver: command[driver [integer!] ]\n"
		"GetDriver: command[]\n"
		"SetHardwareChannels: command[min2d [integer!] max2d [integer!] min3d [integer!] max3d [integer!] ]\n"
		"SetSoftwareChannels: command[numsoftwarechannels [integer!] ]\n"
		"GetSoftwareChannels: command[]\n"
		"SetSoftwareFormat: command[samplerate [integer!] format [integer!] numoutputchannels [integer!] maxinputchannels [integer!] resamplemethod [integer!] ]\n"
		"GetSoftwareFormat: command[]\n"
		"SetDSPBufferSize: command[{TODO}]\n"
		"GetDSPBufferSize: command[{TODO}]\n"
		"SetFileSystem: command[{TODO}]\n"
		"AttachFileSystem: command[{TODO}]\n"
		"SetAdvancedSettings: command[{TODO}]\n"
		"GetAdvancedSettings: command[{TODO}]\n"
		"SetSpeakerMode: command[speakermode [integer!] ]\n"
		"GetSpeakerMode: command[]\n"
		"SetCallback: command[{TODO}]\n"
		"SetPluginPath: command[{TODO}]\n"
		"LoadPlugin: command[{TODO}]\n"
		"UnloadPlugin: command[{TODO}]\n"
		"GetNumPlugins: command[{TODO}]\n"
		"GetPluginHandle: command[{TODO}]\n"
		"GetPluginInfo: command[{TODO}]\n"
		"SetOutputByPlugin: command[{TODO}]\n"
		"GetOutputByPlugin: command[{TODO}]\n"
		"CreateDSPByPlugin: command[{TODO}]\n"
		"CreateCodec: command[{TODO}]\n"
		"Close: command[]\n"
		"Update: command[]\n"
		"Set3DSettings: command[{TODO}]\n"
		"Get3DSettings: command[{TODO}]\n"
		"Set3DNumListeners: command[{TODO}]\n"
		"Get3DNumListeners: command[{TODO}]\n"
		"Set3DListenerAttributes: command[{TODO}]\n"
		"Get3DListenerAttributes: command[{TODO}]\n"
		"Set3DRolloffCallback: command[{TODO}]\n"
		"Set3DSpeakerPosition: command[{TODO}]\n"
		"Get3DSpeakerPosition: command[{TODO}]\n"
		"SetStreamBufferSize: command[filebuffersize [integer!] filebuffersizetype [integer!] ]\n"
		"GetStreamBufferSize: command[]\n"
		"GetVersion: command[]\n"
		"GetOutputHandle: command[]\n"
		"GetChannelsPlaying: command[]\n"
		"GetHardwareChannels: command[]\n"
		"GetCPUUsage: command[]\n"
		"GetSoundRAM: command[]\n"
		"GetNumCDROMDrives: command[]\n"
		"GetCDROMDriveName: command[drive [integer!] ]\n"
		"GetSpectrum: command[numvalues [integer!] channeloffset [integer!] windowtype [integer!] ]\n"
		"GetWaveData: command[numvalues [integer!] channeloffset [integer!] ]\n"
		"CreateSound: command[name_or_data [string!] mode [integer!] ]\n"
		"CreateStream: command[name_or_data [string!] mode [integer!] ]\n"
		"CreateDSP: command[{TODO}]\n"
		"CreateDSPByType: command[{TODO}]\n"
		"CreateChannelGroup: command[name [string!] ]\n"
		"CreateSoundGroup: command[name [string!] ]\n"
		"CreateReverb: command[]\n"
		"PlaySound: command[channelid [integer!] sound [handle!] paused [logic!] ]\n"
		"PlayDSP: command[{TODO}]\n"
		"GetChannel: command[channelid [integer!] ]\n"
		"GetMasterChannelGroup: command[]\n"
		"GetMasterSoundGroup: command[]\n"
		"SetReverbProperties: command[{TODO}]\n"
		"GetReverbProperties: command[{TODO}]\n"
		"SetReverbAmbientProperties: command[{TODO}]\n"
		"GetReverbAmbientProperties: command[{TODO}]\n"
		"GetDSPHead: command[{TODO}]\n"
		"AddDSP: command[{TODO}]\n"
		"LockDSP: command[{TODO}]\n"
		"UnlockDSP: command[{TODO}]\n"
		"GetDSPClock: command[{TODO}]\n"
		"GetRecordNumDrivers: command[]\n"
		"GetRecordDriverInfo: command[id [integer!] ]\n"
		"GetRecordDriverInfoW: command[id [integer!] ]\n"
		"GetRecordDriverCaps: command[id [integer!] ]\n"
		"GetRecordPosition: command[id [integer!] ]\n"
		"RecordStart: command[id [integer!] sound [handle!] loop [logic!] ]\n"
		"RecordStop: command[id [integer!] ]\n"
		"IsRecording: command[id [integer!] ]\n"
		"CreateGeometry: command[{TODO}]\n"
		"SetGeometrySettings: command[{TODO}]\n"
		"GetGeometrySettings: command[{TODO}]\n"
		"LoadGeometry: command[{TODO}]\n"
		"GetGeometryOcclusion: command[{TODO}]\n"
		"SetNetworkProxy: command[proxy [string!] ]\n"
		"GetNetworkProxy: command[]\n"
		"SetNetworkTimeout: command[timeout [integer!] ]\n"
		"GetNetworkTimeout: command[]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"
	"export Sound: context[\n"
		"Release: command[sound [handle!] ]\n"
		"GetSystemObject: command[sound [handle!] ]\n"
		"Lock: command[sound [handle!] offset [integer!] length [integer!] ]\n"
		"Unlock: command[sound [handle!] ptr1 [handle!] ptr2 [handle!] len1 [integer!] len2 [integer!] ]\n"
		"SetDefaults: command[sound [handle!] frequency [decimal!] volume [decimal!] pan [decimal!] priority [integer!] ]\n"
		"GetDefaults: command[sound [handle!] ]\n"
		"SetVariations: command[sound [handle!] frequencyvar [decimal!] volumevar [decimal!] panvar [decimal!] ]\n"
		"GetVariations: command[sound [handle!] ]\n"
		"Set3DMinMaxDistance: command[{TODO}]\n"
		"Get3DMinMaxDistance: command[{TODO}]\n"
		"Set3DConeSettings: command[{TODO}]\n"
		"Get3DConeSettings: command[{TODO}]\n"
		"Set3DCustomRolloff: command[{TODO}]\n"
		"Get3DCustomRolloff: command[{TODO}]\n"
		"SetSubSound: command[sound [handle!] index [integer!] subsound [handle!] ]\n"
		"GetSubSound: command[sound [handle!] index [integer!] ]\n"
		"SetSubSoundSentence: command[sound [handle!] numsubsounds [integer!] ]\n"
		"GetName: command[sound [handle!] ]\n"
		"GetLength: command[sound [handle!] lengthtype [integer!] ]\n"
		"GetFormat: command[sound [handle!] ]\n"
		"GetNumSubSounds: command[sound [handle!] ]\n"
		"GetNumTags: command[sound [handle!] ]\n"
		"GetTag: command[sound [handle!] name [string!] index [integer!] ]\n"
		"GetOpenState: command[sound [handle!] ]\n"
		"ReadData: command[sound [handle!] buffer [handle!] lenbytes [integer!] ]\n"
		"SeekData: command[sound [handle!] pcm [integer!] ]\n"
		"SetSoundGroup: command[sound [handle!] soundgroup [handle!] ]\n"
		"GetSoundGroup: command[sound [handle!] ]\n"
		"GetNumSyncPoints: command[sound [handle!] ]\n"
		"GetSyncPoint: command[sound [handle!] index [integer!] ]\n"
		"GetSyncPointInfo: command[sound [handle!] point [handle!] offsettype [integer!] ]\n"
		"AddSyncPoint: command[sound [handle!] offset [integer!] offsettype [integer!] name [string!] ]\n"
		"DeleteSyncPoint: command[sound [handle!] point [handle!] ]\n"
		"SetMode: command[sound [handle!] mode [integer!] ]\n"
		"GetMode: command[sound [handle!] ]\n"
		"SetLoopCount: command[sound [handle!] loopcount [integer!] ]\n"
		"GetLoopCount: command[sound [handle!] ]\n"
		"SetLoopPoints: command[sound [handle!] loopstart [integer!] loopstarttype [integer!] loopend [integer!] loopendtype [integer!] ]\n"
		"GetLoopPoints: command[sound [handle!] loopstarttype [integer!] loopendtype [integer!] ]\n"
		"GetMusicNumChannels: command[sound [handle!] ]\n"
		"SetMusicChannelVolume: command[sound [handle!] channel [integer!] volume [decimal!] ]\n"
		"GetMusicChannelVolume: command[sound [handle!] channel [integer!] ]\n"
		"SetMusicSpeed: command[sound [handle!] speed [decimal!] ]\n"
		"GetMusicSpeed: command[sound [handle!] ]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[sound [handle!] memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"
	"export Channel: context[\n"
		"GetSystemObject: command[channel [handle!] ]\n"
		"Stop: command[channel [handle!] ]\n"
		"SetPaused: command[channel [handle!] paused [logic!] ]\n"
		"GetPaused: command[channel [handle!] ]\n"
		"SetVolume: command[channel [handle!] volume [decimal!] ]\n"
		"GetVolume: command[channel [handle!] ]\n"
		"SetFrequency: command[channel [handle!] frequency [decimal!] ]\n"
		"GetFrequency: command[channel [handle!] ]\n"
		"SetPan: command[channel [handle!] pan [decimal!] ]\n"
		"GetPan: command[channel [handle!] ]\n"
		"SetDelay: command[channel [handle!] delaytype [integer!] delayhi [integer!] delaylo [integer!] ]\n"
		"GetDelay: command[channel [handle!] delaytype [integer!] ]\n"
		"SetSpeakerMix: command[channel [handle!] mixdata [block!] {[frontleft frontright center lfe backleft backright sideleft sideright]} ]\n"
		"GetSpeakerMix: command[channel [handle!] ]\n"
		"SetSpeakerLevels: command[channel [handle!] speaker [integer!] numlevels [integer!] ]\n"
		"GetSpeakerLevels: command[channel [handle!] speaker [integer!] numlevels [integer!] ]\n"
		"SetInputChannelMix: command[channel [handle!] numlevels [integer!] ]\n"
		"GetInputChannelMix: command[channel [handle!] numlevels [integer!] ]\n"
		"SetMute: command[channel [handle!] mute [logic!] ]\n"
		"GetMute: command[channel [handle!] ]\n"
		"SetPriority: command[channel [handle!] priority [integer!] ]\n"
		"GetPriority: command[channel [handle!] ]\n"
		"SetPosition: command[channel [handle!] position [integer!] postype [integer!] ]\n"
		"GetPosition: command[channel [handle!] postype [integer!] ]\n"
		"SetReverbProperties: command[{TODO}]\n"
		"GetReverbProperties: command[{TODO}]\n"
		"SetLowPassGain: command[channel [handle!] gain [decimal!] ]\n"
		"GetLowPassGain: command[channel [handle!] ]\n"
		"SetChannelGroup: command[channel [handle!] channelgroup [handle!] ]\n"
		"GetChannelGroup: command[channel [handle!] ]\n"
		"SetCallback: command[{TODO}]\n"
		"Set3DAttributes: command[{TODO}]\n"
		"Get3DAttributes: command[{TODO}]\n"
		"Set3DMinMaxDistance: command[{TODO}]\n"
		"Get3DMinMaxDistance: command[{TODO}]\n"
		"Set3DConeSettings: command[{TODO}]\n"
		"Get3DConeSettings: command[{TODO}]\n"
		"Set3DConeOrientation: command[{TODO}]\n"
		"Get3DConeOrientation: command[{TODO}]\n"
		"Set3DCustomRolloff: command[{TODO}]\n"
		"Get3DCustomRolloff: command[{TODO}]\n"
		"Set3DOcclusion: command[{TODO}]\n"
		"Get3DOcclusion: command[{TODO}]\n"
		"Set3DSpread: command[{TODO}]\n"
		"Get3DSpread: command[{TODO}]\n"
		"Set3DPanLevel: command[{TODO}]\n"
		"Get3DPanLevel: command[{TODO}]\n"
		"Set3DDopplerLevel: command[{TODO}]\n"
		"Get3DDopplerLevel: command[{TODO}]\n"
		"GetDSPHead: command[{TODO}]\n"
		"AddDSP: command[{TODO}]\n"
		"IsPlaying: command[channel [handle!] ]\n"
		"IsVirtual: command[channel [handle!] ]\n"
		"GetAudibility: command[channel [handle!] ]\n"
		"GetCurrentSound: command[channel [handle!] ]\n"
		"GetSpectrum: command[channel [handle!] numvalues [integer!] channeloffset [integer!] windowtype [integer!] ]\n"
		"GetWaveData: command[channel [handle!] numvalues [integer!] channeloffset [integer!] ]\n"
		"GetIndex: command[channel [handle!] ]\n"
		"SetMode: command[channel [handle!] mode [integer!] ]\n"
		"GetMode: command[channel [handle!] ]\n"
		"SetLoopCount: command[channel [handle!] loopcount [integer!] ]\n"
		"GetLoopCount: command[channel [handle!] ]\n"
		"SetLoopPoints: command[channel [handle!] loopstart [integer!] loopstarttype [integer!] loopend [integer!] loopendtype [integer!] ]\n"
		"GetLoopPoints: command[channel [handle!] loopstarttype [integer!] loopendtype [integer!] ]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[channel [handle!] memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"
	"export ChannelGroup: context[\n"
		"Release: command[channelgroup [handle!] ]\n"
		"GetSystemObject: command[channelgroup [handle!] ]\n"
		"SetVolume: command[channelgroup [handle!] volume [decimal!] ]\n"
		"GetVolume: command[channelgroup [handle!] ]\n"
		"SetPitch: command[channelgroup [handle!] pitch [decimal!] ]\n"
		"GetPitch: command[channelgroup [handle!] ]\n"
		"Set3DOcclusion: command[{TODO}]\n"
		"Get3DOcclusion: command[{TODO}]\n"
		"SetPaused: command[channelgroup [handle!] paused [logic!] ]\n"
		"GetPaused: command[channelgroup [handle!] ]\n"
		"SetMute: command[channelgroup [handle!] mute [logic!] ]\n"
		"GetMute: command[channelgroup [handle!] ]\n"
		"Stop: command[channelgroup [handle!] ]\n"
		"OverrideVolume: command[channelgroup [handle!] volume [decimal!] ]\n"
		"OverrideFrequency: command[channelgroup [handle!] frequency [decimal!] ]\n"
		"OverridePan: command[channelgroup [handle!] pan [decimal!] ]\n"
		"OverrideReverbProperties: command[{TODO}]\n"
		"Override3DAttributes: command[{TODO}]\n"
		"OverrideSpeakerMix: command[channelgroup [handle!] mixdata [block!] {[frontleft frontright center lfe backleft backright sideleft sideright]} ]\n"
		"AddGroup: command[channelgroup [handle!] group [handle!] ]\n"
		"GetNumGroups: command[channelgroup [handle!] ]\n"
		"GetGroup: command[channelgroup [handle!] index [integer!] ]\n"
		"GetParentGroup: command[channelgroup [handle!] ]\n"
		"GetDSPHead: command[{TODO}]\n"
		"AddDSP: command[{TODO}]\n"
		"GetName: command[channelgroup [handle!] ]\n"
		"GetNumChannels: command[channelgroup [handle!] ]\n"
		"GetChannel: command[channelgroup [handle!] index [integer!] ]\n"
		"GetSpectrum: command[channelgroup [handle!] numvalues [integer!] channeloffset [integer!] windowtype [integer!] ]\n"
		"GetWaveData: command[channelgroup [handle!] numvalues [integer!] channeloffset [integer!] ]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[channelgroup [handle!] memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"
	"export SoundGroup: context[\n"
		"Release: command[soundgroup [handle!] ]\n"
		"GetSystemObject: command[soundgroup [handle!] ]\n"
		"SetMaxAudible: command[soundgroup [handle!] maxaudible [integer!] ]\n"
		"GetMaxAudible: command[soundgroup [handle!] ]\n"
		"SetMaxAudibleBehavior: command[soundgroup [handle!] behavior [integer!] ]\n"
		"GetMaxAudibleBehavior: command[soundgroup [handle!] ]\n"
		"SetMuteFadeSpeed: command[soundgroup [handle!] speed [decimal!] ]\n"
		"GetMuteFadeSpeed: command[soundgroup [handle!] ]\n"
		"SetVolume: command[soundgroup [handle!] volume [decimal!] ]\n"
		"GetVolume: command[soundgroup [handle!] ]\n"
		"Stop: command[soundgroup [handle!] ]\n"
		"GetName: command[soundgroup [handle!] ]\n"
		"GetNumSounds: command[soundgroup [handle!] ]\n"
		"GetSound: command[soundgroup [handle!] index [integer!] ]\n"
		"GetNumPlaying: command[soundgroup [handle!] ]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[soundgroup [handle!] memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"
	"export DSP: context[\n"
		"Release: command[{TODO}]\n"
		"GetSystemObject: command[{TODO}]\n"
		"AddInput: command[{TODO}]\n"
		"DisconnectFrom: command[{TODO}]\n"
		"DisconnectAll: command[{TODO}]\n"
		"Remove: command[{TODO}]\n"
		"GetNumInputs: command[{TODO}]\n"
		"GetNumOutputs: command[{TODO}]\n"
		"GetInput: command[{TODO}]\n"
		"GetOutput: command[{TODO}]\n"
		"SetActive: command[{TODO}]\n"
		"GetActive: command[{TODO}]\n"
		"SetBypass: command[{TODO}]\n"
		"GetBypass: command[{TODO}]\n"
		"SetSpeakerActive: command[{TODO}]\n"
		"GetSpeakerActive: command[{TODO}]\n"
		"Reset: command[{TODO}]\n"
		"SetParameter: command[{TODO}]\n"
		"GetParameter: command[{TODO}]\n"
		"GetNumParameters: command[{TODO}]\n"
		"GetParameterInfo: command[{TODO}]\n"
		"ShowConfigDialog: command[{TODO}]\n"
		"GetInfo: command[{TODO}]\n"
		"GetType: command[{TODO}]\n"
		"SetDefaults: command[{TODO}]\n"
		"GetDefaults: command[{TODO}]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[{TODO}]\n"
	"]\n"
	"export DSPConnection: context[\n"
		"GetInput: command[{TODO}]\n"
		"GetOutput: command[{TODO}]\n"
		"SetMix: command[{TODO}]\n"
		"GetMix: command[{TODO}]\n"
		"SetLevels: command[{TODO}]\n"
		"GetLevels: command[{TODO}]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[{TODO}]\n"
	"]\n"
	"export Geometry: context[\n"
		"Release: command[{TODO}]\n"
		"AddPolygon: command[{TODO}]\n"
		"GetNumPolygons: command[{TODO}]\n"
		"GetMaxPolygons: command[{TODO}]\n"
		"GetPolygonNumVertices: command[{TODO}]\n"
		"SetPolygonVertex: command[{TODO}]\n"
		"GetPolygonVertex: command[{TODO}]\n"
		"SetPolygonAttributes: command[{TODO}]\n"
		"GetPolygonAttributes: command[{TODO}]\n"
		"SetActive: command[{TODO}]\n"
		"GetActive: command[{TODO}]\n"
		"SetRotation: command[{TODO}]\n"
		"GetRotation: command[{TODO}]\n"
		"SetPosition: command[{TODO}]\n"
		"GetPosition: command[{TODO}]\n"
		"SetScale: command[{TODO}]\n"
		"GetScale: command[{TODO}]\n"
		"Save: command[{TODO}]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[{TODO}]\n"
	"]\n"
	"export Reverb: context[\n"
		"Release: command[reverb [handle!] ]\n"
		"Set3DAttributes: command[{TODO}]\n"
		"Get3DAttributes: command[{TODO}]\n"
		"SetProperties: command[{TODO}]\n"
		"GetProperties: command[{TODO}]\n"
		"SetActive: command[reverb [handle!] active [logic!] ]\n"
		"GetActive: command[reverb [handle!] ]\n"
		"SetUserData: command[{TODO}]\n"
		"GetUserData: command[{TODO}]\n"
		"GetMemoryInfo: command[reverb [handle!] memorybits [integer!] event_memorybits [integer!] ]\n"
	"]\n"

;



static FMOD_SYSTEM *sys=NULL;
FMOD_RESULT 	 result;
FMOD_SOUND		*sound		   = 0;
FMOD_CHANNEL	*channel	   = 0;
FMOD_DSP		*dsplowpass    = 0;
FMOD_DSP		*dsphighpass   = 0;
FMOD_DSP		*dspecho	   = 0;
FMOD_DSP		*dspflange	   = 0;
FMOD_DSP		*dspdistortion = 0;
FMOD_DSP		*dspchorus	   = 0;
FMOD_DSP		*dspparameq    = 0;
char  *tmpBuffer1 = 0;
char  *tmpBuffer2 = 0;
char  *tmpBuffer3 = 0;
char  *tmpBuffer4 = 0;


RXIEXT const char *RX_Init(int opts, RL_LIB *lib) {
	RL = lib;
	unsigned int version;
	FMOD_System_Create( &sys );
	FMOD_System_GetVersion(sys, &version);
	RL->print((REBYTE*)"FMOD version: %d, require: %d\n", version, FMOD_VERSION);
	if (version < FMOD_VERSION) return "REBOL[]";
	tmpBuffer1=(char *)malloc(256);
	tmpBuffer2=(char *)malloc(256);
	tmpBuffer3=(char *)malloc(256);
	tmpBuffer4=(char *)malloc(256);
	return init_block;
}

int RX_Call(int cmd, RXIFRM *frm, void *data) {
	switch (cmd) {
		case CMD_FMOD_init: {

			ERRCHECK(FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL));

			/*
				Create some effects to play with.
			*/
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_LOWPASS, &dsplowpass));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_HIGHPASS, &dsphighpass));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_ECHO, &dspecho));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_FLANGE, &dspflange));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_DISTORTION, &dspdistortion));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_CHORUS, &dspchorus));
			ERRCHECK(FMOD_System_CreateDSPByType(sys, FMOD_DSP_TYPE_PARAMEQ, &dspparameq));

			return RXR_TRUE;
		}
		case CMD_FMOD_init_words: {
            fmod_ext_words = RL_MAP_WORDS(RXA_SERIES(frm,1));
            return RXR_UNSET;
		}
		case CMD_FMOD_GetLastError: {
			RXA_INT64(frm, 1) = (int)lastError;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_createSound: {
			FMOD_SOUND *newSound = 0;
			const char *name = RebserToMultiByte(RXA_SERIES(frm, 1));
			result = FMOD_System_CreateSound(
			  sys,
			  name,
			  FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D,
			  0,
			  &newSound
			);
			free((void*)name);
			ERRCHECK(result);

			RL->print((REBYTE*)"newSound: %d\n", (REBINT)newSound);
			RXA_INT64(frm, 1) = (int)newSound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_createStream: {
			FMOD_SOUND *newSound = 0;
			const char *name = RebserToMultiByte(RXA_SERIES(frm, 1));
			result = FMOD_System_CreateStream(
				sys,
				name,
				FMOD_HARDWARE |FMOD_LOOP_NORMAL | FMOD_2D,
				0,
				&newSound
			);
			free((void*)name);
			ERRCHECK(result);

			RL->print((REBYTE*)"newStream: %d\n", (REBINT)newSound);
			RXA_INT64(frm, 1) = (int)newSound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_playSound: {
			RL->print((REBYTE*)"playSound: %d\n", (REBINT)RXA_HANDLE(frm, 1));
			result = FMOD_System_PlaySound(
			  sys,
			  FMOD_CHANNEL_FREE,
			  (FMOD_SOUND *)RXA_HANDLE(frm, 1),
			  FALSE,
			  &channel
			);
			ERRCHECK(result);

			RL->print((REBYTE*)"channel %d\n", (REBINT)channel);

			//FMOD_Channel_SetPan(channel, -1 );


		   // result = FMOD_System_AddDSP(sys, dspecho, 0);
		   // result = FMOD_System_AddDSP(sys, dspflange, 0);
			/*FMOD_Channel_SetReverbProperties(
			  &channel,
			  const FMOD_REVERB_CHANNELPROPERTIES *  prop
			);
			*/

			/*result = FMOD_Channel_SetPosition(
			  channel,
			  2000,
			  FMOD_TIMEUNIT_MS
			);
			ERRCHECK(result);
			*/
			//channel = newChannel;
			RXA_INT64(frm, 1) = (int)channel;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_channelStop: {
			if (RXA_TYPE(frm, 1) == RXT_HANDLE) {
				channel = RXA_HANDLE(frm, 1);
			}
			result = FMOD_Channel_Stop(channel);
			ERRCHECK(result);
			return RXR_TRUE;
		}
		case CMD_test_obj: {

            FMOD_REVERB_PROPERTIES prop;
            ERRCHECK(FMOD_System_GetReverbProperties(sys, &prop));
            REBSER *obj=0;
            RXIARG val;

            RXV_INT64(val) = prop.Instance;
            RL_SET_FIELD(obj,fmod_ext_words[W_Instance],val,RXT_INTEGER);



			    //just testing.......
			    /*
            u32 *words, *w;
            REBSER *obj;
            REBCNT type;
            RXIARG result;
            obj = RXA_OBJECT(frm, 1);
            words = RL_WORDS_OF_OBJECT(obj);
            w = words;
            while ((type = RL_GET_FIELD(obj, w[0], &result))) {
                RL->print((REBYTE*)"word: %d %d\n", w[0], RL_FIND_WORD(fmod_ext_words,w[0]));
                switch(RL_FIND_WORD(fmod_ext_words,w[0])){
                    case W_Room: {
                        RL->print((REBYTE*)"Room: %d\n",4);
                        RXV_DEC64(result) = 3 * RXV_DEC64(result);
                        RL_SET_FIELD(obj, w[0], result, RXT_DECIMAL );
                        break;
                    }
                    case W_Reverb: {
                        RL->print((REBYTE*)"Reverb: %d\n",RXV_DEC64(result));
                        if (type == RXT_INTEGER) {
                            RXV_DEC64(result) = (float)RXV_INT64(result);
                            RL_SET_FIELD(obj, w[0], result, RXT_DECIMAL );
                        }
                        break;
                    }
                }
                //RL->print((REBYTE*)"type:\n");
                w++;
            }

            */
          //  RL_GET_FIELD(obj, RL_FIND_WORD(fmod_ext_words, 2), &result);
          //  RL->print((REBYTE*)"xxx: %d\n", RXV_INT64(result));
            RXA_OBJECT(frm, 1) = obj;
			RXA_TYPE(frm, 1) = RXT_OBJECT;
			return RXR_VALUE;
		}
/* generated */
		case CMD_FMOD_Memory_Initialize: {
/* TODO: (void *poolmem, int poollen, FMOD_MEMORY_ALLOCCALLBACK useralloc, FMOD_MEMORY_REALLOCCALLBACK userrealloc, FMOD_MEMORY_FREECALLBACK userfree, FMOD_MEMORY_TYPE memtypeflags) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Memory_GetStats: {
/* SPEC: (int *currentalloced, int *maxalloced, FMOD_BOOL blocking) */
			int currentalloced;
			int maxalloced;
			ERRCHECK(FMOD_Memory_GetStats(&currentalloced, &maxalloced, (FMOD_BOOL)RXA_LOGIC(frm, 1)));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = currentalloced;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = maxalloced;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Debug_SetLevel: {
/* TODO: (FMOD_DEBUGLEVEL level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Debug_GetLevel: {
/* TODO: (FMOD_DEBUGLEVEL *level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_File_SetDiskBusy: {
/* SPEC: (int busy) */
			ERRCHECK(FMOD_File_SetDiskBusy((int)RXA_INT32(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_File_GetDiskBusy: {
/* SPEC: (int *busy) */
			int busy;
			ERRCHECK(FMOD_File_GetDiskBusy(&busy));
			RXA_INT64(frm, 1) = busy;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetOutput: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_OUTPUTTYPE output) */
			FMOD_OUTPUTTYPE output = RXA_INT32(frm, 1);
			ERRCHECK(FMOD_System_SetOutput(sys, output));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetOutput: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_OUTPUTTYPE *output) */
			FMOD_OUTPUTTYPE output;
			ERRCHECK(FMOD_System_GetOutput(sys, &output));
			RXA_INT64(frm, 1) = output;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetNumDrivers: {
/* SPEC: (FMOD_SYSTEM *system, int *numdrivers) */
			int numdrivers;
			ERRCHECK(FMOD_System_GetNumDrivers(sys, &numdrivers));
			RXA_INT64(frm, 1) = numdrivers;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetDriverInfo: {
/* SPEC: (FMOD_SYSTEM *system, int id, char *name, int namelen, FMOD_GUID *guid) */
			ERRCHECK(FMOD_System_GetDriverInfo(sys, (int)RXA_INT32(frm, 1), tmpBuffer1, 256, (FMOD_GUID *)NULL));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetDriverInfoW: {
/* SPEC: (FMOD_SYSTEM *system, int id, short *name, int namelen, FMOD_GUID *guid) */
			ERRCHECK(FMOD_System_GetDriverInfoW(sys, (int)RXA_INT32(frm, 1), (short *)tmpBuffer1, 256, (FMOD_GUID *)NULL));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetDriverCaps: {
/* SPEC: (FMOD_SYSTEM *system, int id, FMOD_CAPS *caps, int *minfrequency, int *maxfrequency, FMOD_SPEAKERMODE *controlpanelspeakermode) */
			FMOD_CAPS caps;
			int minfrequency;
			int maxfrequency;
			FMOD_SPEAKERMODE controlpanelspeakermode;
			ERRCHECK(FMOD_System_GetDriverCaps(sys, (int)RXA_INT32(frm, 1), &caps, &minfrequency, &maxfrequency, &controlpanelspeakermode));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(4);
			RXV_INT64(val) = caps;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = minfrequency;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = maxfrequency;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXV_INT64(val) = controlpanelspeakermode;
			RL_SET_VALUE(block, 3, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetDriver: {
/* SPEC: (FMOD_SYSTEM *system, int driver) */
			ERRCHECK(FMOD_System_SetDriver(sys, (int)RXA_INT32(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetDriver: {
/* SPEC: (FMOD_SYSTEM *system, int *driver) */
			int driver;
			ERRCHECK(FMOD_System_GetDriver(sys, &driver));
			RXA_INT64(frm, 1) = driver;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetHardwareChannels: {
/* SPEC: (FMOD_SYSTEM *system, int min2d, int max2d, int min3d, int max3d) */
			ERRCHECK(FMOD_System_SetHardwareChannels(sys, (int)RXA_INT32(frm, 1), (int)RXA_INT32(frm, 2), (int)RXA_INT32(frm, 3), (int)RXA_INT32(frm, 4)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_SetSoftwareChannels: {
/* SPEC: (FMOD_SYSTEM *system, int numsoftwarechannels) */
			ERRCHECK(FMOD_System_SetSoftwareChannels(sys, (int)RXA_INT32(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetSoftwareChannels: {
/* SPEC: (FMOD_SYSTEM *system, int *numsoftwarechannels) */
			int numsoftwarechannels;
			ERRCHECK(FMOD_System_GetSoftwareChannels(sys, &numsoftwarechannels));
			RXA_INT64(frm, 1) = numsoftwarechannels;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetSoftwareFormat: {
/* SPEC: (FMOD_SYSTEM *system, int samplerate, FMOD_SOUND_FORMAT format, int numoutputchannels, int maxinputchannels, FMOD_DSP_RESAMPLER resamplemethod) */
			FMOD_SOUND_FORMAT format = RXA_INT32(frm, 2);
			FMOD_DSP_RESAMPLER resamplemethod = RXA_INT32(frm, 5);
			ERRCHECK(FMOD_System_SetSoftwareFormat(sys, (int)RXA_INT32(frm, 1), format, (int)RXA_INT32(frm, 3), (int)RXA_INT32(frm, 4), resamplemethod));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetSoftwareFormat: {
/* SPEC: (FMOD_SYSTEM *system, int *samplerate, FMOD_SOUND_FORMAT *format, int *numoutputchannels, int *maxinputchannels, FMOD_DSP_RESAMPLER *resamplemethod, int *bits) */
			int samplerate;
			FMOD_SOUND_FORMAT format;
			int numoutputchannels;
			int maxinputchannels;
			FMOD_DSP_RESAMPLER resamplemethod;
			int bits;
			ERRCHECK(FMOD_System_GetSoftwareFormat(sys, &samplerate, &format, &numoutputchannels, &maxinputchannels, &resamplemethod, &bits));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(6);
			RXV_INT64(val) = samplerate;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = format;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = numoutputchannels;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXV_INT64(val) = maxinputchannels;
			RL_SET_VALUE(block, 3, val, RXT_INTEGER);
			RXV_INT64(val) = resamplemethod;
			RL_SET_VALUE(block, 4, val, RXT_INTEGER);
			RXV_INT64(val) = bits;
			RL_SET_VALUE(block, 5, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetDSPBufferSize: {
/* TODO: (FMOD_SYSTEM *system, unsigned int bufferlength, int numbuffers) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetDSPBufferSize: {
/* TODO: (FMOD_SYSTEM *system, unsigned int *bufferlength, int *numbuffers) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetFileSystem: {
/* TODO: (FMOD_SYSTEM *system, FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, FMOD_FILE_ASYNCREADCALLBACK userasyncread, FMOD_FILE_ASYNCCANCELCALLBACK userasynccancel, int blockalign) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_AttachFileSystem: {
/* TODO: (FMOD_SYSTEM *system, FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetAdvancedSettings: {
/* TODO: (FMOD_SYSTEM *system, FMOD_ADVANCEDSETTINGS *settings) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetAdvancedSettings: {
/* TODO: (FMOD_SYSTEM *system, FMOD_ADVANCEDSETTINGS *settings) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetSpeakerMode: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_SPEAKERMODE speakermode) */
			FMOD_SPEAKERMODE speakermode = RXA_INT32(frm, 1);
			ERRCHECK(FMOD_System_SetSpeakerMode(sys, speakermode));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetSpeakerMode: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_SPEAKERMODE *speakermode) */
			FMOD_SPEAKERMODE speakermode;
			ERRCHECK(FMOD_System_GetSpeakerMode(sys, &speakermode));
			RXA_INT64(frm, 1) = speakermode;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetCallback: {
/* TODO: (FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACK callback) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetPluginPath: {
/* TODO: (FMOD_SYSTEM *system, const char *path) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_LoadPlugin: {
/* TODO: (FMOD_SYSTEM *system, const char *filename, unsigned int *handle, unsigned int priority) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_UnloadPlugin: {
/* TODO: (FMOD_SYSTEM *system, unsigned int handle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetNumPlugins: {
/* TODO: (FMOD_SYSTEM *system, FMOD_PLUGINTYPE plugintype, int *numplugins) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetPluginHandle: {
/* TODO: (FMOD_SYSTEM *system, FMOD_PLUGINTYPE plugintype, int index, unsigned int *handle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetPluginInfo: {
/* TODO: (FMOD_SYSTEM *system, unsigned int handle, FMOD_PLUGINTYPE *plugintype, char *name, int namelen, unsigned int *version) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetOutputByPlugin: {
/* TODO: (FMOD_SYSTEM *system, unsigned int handle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetOutputByPlugin: {
/* TODO: (FMOD_SYSTEM *system, unsigned int *handle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_CreateDSPByPlugin: {
/* TODO: (FMOD_SYSTEM *system, unsigned int handle, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_CreateCodec: {
/* TODO: (FMOD_SYSTEM *system, FMOD_CODEC_DESCRIPTION *description, unsigned int priority) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Close: {
/* SPEC: (FMOD_SYSTEM *system) */
			ERRCHECK(FMOD_System_Close(sys));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_Update: {
/* SPEC: (FMOD_SYSTEM *system) */
			ERRCHECK(FMOD_System_Update(sys));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_Set3DSettings: {
/* TODO: (FMOD_SYSTEM *system, float dopplerscale, float distancefactor, float rolloffscale) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Get3DSettings: {
/* TODO: (FMOD_SYSTEM *system, float *dopplerscale, float *distancefactor, float *rolloffscale) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Set3DNumListeners: {
/* TODO: (FMOD_SYSTEM *system, int numlisteners) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Get3DNumListeners: {
/* TODO: (FMOD_SYSTEM *system, int *numlisteners) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Set3DListenerAttributes: {
/* TODO: (FMOD_SYSTEM *system, int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Get3DListenerAttributes: {
/* TODO: (FMOD_SYSTEM *system, int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Set3DRolloffCallback: {
/* TODO: (FMOD_SYSTEM *system, FMOD_3D_ROLLOFFCALLBACK callback) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Set3DSpeakerPosition: {
/* TODO: (FMOD_SYSTEM *system, FMOD_SPEAKER speaker, float x, float y, FMOD_BOOL active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_Get3DSpeakerPosition: {
/* TODO: (FMOD_SYSTEM *system, FMOD_SPEAKER speaker, float *x, float *y, FMOD_BOOL *active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetStreamBufferSize: {
/* SPEC: (FMOD_SYSTEM *system, unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype) */
			FMOD_TIMEUNIT filebuffersizetype = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_System_SetStreamBufferSize(sys, (unsigned int)RXA_INT32(frm, 1), filebuffersizetype));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetStreamBufferSize: {
/* SPEC: (FMOD_SYSTEM *system, unsigned int *filebuffersize, FMOD_TIMEUNIT *filebuffersizetype) */
			unsigned int filebuffersize;
			FMOD_TIMEUNIT filebuffersizetype;
			ERRCHECK(FMOD_System_GetStreamBufferSize(sys, &filebuffersize, &filebuffersizetype));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = filebuffersize;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = filebuffersizetype;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetVersion: {
/* SPEC: (FMOD_SYSTEM *system, unsigned int *version) */
			unsigned int version;
			ERRCHECK(FMOD_System_GetVersion(sys, &version));
			RXA_INT64(frm, 1) = version;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetOutputHandle: {
/* SPEC: (FMOD_SYSTEM *system, void **handle) */
			void *handle;
			ERRCHECK(FMOD_System_GetOutputHandle(sys, &handle));
			RXA_HANDLE(frm, 1) = handle;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetChannelsPlaying: {
/* SPEC: (FMOD_SYSTEM *system, int *channels) */
			int channels;
			ERRCHECK(FMOD_System_GetChannelsPlaying(sys, &channels));
			RXA_INT64(frm, 1) = channels;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetHardwareChannels: {
/* SPEC: (FMOD_SYSTEM *system, int *num2d, int *num3d, int *total) */
			int num2d;
			int num3d;
			int total;
			ERRCHECK(FMOD_System_GetHardwareChannels(sys, &num2d, &num3d, &total));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_INT64(val) = num2d;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = num3d;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = total;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetCPUUsage: {
/* SPEC: (FMOD_SYSTEM *system, float *dsp, float *stream, float *geometry, float *update, float *total) */
			float dsp;
			float stream;
			float geometry;
			float update;
			float total;
			ERRCHECK(FMOD_System_GetCPUUsage(sys, &dsp, &stream, &geometry, &update, &total));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(5);
			RXV_DEC64(val) = dsp;
			RL_SET_VALUE(block, 0, val, RXT_DECIMAL);
			RXV_DEC64(val) = stream;
			RL_SET_VALUE(block, 1, val, RXT_DECIMAL);
			RXV_DEC64(val) = geometry;
			RL_SET_VALUE(block, 2, val, RXT_DECIMAL);
			RXV_DEC64(val) = update;
			RL_SET_VALUE(block, 3, val, RXT_DECIMAL);
			RXV_DEC64(val) = total;
			RL_SET_VALUE(block, 4, val, RXT_DECIMAL);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetSoundRAM: {
/* SPEC: (FMOD_SYSTEM *system, int *currentalloced, int *maxalloced, int *total) */
			int currentalloced;
			int maxalloced;
			int total;
			ERRCHECK(FMOD_System_GetSoundRAM(sys, &currentalloced, &maxalloced, &total));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_INT64(val) = currentalloced;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = maxalloced;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = total;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetNumCDROMDrives: {
/* SPEC: (FMOD_SYSTEM *system, int *numdrives) */
			int numdrives;
			ERRCHECK(FMOD_System_GetNumCDROMDrives(sys, &numdrives));
			RXA_INT64(frm, 1) = numdrives;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetCDROMDriveName: {
/* SPEC: (FMOD_SYSTEM *system, int drive, char *drivename, int drivenamelen, char *scsiname, int scsinamelen, char *devicename, int devicenamelen) */
			ERRCHECK(FMOD_System_GetCDROMDriveName(sys, (int)RXA_INT32(frm, 1), tmpBuffer1, 256, tmpBuffer2, 256, tmpBuffer3, 256));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_SERIES(val) = StringToRebser(tmpBuffer1);
			RL_SET_VALUE(block, 0, val, RXT_STRING);
			RXV_SERIES(val) = StringToRebser(tmpBuffer2);
			RL_SET_VALUE(block, 1, val, RXT_STRING);
			RXV_SERIES(val) = StringToRebser(tmpBuffer3);
			RL_SET_VALUE(block, 2, val, RXT_STRING);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetSpectrum: {
/* SPEC: (FMOD_SYSTEM *system, float *spectrumarray, int numvalues, int channeloffset, FMOD_DSP_FFT_WINDOW windowtype) */
			int numvalues = (int)RXA_INT32(frm, 1);
			int n;
			float* spectrumarray;
			spectrumarray=(float*)malloc(numvalues*sizeof(float));
			FMOD_DSP_FFT_WINDOW windowtype = RXA_INT32(frm, 3);
			ERRCHECK(FMOD_System_GetSpectrum(sys, spectrumarray, numvalues, (int)RXA_INT32(frm, 2), windowtype));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = spectrumarray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(spectrumarray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetWaveData: {
/* SPEC: (FMOD_SYSTEM *system, float *wavearray, int numvalues, int channeloffset) */
			int numvalues = (int)RXA_INT32(frm, 1);
			int n;
			float* wavearray;
			wavearray=(float*)malloc(numvalues*sizeof(float));
			ERRCHECK(FMOD_System_GetWaveData(sys, wavearray, numvalues, (int)RXA_INT32(frm, 2)));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = wavearray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(wavearray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateSound: {
/* SPEC: (FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound) */
			const char *name_or_data= RebserToMultiByte(RXA_SERIES(frm, 1));
			FMOD_MODE mode = RXA_INT32(frm, 2);
			FMOD_SOUND *sound = 0;
			ERRCHECK(FMOD_System_CreateSound(sys, name_or_data, mode, (FMOD_CREATESOUNDEXINFO *)NULL, &sound));
			RXA_HANDLE(frm, 1) = sound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateStream: {
/* SPEC: (FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound) */
			const char *name_or_data= RebserToMultiByte(RXA_SERIES(frm, 1));
			FMOD_MODE mode = RXA_INT32(frm, 2);
			FMOD_SOUND *sound = 0;
			ERRCHECK(FMOD_System_CreateStream(sys, name_or_data, mode, (FMOD_CREATESOUNDEXINFO *)NULL, &sound));
			RXA_HANDLE(frm, 1) = sound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateDSP: {
/* TODO: (FMOD_SYSTEM *system, FMOD_DSP_DESCRIPTION *description, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_CreateDSPByType: {
/* TODO: (FMOD_SYSTEM *system, FMOD_DSP_TYPE type, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_CreateChannelGroup: {
/* SPEC: (FMOD_SYSTEM *system, const char *name, FMOD_CHANNELGROUP **channelgroup) */
			const char *name= RebserToMultiByte(RXA_SERIES(frm, 1));
			FMOD_CHANNELGROUP *channelgroup=0;
			ERRCHECK(FMOD_System_CreateChannelGroup(sys, name, &channelgroup));
			RXA_HANDLE(frm, 1) = channelgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateSoundGroup: {
/* SPEC: (FMOD_SYSTEM *system, const char *name, FMOD_SOUNDGROUP **soundgroup) */
			const char *name= RebserToMultiByte(RXA_SERIES(frm, 1));
			FMOD_SOUNDGROUP *soundgroup=0;
			ERRCHECK(FMOD_System_CreateSoundGroup(sys, name, &soundgroup));
			RXA_HANDLE(frm, 1) = soundgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateReverb: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_REVERB **reverb) */
			FMOD_REVERB *reverb=0;
			ERRCHECK(FMOD_System_CreateReverb(sys, &reverb));
			RXA_HANDLE(frm, 1) = reverb;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_PlaySound: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_CHANNELINDEX channelid, FMOD_SOUND *sound, FMOD_BOOL paused, FMOD_CHANNEL **channel) */
			FMOD_CHANNELINDEX channelid = RXA_INT32(frm, 1);
			FMOD_CHANNEL *channel=0;
			ERRCHECK(FMOD_System_PlaySound(sys, channelid, (FMOD_SOUND*)RXA_HANDLE(frm, 2), (FMOD_BOOL)RXA_LOGIC(frm, 3), &channel));
			RXA_HANDLE(frm, 1) = channel;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_PlayDSP: {
/* TODO: (FMOD_SYSTEM *system, FMOD_CHANNELINDEX channelid, FMOD_DSP *dsp, FMOD_BOOL paused, FMOD_CHANNEL **channel) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetChannel: {
/* SPEC: (FMOD_SYSTEM *system, int channelid, FMOD_CHANNEL **channel) */
			FMOD_CHANNEL *channel=0;
			ERRCHECK(FMOD_System_GetChannel(sys, (int)RXA_INT32(frm, 1), &channel));
			RXA_HANDLE(frm, 1) = channel;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetMasterChannelGroup: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_CHANNELGROUP **channelgroup) */
			FMOD_CHANNELGROUP *channelgroup=0;
			ERRCHECK(FMOD_System_GetMasterChannelGroup(sys, &channelgroup));
			RXA_HANDLE(frm, 1) = channelgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetMasterSoundGroup: {
/* SPEC: (FMOD_SYSTEM *system, FMOD_SOUNDGROUP **soundgroup) */
			FMOD_SOUNDGROUP *soundgroup=0;
			ERRCHECK(FMOD_System_GetMasterSoundGroup(sys, &soundgroup));
			RXA_HANDLE(frm, 1) = soundgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetReverbProperties: {
/* TODO: (FMOD_SYSTEM *system, const FMOD_REVERB_PROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetReverbProperties: {
/* TODO: (FMOD_SYSTEM *system, FMOD_REVERB_PROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetReverbAmbientProperties: {
/* TODO: (FMOD_SYSTEM *system, FMOD_REVERB_PROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetReverbAmbientProperties: {
/* TODO: (FMOD_SYSTEM *system, FMOD_REVERB_PROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetDSPHead: {
/* TODO: (FMOD_SYSTEM *system, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_AddDSP: {
/* TODO: (FMOD_SYSTEM *system, FMOD_DSP *dsp, FMOD_DSPCONNECTION **connection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_LockDSP: {
/* TODO: (FMOD_SYSTEM *system) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_UnlockDSP: {
/* TODO: (FMOD_SYSTEM *system) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetDSPClock: {
/* TODO: (FMOD_SYSTEM *system, unsigned int *hi, unsigned int *lo) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetRecordNumDrivers: {
/* SPEC: (FMOD_SYSTEM *system, int *numdrivers) */
			int numdrivers;
			ERRCHECK(FMOD_System_GetRecordNumDrivers(sys, &numdrivers));
			RXA_INT64(frm, 1) = numdrivers;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetRecordDriverInfo: {
/* SPEC: (FMOD_SYSTEM *system, int id, char *name, int namelen, FMOD_GUID *guid) */
			ERRCHECK(FMOD_System_GetRecordDriverInfo(sys, (int)RXA_INT32(frm, 1), tmpBuffer1, 256, (FMOD_GUID *)NULL));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetRecordDriverInfoW: {
/* SPEC: (FMOD_SYSTEM *system, int id, short *name, int namelen, FMOD_GUID *guid) */
			ERRCHECK(FMOD_System_GetRecordDriverInfoW(sys, (int)RXA_INT32(frm, 1), (short *)tmpBuffer1, 256, (FMOD_GUID *)NULL));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetRecordDriverCaps: {
/* SPEC: (FMOD_SYSTEM *system, int id, FMOD_CAPS *caps, int *minfrequency, int *maxfrequency) */
			FMOD_CAPS caps;
			int minfrequency;
			int maxfrequency;
			ERRCHECK(FMOD_System_GetRecordDriverCaps(sys, (int)RXA_INT32(frm, 1), &caps, &minfrequency, &maxfrequency));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_INT64(val) = caps;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = minfrequency;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = maxfrequency;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_GetRecordPosition: {
/* SPEC: (FMOD_SYSTEM *system, int id, unsigned int *position) */
			unsigned int position;
			ERRCHECK(FMOD_System_GetRecordPosition(sys, (int)RXA_INT32(frm, 1), &position));
			RXA_INT64(frm, 1) = position;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_RecordStart: {
/* SPEC: (FMOD_SYSTEM *system, int id, FMOD_SOUND *sound, FMOD_BOOL loop) */
			ERRCHECK(FMOD_System_RecordStart(sys, (int)RXA_INT32(frm, 1), (FMOD_SOUND*)RXA_HANDLE(frm, 2), (FMOD_BOOL)RXA_LOGIC(frm, 3)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_RecordStop: {
/* SPEC: (FMOD_SYSTEM *system, int id) */
			ERRCHECK(FMOD_System_RecordStop(sys, (int)RXA_INT32(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_IsRecording: {
/* SPEC: (FMOD_SYSTEM *system, int id, FMOD_BOOL *recording) */
			FMOD_BOOL recording;
			ERRCHECK(FMOD_System_IsRecording(sys, (int)RXA_INT32(frm, 1), &recording));
			RXA_LOGIC(frm, 1) = recording;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_CreateGeometry: {
/* TODO: (FMOD_SYSTEM *system, int maxpolygons, int maxvertices, FMOD_GEOMETRY **geometry) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetGeometrySettings: {
/* TODO: (FMOD_SYSTEM *system, float maxworldsize) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetGeometrySettings: {
/* TODO: (FMOD_SYSTEM *system, float *maxworldsize) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_LoadGeometry: {
/* TODO: (FMOD_SYSTEM *system, const void *data, int datasize, FMOD_GEOMETRY **geometry) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetGeometryOcclusion: {
/* TODO: (FMOD_SYSTEM *system, const FMOD_VECTOR *listener, const FMOD_VECTOR *source, float *direct, float *reverb) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_SetNetworkProxy: {
/* SPEC: (FMOD_SYSTEM *system, const char *proxy) */
			const char *proxy= RebserToMultiByte(RXA_SERIES(frm, 1));
			ERRCHECK(FMOD_System_SetNetworkProxy(sys, proxy));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetNetworkProxy: {
/* SPEC: (FMOD_SYSTEM *system, char *proxy, int proxylen) */
			ERRCHECK(FMOD_System_GetNetworkProxy(sys, tmpBuffer1, 256));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetNetworkTimeout: {
/* SPEC: (FMOD_SYSTEM *system, int timeout) */
			ERRCHECK(FMOD_System_SetNetworkTimeout(sys, (int)RXA_INT32(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_System_GetNetworkTimeout: {
/* SPEC: (FMOD_SYSTEM *system, int *timeout) */
			int timeout;
			ERRCHECK(FMOD_System_GetNetworkTimeout(sys, &timeout));
			RXA_INT64(frm, 1) = timeout;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_System_SetUserData: {
/* TODO: (FMOD_SYSTEM *system, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetUserData: {
/* TODO: (FMOD_SYSTEM *system, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_System_GetMemoryInfo: {
/* SPEC: (FMOD_SYSTEM *system, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_System_GetMemoryInfo(sys, (unsigned int)RXA_INT32(frm, 1), (unsigned int)RXA_INT32(frm, 2), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_Release: {
/* SPEC: (FMOD_SOUND *sound) */
			ERRCHECK(FMOD_Sound_Release((FMOD_SOUND*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetSystemObject: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SYSTEM **system) */
			FMOD_SYSTEM *system=0;
			ERRCHECK(FMOD_Sound_GetSystemObject((FMOD_SOUND*)RXA_HANDLE(frm, 1), &system));
			RXA_HANDLE(frm, 1) = system;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_Lock: {
/* SPEC: (FMOD_SOUND *sound, unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2) */
			void *ptr1;
			void *ptr2;
			unsigned int len1;
			unsigned int len2;
			ERRCHECK(FMOD_Sound_Lock((FMOD_SOUND*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &ptr1, &ptr2, &len1, &len2));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(4);
			RXV_HANDLE(val) = ptr1;
			RL_SET_VALUE(block, 0, val, RXT_HANDLE);
			RXV_HANDLE(val) = ptr2;
			RL_SET_VALUE(block, 1, val, RXT_HANDLE);
			RXV_INT64(val) = len1;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXV_INT64(val) = len2;
			RL_SET_VALUE(block, 3, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_Unlock: {
/* SPEC: (FMOD_SOUND *sound, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2) */
			ERRCHECK(FMOD_Sound_Unlock((FMOD_SOUND*)RXA_HANDLE(frm, 1), (void *)RXA_HANDLE(frm, 2), (void *)RXA_HANDLE(frm, 3), (unsigned int)RXA_INT32(frm, 4), (unsigned int)RXA_INT32(frm, 5)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_SetDefaults: {
/* SPEC: (FMOD_SOUND *sound, float frequency, float volume, float pan, int priority) */
			ERRCHECK(FMOD_Sound_SetDefaults((FMOD_SOUND*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2), (float)RXA_DEC64(frm, 3), (float)RXA_DEC64(frm, 4), (int)RXA_INT32(frm, 5)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetDefaults: {
/* SPEC: (FMOD_SOUND *sound, float *frequency, float *volume, float *pan, int *priority) */
			float frequency;
			float volume;
			float pan;
			int priority;
			ERRCHECK(FMOD_Sound_GetDefaults((FMOD_SOUND*)RXA_HANDLE(frm, 1), &frequency, &volume, &pan, &priority));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(4);
			RXV_DEC64(val) = frequency;
			RL_SET_VALUE(block, 0, val, RXT_DECIMAL);
			RXV_DEC64(val) = volume;
			RL_SET_VALUE(block, 1, val, RXT_DECIMAL);
			RXV_DEC64(val) = pan;
			RL_SET_VALUE(block, 2, val, RXT_DECIMAL);
			RXV_INT64(val) = priority;
			RL_SET_VALUE(block, 3, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetVariations: {
/* SPEC: (FMOD_SOUND *sound, float frequencyvar, float volumevar, float panvar) */
			ERRCHECK(FMOD_Sound_SetVariations((FMOD_SOUND*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2), (float)RXA_DEC64(frm, 3), (float)RXA_DEC64(frm, 4)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetVariations: {
/* SPEC: (FMOD_SOUND *sound, float *frequencyvar, float *volumevar, float *panvar) */
			float frequencyvar;
			float volumevar;
			float panvar;
			ERRCHECK(FMOD_Sound_GetVariations((FMOD_SOUND*)RXA_HANDLE(frm, 1), &frequencyvar, &volumevar, &panvar));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_DEC64(val) = frequencyvar;
			RL_SET_VALUE(block, 0, val, RXT_DECIMAL);
			RXV_DEC64(val) = volumevar;
			RL_SET_VALUE(block, 1, val, RXT_DECIMAL);
			RXV_DEC64(val) = panvar;
			RL_SET_VALUE(block, 2, val, RXT_DECIMAL);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_Set3DMinMaxDistance: {
/* TODO: (FMOD_SOUND *sound, float min, float max) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_Get3DMinMaxDistance: {
/* TODO: (FMOD_SOUND *sound, float *min, float *max) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_Set3DConeSettings: {
/* TODO: (FMOD_SOUND *sound, float insideconeangle, float outsideconeangle, float outsidevolume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_Get3DConeSettings: {
/* TODO: (FMOD_SOUND *sound, float *insideconeangle, float *outsideconeangle, float *outsidevolume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_Set3DCustomRolloff: {
/* TODO: (FMOD_SOUND *sound, FMOD_VECTOR *points, int numpoints) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_Get3DCustomRolloff: {
/* TODO: (FMOD_SOUND *sound, FMOD_VECTOR **points, int *numpoints) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_SetSubSound: {
/* SPEC: (FMOD_SOUND *sound, int index, FMOD_SOUND *subsound) */
			ERRCHECK(FMOD_Sound_SetSubSound((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), (FMOD_SOUND*)RXA_HANDLE(frm, 3)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetSubSound: {
/* SPEC: (FMOD_SOUND *sound, int index, FMOD_SOUND **subsound) */
			FMOD_SOUND *subsound = 0;
			ERRCHECK(FMOD_Sound_GetSubSound((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &subsound));
			RXA_HANDLE(frm, 1) = subsound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetSubSoundSentence: {
/* SPEC: (FMOD_SOUND *sound, int *subsoundlist, int numsubsounds) */
			int subsoundlist;
			ERRCHECK(FMOD_Sound_SetSubSoundSentence((FMOD_SOUND*)RXA_HANDLE(frm, 1), &subsoundlist, (int)RXA_INT32(frm, 2)));
			RXA_INT64(frm, 1) = subsoundlist;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetName: {
/* SPEC: (FMOD_SOUND *sound, char *name, int namelen) */
			ERRCHECK(FMOD_Sound_GetName((FMOD_SOUND*)RXA_HANDLE(frm, 1), tmpBuffer1, 256));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetLength: {
/* SPEC: (FMOD_SOUND *sound, unsigned int *length, FMOD_TIMEUNIT lengthtype) */
			unsigned int length;
			FMOD_TIMEUNIT lengthtype = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_Sound_GetLength((FMOD_SOUND*)RXA_HANDLE(frm, 1), &length, lengthtype));
			RXA_INT64(frm, 1) = length;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetFormat: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SOUND_TYPE *type, FMOD_SOUND_FORMAT *format, int *channels, int *bits) */
			FMOD_SOUND_TYPE type;
			FMOD_SOUND_FORMAT format;
			int channels;
			int bits;
			ERRCHECK(FMOD_Sound_GetFormat((FMOD_SOUND*)RXA_HANDLE(frm, 1), &type, &format, &channels, &bits));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(4);
			RXV_INT64(val) = type;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = format;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_INT64(val) = channels;
			RL_SET_VALUE(block, 2, val, RXT_INTEGER);
			RXV_INT64(val) = bits;
			RL_SET_VALUE(block, 3, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetNumSubSounds: {
/* SPEC: (FMOD_SOUND *sound, int *numsubsounds) */
			int numsubsounds;
			ERRCHECK(FMOD_Sound_GetNumSubSounds((FMOD_SOUND*)RXA_HANDLE(frm, 1), &numsubsounds));
			RXA_INT64(frm, 1) = numsubsounds;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetNumTags: {
/* SPEC: (FMOD_SOUND *sound, int *numtags, int *numtagsupdated) */
			int numtags;
			int numtagsupdated;
			ERRCHECK(FMOD_Sound_GetNumTags((FMOD_SOUND*)RXA_HANDLE(frm, 1), &numtags, &numtagsupdated));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = numtags;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = numtagsupdated;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetTag: {
/* SPEC: (FMOD_SOUND *sound, const char *name, int index, FMOD_TAG *tag) */
			const char *name= RebserToMultiByte(RXA_SERIES(frm, 2));
			FMOD_TAG tag;
			ERRCHECK(FMOD_Sound_GetTag((FMOD_SOUND*)RXA_HANDLE(frm, 1), name, (int)RXA_INT32(frm, 3), &tag));
			RXIARG val;
			REBSER* blk_tag = RL_MAKE_BLOCK(6);
			RXV_INT64(val) = tag.type;
			RL_SET_VALUE(blk_tag, 0, val, RXT_INTEGER);
			RXV_INT64(val) = tag.datatype;
			RL_SET_VALUE(blk_tag, 1, val, RXT_INTEGER);
			RXV_SERIES(val) = StringToRebser(tag.name);
			RL_SET_VALUE(blk_tag, 2, val, RXT_STRING);
			RXV_SERIES(val) = StringToRebser(tag.data);
			RL_SET_VALUE(blk_tag, 3, val, RXT_STRING);
			RXV_INT64(val) = tag.datalen;
			RL_SET_VALUE(blk_tag, 4, val, RXT_INTEGER);
			RXV_LOGIC(val) = tag.updated;
			RL_SET_VALUE(blk_tag, 5, val, RXT_LOGIC);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_tag;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetOpenState: {
/* SPEC: (FMOD_SOUND *sound, FMOD_OPENSTATE *openstate, unsigned int *percentbuffered, FMOD_BOOL *starving) */
			FMOD_OPENSTATE openstate;
			unsigned int percentbuffered;
			FMOD_BOOL starving;
			ERRCHECK(FMOD_Sound_GetOpenState((FMOD_SOUND*)RXA_HANDLE(frm, 1), &openstate, &percentbuffered, &starving));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(3);
			RXV_INT64(val) = openstate;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = percentbuffered;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXV_LOGIC(val) = starving;
			RL_SET_VALUE(block, 2, val, RXT_LOGIC);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_ReadData: {
/* SPEC: (FMOD_SOUND *sound, void *buffer, unsigned int lenbytes, unsigned int *read) */
			unsigned int read;
			ERRCHECK(FMOD_Sound_ReadData((FMOD_SOUND*)RXA_HANDLE(frm, 1), (void *)RXA_HANDLE(frm, 2), (unsigned int)RXA_INT32(frm, 3), &read));
			RXA_INT64(frm, 1) = read;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SeekData: {
/* SPEC: (FMOD_SOUND *sound, unsigned int pcm) */
			ERRCHECK(FMOD_Sound_SeekData((FMOD_SOUND*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_SetSoundGroup: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SOUNDGROUP *soundgroup) */
			ERRCHECK(FMOD_Sound_SetSoundGroup((FMOD_SOUND*)RXA_HANDLE(frm, 1), (FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetSoundGroup: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SOUNDGROUP **soundgroup) */
			FMOD_SOUNDGROUP *soundgroup=0;
			ERRCHECK(FMOD_Sound_GetSoundGroup((FMOD_SOUND*)RXA_HANDLE(frm, 1), &soundgroup));
			RXA_HANDLE(frm, 1) = soundgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetNumSyncPoints: {
/* SPEC: (FMOD_SOUND *sound, int *numsyncpoints) */
			int numsyncpoints;
			ERRCHECK(FMOD_Sound_GetNumSyncPoints((FMOD_SOUND*)RXA_HANDLE(frm, 1), &numsyncpoints));
			RXA_INT64(frm, 1) = numsyncpoints;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetSyncPoint: {
/* SPEC: (FMOD_SOUND *sound, int index, FMOD_SYNCPOINT **point) */
			FMOD_SYNCPOINT *point=0;
			ERRCHECK(FMOD_Sound_GetSyncPoint((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &point));
			RXA_HANDLE(frm, 1) = point;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetSyncPointInfo: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SYNCPOINT *point, char *name, int namelen, unsigned int *offset, FMOD_TIMEUNIT offsettype) */
			unsigned int offset;
			FMOD_TIMEUNIT offsettype = RXA_INT32(frm, 3);
			ERRCHECK(FMOD_Sound_GetSyncPointInfo((FMOD_SOUND*)RXA_HANDLE(frm, 1), (FMOD_SYNCPOINT*)RXA_HANDLE(frm, 2), tmpBuffer1, 256, &offset, offsettype));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_SERIES(val) = StringToRebser(tmpBuffer1);
			RL_SET_VALUE(block, 0, val, RXT_STRING);
			RXV_INT64(val) = offset;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_AddSyncPoint: {
/* SPEC: (FMOD_SOUND *sound, unsigned int offset, FMOD_TIMEUNIT offsettype, const char *name, FMOD_SYNCPOINT **point) */
			FMOD_TIMEUNIT offsettype = RXA_INT32(frm, 3);
			const char *name= RebserToMultiByte(RXA_SERIES(frm, 4));
			FMOD_SYNCPOINT *point=0;
			ERRCHECK(FMOD_Sound_AddSyncPoint((FMOD_SOUND*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), offsettype, name, &point));
			RXA_HANDLE(frm, 1) = point;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_DeleteSyncPoint: {
/* SPEC: (FMOD_SOUND *sound, FMOD_SYNCPOINT *point) */
			ERRCHECK(FMOD_Sound_DeleteSyncPoint((FMOD_SOUND*)RXA_HANDLE(frm, 1), (FMOD_SYNCPOINT*)RXA_HANDLE(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_SetMode: {
/* SPEC: (FMOD_SOUND *sound, FMOD_MODE mode) */
			FMOD_MODE mode = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_Sound_SetMode((FMOD_SOUND*)RXA_HANDLE(frm, 1), mode));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetMode: {
/* SPEC: (FMOD_SOUND *sound, FMOD_MODE *mode) */
			FMOD_MODE mode;
			ERRCHECK(FMOD_Sound_GetMode((FMOD_SOUND*)RXA_HANDLE(frm, 1), &mode));
			RXA_INT64(frm, 1) = mode;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetLoopCount: {
/* SPEC: (FMOD_SOUND *sound, int loopcount) */
			ERRCHECK(FMOD_Sound_SetLoopCount((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetLoopCount: {
/* SPEC: (FMOD_SOUND *sound, int *loopcount) */
			int loopcount;
			ERRCHECK(FMOD_Sound_GetLoopCount((FMOD_SOUND*)RXA_HANDLE(frm, 1), &loopcount));
			RXA_INT64(frm, 1) = loopcount;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetLoopPoints: {
/* SPEC: (FMOD_SOUND *sound, unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype) */
			FMOD_TIMEUNIT loopstarttype = RXA_INT32(frm, 3);
			FMOD_TIMEUNIT loopendtype = RXA_INT32(frm, 5);
			ERRCHECK(FMOD_Sound_SetLoopPoints((FMOD_SOUND*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), loopstarttype, (unsigned int)RXA_INT32(frm, 4), loopendtype));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetLoopPoints: {
/* SPEC: (FMOD_SOUND *sound, unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype) */
			unsigned int loopstart;
			FMOD_TIMEUNIT loopstarttype = RXA_INT32(frm, 2);
			unsigned int loopend;
			FMOD_TIMEUNIT loopendtype = RXA_INT32(frm, 3);
			ERRCHECK(FMOD_Sound_GetLoopPoints((FMOD_SOUND*)RXA_HANDLE(frm, 1), &loopstart, loopstarttype, &loopend, loopendtype));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = loopstart;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = loopend;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_GetMusicNumChannels: {
/* SPEC: (FMOD_SOUND *sound, int *numchannels) */
			int numchannels;
			ERRCHECK(FMOD_Sound_GetMusicNumChannels((FMOD_SOUND*)RXA_HANDLE(frm, 1), &numchannels));
			RXA_INT64(frm, 1) = numchannels;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetMusicChannelVolume: {
/* SPEC: (FMOD_SOUND *sound, int channel, float volume) */
			ERRCHECK(FMOD_Sound_SetMusicChannelVolume((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), (float)RXA_DEC64(frm, 3)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetMusicChannelVolume: {
/* SPEC: (FMOD_SOUND *sound, int channel, float *volume) */
			float volume;
			ERRCHECK(FMOD_Sound_GetMusicChannelVolume((FMOD_SOUND*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &volume));
			RXA_DEC64(frm, 1) = volume;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetMusicSpeed: {
/* SPEC: (FMOD_SOUND *sound, float speed) */
			ERRCHECK(FMOD_Sound_SetMusicSpeed((FMOD_SOUND*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Sound_GetMusicSpeed: {
/* SPEC: (FMOD_SOUND *sound, float *speed) */
			float speed;
			ERRCHECK(FMOD_Sound_GetMusicSpeed((FMOD_SOUND*)RXA_HANDLE(frm, 1), &speed));
			RXA_DEC64(frm, 1) = speed;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Sound_SetUserData: {
/* TODO: (FMOD_SOUND *sound, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_GetUserData: {
/* TODO: (FMOD_SOUND *sound, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Sound_GetMemoryInfo: {
/* SPEC: (FMOD_SOUND *sound, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_Sound_GetMemoryInfo((FMOD_SOUND*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetSystemObject: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_SYSTEM **system) */
			FMOD_SYSTEM *system=0;
			ERRCHECK(FMOD_Channel_GetSystemObject((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &system));
			RXA_HANDLE(frm, 1) = system;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_Stop: {
/* SPEC: (FMOD_CHANNEL *channel) */
			ERRCHECK(FMOD_Channel_Stop((FMOD_CHANNEL*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_SetPaused: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL paused) */
			ERRCHECK(FMOD_Channel_SetPaused((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (FMOD_BOOL)RXA_LOGIC(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetPaused: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL *paused) */
			FMOD_BOOL paused;
			ERRCHECK(FMOD_Channel_GetPaused((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &paused));
			RXA_LOGIC(frm, 1) = paused;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetVolume: {
/* SPEC: (FMOD_CHANNEL *channel, float volume) */
			ERRCHECK(FMOD_Channel_SetVolume((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetVolume: {
/* SPEC: (FMOD_CHANNEL *channel, float *volume) */
			float volume;
			ERRCHECK(FMOD_Channel_GetVolume((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &volume));
			RXA_DEC64(frm, 1) = volume;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetFrequency: {
/* SPEC: (FMOD_CHANNEL *channel, float frequency) */
			ERRCHECK(FMOD_Channel_SetFrequency((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetFrequency: {
/* SPEC: (FMOD_CHANNEL *channel, float *frequency) */
			float frequency;
			ERRCHECK(FMOD_Channel_GetFrequency((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &frequency));
			RXA_DEC64(frm, 1) = frequency;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetPan: {
/* SPEC: (FMOD_CHANNEL *channel, float pan) */
			ERRCHECK(FMOD_Channel_SetPan((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetPan: {
/* SPEC: (FMOD_CHANNEL *channel, float *pan) */
			float pan;
			ERRCHECK(FMOD_Channel_GetPan((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &pan));
			RXA_DEC64(frm, 1) = pan;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetDelay: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_DELAYTYPE delaytype, unsigned int delayhi, unsigned int delaylo) */
			FMOD_DELAYTYPE delaytype = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_Channel_SetDelay((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), delaytype, (unsigned int)RXA_INT32(frm, 3), (unsigned int)RXA_INT32(frm, 4)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetDelay: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_DELAYTYPE delaytype, unsigned int *delayhi, unsigned int *delaylo) */
			FMOD_DELAYTYPE delaytype = RXA_INT32(frm, 2);
			unsigned int delayhi;
			unsigned int delaylo;
			ERRCHECK(FMOD_Channel_GetDelay((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), delaytype, &delayhi, &delaylo));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = delayhi;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = delaylo;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}

		case CMD_FMOD_Channel_SetSpeakerMix: {
/* SPEC: (FMOD_CHANNEL *channel, float frontleft, float frontright, float center, float lfe, float backleft, float backright, float sideleft, float sideright) */
			RXIARG val;
			RL_GET_VALUE(RXA_SERIES(frm, 2),0,&val);
			float frontleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),1,&val);
			float frontright = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),2,&val);
			float center = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),3,&val);
			float lfe = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),4,&val);
			float backleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),5,&val);
			float backright = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),6,&val);
			float sideleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),7,&val);
			float sideright = RXV_DEC64(val);
			ERRCHECK(FMOD_Channel_SetSpeakerMix((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), frontleft, frontright, center, lfe, backleft, backright, sideleft, sideright));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetSpeakerMix: {
/* SPEC: (FMOD_CHANNEL *channel, float *frontleft, float *frontright, float *center, float *lfe, float *backleft, float *backright, float *sideleft, float *sideright) */
			float frontleft;
			float frontright;
			float center;
			float lfe;
			float backleft;
			float backright;
			float sideleft;
			float sideright;
			ERRCHECK(FMOD_Channel_GetSpeakerMix((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &frontleft, &frontright, &center, &lfe, &backleft, &backright, &sideleft, &sideright));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(8);
			RXV_DEC64(val) = frontleft;
			RL_SET_VALUE(block, 0, val, RXT_DECIMAL);
			RXV_DEC64(val) = frontright;
			RL_SET_VALUE(block, 1, val, RXT_DECIMAL);
			RXV_DEC64(val) = center;
			RL_SET_VALUE(block, 2, val, RXT_DECIMAL);
			RXV_DEC64(val) = lfe;
			RL_SET_VALUE(block, 3, val, RXT_DECIMAL);
			RXV_DEC64(val) = backleft;
			RL_SET_VALUE(block, 4, val, RXT_DECIMAL);
			RXV_DEC64(val) = backright;
			RL_SET_VALUE(block, 5, val, RXT_DECIMAL);
			RXV_DEC64(val) = sideleft;
			RL_SET_VALUE(block, 6, val, RXT_DECIMAL);
			RXV_DEC64(val) = sideright;
			RL_SET_VALUE(block, 7, val, RXT_DECIMAL);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetSpeakerLevels: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_SPEAKER speaker, float *levels, int numlevels) */
			FMOD_SPEAKER speaker = RXA_INT32(frm, 2);
			float levels;
			ERRCHECK(FMOD_Channel_SetSpeakerLevels((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), speaker, &levels, (int)RXA_INT32(frm, 3)));
			RXA_DEC64(frm, 1) = levels;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetSpeakerLevels: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_SPEAKER speaker, float *levels, int numlevels) */
			FMOD_SPEAKER speaker = RXA_INT32(frm, 2);
			float levels;
			ERRCHECK(FMOD_Channel_GetSpeakerLevels((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), speaker, &levels, (int)RXA_INT32(frm, 3)));
			RXA_DEC64(frm, 1) = levels;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetInputChannelMix: {
/* SPEC: (FMOD_CHANNEL *channel, float *levels, int numlevels) */
			float levels;
			ERRCHECK(FMOD_Channel_SetInputChannelMix((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &levels, (int)RXA_INT32(frm, 2)));
			RXA_DEC64(frm, 1) = levels;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetInputChannelMix: {
/* SPEC: (FMOD_CHANNEL *channel, float *levels, int numlevels) */
			float levels;
			ERRCHECK(FMOD_Channel_GetInputChannelMix((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &levels, (int)RXA_INT32(frm, 2)));
			RXA_DEC64(frm, 1) = levels;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetMute: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL mute) */
			ERRCHECK(FMOD_Channel_SetMute((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (FMOD_BOOL)RXA_LOGIC(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetMute: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL *mute) */
			FMOD_BOOL mute;
			ERRCHECK(FMOD_Channel_GetMute((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &mute));
			RXA_LOGIC(frm, 1) = mute;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetPriority: {
/* SPEC: (FMOD_CHANNEL *channel, int priority) */
			ERRCHECK(FMOD_Channel_SetPriority((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetPriority: {
/* SPEC: (FMOD_CHANNEL *channel, int *priority) */
			int priority;
			ERRCHECK(FMOD_Channel_GetPriority((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &priority));
			RXA_INT64(frm, 1) = priority;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetPosition: {
/* SPEC: (FMOD_CHANNEL *channel, unsigned int position, FMOD_TIMEUNIT postype) */
			FMOD_TIMEUNIT postype = RXA_INT32(frm, 3);
			ERRCHECK(FMOD_Channel_SetPosition((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), postype));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetPosition: {
/* SPEC: (FMOD_CHANNEL *channel, unsigned int *position, FMOD_TIMEUNIT postype) */
			unsigned int position;
			FMOD_TIMEUNIT postype = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_Channel_GetPosition((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &position, postype));
			RXA_INT64(frm, 1) = position;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetReverbProperties: {
/* TODO: (FMOD_CHANNEL *channel, const FMOD_REVERB_CHANNELPROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_GetReverbProperties: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_REVERB_CHANNELPROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_SetLowPassGain: {
/* SPEC: (FMOD_CHANNEL *channel, float gain) */
			ERRCHECK(FMOD_Channel_SetLowPassGain((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetLowPassGain: {
/* SPEC: (FMOD_CHANNEL *channel, float *gain) */
			float gain;
			ERRCHECK(FMOD_Channel_GetLowPassGain((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &gain));
			RXA_DEC64(frm, 1) = gain;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetChannelGroup: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_CHANNELGROUP *channelgroup) */
			ERRCHECK(FMOD_Channel_SetChannelGroup((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetChannelGroup: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_CHANNELGROUP **channelgroup) */
			FMOD_CHANNELGROUP *channelgroup=0;
			ERRCHECK(FMOD_Channel_GetChannelGroup((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &channelgroup));
			RXA_HANDLE(frm, 1) = channelgroup;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetCallback: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACK callback) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DAttributes: {
/* TODO: (FMOD_CHANNEL *channel, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DAttributes: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_VECTOR *pos, FMOD_VECTOR *vel) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DMinMaxDistance: {
/* TODO: (FMOD_CHANNEL *channel, float mindistance, float maxdistance) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DMinMaxDistance: {
/* TODO: (FMOD_CHANNEL *channel, float *mindistance, float *maxdistance) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DConeSettings: {
/* TODO: (FMOD_CHANNEL *channel, float insideconeangle, float outsideconeangle, float outsidevolume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DConeSettings: {
/* TODO: (FMOD_CHANNEL *channel, float *insideconeangle, float *outsideconeangle, float *outsidevolume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DConeOrientation: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_VECTOR *orientation) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DConeOrientation: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_VECTOR *orientation) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DCustomRolloff: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_VECTOR *points, int numpoints) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DCustomRolloff: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_VECTOR **points, int *numpoints) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DOcclusion: {
/* TODO: (FMOD_CHANNEL *channel, float directocclusion, float reverbocclusion) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DOcclusion: {
/* TODO: (FMOD_CHANNEL *channel, float *directocclusion, float *reverbocclusion) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DSpread: {
/* TODO: (FMOD_CHANNEL *channel, float angle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DSpread: {
/* TODO: (FMOD_CHANNEL *channel, float *angle) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DPanLevel: {
/* TODO: (FMOD_CHANNEL *channel, float level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DPanLevel: {
/* TODO: (FMOD_CHANNEL *channel, float *level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Set3DDopplerLevel: {
/* TODO: (FMOD_CHANNEL *channel, float level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_Get3DDopplerLevel: {
/* TODO: (FMOD_CHANNEL *channel, float *level) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_GetDSPHead: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_AddDSP: {
/* TODO: (FMOD_CHANNEL *channel, FMOD_DSP *dsp, FMOD_DSPCONNECTION **connection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_IsPlaying: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL *isplaying) */
			FMOD_BOOL isplaying;
			ERRCHECK(FMOD_Channel_IsPlaying((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &isplaying));
			RXA_LOGIC(frm, 1) = isplaying;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_IsVirtual: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_BOOL *isvirtual) */
			FMOD_BOOL isvirtual;
			ERRCHECK(FMOD_Channel_IsVirtual((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &isvirtual));
			RXA_LOGIC(frm, 1) = isvirtual;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetAudibility: {
/* SPEC: (FMOD_CHANNEL *channel, float *audibility) */
			float audibility;
			ERRCHECK(FMOD_Channel_GetAudibility((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &audibility));
			RXA_DEC64(frm, 1) = audibility;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetCurrentSound: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_SOUND **sound) */
			FMOD_SOUND *sound = 0;
			ERRCHECK(FMOD_Channel_GetCurrentSound((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &sound));
			RXA_HANDLE(frm, 1) = sound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetSpectrum: {
/* SPEC: (FMOD_CHANNEL *channel, float *spectrumarray, int numvalues, int channeloffset, FMOD_DSP_FFT_WINDOW windowtype) */
			int numvalues = (int)RXA_INT32(frm, 2);
			int n;
			float* spectrumarray;
			spectrumarray=(float*)malloc(numvalues*sizeof(float));
			FMOD_DSP_FFT_WINDOW windowtype = RXA_INT32(frm, 4);
			ERRCHECK(FMOD_Channel_GetSpectrum((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), spectrumarray, numvalues, (int)RXA_INT32(frm, 3), windowtype));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = spectrumarray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(spectrumarray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetWaveData: {
/* SPEC: (FMOD_CHANNEL *channel, float *wavearray, int numvalues, int channeloffset) */
			int numvalues = (int)RXA_INT32(frm, 2);
			int n;
			float* wavearray;
			wavearray=(float*)malloc(numvalues*sizeof(float));
			ERRCHECK(FMOD_Channel_GetWaveData((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), wavearray, numvalues, (int)RXA_INT32(frm, 3)));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = wavearray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(wavearray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_GetIndex: {
/* SPEC: (FMOD_CHANNEL *channel, int *index) */
			int index;
			ERRCHECK(FMOD_Channel_GetIndex((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &index));
			RXA_INT64(frm, 1) = index;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetMode: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_MODE mode) */
			FMOD_MODE mode = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_Channel_SetMode((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), mode));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetMode: {
/* SPEC: (FMOD_CHANNEL *channel, FMOD_MODE *mode) */
			FMOD_MODE mode;
			ERRCHECK(FMOD_Channel_GetMode((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &mode));
			RXA_INT64(frm, 1) = mode;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetLoopCount: {
/* SPEC: (FMOD_CHANNEL *channel, int loopcount) */
			ERRCHECK(FMOD_Channel_SetLoopCount((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetLoopCount: {
/* SPEC: (FMOD_CHANNEL *channel, int *loopcount) */
			int loopcount;
			ERRCHECK(FMOD_Channel_GetLoopCount((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &loopcount));
			RXA_INT64(frm, 1) = loopcount;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetLoopPoints: {
/* SPEC: (FMOD_CHANNEL *channel, unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype) */
			FMOD_TIMEUNIT loopstarttype = RXA_INT32(frm, 3);
			FMOD_TIMEUNIT loopendtype = RXA_INT32(frm, 5);
			ERRCHECK(FMOD_Channel_SetLoopPoints((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), loopstarttype, (unsigned int)RXA_INT32(frm, 4), loopendtype));
			return RXR_TRUE;
		}
		case CMD_FMOD_Channel_GetLoopPoints: {
/* SPEC: (FMOD_CHANNEL *channel, unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype) */
			unsigned int loopstart;
			FMOD_TIMEUNIT loopstarttype = RXA_INT32(frm, 2);
			unsigned int loopend;
			FMOD_TIMEUNIT loopendtype = RXA_INT32(frm, 3);
			ERRCHECK(FMOD_Channel_GetLoopPoints((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), &loopstart, loopstarttype, &loopend, loopendtype));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = loopstart;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			RXV_INT64(val) = loopend;
			RL_SET_VALUE(block, 1, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_Channel_SetUserData: {
/* TODO: (FMOD_CHANNEL *channel, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_GetUserData: {
/* TODO: (FMOD_CHANNEL *channel, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Channel_GetMemoryInfo: {
/* SPEC: (FMOD_CHANNEL *channel, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_Channel_GetMemoryInfo((FMOD_CHANNEL*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_Release: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup) */
			ERRCHECK(FMOD_ChannelGroup_Release((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetSystemObject: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_SYSTEM **system) */
			FMOD_SYSTEM *system=0;
			ERRCHECK(FMOD_ChannelGroup_GetSystemObject((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &system));
			RXA_HANDLE(frm, 1) = system;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_SetVolume: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float volume) */
			ERRCHECK(FMOD_ChannelGroup_SetVolume((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetVolume: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float *volume) */
			float volume;
			ERRCHECK(FMOD_ChannelGroup_GetVolume((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &volume));
			RXA_DEC64(frm, 1) = volume;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_SetPitch: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float pitch) */
			ERRCHECK(FMOD_ChannelGroup_SetPitch((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetPitch: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float *pitch) */
			float pitch;
			ERRCHECK(FMOD_ChannelGroup_GetPitch((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &pitch));
			RXA_DEC64(frm, 1) = pitch;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_Set3DOcclusion: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, float directocclusion, float reverbocclusion) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_Get3DOcclusion: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, float *directocclusion, float *reverbocclusion) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_SetPaused: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused) */
			ERRCHECK(FMOD_ChannelGroup_SetPaused((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (FMOD_BOOL)RXA_LOGIC(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetPaused: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *paused) */
			FMOD_BOOL paused;
			ERRCHECK(FMOD_ChannelGroup_GetPaused((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &paused));
			RXA_LOGIC(frm, 1) = paused;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_SetMute: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL mute) */
			ERRCHECK(FMOD_ChannelGroup_SetMute((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (FMOD_BOOL)RXA_LOGIC(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetMute: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *mute) */
			FMOD_BOOL mute;
			ERRCHECK(FMOD_ChannelGroup_GetMute((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &mute));
			RXA_LOGIC(frm, 1) = mute;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_Stop: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup) */
			ERRCHECK(FMOD_ChannelGroup_Stop((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_OverrideVolume: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float volume) */
			ERRCHECK(FMOD_ChannelGroup_OverrideVolume((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_OverrideFrequency: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float frequency) */
			ERRCHECK(FMOD_ChannelGroup_OverrideFrequency((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_OverridePan: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float pan) */
			ERRCHECK(FMOD_ChannelGroup_OverridePan((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_OverrideReverbProperties: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, const FMOD_REVERB_CHANNELPROPERTIES *prop) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_Override3DAttributes: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel) */
			return RXR_ERROR;
		}

		case CMD_FMOD_ChannelGroup_OverrideSpeakerMix: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float frontleft, float frontright, float center, float lfe, float backleft, float backright, float sideleft, float sideright) */
			RXIARG val;
			RL_GET_VALUE(RXA_SERIES(frm, 2),0,&val);
			float frontleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),1,&val);
			float frontright = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),2,&val);
			float center = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),3,&val);
			float lfe = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),4,&val);
			float backleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),5,&val);
			float backright = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),6,&val);
			float sideleft = RXV_DEC64(val);
			RL_GET_VALUE(RXA_SERIES(frm, 2),7,&val);
			float sideright = RXV_DEC64(val);
			ERRCHECK(FMOD_ChannelGroup_OverrideSpeakerMix((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), frontleft, frontright, center, lfe, backleft, backright, sideleft, sideright));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_AddGroup: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP *group) */
			ERRCHECK(FMOD_ChannelGroup_AddGroup((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_ChannelGroup_GetNumGroups: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, int *numgroups) */
			int numgroups;
			ERRCHECK(FMOD_ChannelGroup_GetNumGroups((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &numgroups));
			RXA_INT64(frm, 1) = numgroups;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetGroup: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_CHANNELGROUP **group) */
			FMOD_CHANNELGROUP *group=0;
			ERRCHECK(FMOD_ChannelGroup_GetGroup((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &group));
			RXA_HANDLE(frm, 1) = group;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetParentGroup: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP **group) */
			FMOD_CHANNELGROUP *group=0;
			ERRCHECK(FMOD_ChannelGroup_GetParentGroup((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &group));
			RXA_HANDLE(frm, 1) = group;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetDSPHead: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, FMOD_DSP **dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_AddDSP: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, FMOD_DSP *dsp, FMOD_DSPCONNECTION **connection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_GetName: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, char *name, int namelen) */
			ERRCHECK(FMOD_ChannelGroup_GetName((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), tmpBuffer1, 256));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetNumChannels: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, int *numchannels) */
			int numchannels;
			ERRCHECK(FMOD_ChannelGroup_GetNumChannels((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), &numchannels));
			RXA_INT64(frm, 1) = numchannels;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetChannel: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_CHANNEL **channel) */
			FMOD_CHANNEL *channel=0;
			ERRCHECK(FMOD_ChannelGroup_GetChannel((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &channel));
			RXA_HANDLE(frm, 1) = channel;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetSpectrum: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float *spectrumarray, int numvalues, int channeloffset, FMOD_DSP_FFT_WINDOW windowtype) */
			int numvalues = (int)RXA_INT32(frm, 2);
			int n;
			float* spectrumarray;
			spectrumarray=(float*)malloc(numvalues*sizeof(float));
			FMOD_DSP_FFT_WINDOW windowtype = RXA_INT32(frm, 4);
			ERRCHECK(FMOD_ChannelGroup_GetSpectrum((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), spectrumarray, numvalues, (int)RXA_INT32(frm, 3), windowtype));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = spectrumarray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(spectrumarray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_GetWaveData: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, float *wavearray, int numvalues, int channeloffset) */
			int numvalues = (int)RXA_INT32(frm, 2);
			int n;
			float* wavearray;
			wavearray=(float*)malloc(numvalues*sizeof(float));
			ERRCHECK(FMOD_ChannelGroup_GetWaveData((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), wavearray, numvalues, (int)RXA_INT32(frm, 3)));
			REBSER* block = RL_MAKE_BLOCK(numvalues);
			RXIARG val;
			for(n=0; n<numvalues;n++) {
			    RXV_DECIMAL(val) = wavearray[n];
			    RL_SET_VALUE(block, n, val, RXT_DECIMAL);
			}
			free(wavearray);
			RXA_SERIES(frm, 1) = block;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_ChannelGroup_SetUserData: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_GetUserData: {
/* TODO: (FMOD_CHANNELGROUP *channelgroup, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_ChannelGroup_GetMemoryInfo: {
/* SPEC: (FMOD_CHANNELGROUP *channelgroup, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_ChannelGroup_GetMemoryInfo((FMOD_CHANNELGROUP*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_Release: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup) */
			ERRCHECK(FMOD_SoundGroup_Release((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetSystemObject: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, FMOD_SYSTEM **system) */
			FMOD_SYSTEM *system=0;
			ERRCHECK(FMOD_SoundGroup_GetSystemObject((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &system));
			RXA_HANDLE(frm, 1) = system;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_SetMaxAudible: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, int maxaudible) */
			ERRCHECK(FMOD_SoundGroup_SetMaxAudible((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetMaxAudible: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, int *maxaudible) */
			int maxaudible;
			ERRCHECK(FMOD_SoundGroup_GetMaxAudible((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &maxaudible));
			RXA_INT64(frm, 1) = maxaudible;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_SetMaxAudibleBehavior: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, FMOD_SOUNDGROUP_BEHAVIOR behavior) */
			FMOD_SOUNDGROUP_BEHAVIOR behavior = RXA_INT32(frm, 2);
			ERRCHECK(FMOD_SoundGroup_SetMaxAudibleBehavior((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), behavior));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetMaxAudibleBehavior: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, FMOD_SOUNDGROUP_BEHAVIOR *behavior) */
			FMOD_SOUNDGROUP_BEHAVIOR behavior;
			ERRCHECK(FMOD_SoundGroup_GetMaxAudibleBehavior((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &behavior));
			RXA_INT64(frm, 1) = behavior;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_SetMuteFadeSpeed: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, float speed) */
			ERRCHECK(FMOD_SoundGroup_SetMuteFadeSpeed((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetMuteFadeSpeed: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, float *speed) */
			float speed;
			ERRCHECK(FMOD_SoundGroup_GetMuteFadeSpeed((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &speed));
			RXA_DEC64(frm, 1) = speed;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_SetVolume: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, float volume) */
			ERRCHECK(FMOD_SoundGroup_SetVolume((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), (float)RXA_DEC64(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetVolume: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, float *volume) */
			float volume;
			ERRCHECK(FMOD_SoundGroup_GetVolume((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &volume));
			RXA_DEC64(frm, 1) = volume;
			RXA_TYPE(frm, 1) = RXT_DECIMAL;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_Stop: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup) */
			ERRCHECK(FMOD_SoundGroup_Stop((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_SoundGroup_GetName: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, char *name, int namelen) */
			ERRCHECK(FMOD_SoundGroup_GetName((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), tmpBuffer1, 256));
			RXA_SERIES(frm, 1) = StringToRebser(tmpBuffer1);
			RXA_TYPE(frm, 1) = RXT_STRING;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_GetNumSounds: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, int *numsounds) */
			int numsounds;
			ERRCHECK(FMOD_SoundGroup_GetNumSounds((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &numsounds));
			RXA_INT64(frm, 1) = numsounds;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_GetSound: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, int index, FMOD_SOUND **sound) */
			FMOD_SOUND *sound = 0;
			ERRCHECK(FMOD_SoundGroup_GetSound((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), (int)RXA_INT32(frm, 2), &sound));
			RXA_HANDLE(frm, 1) = sound;
			RXA_TYPE(frm, 1) = RXT_HANDLE;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_GetNumPlaying: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, int *numplaying) */
			int numplaying;
			ERRCHECK(FMOD_SoundGroup_GetNumPlaying((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), &numplaying));
			RXA_INT64(frm, 1) = numplaying;
			RXA_TYPE(frm, 1) = RXT_INTEGER;
			return RXR_VALUE;
		}
		case CMD_FMOD_SoundGroup_SetUserData: {
/* TODO: (FMOD_SOUNDGROUP *soundgroup, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_SoundGroup_GetUserData: {
/* TODO: (FMOD_SOUNDGROUP *soundgroup, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_SoundGroup_GetMemoryInfo: {
/* SPEC: (FMOD_SOUNDGROUP *soundgroup, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_SoundGroup_GetMemoryInfo((FMOD_SOUNDGROUP*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}
		case CMD_FMOD_DSP_Release: {
/* TODO: (FMOD_DSP *dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetSystemObject: {
/* TODO: (FMOD_DSP *dsp, FMOD_SYSTEM **system) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_AddInput: {
/* TODO: (FMOD_DSP *dsp, FMOD_DSP *target, FMOD_DSPCONNECTION **connection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_DisconnectFrom: {
/* TODO: (FMOD_DSP *dsp, FMOD_DSP *target) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_DisconnectAll: {
/* TODO: (FMOD_DSP *dsp, FMOD_BOOL inputs, FMOD_BOOL outputs) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_Remove: {
/* TODO: (FMOD_DSP *dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetNumInputs: {
/* TODO: (FMOD_DSP *dsp, int *numinputs) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetNumOutputs: {
/* TODO: (FMOD_DSP *dsp, int *numoutputs) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetInput: {
/* TODO: (FMOD_DSP *dsp, int index, FMOD_DSP **input, FMOD_DSPCONNECTION **inputconnection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetOutput: {
/* TODO: (FMOD_DSP *dsp, int index, FMOD_DSP **output, FMOD_DSPCONNECTION **outputconnection) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetActive: {
/* TODO: (FMOD_DSP *dsp, FMOD_BOOL active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetActive: {
/* TODO: (FMOD_DSP *dsp, FMOD_BOOL *active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetBypass: {
/* TODO: (FMOD_DSP *dsp, FMOD_BOOL bypass) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetBypass: {
/* TODO: (FMOD_DSP *dsp, FMOD_BOOL *bypass) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetSpeakerActive: {
/* TODO: (FMOD_DSP *dsp, FMOD_SPEAKER speaker, FMOD_BOOL active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetSpeakerActive: {
/* TODO: (FMOD_DSP *dsp, FMOD_SPEAKER speaker, FMOD_BOOL *active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_Reset: {
/* TODO: (FMOD_DSP *dsp) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetParameter: {
/* TODO: (FMOD_DSP *dsp, int index, float value) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetParameter: {
/* TODO: (FMOD_DSP *dsp, int index, float *value, char *valuestr, int valuestrlen) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetNumParameters: {
/* TODO: (FMOD_DSP *dsp, int *numparams) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetParameterInfo: {
/* TODO: (FMOD_DSP *dsp, int index, char *name, char *label, char *description, int descriptionlen, float *min, float *max) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_ShowConfigDialog: {
/* TODO: (FMOD_DSP *dsp, void *hwnd, FMOD_BOOL show) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetInfo: {
/* TODO: (FMOD_DSP *dsp, char *name, unsigned int *version, int *channels, int *configwidth, int *configheight) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetType: {
/* TODO: (FMOD_DSP *dsp, FMOD_DSP_TYPE *type) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetDefaults: {
/* TODO: (FMOD_DSP *dsp, float frequency, float volume, float pan, int priority) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetDefaults: {
/* TODO: (FMOD_DSP *dsp, float *frequency, float *volume, float *pan, int *priority) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_SetUserData: {
/* TODO: (FMOD_DSP *dsp, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetUserData: {
/* TODO: (FMOD_DSP *dsp, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSP_GetMemoryInfo: {
/* TODO: (FMOD_DSP *dsp, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetInput: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, FMOD_DSP **input) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetOutput: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, FMOD_DSP **output) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_SetMix: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, float volume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetMix: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, float *volume) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_SetLevels: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, FMOD_SPEAKER speaker, float *levels, int numlevels) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetLevels: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, FMOD_SPEAKER speaker, float *levels, int numlevels) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_SetUserData: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetUserData: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_DSPConnection_GetMemoryInfo: {
/* TODO: (FMOD_DSPCONNECTION *dspconnection, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_Release: {
/* TODO: (FMOD_GEOMETRY *geometry) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_AddPolygon: {
/* TODO: (FMOD_GEOMETRY *geometry, float directocclusion, float reverbocclusion, FMOD_BOOL doublesided, int numvertices, const FMOD_VECTOR *vertices, int *polygonindex) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetNumPolygons: {
/* TODO: (FMOD_GEOMETRY *geometry, int *numpolygons) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetMaxPolygons: {
/* TODO: (FMOD_GEOMETRY *geometry, int *maxpolygons, int *maxvertices) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetPolygonNumVertices: {
/* TODO: (FMOD_GEOMETRY *geometry, int index, int *numvertices) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetPolygonVertex: {
/* TODO: (FMOD_GEOMETRY *geometry, int index, int vertexindex, const FMOD_VECTOR *vertex) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetPolygonVertex: {
/* TODO: (FMOD_GEOMETRY *geometry, int index, int vertexindex, FMOD_VECTOR *vertex) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetPolygonAttributes: {
/* TODO: (FMOD_GEOMETRY *geometry, int index, float directocclusion, float reverbocclusion, FMOD_BOOL doublesided) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetPolygonAttributes: {
/* TODO: (FMOD_GEOMETRY *geometry, int index, float *directocclusion, float *reverbocclusion, FMOD_BOOL *doublesided) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetActive: {
/* TODO: (FMOD_GEOMETRY *geometry, FMOD_BOOL active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetActive: {
/* TODO: (FMOD_GEOMETRY *geometry, FMOD_BOOL *active) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetRotation: {
/* TODO: (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *forward, const FMOD_VECTOR *up) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetRotation: {
/* TODO: (FMOD_GEOMETRY *geometry, FMOD_VECTOR *forward, FMOD_VECTOR *up) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetPosition: {
/* TODO: (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *position) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetPosition: {
/* TODO: (FMOD_GEOMETRY *geometry, FMOD_VECTOR *position) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetScale: {
/* TODO: (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *scale) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetScale: {
/* TODO: (FMOD_GEOMETRY *geometry, FMOD_VECTOR *scale) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_Save: {
/* TODO: (FMOD_GEOMETRY *geometry, void *data, int *datasize) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_SetUserData: {
/* TODO: (FMOD_GEOMETRY *geometry, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetUserData: {
/* TODO: (FMOD_GEOMETRY *geometry, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Geometry_GetMemoryInfo: {
/* TODO: (FMOD_GEOMETRY *geometry, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_Release: {
/* SPEC: (FMOD_REVERB *reverb) */
			ERRCHECK(FMOD_Reverb_Release((FMOD_REVERB*)RXA_HANDLE(frm, 1)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Reverb_Set3DAttributes: {
/* TODO: (FMOD_REVERB *reverb, const FMOD_VECTOR *position, float mindistance, float maxdistance) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_Get3DAttributes: {
/* TODO: (FMOD_REVERB *reverb, FMOD_VECTOR *position, float *mindistance, float *maxdistance) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_SetProperties: {
/* TODO: (FMOD_REVERB *reverb, const FMOD_REVERB_PROPERTIES *properties) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_GetProperties: {
/* TODO: (FMOD_REVERB *reverb, FMOD_REVERB_PROPERTIES *properties) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_SetActive: {
/* SPEC: (FMOD_REVERB *reverb, FMOD_BOOL active) */
			ERRCHECK(FMOD_Reverb_SetActive((FMOD_REVERB*)RXA_HANDLE(frm, 1), (FMOD_BOOL)RXA_LOGIC(frm, 2)));
			return RXR_TRUE;
		}
		case CMD_FMOD_Reverb_GetActive: {
/* SPEC: (FMOD_REVERB *reverb, FMOD_BOOL *active) */
			FMOD_BOOL active;
			ERRCHECK(FMOD_Reverb_GetActive((FMOD_REVERB*)RXA_HANDLE(frm, 1), &active));
			RXA_LOGIC(frm, 1) = active;
			RXA_TYPE(frm, 1) = RXT_LOGIC;
			return RXR_VALUE;
		}
		case CMD_FMOD_Reverb_SetUserData: {
/* TODO: (FMOD_REVERB *reverb, void *userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_GetUserData: {
/* TODO: (FMOD_REVERB *reverb, void **userdata) */
			return RXR_ERROR;
		}
		case CMD_FMOD_Reverb_GetMemoryInfo: {
/* SPEC: (FMOD_REVERB *reverb, unsigned int memorybits, unsigned int event_memorybits, unsigned int *memoryused, FMOD_MEMORY_USAGE_DETAILS *memoryused_details) */
			unsigned int memoryused;
			FMOD_MEMORY_USAGE_DETAILS memoryused_details;
			ERRCHECK(FMOD_Reverb_GetMemoryInfo((FMOD_REVERB*)RXA_HANDLE(frm, 1), (unsigned int)RXA_INT32(frm, 2), (unsigned int)RXA_INT32(frm, 3), &memoryused, &memoryused_details));
			RXIARG val;
			REBSER* block = RL_MAKE_BLOCK(2);
			RXV_INT64(val) = memoryused;
			RL_SET_VALUE(block, 0, val, RXT_INTEGER);
			REBSER* blk_memoryused_details = RL_MAKE_BLOCK(48);
			RXV_INT64(val) = memoryused_details.other;
			RL_SET_VALUE(blk_memoryused_details, 0, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.string;
			RL_SET_VALUE(blk_memoryused_details, 1, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.system;
			RL_SET_VALUE(blk_memoryused_details, 2, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.plugins;
			RL_SET_VALUE(blk_memoryused_details, 3, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.output;
			RL_SET_VALUE(blk_memoryused_details, 4, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channel;
			RL_SET_VALUE(blk_memoryused_details, 5, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.channelgroup;
			RL_SET_VALUE(blk_memoryused_details, 6, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.codec;
			RL_SET_VALUE(blk_memoryused_details, 7, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.file;
			RL_SET_VALUE(blk_memoryused_details, 8, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sound;
			RL_SET_VALUE(blk_memoryused_details, 9, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.secondaryram;
			RL_SET_VALUE(blk_memoryused_details, 10, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundgroup;
			RL_SET_VALUE(blk_memoryused_details, 11, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streambuffer;
			RL_SET_VALUE(blk_memoryused_details, 12, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspconnection;
			RL_SET_VALUE(blk_memoryused_details, 13, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dsp;
			RL_SET_VALUE(blk_memoryused_details, 14, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.dspcodec;
			RL_SET_VALUE(blk_memoryused_details, 15, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.profile;
			RL_SET_VALUE(blk_memoryused_details, 16, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.recordbuffer;
			RL_SET_VALUE(blk_memoryused_details, 17, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverb;
			RL_SET_VALUE(blk_memoryused_details, 18, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbchannelprops;
			RL_SET_VALUE(blk_memoryused_details, 19, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.geometry;
			RL_SET_VALUE(blk_memoryused_details, 20, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.syncpoint;
			RL_SET_VALUE(blk_memoryused_details, 21, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventsystem;
			RL_SET_VALUE(blk_memoryused_details, 22, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.musicsystem;
			RL_SET_VALUE(blk_memoryused_details, 23, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.fev;
			RL_SET_VALUE(blk_memoryused_details, 24, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.memoryfsb;
			RL_SET_VALUE(blk_memoryused_details, 25, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventproject;
			RL_SET_VALUE(blk_memoryused_details, 26, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventgroupi;
			RL_SET_VALUE(blk_memoryused_details, 27, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbankclass;
			RL_SET_VALUE(blk_memoryused_details, 28, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.soundbanklist;
			RL_SET_VALUE(blk_memoryused_details, 29, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.streaminstance;
			RL_SET_VALUE(blk_memoryused_details, 30, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefclass;
			RL_SET_VALUE(blk_memoryused_details, 31, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefdefclass;
			RL_SET_VALUE(blk_memoryused_details, 32, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.sounddefpool;
			RL_SET_VALUE(blk_memoryused_details, 33, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.reverbdef;
			RL_SET_VALUE(blk_memoryused_details, 34, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventreverb;
			RL_SET_VALUE(blk_memoryused_details, 35, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.userproperty;
			RL_SET_VALUE(blk_memoryused_details, 36, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance;
			RL_SET_VALUE(blk_memoryused_details, 37, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_complex;
			RL_SET_VALUE(blk_memoryused_details, 38, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_simple;
			RL_SET_VALUE(blk_memoryused_details, 39, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_layer;
			RL_SET_VALUE(blk_memoryused_details, 40, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstance_sound;
			RL_SET_VALUE(blk_memoryused_details, 41, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelope;
			RL_SET_VALUE(blk_memoryused_details, 42, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopedef;
			RL_SET_VALUE(blk_memoryused_details, 43, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventparameter;
			RL_SET_VALUE(blk_memoryused_details, 44, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventcategory;
			RL_SET_VALUE(blk_memoryused_details, 45, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventenvelopepoint;
			RL_SET_VALUE(blk_memoryused_details, 46, val, RXT_INTEGER);
			RXV_INT64(val) = memoryused_details.eventinstancepool;
			RL_SET_VALUE(blk_memoryused_details, 47, val, RXT_INTEGER);
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = blk_memoryused_details;
			RXA_INDEX(frm, 1)  = 0;
			RXA_TYPE(frm, 1) = RXT_BLOCK;
			RXA_SERIES(frm, 1) = block;
			RXA_INDEX(frm, 1)  = 0;
			return RXR_VALUE;
		}

	}
	return RXR_NO_COMMAND;
}
