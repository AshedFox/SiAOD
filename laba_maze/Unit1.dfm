object Form1: TForm1
  Left = 0
  Top = 0
  BiDiMode = bdLeftToRight
  Caption = 'Maze escape'
  ClientHeight = 509
  ClientWidth = 878
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -33
  Font.Name = 'Comic Sans MS'
  Font.Style = []
  OldCreateOrder = False
  ParentBiDiMode = False
  OnActivate = FormActivate
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 46
  object lbEscapeMessage: TLabel
    Left = 64
    Top = 127
    Width = 10
    Height = 46
  end
  object Panel1: TPanel
    Left = 672
    Top = 0
    Width = 206
    Height = 509
    Align = alRight
    ShowCaption = False
    TabOrder = 0
    DesignSize = (
      206
      509)
    object lbScore: TLabel
      Left = 33
      Top = 23
      Width = 69
      Height = 26
      Caption = 'SCORE:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
    end
    object btnFirstFloor: TSpeedButton
      Left = 28
      Top = 73
      Width = 149
      Height = 41
      Anchors = [akRight]
      Caption = 'First floor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      OnClick = btnFirstFloorClick
      ExplicitTop = 68
    end
    object btnSecondFloor: TSpeedButton
      Left = 28
      Top = 141
      Width = 149
      Height = 41
      Anchors = [akRight]
      Caption = 'Second floor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      OnClick = btnSecondFloorClick
      ExplicitTop = 132
    end
    object btnThirdFloor: TSpeedButton
      Left = 28
      Top = 207
      Width = 149
      Height = 41
      Anchors = [akRight]
      Caption = 'Third floor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      OnClick = btnThirdFloorClick
      ExplicitTop = 195
    end
    object lbCharacter: TLabel
      Left = 56
      Top = 297
      Width = 32
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'You'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 270
    end
    object lbNextFloor: TLabel
      Left = 56
      Top = 361
      Width = 94
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'Next floor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 334
    end
    object lbPrevFloor: TLabel
      Left = 56
      Top = 393
      Width = 121
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'Previous floor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 366
    end
    object pbCharacterColor: TPaintBox
      Left = 33
      Top = 298
      Width = 17
      Height = 17
      Anchors = [akLeft, akBottom]
      ExplicitTop = 271
    end
    object pbNextFloorColor: TPaintBox
      Left = 33
      Top = 362
      Width = 17
      Height = 17
      Anchors = [akLeft, akBottom]
      ExplicitTop = 335
    end
    object pbPrevFloorColor: TPaintBox
      Left = 33
      Top = 393
      Width = 17
      Height = 17
      Anchors = [akLeft, akBottom]
      ExplicitTop = 366
    end
    object lbLoot: TLabel
      Left = 56
      Top = 329
      Width = 38
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'Loot'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 302
    end
    object pbLootColor: TPaintBox
      Left = 33
      Top = 330
      Width = 17
      Height = 17
      Anchors = [akLeft, akBottom]
      ExplicitTop = 303
    end
    object btnVisionMode: TSpeedButton
      Left = 10
      Top = 441
      Width = 186
      Height = 36
      Anchors = [akLeft, akBottom]
      Caption = 'Change vision mode'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Comic Sans MS'
      Font.Style = []
      ParentFont = False
      OnClick = btnVisionModeClick
      ExplicitTop = 414
    end
  end
end
