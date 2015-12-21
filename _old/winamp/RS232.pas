unit RS232;

interface 	

uses windows;

//RS232-Funktionen des Treibers

const DLL='COM.DLL'; 	
Procedure DELAY(i:WORD); stdcall; external DLL; 	
Procedure TIMEINIT; stdcall; external DLL; 	
Procedure CLOSECOM; stdcall; external DLL; 	
Function TIMEREAD: DWORD; stdcall; external DLL; 	
Procedure DELAYUS(i:DWORD); stdcall; external DLL; 	
Procedure TIMEINITUS; stdcall; external DLL; 	
Function TIMEREADUS: DWORD; stdcall; external DLL; 	
Procedure OUTPORT(PortAddr:Word; Data:byte); stdcall; external DLL; 	
Function INPORT(PortAddr:Word):Byte;stdcall; external DLL; 	
Function OPENCOM(S:PCHAR):Integer;stdcall; external DLL; 	
Function READBYTE:Integer;stdcall; external DLL; 	
Procedure SENDBYTE(d:WORD);stdcall; external DLL; 	
Procedure DTR(d:WORD);stdcall; external DLL; 	
Procedure RTS(d:WORD);stdcall; external DLL; 	
Procedure TXD(d:WORD);stdcall; external DLL; 	
Function CTS:Integer;stdcall; external DLL; 	
Function DSR:Integer;stdcall; external DLL; 	
Function RI:Integer;stdcall; external DLL; 	
Function DCD:Integer;stdcall; external DLL; 	
Procedure REALTIME(d:BOOLEAN);stdcall; external DLL; 	
Function SOUNDSETRATE(Rate:DWORD):DWORD; stdcall; external DLL; 	
Function SOUNDGETRATE:DWORD; stdcall; external DLL; 	
Function SOUNDBUSY:Boolean; stdcall; external DLL; 	
Function SOUNDIS:Boolean; stdcall; external DLL; 	
Procedure SOUNDIN(Puffer:Pchar;Size:DWORD); stdcall; external DLL; 	
Procedure SOUNDOUT(Puffer:Pchar;Size:DWORD); stdcall; external DLL; 	
Function SOUNDGETBYTES:DWORD; stdcall; external DLL; 	
Function SOUNDSETBYTES(B:DWORD):DWORD; stdcall; external DLL; 	
Procedure SOUNDCAPIN; stdcall; external DLL; 	
Procedure SOUNDCAPOUT; stdcall; external DLL; 	
Function JOYX:DWORD;stdcall; external DLL; 	
Function JOYY:DWORD;stdcall; external DLL; 	
Function JOYZ:DWORD;stdcall; external DLL; 	
Function JOYR:DWORD;stdcall; external DLL; 	
Function JOYBUTTON:DWORD;stdcall; external DLL;	

implementation


{$R-}

end.
