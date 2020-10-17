unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ExtCtrls, Vcl.StdCtrls, Vcl.Buttons,
  Vcl.Menus, System.Actions, Vcl.ActnList;

type
  ColorSet = record
    WallsColor, CorridorsColor, CharacterColor, EnterColor, ExitColor, LootColor: integer;
  end;
  Room = record
     X1, Y1, X2, Y2: integer;
     width, height: integer;
     centerX, centerY: integer;
  end;
  TDungeonField=array of array of integer;
  TForm1 = class(TForm)
    Panel1: TPanel;
    lbScore: TLabel;
    btnFirstFloor: TSpeedButton;
    btnSecondFloor: TSpeedButton;
    btnThirdFloor: TSpeedButton;
    lbCharacter: TLabel;
    lbNextFloor: TLabel;
    lbPrevFloor: TLabel;
    pbCharacterColor: TPaintBox;
    pbNextFloorColor: TPaintBox;
    pbPrevFloorColor: TPaintBox;
    lbLoot: TLabel;
    pbLootColor: TPaintBox;
    lbEscapeMessage: TLabel;
    btnVisionMode: TSpeedButton;
    procedure btnFirstFloorClick(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormResize(Sender: TObject);
    procedure btnSecondFloorClick(Sender: TObject);
    procedure btnThirdFloorClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure actLimitVisionExecute(Sender: TObject);
    procedure btnVisionModeClick(Sender: TObject);
  private

  public
    procedure MoveCharacter(var Dungeon: TDungeonField; Direction:char);
    procedure DrawDungeon(const Dungeon: TDungeonField);
    procedure SetBasicMap(var Dungeon: TDungeonField);
    procedure GenerateMaze(var Dungeon: TDungeonField);
    procedure SetFloor(const floor: byte);
    procedure SetColorSet(const Floor: byte);
    procedure SetColorsHelp;
    procedure ReEnableButtons(const Floor: byte);
    procedure ReNewScore;
    procedure MazeIsFinished;
  end;

var
  Form1: TForm1;

implementation

 {$R *.DFM}

var CharacterPositionY, CharacterPositionX: integer;
    CurFloor: integer=0;
    AmountOfLoot, Score: integer;
    CurDungeon: TDungeonField;
    MyColorSet: ColorSet;
    IsVisionLimited: boolean=True;

procedure TForm1.FormActivate(Sender: TObject);
begin
  Score:=0;
end;

procedure TForm1.SetColorSet(const floor: byte);
begin
  with MyColorSet do
  begin
    CharacterColor:=clWebRed;
    LootColor:=clYellow;
    case Floor of
      1:
      begin
        WallsColor:=clBlack;
        CorridorsColor:=ClWhite;
        EnterColor:=Panel1.Color;
        ExitColor:=clLtGray;
      end;
      2:
      begin
        WallsColor:=clSilver;
        CorridorsColor:=ClBlack;
        EnterColor:=clDkGray;
        ExitColor:=clWebWhiteSmoke;
      end;
      3:
      begin
        WallsColor:=clSkyBlue;
        CorridorsColor:=clCream;
        EnterColor:=clWebDarkCyan;
        ExitColor:=clWebBlueViolet;
      end;
    end;
  end;
end;

procedure TForm1.SetColorsHelp;
begin
  with pbCharacterColor.Canvas do
  begin
    Brush.Color:=MyColorSet.CharacterColor;
    Pen.Color:=clBlack;
    Rectangle(0,0,Width,Height);
  end;
  with pbLootColor.Canvas do
  begin
    Brush.Color:=MyColorSet.LootColor;
    Pen.Color:=clBlack;
    Rectangle(0,0,Width,Height);
  end;
  with pbNextFloorColor.Canvas do
  begin
    Brush.Color:=MyColorSet.ExitColor;
    Pen.Color:=clBlack;
    Rectangle(0,0,Width,Height);
  end;
  with pbPrevFloorColor.Canvas do
  begin
    Brush.Color:=MyColorSet.EnterColor;
    Pen.Color:=clBlack;
    Rectangle(0,0,Width,Height);
  end;
  if CurFloor=1 then
    lbPrevFloor.Visible:=False
  else
    lbPrevFloor.Visible:=True;
end;

procedure TForm1.SetFloor(const floor: byte);
begin
  case floor of
    1:
      SetLength(CurDungeon, 21, 21);
    2:
      SetLength(CurDungeon, 36, 36);
    3:
      SetLength(CurDungeon, 51, 51);
  end;
  CurFloor:=Floor;
  SetColorSet(floor);
  AmountOfLoot:=Round(High(CurDungeon) div 4);
  if (CharacterPositionY=1) and (CharacterPositionX=0) then
  begin
    CharacterPositionX:=High(CurDungeon)-1;
    CharacterPositionY:=High(CurDungeon)-1;
  end
  else
  begin
    CharacterPositionX:=1;
    CharacterPositionY:=1;
  end;
  SetBasicMap(CurDungeon);
  GenerateMaze(CurDungeon);
  Form1.Repaint;
  SetColorsHelp;
  DrawDungeon(CurDungeon);
end;

procedure TForm1.btnVisionModeClick(Sender: TObject);
begin
  IsVisionLimited:=not IsVisionLimited;
  DrawDungeon(CurDungeon);
end;

procedure TForm1.ReEnableButtons(const Floor: byte);
begin
  btnFirstFloor.Enabled:=True;
  btnSecondFloor.Enabled:=True;
  btnThirdFloor.Enabled:=True;
  btnVisionMode.Enabled:=True;
  case floor of
    1:
      btnFirstFloor.Enabled:=False;
    2:
      btnSecondFloor.Enabled:=False;
    3:
      btnThirdFloor.Enabled:=False;
    4:
      btnVisionMode.Enabled:=False;
  end;
end;

procedure TForm1.ReNewScore;
begin
  inc(Score, 50);
  lbScore.Caption:=('SCORE: ' + IntToStr(Score));
end;

procedure TForm1.actLimitVisionExecute(Sender: TObject);
begin
  //
end;

procedure TForm1.btnFirstFloorClick(Sender: TObject);
begin
  lbEscapeMessage.Visible:=False;
  SetFloor(1);
  ReEnableButtons(1);
end;

procedure TForm1.btnSecondFloorClick(Sender: TObject);
begin
    lbEscapeMessage.Visible:=False;
  SetFloor(2);
  ReEnableButtons(2);
end;

procedure TForm1.btnThirdFloorClick(Sender: TObject);
begin
  lbEscapeMessage.Visible:=False;
  SetFloor(3);
  ReEnableButtons(3);
end;

procedure TForm1.DrawDungeon(const Dungeon: TDungeonField);
var i, j: integer;
    x, y, step: integer;
begin
  step:=Round(0.8*Form1.Height/(length(Dungeon)+1));
  y:=Round(Form1.Height/12);
  with Form1.Canvas do
  begin
    for i := 0 to High(Dungeon) do
    begin
      x:=Round(Form1.Width/7);
      for j := 0 to High(Dungeon) do
      begin
        case Dungeon[i,j] of
            0:
            begin
              Brush.Color:=MyColorSet.WallsColor;
              Pen.Color:=MyColorSet.WallsColor;
            end;
            1:
            begin
              Brush.Color:=MyColorSet.CorridorsColor;
              Pen.Color:=MyColorSet.CorridorsColor;
            end;
            2:
            begin
              Brush.Color:=MyColorSet.CharacterColor;
              Pen.Color:=MyColorSet.CharacterColor;
            end;
            3:
            begin
              Brush.Color:=MyColorSet.EnterColor;
              Pen.Color:=MyColorSet.EnterColor;
            end;
            4:
            begin
              Brush.Color:=MyColorSet.ExitColor;
              Pen.Color:=MyColorSet.ExitColor;
            end;
            7:
            begin
              Brush.Color:=MyColorSet.LootColor;
              Pen.Color:=MyColorSet.LootColor;
            end;
        end;
        if IsVisionLimited then
          if (abs(CharacterPositionY-i)>=High(Dungeon)div 4) or
           (abs(CharacterPositionX-j)>=High(Dungeon)div 4) then
          begin
            Brush.Color:=MyColorSet.WallsColor;
            Pen.Color:=MyColorSet.WallsColor;
          end;
        Rectangle(x,y,x+step,y+step);
        inc(x,step);
      end;
      inc(y, step);
    end;
  end;
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if CurFloor<>0 then
  begin
    if (Key=ord('W')) or (Key=ord('w')) or (Key=VK_UP) then
      MoveCharacter(CurDungeon, 'W');
    if (Key=ord('S')) or (Key=ord('s')) or (Key=VK_DOWN) then
      MoveCharacter(CurDungeon,'S');
    if (Key=ord('A')) or (Key=ord('a')) or (Key=VK_LEFT) then
      MoveCharacter(CurDungeon,'A');
    if (Key=ord('D')) or (Key=ord('d')) or (Key=VK_RIGHT) then
      MoveCharacter(CurDungeon,'D');
  end;
end;

procedure TForm1.FormResize(Sender: TObject);
begin
  Form1.Repaint;
  SetColorsHelp;
  if not lbEscapeMessage.Visible then
    DrawDungeon(CurDungeon);
end;

function IsRoomIntersects(CheckRoom1, CheckRoom2:room): boolean;
begin
  result:=(CheckRoom1.x1-1 <= CheckRoom2.x2) and (CheckRoom1.x2+1 >= CheckRoom2.x1) and
          (CheckRoom1.y1-1 <= CheckRoom2.y2) and (CheckRoom1.y2+1 >= CheckRoom2.y1)
end;

procedure SetRoomData(const DungeonSize: integer;var MyRoom: Room);
begin
  with MyRoom do
  begin
    X1:=Random(DungeonSize-1)+1;
    Y1:=Random(DungeonSize-1)+1;
    Width:=Random(Trunc(DungeonSize/20))+2;
    Height:=Random(Trunc(DungeonSize/20))+2;
    if X1+Width>=DungeonSize then
    begin
      dec(x1);
      width:=DungeonSize-x1-1;
    end;
    if Y1+Height>=DungeonSize then
    begin
      dec(y1);
      height:=DungeonSize-y1-1;
    end;
    X2:=X1+Width;
    Y2:=Y1+Height;
    centerX:=Round(X1+Width/2);
    centerY:=Round(Y1+Height/2);
  end;
end;

procedure AddRoom(var Dungeon: TDungeonField; const RoomToAdd: Room);
var i, j: integer;
begin
  for i := RoomToAdd.Y1 to RoomToAdd.Y2 do
    for j := RoomToAdd.X1 to RoomToAdd.X2 do
      Dungeon[i,j]:=1;
end;

procedure AddHorizontalCorridor(var Dungeon: TDungeonField; const X1, X2, Y: integer);
var i: integer;
begin
  if X1<X2 then
    for i := X1 to X2 do
    begin
      Dungeon[Y,i]:=1;
    end
  else
    for i:= X1 downto X2 do
      Dungeon[Y,i]:=1;
end;

procedure AddVerticalCorridor(var Dungeon: TDungeonField; const Y1, Y2, X: integer);
var i: integer;
begin
  if Y1<Y2 then
    for i := Y1 to Y2 do
    begin
      Dungeon[i,X]:=1;
    end
  else
    for i:= Y1 downto Y2 do
      Dungeon[i,X]:=1;
end;

Procedure SortRooms(var RoomsToAdd: array of Room);
Procedure QuickSortRooms(min, max: integer);
var CentralElement, i, j: integer;
    temp: Room;
begin
  with RoomsToAdd[max-((max-min) div 2)] do
    CentralElement:=centerX+centerY;
  i:=min; j:=max;
  while i<j do
  begin
    while RoomsToAdd[i].centerX+RoomsToAdd[i].centerY<CentralElement do
      inc(i);
    while RoomsToAdd[j].centerX+RoomsToAdd[j].centerY>CentralElement do
      dec(j);
    if i<=j then
    begin
      temp:=RoomsToAdd[i];
      RoomsToAdd[i]:=RoomsToAdd[j];
      RoomsToAdd[j]:=temp;
      inc(i); dec(j);
    end;
  end;
  if min<j then
    QuickSortRooms(min, j);
  if i<max then
    QuickSortRooms(i, max);
end;
begin
  QuickSortRooms(0, High(RoomsToAdd));
end;

procedure AddLoot(var Dungeon: TDungeonField; const Rooms: array of Room);
var PlacedLoot, AmountOfRooms, RoomNum: integer;
    i,j: integer;
begin
  AmountOfRooms:=High(Rooms);
  PlacedLoot:=0;
  while PlacedLoot < AmountOfLoot do
  begin
    RoomNum:=Random(AmountOfRooms+1);
    i:=Rooms[RoomNum].Y1+Random(Rooms[RoomNum].Height);
    j:=Rooms[RoomNum].X1+Random(Rooms[RoomNum].Width);
    if Dungeon[i,j]<>7 then
    begin
      Dungeon[i, j]:=7;
      inc(PlacedLoot);
    end;
  end;
end;

procedure TForm1.GenerateMaze(var Dungeon: TDungeonField);
var RoomsToAdd: array of Room;
    i,j: integer;
    DungeonSize: integer;
    IntersectionFlaq: boolean;
begin
  DungeonSize:=High(Dungeon);
  SetLength(RoomsToAdd, DungeonSize div 3);
  SetRoomData(DungeonSize, RoomsToAdd[0]);
  i := 1;
  while i<=High(RoomsToAdd) do
  begin
    IntersectionFlaq:=False;
    SetRoomData(DungeonSize, RoomsToAdd[i]);
    j:=0;
    while (j <= i-1) and (not IntersectionFlaq) do
    begin
      if IsRoomIntersects(RoomsToAdd[i], RoomsToAdd[j]) then
        IntersectionFlaq:=True;
      inc(j);
    end;
    if not IntersectionFlaq then
      inc(i);
  end;
  for i := 0 to High(RoomsToAdd) do
    AddRoom(Dungeon, RoomsToAdd[i]);
  SortRooms(RoomsToAdd);
  if Dungeon[1,1]<>1 then
  begin
    AddHorizontalCorridor(Dungeon, RoomsToAdd[0].centerX, 1, RoomsToAdd[0].centerY);
    AddVerticalCorridor(Dungeon, RoomsToAdd[0].centerY, 1, 1);
  end;
  if Dungeon[DungeonSize-1, DungeonSize-1]<>1 then
  begin
    i:=High(RoomsToAdd);
    AddHorizontalCorridor(Dungeon, RoomsToAdd[i].centerX, DungeonSize-1, RoomsToAdd[i].centerY);
    AddVerticalCorridor(Dungeon, RoomsToAdd[i].centerY, DungeonSize-1, DungeonSize-1);
  end;
  for i := 0 to High(RoomsToAdd)-1 do
  begin
    AddHorizontalCorridor(Dungeon, RoomsToAdd[i].centerX, RoomsToAdd[i+1].centerX, RoomsToAdd[i].centerY);
    AddVerticalCorridor(Dungeon, RoomsToAdd[i].centerY, RoomsToAdd[i+1].centerY, RoomsToAdd[i+1].centerX);
  end;

  AddLoot(Dungeon, RoomsToAdd);
  Dungeon[CharacterPositionY,CharacterPositionX]:=2;
  if CurFloor<>1 then
    Dungeon[1,0]:=3;
  Dungeon[DungeonSize, DungeonSize-1]:=4;
end;

procedure TForm1.MazeIsFinished;
begin
  CurFloor:=0;
  ReEnableButtons(4);
  Form1.Repaint;
  lbEscapeMessage.Visible:=True;
  lbEscapeMessage.Caption:='Conratulations!' + #10#13 + 'You escaped with score ' + IntTOStr(SCORE);
  lbScore.Caption:='SCORE: ';
  SCORE:=0;
end;

procedure TForm1.MoveCharacter(var Dungeon: TDungeonField; Direction: char);
begin
  case Direction of
    'W':
    begin
      if Dungeon[CharacterPositionY-1,CharacterPositionX]<>0 then
      begin
        Dungeon[CharacterPositionY, CharacterPositionX]:=1;
        dec(CharacterPositionY);
        if Dungeon[CharacterPositionY, CharacterPositionX]=7 then
          ReNewScore;
        Dungeon[CharacterPositionY, CharacterPositionX]:=2;
        DrawDungeon(CurDungeon);
      end;
    end;
    'A':
    begin
      if Dungeon[CharacterPositionY,CharacterPositionX-1]<>0 then
      begin
        Dungeon[CharacterPositionY, CharacterPositionX]:=1;
        dec(CharacterPositionX);
        if CharacterPositionX=0 then
        begin
          ReEnableButtons(CurFloor-1);
          SetFloor(CurFloor-1);
        end
        else
        begin
          if Dungeon[CharacterPositionY, CharacterPositionX]=7 then
            ReNewScore;
          Dungeon[CharacterPositionY, CharacterPositionX]:=2;
          DrawDungeon(CurDungeon);
        end;
      end;
    end;
    'S':
    begin
      if Dungeon[CharacterPositionY+1,CharacterPositionX]<>0 then
      begin
        Dungeon[CharacterPositionY, CharacterPositionX]:=1;
        inc(CharacterPositionY);
        if CharacterPositionY=High(Dungeon) then
        begin
          if CurFloor<>3 then
          begin
            ReEnableButtons(CurFloor+1);
            SetFloor(CurFloor+1);
          end
          else
            MazeIsFinished;
        end
        else
        begin
          if Dungeon[CharacterPositionY, CharacterPositionX]=7 then
            ReNewScore;
          Dungeon[CharacterPositionY, CharacterPositionX]:=2;
          DrawDungeon(CurDungeon);
        end;
      end;
    end;
    'D':
    begin
      if Dungeon[CharacterPositionY,CharacterPositionX+1]<>0 then
      begin
        Dungeon[CharacterPositionY, CharacterPositionX]:=1;
        inc(CharacterPositionX);
        if Dungeon[CharacterPositionY, CharacterPositionX]=7 then
          ReNewScore;
        Dungeon[CharacterPositionY, CharacterPositionX]:=2;
        DrawDungeon(CurDungeon);
      end;
    end;
  end;
end;

procedure TForm1.SetBasicMap(var Dungeon: TDungeonField);
var
  i: Integer;
  j: Integer;
begin
  for i := 0 to high(Dungeon) do
    for j := 0 to High(Dungeon) do
      Dungeon[i,j]:=0;
end;

end.
