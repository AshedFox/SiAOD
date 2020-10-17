program laba_2;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils;

type
PlayerADR = ^Player;
Player = record
  Number: byte;
  Next: PlayerADR;
end;

var Head: PlayerADR;
    N, k, i: integer;

procedure SetPlayersSequence(const Head: PlayerADR; const NumOfPlayers: byte);
var i: integer;
    CurPlayer: PlayerADR;
begin
  CurPlayer:=Head;
  for i := 1 to NumOfPlayers-1 do
  begin
    CurPlayer.Number:=i;
    New(CurPlayer.Next);
    CurPlayer:=CurPlayer.Next;
  end;
  CurPlayer.Number:=NumOfPlayers;
  CurPlayer.Next:=Head;
end;

procedure CountGame(var Head: PlayerADR;const NumOfPlayers: byte; const step: integer);
var i, j, separator: integer;
    CurPlayer, PrevPlayer: PlayerADR;
begin
  Randomize;
  separator:=198;
  writeln('|----------------------------------------------------------------------------------------------' +
        '----------------------------------------------------------------------------------------------' +
        '--------------------|');
  write('|', NumOfPlayers:3,'|');
  CurPlayer:=Head;
  PrevPlayer:=CurPlayer;
  for i := 1 to NumOfPlayers-1 do
  begin
    for j := 1 to step do
    begin
      PrevPlayer:=CurPlayer;
      CurPlayer:=CurPlayer.Next;
    end;
    write('|',CurPlayer.Number:2);
    separator:=separator-3;
    PrevPlayer.Next:=CurPlayer.Next;
    CurPlayer:=CurPlayer.Next;
  end;
  writeln('|':separator,CurPlayer.Number:6,'|');
  Dispose(CurPlayer);
end;

begin
  N:=64;
  repeat
    write('Enter step(k>=0): ');
    readln(k);
  until (k>=0);
  writeln('+----------------------------------------------------------------------------------------------' +
        '----------------------------------------------------------------------------------------------' +
        '--------------------+');
  writeln('| N ||', ' Out sequence ', '':182, '|Winner|');
  for i := 1 to N do
  begin
    New(Head);
    SetPlayersSequence(Head, i);
    CountGame(Head, i, k);
  end;
  writeln('+----------------------------------------------------------------------------------------------' +
        '----------------------------------------------------------------------------------------------' +
        '--------------------+');
  readln;
end.
