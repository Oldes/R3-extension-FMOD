REBOL [
	title: "Build FMOD extension"
]
ajoin: func [
    {Faster way how to create string from a block (in R3 it's native!)}
    block [block!]
][make string! reduce block]



RXT-CMD-INIT-BLOCK: copy ""
RXT-CMD-ENUM: copy ""
RXT-CMD-CALL: copy ""

skip-functions: [
	"FMOD_System_Create"
	"FMOD_System_Init"
	"FMOD_System_Release"
]
todo-functions: [
	"FMOD_Memory_Initialize"
	"FMOD_Debug_SetLevel"
	"FMOD_Debug_GetLevel"
	;"FMOD_File_SetDiskBusy"
	;"FMOD_File_GetDiskBusy"
	
	;"FMOD_System_SetOutput"
	"FMOD_System_SetFileSystem"
	"FMOD_System_AttachFileSystem"
	"FMOD_System_SetAdvancedSettings"
	"FMOD_System_GetAdvancedSettings"
	;"FMOD_System_GetDriverCaps"
	"FMOD_System_SetCallback"
	
	;"FMOD_System_GetCDROMDriveName"
	;"FMOD_System_CreateSound"
	"FMOD_System_SetReverbProperties"
	"FMOD_System_GetReverbProperties"
	"FMOD_System_SetReverbAmbientProperties"
	"FMOD_System_GetReverbAmbientProperties"
	"FMOD_Channel_SetReverbProperties"
	"FMOD_Channel_GetReverbProperties"
	"FMOD_Reverb_SetProperties"
]


get-blk-from-struct: func[sname spec frm /local result n][
	result: ajoin [
		"^-^-^-RXIARG val;^/"
		"^-^-^-REBSER* blk_" sname " = RL_MAKE_BLOCK(" (length? spec) / 3 ");^/"
	]
	n: 0
	foreach [name rxv rxt] spec [
		append result ajoin [
			"^-^-^-RXV_" rxv "(val) = " 
				ajoin switch/default rxt [
					"STRING" [ ["StringToRebser(" sname "." name ")"] ]
				][	[sname "." name] ]
				";^/"
			"^-^-^-RL_SET_VALUE(blk_" sname ", " n ", val, RXT_" rxt ");^/"
		]
		n: n + 1
	]
	head append result ajoin [
		"^-^-^-RXA_TYPE(frm, " frm ") = RXT_BLOCK;^/"
		"^-^-^-RXA_SERIES(frm, " frm ") = blk_" sname ";^/"
		"^-^-^-RXA_INDEX(frm, " frm ")  = 0;^/"
	]
]

r-spec: copy "" 
c-pre:  copy ""
c-post: copy ""
c-spec: copy ""
ret-vals: copy []

ch_space:   charset " ^-^/^M"
ch_digits:  charset "0123456789"
ch_alfanum: complement ch_space
ch_name:    exclude ch_alfanum charset ",="
ch_hex:     charset "0123456789abcdef"
enums: copy []

fmod.h: read %inc/fmod.h
parse/all fmod.h [
	some [
		thru "typedef enum" thru "{" copy tmp to "}"  1 skip any ch_space copy ename to ";" (
			;probe ename
			enum: copy []
			v: 0
			parse/all tmp [
				some [
					any ch_space
					copy val some ch_name
					any ch_space
					(val2: none)
					opt [
						"=" any ch_space [
							copy val2 [opt "-" some ch_digits] (val2: to-integer val2)
							|
							copy val2 some ch_name (val2: any [select enum val2 val2])
						]
					]
					thru "^/" (
						;print [val val2]
						repend enum [
							val
							either val2 [ v: val2 val2][ v ]
						]
						v: v + 1
					)
				]
			]
			repend enums [ename	new-line/skip enum true 2]
		)
	]
]

parse/all fmod.h [
	some [
		thru {[DEFINE]} thru {[NAME]} some ch_space copy name some ch_name copy tmp to {[DEFINE_END]} (
			probe name
			enum: copy []
			parse/all tmp [
				some [
					thru "#define"
					some ch_space
					copy val some ch_name
					any ch_space
					[
						  "0x" copy val2 some ch_hex (val2: to-integer debase/base val2 16)
						| copy val2 [opt "-" some ch_digits] (val2: to-integer val2)
						;| "(" copy val2 to ")" (
						;	replace/all val2 "\" ""
						;	res: none
						;	parse val2 [
						;		some [
						;			[
						;				"0x" copy val3 ch_hex (val3: to-integer debase/base val3 16)
						;				| copy val3 [opt "-" some ch_digits] (val3: to-integer val3)
						;				| copy val3 some ch_name (val3: any [select enum val3 val3])
						;			](
						;				either res [
						;				][]
						;			)
						;		]
						;	]
						;	ask ""
						;)
						| copy val2 some ch_alfanum (
							; ask ["unknown def:" mold val2]
							val2: none
						)
						
					]
					(
						;print [val val2]
						repend enum [
							val
							val2
						]
					)
				]
			]
			repend enums [name	new-line/skip enum true 2]
		)
	]
]
;ask ""
;append enums defines
new-line/skip enums true 2
;probe enums
rl_enums: copy []
foreach [enum vals] enums [ repend rl_enums [enum '|] ]
remove back tail rl_enums
;halt

export-contexts: copy []
num-skiped:   0
num-commands: 0

parse fmod.h [
	some [
		thru {FMOD_RESULT F_API} copy name to "(" 1 skip copy args to ");"
		(
			probe trim name
			
			unless find skip-functions name [
				clear r-spec
				clear c-spec
				clear c-pre
				clear c-post
				clear ret-vals
				
				num-commands: num-commands + 1
				
				parse name [
					"FMOD_"
					copy ctx-name to "_" 1 skip
					copy exp-name to end (
						unless ctx-spec: select export-contexts ctx-name [
							repend export-contexts [ctx-name ctx-spec: copy ""]
						]
					)
				]
				
				append RXT-CMD-ENUM ajoin ["^-CMD_" name ",^/"]
				
				either any [
					find todo-functions name
					find name "Plugin"
					find name "Codec"
					find name "3D"
					find/case name "DSP"
					find/case name "Geometry"
					find/case name "Properties"
					find/case name "Callback"
					find/case name "MemoryInfo"
					find/case name "UserData"
					;find/case name "Network"
				][
					;SKIPING...
					num-skiped: num-skiped + 1
					append ctx-spec rejoin [{^-^-"} exp-name {: command[{TODO}]\n"^/}]
					;append RXT-CMD-INIT-BLOCK rejoin [{^-"export } exp-name {: command[{TODO}]\n"^/}]
					append RXT-CMD-CALL rejoin [
						"^-^-case CMD_" name ": {^/"
						"/* TODO: (" args ") */^/"
						"^-^-^-return RXR_ERROR;^/"
						"^-^-}^/"
					]
					
				][
					
	
					n: 0 buf: 0
					foreach arg probe parse/all args "," [
						trim arg
						;probe c-pre
						
						r-type: none
						struct-name: none
						parse/all arg [
							  "FMOD_SYSTEM *system"   ( append c-spec "sys, " )
							| "FMOD_SOUND **" copy tmp to end (
								append c-pre  rejoin ["^-^-^-FMOD_SOUND *" tmp " = 0;^/"]
								append c-spec rejoin ["&" tmp ", "]
								repend ret-vals [tmp "HANDLE" "HANDLE"]
							)
							| "FMOD_SOUND *" copy tmp to end     (
								n: n + 1
								append c-spec rejoin ["(FMOD_SOUND*)RXA_HANDLE(frm, " n "), "]
								append r-spec join tmp " [handle!] "
							)
							| "FMOD_CHANNEL *channel" (
								n: n + 1
								append c-spec rejoin ["(FMOD_CHANNEL*)RXA_HANDLE(frm, " n "), "]
								append r-spec "channel [handle!] "
							)
							| "FMOD_CHANNELGROUP *" copy tmp ["channelgroup" | "group"] (
								n: n + 1
								append c-spec rejoin ["(FMOD_CHANNELGROUP*)RXA_HANDLE(frm, " n "), "]
								append r-spec join tmp " [handle!] "
							)
							| "FMOD_SOUNDGROUP *soundgroup" (
								n: n + 1
								append c-spec rejoin ["(FMOD_SOUNDGROUP*)RXA_HANDLE(frm, " n "), "]
								append r-spec "soundgroup [handle!] "
							)
							| "FMOD_SYNCPOINT *point" (
								n: n + 1
								append c-spec rejoin ["(FMOD_SYNCPOINT*)RXA_HANDLE(frm, " n "), "]
								append r-spec "point [handle!] "
							)
							| "FMOD_REVERB *reverb" (
								n: n + 1
								append c-spec rejoin ["(FMOD_REVERB*)RXA_HANDLE(frm, " n "), "]
								append r-spec "reverb [handle!] "
							)
							| "char *" copy tmp ["name" | "drivename" | "scsiname" | "devicename" | "proxy"] ( 
								buf: buf + 1
								;append c-pre  rejoin ["^-^-^-char " tmp "[256];^/"]
								append c-spec rejoin ["tmpBuffer" buf ", "]
								repend ret-vals [join "tmpBuffer" buf "SERIES" "STRING"]
							)
							| "const char" some ch_space copy p any #"*" copy arg-name to end (
								n: n + 1
								append c-pre  rejoin ["^-^-^-const char *" arg-name "= RebserToMultiByte(RXA_SERIES(frm, " n "));^/"]
								append c-spec rejoin [arg-name ", "]
								append r-spec rejoin [arg-name " [string!] "]
							)
							| "short *name" (
								buf: buf + 1
								append c-spec   rejoin["(short *)tmpBuffer" buf ", "]
								repend ret-vals [join "tmpBuffer" buf "SERIES" "STRING"]
							)
							
							| "int " copy tmp ["namelen" | "drivenamelen" | "scsinamelen" | "devicenamelen" | "proxylen"] (
								append c-spec "256, "
							)
							| "FMOD_GUID *guid" (
								append c-spec "(FMOD_GUID *)NULL, "
							)
							| "FMOD_CREATESOUNDEXINFO" to end (
								append c-spec "(FMOD_CREATESOUNDEXINFO *)NULL, "
							)
							| "void" some ch_space copy p any #"*" copy arg-name to end (
								;probe args
								switch/default p [
									"*" [
										n: n + 1
										append r-spec rejoin [arg-name " [handle!] "]
										append c-spec rejoin [ "(void *)RXA_HANDLE(frm, " n "), " ]
									]
									"**" [
										append c-pre rejoin [
											"^-^-^-void *" arg-name ";^/"
			        					]
			        					append c-spec rejoin [ "&" arg-name ", " ]
			        					repend ret-vals [arg-name "HANDLE" "HANDLE"]
									]
								][
									ask ["unknown void arg:" p arg-name]
								]
								;ask ""
							)
							| copy type  [
								rl_enums
								| ["int" | "unsigned int" ] (r-type: "integer!" rxa: "INT32" rxt: "INTEGER" rxv: "INT64")
								| "FMOD_BOOL"   (r-type: "logic!"  rxa: rxt: rxv: "LOGIC")
								| "float" (r-type: "decimal!" rxa: rxv: "DEC64" rxt: "DECIMAL")
								| copy struct-name [
									  "FMOD_CHANNELGROUP"
									| "FMOD_SOUNDGROUP"
									| "FMOD_REVERB"
									| "FMOD_CHANNEL"
									| "FMOD_SOUND"
									| "FMOD_GEOMETRY"
									| "FMOD_SYSTEM"
									| "FMOD_TAG"
									| "FMOD_SYNCPOINT"
									| "FMOD_CAPS"
								] (r-type: "handle!" rxa: rxt: rxv: "HANDLE")
							] some ch_space copy p any #"*" copy arg-name to end (
								
								;print [p arg-name struct-name]
								if arg-name = "handle" [r-type: "handle!" rxa: rxt: rxv: "HANDLE"]
								switch/default p [
									"*"  [
										either r-type [
											switch struct-name [
				        						"FMOD_TAG" [
				        							r-type: none
				        							rxv: none
				        							rxt: [
				        								"type"     "INT64"  "INTEGER"
				        								"datatype" "INT64"  "INTEGER"
														"name"     "SERIES" "STRING"
														"data"     "SERIES" "STRING"
														"datalen"  "INT64"  "INTEGER"
														"updated"  "LOGIC"  "LOGIC"
													]
			        							]
			        						] 
											append c-pre rejoin [
												"^-^-^-" type " " either r-type = "handle!" ["*"][""] arg-name ";^/"
				        					]
				        					append c-spec rejoin [ "&" arg-name ", " ]
				        					
				        					repend ret-vals [arg-name rxv rxt]
										][
											case [
												en: select enums type [
													append c-pre rejoin [
														"^-^-^-" type " " arg-name ";^/"
						        					]
						        					append c-spec rejoin [ "&" arg-name ", " ]
						        					repend ret-vals [arg-name "INT64" "INTEGER"]
					        					]
				        					]
			        					]
		        					
										
									]
									"**" [
										either r-type [
											append c-pre rejoin [
												"^-^-^-" type " *" arg-name "=0;^/"
				        					]
				        					append c-spec rejoin [ "&" arg-name ", " ]
				        					repend ret-vals [arg-name rxv rxt]
										][
											case [
												en: select enums type [
													append c-pre rejoin [
														"^-^-^-" type " *" arg-name ";^/"
						        					]
						        					append c-spec rejoin [ "&" arg-name ", " ]
						        					repend ret-vals [arg-name "INT64" "INTEGER"]
					        					]
				        					]
			        					]
									]
								][
									n: n + 1							
									either r-type [
										append r-spec rejoin [arg-name " [" r-type "] "]
										append c-spec rejoin ["(" type ")RXA_" rxa "(frm, " n "), "]
									][
										case [
											en: select enums type [
												append c-pre rejoin [
													"^-^-^-" type " " arg-name " = RXA_INT32(frm, " n ");^/"
					            					;"^-^-^-if(" arg-name " < 0 || " arg-name " > 21) " arg-name " = 0;^/"
					        					]
					        					append c-spec rejoin [ arg-name ", " ]
				        					]
			        					]
		        					]
		        					
	        					]
								
							)
							| copy tmp to end (ask ["!!!!! unlknown arg:" mold tmp])
						]
	
					]
					either empty? ret-vals [
						append c-post "^-^-^-return RXR_TRUE;^/"
					][
						
						either 3 = length? ret-vals [
							foreach [name rxv rxt] ret-vals [ 
		    					either rxv [
			    					append c-post rejoin [
			    						"^-^-^-RXA_" rxv "(frm, 1) = " either rxt = "STRING" [rejoin ["StringToRebser(" name ")"]][name] ";^/"
				            			"^-^-^-RXA_TYPE(frm, 1) = RXT_" rxt ";^/"
				            			"^-^-^-return RXR_VALUE;^/"
									]
								][
									append c-post get-blk-from-struct name rxt 1
									append c-post "^-^-^-return RXR_VALUE;^/"
								]
							]
							
						] [
							n: 0
		    				foreach [name rxv rxt] ret-vals [ 
		    					either rxv [
			    					append c-post rejoin [
			    						"^-^-^-RXV_" rxv "(val) = " either rxt = "STRING" [rejoin ["StringToRebser(" name ")"]][name] ";^/"
										"^-^-^-RL_SET_VALUE(block, " n ", val, RXT_" rxt ");^/"
									]
								][
									append c-post get-blk-from-struct name rxt n
								]
								n: n + 1
							]
							insert c-post rejoin [
								"^-^-^-REBSER* block = RL_MAKE_BLOCK(" n ");^/"
								"^-^-^-RXIARG val;^/"
							]
							append c-post rejoin [
								"^-^-^-RXA_TYPE(frm, 1) = RXT_BLOCK;^/"
								"^-^-^-RXA_SERIES(frm, 1) = block;^/"
								"^-^-^-RXA_INDEX(frm, 1)  = 0;^/"
								"^-^-^-return RXR_VALUE;^/"
							]
						]
					]				
							
					print "----------"
					append ctx-spec rejoin [{^-^-"} exp-name {: command[} r-spec {]\n"^/}]
					;append RXT-CMD-INIT-BLOCK rejoin [{^-"export } exp-name {: command[} r-spec {]\n"^/}]
					append RXT-CMD-CALL rejoin [
						"^-^-case CMD_" name ": {^/"
						"/* SPEC: (" args ") */^/"
						c-pre
						"^-^-^-ERRCHECK(" name "(" head clear skip tail c-spec -2 "));^/" 
						c-post
						"^-^-}^/"
					]
					;ask ""
				]
			]
		)
	]
]
foreach [ctx-name ctx-spec] export-contexts [
	append RXT-CMD-INIT-BLOCK rejoin [
		{^-"export } ctx-name {: context[\n"^/}
		ctx-spec
		{^-"]\n"^/}
	]
]
 result: build-markup read %main.c.tmpl
if exists? %main.c [
	if exists? %main.c.back [delete %main.c.back]
	rename %main.c %main.c.back
]
write %main.c result 
print ["Commads total:" num-commands ", skiped:" num-skiped]
halt