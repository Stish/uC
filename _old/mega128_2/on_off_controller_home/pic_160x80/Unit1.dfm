object Form1: TForm1
  Left = 472
  Top = 158
  Width = 338
  Height = 444
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 165
    Height = 85
  end
  object Button1: TButton
    Left = 168
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 0
    Top = 96
    Width = 329
    Height = 321
    Lines.Strings = (
      'Memo1')
    TabOrder = 1
  end
  object Button2: TButton
    Left = 248
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Generate'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 168
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 3
    OnClick = Button3Click
  end
  object OpenDialog1: TOpenDialog
    Filter = '*.bmp|*.bmp'
    Left = 256
    Top = 64
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = '*.c'
    Filter = '*.c|*.c'
    Left = 288
    Top = 64
  end
end
