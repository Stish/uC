unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    Image1: TImage;
    OpenDialog1: TOpenDialog;
    Button1: TButton;
    Memo1: TMemo;
    Button2: TButton;
    Button3: TButton;
    SaveDialog1: TSaveDialog;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
begin
	if form1.OpenDialog1.Execute then
        	form1.Image1.Picture.LoadFromFile(form1.opendialog1.filename);
end;                                                                    

procedure TForm1.Button2Click(Sender: TObject);
var
	x : integer;
        y : integer;
begin
	form1.Memo1.Clear;
        form1.Memo1.Lines.Add('void draw_pic(void)');
        form1.Memo1.Lines.Add('{');
        for x := 1 to 160 do
        begin
        	for y := 1 to 80 do
                begin
                	if form1.Image1.Canvas.Pixels[x, y] = clblack then
                        	form1.Memo1.Lines.Add('    lcd_graphics_plot_pixel(' + inttostr(x) + ', ' + inttostr(y) + ', PIXEL_ON);');
                end;
        end;
        form1.Memo1.Lines.Add('}');
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
	if form1.SaveDialog1.Execute then
        	form1.Memo1.Lines.SaveToFile(form1.SaveDialog1.FileName);
end;

end.
