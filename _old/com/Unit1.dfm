object Form1: TForm1
  Left = 596
  Top = 124
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Switch It!'
  ClientHeight = 185
  ClientWidth = 369
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 26
    Height = 20
    Caption = 'Pin:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 121
    Height = 73
    Shape = bsFrame
    Style = bsRaised
  end
  object Shape1: TShape
    Left = 8
    Top = 88
    Width = 33
    Height = 33
    Brush.Color = clRed
    Pen.Width = 2
    Shape = stCircle
  end
  object Shape2: TShape
    Left = 269
    Top = 145
    Width = 50
    Height = 15
    Shape = stRoundRect
  end
  object Shape3: TShape
    Left = 269
    Top = 80
    Width = 50
    Height = 15
    Shape = stRoundRect
  end
  object Shape4: TShape
    Left = 269
    Top = 15
    Width = 50
    Height = 15
    Shape = stRoundRect
  end
  object Shape5: TShape
    Left = 319
    Top = 30
    Width = 15
    Height = 50
    Shape = stRoundRect
  end
  object Shape6: TShape
    Left = 254
    Top = 30
    Width = 15
    Height = 50
    Shape = stRoundRect
  end
  object Shape7: TShape
    Left = 254
    Top = 95
    Width = 15
    Height = 50
    Shape = stRoundRect
  end
  object Shape8: TShape
    Left = 319
    Top = 95
    Width = 15
    Height = 50
    Shape = stRoundRect
  end
  object Shape9: TShape
    Left = 334
    Top = 145
    Width = 15
    Height = 15
    Shape = stCircle
  end
  object Button1: TButton
    Left = 8
    Top = 40
    Width = 49
    Height = 25
    Caption = 'On'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 64
    Top = 40
    Width = 49
    Height = 25
    Caption = 'Off'
    TabOrder = 1
    OnClick = Button2Click
  end
  object ComboBox1: TComboBox
    Left = 40
    Top = 8
    Width = 73
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8')
  end
  object Edit1: TEdit
    Left = 48
    Top = 93
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'none'
  end
  object Edit2: TEdit
    Left = 48
    Top = 128
    Width = 121
    Height = 21
    TabOrder = 4
    Text = '1'
  end
  object Edit3: TEdit
    Left = 48
    Top = 160
    Width = 121
    Height = 21
    TabOrder = 5
    Text = 'Edit3'
  end
  object Button3: TButton
    Left = 144
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Button3'
    TabOrder = 6
    OnClick = Button3Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer1Timer
    Left = 200
    Top = 104
  end
end
