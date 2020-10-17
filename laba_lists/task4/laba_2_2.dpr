program laba_2_2;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils;

type
PhNumADR2 = ^PhNums2;
PhNums2 = record
  Number: string;
  Next: PhNumADR2;
  Prev: PhNumADR2;
end;

PhNumADR1 = ^PhNums1;
PhNums1 = record
  Number: string;
  Next: PhNumAdr1;
end;


var Head, EndHead: PhNumADR2;
    NewHead: PhNumADR1;
    N: integer;

procedure SetList(var Head, EndHead: PhNumADR2; const NumOfNumbers: integer);
var i, j: integer;
    CurNum, PrevNum: PhNumADR2;
begin
  CurNum:=Head;
  CurNum.Prev:=nil;
  for i := 1 to NumOfNumbers-1 do
  begin
    CurNum.Number:=IntToStr(Random(8999999)+1000000);
    New(CurNum.Next);
    PrevNum:=CurNum;
    CurNum:=CurNum.Next;
    CurNum.Prev:=PrevNum;
  end;
  CurNum.Number:=IntToStr(Random(8999999)+1000000);
  CurNum.Next:=nil;
  EndHead:=CurNum;
  for i := 1 to 3 do
  begin
    CurNum:=Head;
    for j := 1 to Random(NumOfNumbers) do
      CurNum:=CurNum.Next;
    CurNum.Number:=IntToStr(100+i);
  end;
end;

Procedure TransformToOneDest(const EndHead: PhNumADR2; const NewHead: PhNumADR1;
                            const NumOfNumbers: integer);
var i: integer;
    CurNum: PhNumADR2;
    CurNewNum: PhNumADR1;
begin
  CurNum:=EndHead;
  CurNewNum:=NewHead;
  for i := 1 to NumOfNumbers-1 do
  begin
    if length(CurNum.Number)>3 then
    begin
      CurNewNum.Number:=CurNum.Number;
      New(CurNewNum.Next);
      CurNewNum:=CurNewNum.Next;
    end;
    CurNum:=CurNum.Prev;
  end;
  CurNewNum.Next:=nil;
end;

procedure ShowTwoDestList(const Head: PhNumADR2);
var CurNum: PhNumADR2;
begin
  CurNum:=Head;
  write('Basis numbers list left-to-right: ');
  while CurNum<>nil do
  begin
    write(CurNum.Number:7,'|');
    CurNum:=CurNum.Next;
  end;
  writeln;
end;

procedure ShowTwoDestListBack(const EndHead: PhNumADR2);
var CurNum: PhNumADR2;
begin
  CurNum:=EndHead;
  write('Basis numbers list right-to-left: ');
  while CurNum<>nil do
  begin
    write(CurNum.Number:7,'|');
    CurNum:=CurNum.Prev;
  end;
  writeln;
end;


procedure ShowOneDestList(const Head: PhNumADR1);
var CurNum: PhNumADR1;
begin
  CurNum:=Head;
  write('Only 7-digit numbers list: ');
  while CurNum.next<>nil do
  begin
    write(CurNum.Number:7,'|');
    CurNum:=CurNum.Next;
  end;
  writeln;
end;


begin
  N:=9;
  New(Head);
  SetList(Head, EndHead, N);
  ShowTwoDestList(Head);
  ShowTwoDestListBack(EndHead);
  New(NewHead);
  TransformToOneDest(EndHead, NewHead, N);
  ShowOneDestList(NewHead);
  readln;
end.
